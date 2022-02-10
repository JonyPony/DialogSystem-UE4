// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PZ_DialogueEvent.generated.h"


class UPZ_DialogueContext;


/*
	Base class for dialog events.
*/
UCLASS(Blueprintable, BlueprintType, EditInlineNew, Abstract)
class PZ_DIALOGUESYSTEM_API UPZ_DialogueEvent : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, BlueprintPure = false, Category = "DialogueSystem|DialogueEvent")
		void Execute(UPZ_DialogueContext* DialogueContext, AActor* ParticipantActor, const FName& RoleName) const;
		virtual void Execute_Implementation(UPZ_DialogueContext* DialogueContext, AActor* ParticipantActor, const FName& RoleName) const {}
	
};
