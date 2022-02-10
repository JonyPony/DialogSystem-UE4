// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PZ_DialogueCondition.generated.h"


class UPZ_DialogueContext;


/*
	Base class of dialog conditions.
*/
UCLASS(Blueprintable, BlueprintType, EditInlineNew, Abstract)
class PZ_DIALOGUESYSTEM_API UPZ_DialogueCondition : public UObject
{
	GENERATED_BODY()


public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "DialogueSystem|DialogueCondition")
		bool Check(UPZ_DialogueContext* DialogueContext);
		virtual bool Check_Implementation(UPZ_DialogueContext* DialogueContext) { return false; }
};
