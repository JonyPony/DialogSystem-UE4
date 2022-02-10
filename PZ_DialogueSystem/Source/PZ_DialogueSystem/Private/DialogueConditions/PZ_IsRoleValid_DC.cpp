// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueConditions/PZ_IsRoleValid_DC.h"
#include "PZ_DialogueContext.h"



bool UPZ_IsRoleValid_DC::Check_Implementation(UPZ_DialogueContext* DialogueContext)
{
	return DialogueContext->CheckParticipantAtRoleValid(RoleName);
}