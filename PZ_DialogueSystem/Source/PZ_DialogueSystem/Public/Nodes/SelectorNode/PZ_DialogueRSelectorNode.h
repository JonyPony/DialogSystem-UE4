// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Nodes/BaseNode/PZ_DialogueRBaseNode.h"
#include "PZ_DialogueRSelectorNode.generated.h"





UENUM(BlueprintType)
enum class EChoseNextNodeType : uint8
{
	FullRandom							UMETA(DisplayName = "Full random"),

	FirstSuccess						UMETA(DisplayName = "First success"),
	RandomSuccess						UMETA(DisplayName = "Random success"),

	FirstSuccessWithTheHighestPriority	UMETA(DisplayName = "First success with the highest priority"),
	RandomSuccessWithTheHighestPriority	UMETA(DisplayName = "Random success with the highest priority"),

	FirstFromTheHighestPriority			UMETA(DisplayName = "First from the highest priority"),
	RandomFromTheHighestPriority		UMETA(DisplayName = "Random from the highest priority")
};




UCLASS()
class PZ_DIALOGUESYSTEM_API UPZ_DialogueRSelectorNode : public UPZ_DialogueRBaseNode
{
	GENERATED_BODY()


//Constructor
public:

	UPZ_DialogueRSelectorNode();



//>>............................................................................................................<<//


//Blueprint methods
public:

	virtual bool Execute_Implementation(UPZ_DialogueContext* DialogueContext) override;
	virtual UPZ_DialogueRBaseNode* ChooseNextNode_Implementation(UPZ_DialogueContext* DialogueContext) const override;


//Blueprint values
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		EChoseNextNodeType ChoosingType;



//only for editor settings
#if WITH_EDITORONLY_DATA


#endif

};
