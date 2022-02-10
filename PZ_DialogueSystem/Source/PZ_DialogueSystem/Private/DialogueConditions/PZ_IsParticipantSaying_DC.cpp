// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueConditions/PZ_IsParticipantSaying_DC.h"
#include "PZ_DialogueContext.h"



bool UPZ_IsParticipantSaying_DC::Check_Implementation(UPZ_DialogueContext* DialogueContext)
{
	if(NOT)		return !DialogueContext->IsParticipantSayingNow(RoleName);
	else		return DialogueContext->IsParticipantSayingNow(RoleName);
}