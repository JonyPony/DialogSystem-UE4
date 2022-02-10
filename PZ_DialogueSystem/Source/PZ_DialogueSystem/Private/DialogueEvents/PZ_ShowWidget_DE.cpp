// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueEvents/PZ_ShowWidget_DE.h"

#include "Blueprint/UserWidget.h"




void UPZ_ShowWidget_DE::Execute_Implementation(UPZ_DialogueContext* DialogueContext, AActor* ParticipantActor, const FName& RoleName) const
{
	if( !IsValid(ParticipantActor) ) return;


	if( auto NewWidget = CreateWidget<UUserWidget>(ParticipantActor->GetWorld(), Widget) )
	{
		NewWidget->AddToViewport();
	}
}