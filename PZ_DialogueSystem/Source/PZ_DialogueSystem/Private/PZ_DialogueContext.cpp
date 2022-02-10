// Fill out your copyright notice in the Description page of Project Settings.


#include "PZ_DialogueContext.h"
#include "PZ_DialogueEditor.h"


#include "DialogueSystemComponent/PZ_DialogueSystemComponent.h"
#include "DialogueSystemComponent/PZ_BaseParticipantNickName.h"

#include "DialogueEvents/PZ_DialogueEvent.h"
#include "DialogueServices/PZ_DialogueService.h"

#include "Nodes/BaseNode/PZ_DialogueRBaseNode.h"
#include "Nodes/EdgePoint/PZ_DialogueREdgeNode.h"






UPZ_DialogueContext::UPZ_DialogueContext(UPZ_DialogueEditor* Dialog)
{
	InitContext(Dialog);
}

void UPZ_DialogueContext::InitContext(UPZ_DialogueEditor* Dialog)
{
	if( !IsValid(Dialog) )
	{
		FMessageLog(FName("PIE")).Warning(FText::FromString("DialogueContext: Initialization error. Dialog asset invalid!"));
		return;
	}


	ImplementDialog = Dialog;

	ParticipantsInstance.Empty();
	OnlyOnceChosenEdges.Empty();
	ParticipantsWhoIsSaying.Empty();
	ActiveServices.Empty();

	DialodState = EDialogState::NotStarted;
	CurrentNode = ImplementDialog->CompiledNodes[0];

	for( auto& LDialogParticipantInfo : Dialog->Participants )
	{
		ParticipantsInstance.Add(LDialogParticipantInfo.Key, FParticipantInstanceInfo(&LDialogParticipantInfo.Value));
	}


	if( !Dialog->IsParticipantsInfoValid() )
	{
		FMessageLog(FName("PIE")).Warning(FText::FromString("DialogueContext: Initialization error. Dialog asset participants info has compilation errors!"));
	}
}


void UPZ_DialogueContext::BeginDestroy()
{
	EndDialog();
	CleanParticipants();
	StopAllServices();

	Super::BeginDestroy();
}







void UPZ_DialogueContext::IterateDialog()
{
	if( IsDialogIteratingNow ) return;
	IsDialogIteratingNow = true;


	bool IsAlreadyExecuted = CurrentNode != ImplementDialog->CompiledNodes[0];

	while( DialodState == EDialogState::InProcess )
	{
		if( !IsAlreadyExecuted )
		{
			if( CurrentNode->Execute(this) )
			{
				IsDialogIteratingNow = false;
				return;
			}
		}

		if( !GoToNextNode() )
		{
			IsDialogIteratingNow = false;
			return;
		}

		IsAlreadyExecuted = false;
	}


	IsDialogIteratingNow = false;
}


bool UPZ_DialogueContext::GoToNextNode()
{
	UPZ_DialogueRBaseNode* NextNode = CurrentNode->ChooseNextNode(this);
	if( NextNode == nullptr ) return false;

	if( UPZ_DialogueREdgeNode* LEdgePoint = Cast<UPZ_DialogueREdgeNode>(NextNode) )
	{
		if( LEdgePoint->CanBeChosenOnlyOnce )
		{
			OnlyOnceChosenEdges.AddUnique(LEdgePoint->MyIndex);
		}

		NextNode = LEdgePoint->NextNodes[0];
	}

	bool BreakIterating = CurrentNode->StopExecute(this);

	CurrentNode = NextNode;
	return !BreakIterating;
}





bool UPZ_DialogueContext::RestartDialog()
{
	if( !CanStartDialog() ) return false;


	CurrentNode = ImplementDialog->CompiledNodes[0];
	ParticipantsWhoIsSaying.Empty();
	OnlyOnceChosenEdges.Empty();
	StopAllServices();

	DialodState = EDialogState::InProcess;
	OnDialogStartBind.Broadcast();
	IterateDialog();

	return true;
}

void UPZ_DialogueContext::EndDialog()
{
	if( !GetIsDialogStarted() ) return;


	ParticipantsWhoIsSaying.Empty();
	StopAllServices();

	DialodState = EDialogState::NotStarted;
	OnDialogEndBind.Broadcast();
}

