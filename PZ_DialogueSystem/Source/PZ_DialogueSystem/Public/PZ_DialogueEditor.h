// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PZ_DialogueParticipantInfo.h"
#include "PZ_DialogueEditor.generated.h"



class UEdGraph;
class UPZ_DialogueRBaseNode;



/*
	Storage for compiled nodes.
	Asset of dialogue graph.
*/
UCLASS(BlueprintType)
class PZ_DIALOGUESYSTEM_API UPZ_DialogueEditor : public UObject
{
	GENERATED_BODY()



//constructor
public:

	UPZ_DialogueEditor();





//c++ public methods
public:

#if WITH_EDITOR
	void CleanDialog();
#endif





//c++ public values
public:

#if WITH_EDITORONLY_DATA

	UPROPERTY()
		bool NeedToRebuild = false;
	UPROPERTY()
		bool HasError = false;
	UPROPERTY()
		UEdGraph* EdGraph = nullptr;

#endif




//>>..........................................................................................................<<//



//Blueprint methods
public:

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "DialogueEditor")
		bool IsParticipantsInfoValid() const;
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "DialogueEditor")
		bool AreCompiledNodesValid() const;
	

//Blueprint values
public:

	UPROPERTY(EditDefaultsOnly, Category = "DialogueSettings")
		TMap<FName, FParticipantInfo> Participants;

	/*
		0 index always root node.
	*/
	UPROPERTY(BlueprintReadOnly)
		TArray<UPZ_DialogueRBaseNode*> CompiledNodes; 

};





