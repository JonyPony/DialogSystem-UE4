// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogueConditions/PZ_DialogueCondition.h"
#include "PZ_DoesAllPhrasesEnded_DC.generated.h"



UCLASS()
class PZ_DIALOGUESYSTEM_API UPZ_DoesAllPhrasesEnded_DC : public UPZ_DialogueCondition
{
	GENERATED_BODY()

public:

	virtual bool Check_Implementation(UPZ_DialogueContext* DialogueContext) override;

};