void UPZ_DialogueContext::PauseDialog()
{
	if( DialodState == EDialogState::InProcess )
	{
		DialodState = EDialogState::Paused;
		OnDialogPausedBind.Broadcast();
	}
}

void UPZ_DialogueContext::ContinueDialog()
{
	if( !GetIsDialogStarted() ) return;


	DialodState = EDialogState::InProcess;
	OnDialogContinuedBind.Broadcast();
	IterateDialog();
}



bool UPZ_DialogueContext::CanStartDialog() const
{
	if( ParticipantsInstance.Num() == 0 ) return false;


	for( const auto& LParticipant : ParticipantsInstance )
	{
		if( LParticipant.Value.ParticipantInfo->IsNecessaryForStart && !IsValid(LParticipant.Value.ParticipantActor) )
		{
			return false;
		}
	}

	return true;
}

EDialogState UPZ_DialogueContext::GetDialogueState() const
{
	return DialodState;
}

bool UPZ_DialogueContext::GetIsDialogStarted() const
{
	return DialodState != EDialogState::NotStarted;
}






bool UPZ_DialogueContext::CanAddParticipant(AActor* ParticipantActor) const
{
	if( !IsValid(ParticipantActor) ) return false;

	if( UPZ_DialogueSystemComponent* LDialogueComponent = ParticipantActor->FindComponentByClass<UPZ_DialogueSystemComponent>() )
	{
		if( LDialogueComponent->NickName == nullptr || IsValid(LDialogueComponent->GetCurrentDialog()) || !LDialogueComponent->GetCanParticipateInDialog(this) ) return false;

		FName EmptyRole;
		return HasEmptyRoleFor(LDialogueComponent->NickName, EmptyRole);
	}

	return false;
}

bool UPZ_DialogueContext::CanAddParticipantToRole(AActor* ParticipantActor, const FName& RoleName) const
{
	if( !IsValid(ParticipantActor) || !ParticipantsInstance.Contains(RoleName) ) return false;
	if( GetIsDialogStarted() && (!ParticipantsInstance[RoleName].ParticipantInfo->CanJoinToDialogAfterStart || ParticipantsInstance[RoleName].ParticipantActor) ) return false;

	if( UPZ_DialogueSystemComponent* LDialogueComponent = ParticipantActor->FindComponentByClass<UPZ_DialogueSystemComponent>() )
	{
		if( !LDialogueComponent->NickName || IsValid(LDialogueComponent->GetCurrentDialog()) || !LDialogueComponent->GetCanParticipateInDialog(this) ) return false;

		return DoesRoleSupportNickName(RoleName, LDialogueComponent->NickName);
	}

	return false;
}


bool UPZ_DialogueContext::TryAddParticipant(AActor* ParticipantActor)
{
	if( !CanAddParticipant(ParticipantActor) ) return false;


	for( auto& LParticipantInfo : ParticipantsInstance )
	{
		if( TryAddParticipantToRole(ParticipantActor, LParticipantInfo.Key) )
		{
			return true;
		}
	}

	return false;
}

bool UPZ_DialogueContext::TryAddParticipantToRole(AActor* ParticipantActor, const FName& RoleName)
{
	if( !CanAddParticipantToRole(ParticipantActor, RoleName) ) return false;


	UPZ_DialogueSystemComponent* LDialogueComponent = ParticipantActor->FindComponentByClass<UPZ_DialogueSystemComponent>();

	if( ParticipantsInstance[RoleName].ParticipantActor )
	{
		int NewParticipantPriority = ParticipantsInstance[RoleName].ParticipantInfo->CanBeChosenBy[LDialogueComponent->NickName].Priority;
		int LastParticipantPriority = ParticipantsInstance[RoleName].ParticipantInfo->CanBeChosenBy[ParticipantsInstance[RoleName].ParticipantDialogueComponent->NickName].Priority;

		FName EmptyRole;
		if( NewParticipantPriority > LastParticipantPriority && HasEmptyRoleFor(ParticipantsInstance[RoleName].ParticipantDialogueComponent->NickName, EmptyRole) )
		{
			SetParticipant(ParticipantsInstance[RoleName].ParticipantActor, ParticipantsInstance[RoleName].ParticipantDialogueComponent, EmptyRole);
			SetParticipant(ParticipantActor, LDialogueComponent, RoleName);

			OnParticipantEnter(ParticipantActor, RoleName);

			return true;
		}
	}
	else
	{
		SetParticipant(ParticipantActor, LDialogueComponent, RoleName);
		OnParticipantEnter(ParticipantActor, RoleName);

		return true;
	}

	return false;
}




