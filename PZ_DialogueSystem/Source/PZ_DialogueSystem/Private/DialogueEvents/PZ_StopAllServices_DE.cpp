// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueEvents/PZ_StopAllServices_DE.h"
#include "PZ_DialogueContext.h"




void UPZ_StopAllServices_DE::Execute_Implementation(UPZ_DialogueContext* DialogueContext, AActor* ParticipantActor, const FName& RoleName) const
{
	DialogueContext->StopAllServices();
}