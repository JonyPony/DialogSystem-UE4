// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PZ_DialogueNodesInfo.h"
#include "Nodes/PZ_DialoguePrioritySortNodes.h"
#include "PZ_DialogueRBaseNode.generated.h"



class UPZ_DialogueContext;




/*
	R - runtime.
	Base class for runtime nodes.
*/  
UCLASS(EditInlineNew, Abstract)
class PZ_DIALOGUESYSTEM_API UPZ_DialogueRBaseNode : public UObject
{
	GENERATED_BODY()


//Constructor
public:

	UPZ_DialogueRBaseNode();




//c++ public methods
public:

#if WITH_EDITORONLY_DATA

	virtual void PrepareToCompile();

#endif



//>>..........................................................................................................<<//


//Blueprint methods
public:

	/*
		@return true if need to break iterating.
	*/ 
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		bool Execute(UPZ_DialogueContext* DialogueContext);
		virtual bool Execute_Implementation(UPZ_DialogueContext* DialogueContext) { return true; }
	/*
		@return true if need to break iterating.
	*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		bool StopExecute(UPZ_DialogueContext* DialogueContext);
		virtual bool StopExecute_Implementation(UPZ_DialogueContext* DialogueContext) { return false; }

	/*
		Select the best node from it's children.
		@return Child node.
	*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, BlueprintPure = false)
		UPZ_DialogueRBaseNode* ChooseNextNode(UPZ_DialogueContext* DialogueContext) const;
		virtual UPZ_DialogueRBaseNode* ChooseNextNode_Implementation(UPZ_DialogueContext* DialogueContext) const;
	/*
		Sort children nodes and fill specific structure.
		@param PrioritySortedNodes - result of sorting.
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure = false)
		void FillPrioritySortedNodes(UPZ_DialogueContext* DialogueContext, FPrioritySortNodes& PrioritySortedNodes) const;


//Blueprint values
public:

	/*
		Index of this node in compiled array.
	*/
	UPROPERTY(BlueprintReadOnly)
		int32 MyIndex = 0;
	/*
		Node type for determining without cast.
	*/
	UPROPERTY(BlueprintReadOnly)
		EDialogueNodeType NodeType;
	/*
		Children of this node.
		They are installed at the linking stage during compilation.
	*/
	UPROPERTY(BlueprintReadOnly)
		TArray<UPZ_DialogueRBaseNode*> NextNodes;
	/*
		Parant nodes(who has link to this node).
		They are installed at the linking stage during compilation.
	*/
	UPROPERTY(BlueprintReadOnly)
		TArray<UPZ_DialogueRBaseNode*> ParentNodes;


//only for editor settings
#if WITH_EDITORONLY_DATA


#endif

};
