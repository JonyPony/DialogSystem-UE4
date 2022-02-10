// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueConditions/PZ_DoesAllPhrasesEnded_DC.h"
#include "PZ_DialogueContext.h"



bool UPZ_DoesAllPhrasesEnded_DC::Check_Implementation(UPZ_DialogueContext* DialogueContext)
{
	return !DialogueContext->IsAnybodySaying();
}