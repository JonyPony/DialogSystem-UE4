// Fill out your copyright notice in the Description page of Project Settings.


#include "PZ_DialogueManager.h"
#include "PZ_DialogueContext.h"
#include "PZ_DialogueEditor.h"




UPZ_DialogueContext* UPZ_DialogueManager::CreateDialogContext(UPZ_DialogueEditor* Dialog, AActor* Outer)
{
	if( Outer && Dialog )
	{
		UPZ_DialogueContext* LDialogueContext = NewObject<UPZ_DialogueContext>(Outer, UPZ_DialogueContext::StaticClass());
		LDialogueContext->InitContext(Dialog);
		LDialogueContext->SetFlags(RF_StrongRefOnFrame);

		return LDialogueContext;
	}

	return nullptr;
}



bool UPZ_DialogueManager::CreateDialog(UPZ_DialogueEditor* Dialog, const TArray<AActor*>& Participants, UPZ_DialogueContext*& CreatedDialog)
{
	if( Participants.Num() == 0 ) return false;

	UPZ_DialogueContext* LDialogueContext = CreateDialogContext(Dialog, Participants[0]);
	if( !IsValid(LDialogueContext) ) return false;


	for( AActor* LParticipant : Participants )
	{
		if( !LDialogueContext->TryAddParticipant(LParticipant) )
		{
			LDialogueContext->CleanParticipants();
			return false;
		}
	}

	CreatedDialog = LDialogueContext;
	return true;
}



bool UPZ_DialogueManager::TryCreateDialogWithActorsInRadius(UPZ_DialogueEditor* Dialog, AActor* Outer, FVector Location, float Radius, const TArray<AActor*>& IgnoreActors, UPZ_DialogueContext*& CreatedDialog)
{
	if( !IsValid(Outer) || !IsValid(Dialog) ) return false;


	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = false;
	QueryParams.AddIgnoredActors(IgnoreActors);


	TArray<FHitResult> HitResultsAround;
	Outer->GetWorld()->SweepMultiByChannel(HitResultsAround, Location, Location, FQuat::Identity, ECollisionChannel::ECC_Visibility, FCollisionShape::MakeSphere(Radius), QueryParams);

	if( HitResultsAround.Num() == 0 ) return false;


	UPZ_DialogueContext* LDialogueContext = CreateDialogContext(Dialog, Outer);



	bool OneAdded = false;
	for( const auto& LHit : HitResultsAround )
	{
		if( LHit.GetActor() )
		{
			if( LDialogueContext->TryAddParticipant(LHit.GetActor()) )
			{
				OneAdded = true;
			}
		}
	}
	if( LDialogueContext->TryAddParticipant(Outer) )
	{
		OneAdded = true;
	}


	if( !OneAdded )
	{
		LDialogueContext->ConditionalBeginDestroy();
		LDialogueContext = nullptr;
	}

	CreatedDialog = LDialogueContext;
	return OneAdded;
}