bool UPZ_DialogueContext::CanRemoveParticipant(AActor* ParticipantActor) const
{
	FName LRole = GetActorRole(ParticipantActor);
	return CanRemoveParticipantFromRole(LRole);
}

bool UPZ_DialogueContext::CanRemoveParticipantFromRole(const FName& RoleName) const
{
	if( RoleName.IsNone() ) return false;

	if( CheckParticipantAtRoleValid(RoleName) )
	{
		return (GetIsDialogStarted()) ? ParticipantsInstance[RoleName].ParticipantInfo->CanLeaveFromDialog : true;
	}

	return false;
}


bool UPZ_DialogueContext::TryRemoveParticipant(AActor* ParticipantActor)
{
	FName LRole = GetActorRole(ParticipantActor);
	return TryRemoveParticipantFromRole(LRole);
}

bool UPZ_DialogueContext::TryRemoveParticipantFromRole(const FName& RoleName)
{
	if( CanRemoveParticipantFromRole(RoleName) )
	{
		OnParticipantLeave(ParticipantsInstance[RoleName].ParticipantActor, RoleName);
		ParticipantsInstance[RoleName].ParticipantActor = nullptr;
		ParticipantsInstance[RoleName].ParticipantDialogueComponent = nullptr;
		return true;
	}

	return false;
}

void UPZ_DialogueContext::CleanParticipants()
{
	for( const auto& LParticipant : ParticipantsInstance )
	{
		TryRemoveParticipantFromRole(LParticipant.Key);
	}
}



void UPZ_DialogueContext::OnParticipantEnter(AActor* Participant, const FName& ToRole)
{
	ParticipantsInstance[ToRole].ParticipantDialogueComponent->OnEnterToDialog();
	OnParticipantEnterBind.Broadcast(Participant, ToRole);
	IterateDialog();
}

void UPZ_DialogueContext::OnParticipantLeave(AActor* Participant, const FName& FromRole)
{
	ParticipantsInstance[FromRole].ParticipantDialogueComponent->OnLeaveFromDialog(FromRole);
	ParticipantsInstance[FromRole].ParticipantDialogueComponent->ParticipateInDialog = nullptr;
	ParticipantsInstance[FromRole].ParticipantDialogueComponent->MyCurrentRole = "None";

	if( DialodState != EDialogState::NotStarted )
	{
		if( ParticipantsInstance[FromRole].ParticipantInfo->EndDialogWhenLeave )
		{
			EndDialog();
			return;
		}

		ParticipantFinishSaying(FromRole);

		OnParticipantLeaveBind.Broadcast(Participant, FromRole);
		IterateDialog();
	}
}







void UPZ_DialogueContext::SkipCurrentPhrases()
{
	if( !IsAnybodySaying() || DialodState != EDialogState::InProcess ) return;

	for( const FName& LRole : ParticipantsWhoIsSaying )
	{
		if( !ParticipantsInstance[LRole].ParticipantDialogueComponent->IsMyPhraseCanBeSkipped ) return;
	}


	for( const FName& LRole : ParticipantsWhoIsSaying )
	{
		ParticipantsInstance[LRole].ParticipantDialogueComponent->Stop(); //should call finish saying
	}

	OnSkippedCurrentPhrasesBind.Broadcast();
}


void UPZ_DialogueContext::RequestParticipantChoseNextActions(const FName& ParticipantRoleName, const TArray<UPZ_DialogueRBaseNode*>& PotentialActions, TArray<UPZ_DialogueRBaseNode*>& ChosenNodes)
{
	if( !CheckParticipantAtRoleValid(ParticipantRoleName) ) return;

	ChosenNodes.Empty();
	ParticipantsInstance[ParticipantRoleName].ParticipantDialogueComponent->ChoseNextDialogActions(PotentialActions, ChosenNodes);
}


