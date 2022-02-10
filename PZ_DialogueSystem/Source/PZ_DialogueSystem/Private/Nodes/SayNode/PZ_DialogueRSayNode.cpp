// Fill out your copyright notice in the Description page of Project Settings.


#include "Nodes/SayNode/PZ_DialogueRSayNode.h"
#include "PZ_DialogueContext.h"

#include "DialogueSystemComponent/PZ_DialogueSystemComponent.h"
#include "PhrasePropagetion/PZ_PhrasePropagation.h"






UPZ_DialogueRSayNode::UPZ_DialogueRSayNode()
{
}






bool UPZ_DialogueRSayNode::Execute_Implementation(UPZ_DialogueContext* DialogueContext)
{
	if( !DialogueContext || !DialogueContext->ParticipantStartSaying(WhoSay) ) return false;


	DialogueContext->RunServices(Services);


	FParticipantInstanceInfo LRoleInfo;
	DialogueContext->GetRoleInfo(WhoSay, LRoleInfo);

	LRoleInfo.ParticipantDialogueComponent->SayPhrase(Phrase, PhraseSound, PhrasePropagation);
	LRoleInfo.ParticipantDialogueComponent->IsMyPhraseCanBeSkipped = CanBeSkipped;


	if( PhrasePropagation )
	{
		FCollisionQueryParams QueryParams;
		QueryParams.bTraceComplex = false;
		QueryParams.AddIgnoredActor(LRoleInfo.ParticipantActor);

		TArray<FHitResult> HitResultsAround;
		FVector Location = LRoleInfo.ParticipantActor->GetActorLocation();
		float PropagationRadius = PhrasePropagation->PropagationRadius;
		DialogueContext->GetWorld()->SweepMultiByChannel(HitResultsAround, Location, Location, FQuat::Identity, ECollisionChannel::ECC_Visibility, FCollisionShape::MakeSphere(PropagationRadius), QueryParams);



		bool IsForRandomParticipant = SayOption == EChoseOptionType::ToRandomFromList || SayOption == EChoseOptionType::ToRandomParticipant;
		FName ChosenRoleName;
		if( IsForRandomParticipant )
		{
			if( SayOption == EChoseOptionType::ToRandomFromList )
			{
				ChosenRoleName = SayToRoles[FMath::RandRange(0, SayToRoles.Num() - 1)];
			}
			else
			{
				TArray<FName> ActiveRoles;
				DialogueContext->GetActiveRoles(ActiveRoles);
				ChosenRoleName = ActiveRoles[FMath::RandRange(0, ActiveRoles.Num() - 1)];
			}
		}


		PhrasePropagation->DialogInstigator = DialogueContext;
		for( const auto& LHit : HitResultsAround )
		{
			if( !IsValid(LHit.GetActor()) ) continue;

			if( UPZ_DialogueSystemComponent* LDialogueComponent = LHit.GetActor()->FindComponentByClass<UPZ_DialogueSystemComponent>() )
			{
				bool IsThisPhraseForHim = false;

				if( IsForRandomParticipant )
				{
					IsThisPhraseForHim = LDialogueComponent->GetCurrentRole() == ChosenRoleName;
				}
				else
				{
					switch( SayOption )
					{
					case EChoseOptionType::ToAllFromList:
					{
						IsThisPhraseForHim = SayToRoles.Contains(LDialogueComponent->GetCurrentRole()) && DialogueContext->CheckParticipantAtRoleValid(LDialogueComponent->GetCurrentRole());
						break;
					}

					case EChoseOptionType::ToAllParticipants:
					{
						IsThisPhraseForHim = DialogueContext->CheckParticipantAtRoleValid(LDialogueComponent->GetCurrentRole());
						break;
					}
					}
				}

				LDialogueComponent->HearPhrase(IsThisPhraseForHim, Phrase, PhrasePropagation);
			}
		}
	}


	return false;
}

bool UPZ_DialogueRSayNode::StopExecute_Implementation(UPZ_DialogueContext* DialogueContext)
{
	if( !DialogueContext ) return false;

	DialogueContext->StopServices(Services);

	return false;
}
