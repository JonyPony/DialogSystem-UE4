// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PZ_DialogueNodesInfo.generated.h"



class UPZ_DialogueEvent;




UENUM(BlueprintType)
enum class EDialogueNodeType : uint8
{
	None			UMETA(DisplayName = "None"),
	RootNode		UMETA(DisplayName = "Root Node"),
	EndNode			UMETA(DisplayName = "End Node"), 
	SayNode			UMETA(DisplayName = "Say Node"), 
	SendEventNode	UMETA(DisplayName = "Send Event Node"), 
	SelectorNode	UMETA(DisplayName = "Selector Node"), 
	DelayNode		UMETA(DisplayName = "Delay Node"),
	RunServiceNode	UMETA(DisplayName = "Run Service Node"),
	EdgeNode		UMETA(DisplayName = "Edge Node")
};




UENUM(BlueprintType)
enum class EChoseOptionType : uint8
{
	ToAllParticipants		UMETA(DisplayName = "To all participants"),
	ToAllFromList			UMETA(DisplayName = "To all from list"),
	ToRandomParticipant		UMETA(DisplayName = "To random participant"),
	ToRandomFromList		UMETA(DisplayName = "To random from list")
};

USTRUCT(BlueprintType)
struct PZ_DIALOGUESYSTEM_API FEventInfo
{
	GENERATED_USTRUCT_BODY()

	/*
		Event which will be executed.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced)
		UPZ_DialogueEvent* Event = nullptr;
	/*
		Option to select participants in the dialog to apply the event.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		EChoseOptionType SendEventOption;
	/*
		Names of roles to select. Result will be based on SendEventOption.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TArray<FName> RoleNames;

};