void UPZ_DialogueContext::ExecuteEvents(const TArray<FEventInfo>& Events)
{
	for( const auto& LEvent : Events )
	{
		if( !IsValid(LEvent.Event) ) continue;


		switch( LEvent.SendEventOption )
		{
		case EChoseOptionType::ToAllFromList:
		{
			for( const FName& RoleName : LEvent.RoleNames )
			{
				if( CheckParticipantAtRoleValid(RoleName) )
				{
					LEvent.Event->Execute(this, ParticipantsInstance[RoleName].ParticipantActor, RoleName);
				}
			}

			break;
		}

		case EChoseOptionType::ToAllParticipants:
		{
			for( const auto& LParticipant : ParticipantsInstance )
			{
				if( LParticipant.Value.ParticipantActor )
				{
					LEvent.Event->Execute(this, LParticipant.Value.ParticipantActor, LParticipant.Key);
				}
			}

			break;
		}


		case EChoseOptionType::ToRandomFromList:
		{
			FName LChosenRole = LEvent.RoleNames[FMath::RandRange(0, LEvent.RoleNames.Num() - 1)];

			if( ParticipantsInstance[LChosenRole].ParticipantActor )
			{
				LEvent.Event->Execute(this, ParticipantsInstance[LChosenRole].ParticipantActor, LChosenRole);
			}

			break;
		}

		case EChoseOptionType::ToRandomParticipant:
		{
			TArray<FName> ActiveRoles;
			GetActiveRoles(ActiveRoles);
			FName LChosenRole = ActiveRoles[FMath::RandRange(0, ActiveRoles.Num() - 1)];


			if( ParticipantsInstance[LChosenRole].ParticipantActor )
			{
				LEvent.Event->Execute(this, ParticipantsInstance[LChosenRole].ParticipantActor, LChosenRole);
			}

			break;
		}
		}
	}
}


void UPZ_DialogueContext::RunServices(const TArray<UPZ_DialogueService*>& Services)
{
	for( UPZ_DialogueService* LService : Services )
	{
		if( !IsValid(LService) ) continue;

		LService->BeginService(this);
		ActiveServices.Add(LService);
	}
}


void UPZ_DialogueContext::StopService(UPZ_DialogueService* Service)
{
	if( !IsValid(Service) ) return;

	Service->EndService(this);
	ActiveServices.Remove(Service);
}

void UPZ_DialogueContext::StopServices(const TArray<UPZ_DialogueService*>& Services)
{
	for( UPZ_DialogueService* LService : Services )
	{
		StopService(LService);
	}
}

void UPZ_DialogueContext::StopAllServices()
{
	for( UPZ_DialogueService* LService : ActiveServices )
	{
		LService->EndService(this);
	}

	ActiveServices.Empty();
}


bool UPZ_DialogueContext::ParticipantStartSaying(const FName& ParticipantRoleName)
{
	if( ParticipantsWhoIsSaying.Contains(ParticipantRoleName) ) return false;
	if( !CheckParticipantAtRoleValid(ParticipantRoleName) ) return false;

	ParticipantsWhoIsSaying.Add(ParticipantRoleName);
	return true;
}

void UPZ_DialogueContext::ParticipantFinishSaying(const FName& ParticipantRoleName)
{
	if( ParticipantsWhoIsSaying.Contains(ParticipantRoleName) )
	{
		ParticipantsWhoIsSaying.Remove(ParticipantRoleName);
		IterateDialog();
	}
}






void UPZ_DialogueContext::GetParticipantsActors(TArray<AActor*>& Participants) const
{
	Participants.Empty();

	for( const auto& LParticipant : ParticipantsInstance )
	{
		if( LParticipant.Value.ParticipantActor )
		{
			Participants.Add(LParticipant.Value.ParticipantActor);
		}
	}
}

void UPZ_DialogueContext::GetActiveRoles(TArray<FName>& ActiveRoles) const
{
	ActiveRoles.Empty();

	for( const auto& LParticipant : ParticipantsInstance )
	{
		if( LParticipant.Value.ParticipantActor )
		{
			ActiveRoles.Add(LParticipant.Key);
		}
	}
}

void UPZ_DialogueContext::GetUnusedRoles(TArray<FName>& UnusedRoles) const
{
	UnusedRoles.Empty();

	for( const auto& LParticipant : ParticipantsInstance )
	{
		if( !IsValid(LParticipant.Value.ParticipantActor) )
		{
			UnusedRoles.Add(LParticipant.Key);
		}
	}
}

void UPZ_DialogueContext::GetAllRoles(TArray<FName>& AllRoles) const
{
	ParticipantsInstance.GenerateKeyArray(AllRoles);
}

