// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PZ_DialogueEvent.h"
#include "PZ_ShowWidget_DE.generated.h"


class UUserWidget;


UCLASS()
class PZ_DIALOGUESYSTEM_API UPZ_ShowWidget_DE : public UPZ_DialogueEvent
{
	GENERATED_BODY()


//Blueprint methods
public:

	virtual void Execute_Implementation(UPZ_DialogueContext* DialogueContext, AActor* ParticipantActor, const FName& RoleName) const override;


//Blueprint values
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UUserWidget> Widget = nullptr;
	
};
