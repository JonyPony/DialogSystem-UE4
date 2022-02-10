// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Nodes/BaseNode/PZ_DialogueRBaseNode.h"
#include "PZ_DialogueREdgeNode.generated.h"



class UPZ_DialogueContext;
class UPZ_DialogueCondition;




UCLASS()
class PZ_DIALOGUESYSTEM_API UPZ_DialogueREdgeNode : public UPZ_DialogueRBaseNode
{
	GENERATED_BODY()

//Constructor
public:

	UPZ_DialogueREdgeNode();



//>>..........................................................................................................<<//


//Blueprint methods
public:

	virtual bool Execute_Implementation(UPZ_DialogueContext* DialogueContext) { return false; }
	

	UFUNCTION(BlueprintCallable)
		void AddNextNodeToPriorityChoice(UPZ_DialogueContext* DialogueContext, FPrioritySortNodes& PrioritySortedNodes);



//Blueprint values
public:

	/*
		Is condition always true.
	*/
	UPROPERTY(EditDefaultsOnly)
		bool AlwaysTrue = false;
	/*
		Can child of this node be chosen only once.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		bool CanBeChosenOnlyOnce = false;
	/*
		Priority of condition. Is is important when we have a lot of success conditions in one pool.
	*/
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "1", UIMin = "1"))
		int Priority = 1;
	/* 
		Who controls the condition (all requests are sent at once when selected).
		None - means no one controls and there will be no request.
		Priority will be applied only if the selection is successful otherwise it will be taken up.
		Triggered when Condition is successful.
	*/
	UPROPERTY(EditDefaultsOnly)
		FName NeedToBeChosenBy = "None";
	/*
		If valid then will be used as a main condition.
	*/
	UPROPERTY(EditDefaultsOnly, Instanced)
		UPZ_DialogueCondition* Condition = nullptr;


//only for editor settings
#if WITH_EDITORONLY_DATA


#endif

};