FName UPZ_DialogueContext::GetActorRole(AActor* Actor) const
{
	if( !IsValid(Actor) ) return "None";


	for( const auto& LParticipant : ParticipantsInstance )
	{
		if( LParticipant.Value.ParticipantActor == Actor )
		{
			return LParticipant.Key;
		}
	}

	return "None";
}

bool UPZ_DialogueContext::GetRoleInfo(const FName& Role, FParticipantInstanceInfo& RoleInfo) const
{
	if( !ParticipantsInstance.Contains(Role) ) return false;

	RoleInfo = ParticipantsInstance[Role];
	return true;
}



bool UPZ_DialogueContext::CheckParticipantAtRoleValid(const FName& RoleName) const
{
	return (ParticipantsInstance.Contains(RoleName) && IsValid(ParticipantsInstance[RoleName].ParticipantActor));
}

bool UPZ_DialogueContext::IsAnybodySaying() const
{
	return ParticipantsWhoIsSaying.Num() > 0;
}

bool UPZ_DialogueContext::IsParticipantSayingNow(const FName& ParticipantName) const
{
	return ParticipantsWhoIsSaying.Contains(ParticipantName);
}


void UPZ_DialogueContext::GetAllParticipantsWhoIsSaying(TArray<AActor*>& SayingParticipants) const
{
	SayingParticipants.Empty();

	for( const FName& LRole : ParticipantsWhoIsSaying )
	{
		SayingParticipants.Add(ParticipantsInstance[LRole].ParticipantActor);
	}
}

void UPZ_DialogueContext::GetAllDialogueComponentsWhoIsSaying(TArray<UPZ_DialogueSystemComponent*>& SayingComponents) const
{
	SayingComponents.Empty();

	for( const FName& LRole : ParticipantsWhoIsSaying )
	{
		SayingComponents.Add(ParticipantsInstance[LRole].ParticipantDialogueComponent);
	}
}

TArray<FName> UPZ_DialogueContext::GetAllParticipantsRolesWhoIsSaying() const
{
	return ParticipantsWhoIsSaying;
}




UPZ_DialogueEditor* UPZ_DialogueContext::GetImplementedDialog() const
{
	return ImplementDialog;
}







bool UPZ_DialogueContext::IsEdgeAlreadyChosen(int32 EdgeIndex) const
{
	return OnlyOnceChosenEdges.Contains(EdgeIndex);
}


void UPZ_DialogueContext::SetParticipant(AActor* Participant, UPZ_DialogueSystemComponent* DialogueComponent, const FName& RoleName)
{
	if( !IsValid(Participant) || !IsValid(DialogueComponent) || RoleName.IsNone() ) return;


	ParticipantsInstance[RoleName].ParticipantActor = Participant;
	ParticipantsInstance[RoleName].ParticipantDialogueComponent = DialogueComponent;

	DialogueComponent->MyCurrentRole = RoleName;
	DialogueComponent->ParticipateInDialog = this;
}

bool UPZ_DialogueContext::HasEmptyRoleFor(TSubclassOf<UPZ_BaseParticipantNickName> NickName, FName& EmptyRoleName) const
{
	for( const auto& LParticipant : ParticipantsInstance )
	{
		if( GetIsDialogStarted() && !LParticipant.Value.ParticipantInfo->CanJoinToDialogAfterStart ) continue;
		if( !IsValid(LParticipant.Value.ParticipantActor) && DoesRoleSupportNickName(LParticipant.Key, NickName) )
		{
			EmptyRoleName = LParticipant.Key;
			return true;
		}
	}

	return false;
}

bool UPZ_DialogueContext::DoesRoleSupportNickName(const FName& RoleName, TSubclassOf<UPZ_BaseParticipantNickName> NickName) const
{
	if( RoleName.IsNone() || NickName == nullptr ) return false;


	for( const auto& LToRoleParticipantInfo : ParticipantsInstance[RoleName].ParticipantInfo->CanBeChosenBy )
	{
		if( LToRoleParticipantInfo.Value.SupportNickNameClassChildren )
		{
			if( NickName->GetClass()->IsChildOf(LToRoleParticipantInfo.Key) ) return true;
		}
		else
		{
			if( NickName == LToRoleParticipantInfo.Key ) return true;
		}
	}


	return false;
}
