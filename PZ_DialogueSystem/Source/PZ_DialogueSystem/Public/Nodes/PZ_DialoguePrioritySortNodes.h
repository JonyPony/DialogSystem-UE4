
#pragma once

#include "CoreMinimal.h"
#include "PZ_DialoguePrioritySortNodes.generated.h"



class UPZ_DialogueRBaseNode;




UENUM(BlueprintType)
enum class EPrioritySortAddType : uint8
{
	NotSuccess		UMETA(DisplayName = "Not success"),
	Success			UMETA(DisplayName = "Success"),
	NeedToBeChosen 	UMETA(DisplayName = "Need to be chosen")
};

USTRUCT(BlueprintType)
struct FPrioritySortNodes
{

	GENERATED_USTRUCT_BODY()


public:

	void Add(UPZ_DialogueRBaseNode* RNode, EPrioritySortAddType AddType);


public:

	/*
		All successful nodes.
	*/
	TArray<UPZ_DialogueRBaseNode*> SucceededNodes;
	/*
		Successful nodes which undecided yet. Key = RoleName.
	*/
	TMap<FName, TArray<UPZ_DialogueRBaseNode*>> NeedToBeChosenPool;
	/*
		All successful and unsuccessful nodes with the highest Priority.
	*/
	TArray<UPZ_DialogueRBaseNode*> NodesWithTheHighestPriority;
	/*
		Only successful nodes with the highest Priority.
	*/
	TArray<UPZ_DialogueRBaseNode*> SucceededNodesWithTheHighestPriority;

private:

	bool FoundFirstPriority = false;
	uint8 LHighestPriority = 0;

};