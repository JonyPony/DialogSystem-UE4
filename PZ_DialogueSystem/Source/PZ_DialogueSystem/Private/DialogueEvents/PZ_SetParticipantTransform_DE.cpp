// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueEvents/PZ_SetParticipantTransform_DE.h"




void UPZ_SetParticipantTransform_DE::Execute_Implementation(UPZ_DialogueContext* DialogueContext, AActor* ParticipantActor, const FName& RoleName) const
{
	if( !IsValid(ParticipantActor) ) return;


	ParticipantActor->SetActorTransform(NewTransform);
}