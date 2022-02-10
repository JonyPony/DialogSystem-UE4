// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogueConditions/PZ_DialogueCondition.h"
#include "PZ_IsRoleValid_DC.generated.h"


UCLASS()
class PZ_DIALOGUESYSTEM_API UPZ_IsRoleValid_DC : public UPZ_DialogueCondition
{
	GENERATED_BODY()


//Blueprint methods
public:

	virtual bool Check_Implementation(UPZ_DialogueContext* DialogueContext) override;

//Blueprint values
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FName RoleName = "None";

};
