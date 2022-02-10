// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueSystemComponent/PZ_DialogueSystemComponent.h"
#include "DialogueSystemComponent/PZ_BaseParticipantNickName.h"

#include "PZ_DialogueContext.h"
#include "PZ_DialogueManager.h"
#include "PZ_DialogueEditor.h"
#include "PhrasePropagetion/PZ_PhrasePropagation.h"





UPZ_DialogueSystemComponent::UPZ_DialogueSystemComponent()
{
	bStopWhenOwnerDestroyed = true;
}





void UPZ_DialogueSystemComponent::BeginPlay()
{
	Super::BeginPlay();

	OnAudioFinished.AddDynamic(this, &UPZ_DialogueSystemComponent::FinishSaying);


	if(NickName == nullptr)
	{
		FMessageLog(FName("PIE")).Warning
		(
			FText::FromString("DialogueSystemComponent: NickName default value is None!")
		);
	}
}






bool UPZ_DialogueSystemComponent::JoinToDialog(UPZ_DialogueContext* Dialog)
{
	if( Dialog && GetCanParticipateInDialog(Dialog) )
	{
		return Dialog->TryAddParticipant(GetOwner());
	}

	return false;
}

bool UPZ_DialogueSystemComponent::StartDialogWith(UPZ_DialogueEditor* Dialog, TArray<AActor*> OtherParticipants, UPZ_DialogueContext*& CreatedDialog)
{
	OtherParticipants.Add(GetOwner());
	bool Success = UPZ_DialogueManager::CreateDialog(Dialog, OtherParticipants, CreatedDialog);

	if(!Success)
	{
		if(NickName == nullptr)
		{
			FMessageLog(FName("PIE")).Error
			(
				FText::FromString("DialogueSystemComponent: Can't start dialog. NickName is None!")
			);
		}
	}


	return Success && GetCanParticipateInDialog(CreatedDialog) && CreatedDialog->RestartDialog();
}

bool UPZ_DialogueSystemComponent::LeaveFromDialog()
{
	if( ParticipateInDialog )
	{
		return ParticipateInDialog->TryRemoveParticipantFromRole(MyCurrentRole);
	}

	return false;
}




void UPZ_DialogueSystemComponent::SayPhrase_Implementation(const FText& Phrase, USoundBase* PhraseSound, UPZ_PhrasePropagation* PhrasePropagation)
{
	if( PhraseSound )
	{
		SetSound(PhraseSound);
		Play();
	}
	else
	{
		FinishSaying();
	}
}






void UPZ_DialogueSystemComponent::ChoseNextDialogActions_Implementation(const TArray<UPZ_DialogueRBaseNode*>& InNextActions, TArray<UPZ_DialogueRBaseNode*>& OutNextActions)
{
	for( UPZ_DialogueRBaseNode* PostChosenAction : PostChosedActions )
	{
		if( InNextActions.Contains(PostChosenAction) )
		{
			OutNextActions.Add(PostChosenAction);
		}
	}

	PostChosedActions.Empty();

	//TODO on child
}

void UPZ_DialogueSystemComponent::PostActionChose(const TArray<UPZ_DialogueRBaseNode*>& ChosedActions)
{
	if( !IsValid(ParticipateInDialog) ) return;

	PostChosedActions = ChosedActions;

	ParticipateInDialog->IterateDialog();
}





void UPZ_DialogueSystemComponent::FinishSaying()
{
	if( ParticipateInDialog )
	{
		ParticipateInDialog->ParticipantFinishSaying(MyCurrentRole);
	}
}


void UPZ_DialogueSystemComponent::ShutDownAll()
{
	LeaveFromDialog();
	Stop(); //Stop audio
}





UPZ_DialogueContext* UPZ_DialogueSystemComponent::GetCurrentDialog() const
{
	return ParticipateInDialog;
}

UPZ_DialogueEditor* UPZ_DialogueSystemComponent::GetCurrentDialogEditor() const
{
	if( !IsValid(ParticipateInDialog) ) return nullptr;

	return ParticipateInDialog->GetImplementedDialog();
}

FName UPZ_DialogueSystemComponent::GetCurrentRole() const
{
	return MyCurrentRole;
}
