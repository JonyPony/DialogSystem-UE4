// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueServices/PZ_DialogueService.h"
#include "PZ_DialogueContext.h"




UPZ_DialogueService::UPZ_DialogueService()
{
}






void UPZ_DialogueService::BeginService(UPZ_DialogueContext* DialogueContext)
{
	if( !IsValid(DialogueContext) ) return;


	MyDialogueContext = DialogueContext;
	OnServiceBegin(DialogueContext);

	if( UpdateInterval > 0.0f )
	{
		UpdateDelegate.BindUFunction(this, FName("UserUpdate"));
		DialogueContext->GetWorld()->GetTimerManager().SetTimer(UpdateTimerHandle, UpdateDelegate, UpdateInterval, true);
	}
	else
	{
		UpdateDelegate.BindUFunction(this, FName("TickUpdate"));
		DialogueContext->GetWorld()->GetTimerManager().SetTimerForNextTick(UpdateDelegate);
	}
}

void UPZ_DialogueService::EndService(UPZ_DialogueContext* DialogueContext)
{
	if(IsValid(DialogueContext))
	{
		DialogueContext->GetWorld()->GetTimerManager().ClearTimer(UpdateTimerHandle);
		OnServiceEnd(DialogueContext);
	}

	MyDialogueContext = nullptr;
}




void UPZ_DialogueService::StopThisService()
{
	if( !IsValid(MyDialogueContext) ) return;

	MyDialogueContext->StopService(this);
}

UPZ_DialogueContext* UPZ_DialogueService::GetDialogueContext() const
{
	return MyDialogueContext;
}







void UPZ_DialogueService::TickUpdate()
{
	if( !IsValid(MyDialogueContext) ) return;


	MyDialogueContext->GetWorld()->GetTimerManager().SetTimerForNextTick(UpdateDelegate);
	UserUpdate();
}

void UPZ_DialogueService::UserUpdate()
{
	if( !IsValid(MyDialogueContext) || MyDialogueContext->GetDialogueState() == EDialogState::NotStarted)
	{
		StopThisService();
	}
		
	if(MyDialogueContext->GetDialogueState() == EDialogState::Paused) return;
	

	OnServiceUpdate(MyDialogueContext->GetWorld()->DeltaTimeSeconds);
}
