// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PZ_DialogueEvent.h"
#include "PZ_SetParticipantTransform_DE.generated.h"




UCLASS()
class PZ_DIALOGUESYSTEM_API UPZ_SetParticipantTransform_DE : public UPZ_DialogueEvent
{
	GENERATED_BODY()


//Blueprint methods
public:

	virtual void Execute_Implementation(UPZ_DialogueContext* DialogueContext, AActor* ParticipantActor, const FName& RoleName) const override;


//Blueprint values
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FTransform NewTransform;
	
};
