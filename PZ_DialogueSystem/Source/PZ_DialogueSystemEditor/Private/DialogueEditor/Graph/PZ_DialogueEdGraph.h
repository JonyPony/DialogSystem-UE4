// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraph.h"
#include "PZ_DialogueEdGraph.generated.h"




class UPZ_DialogueEdNode_Root;

class UPZ_DialogueEditor;
class FPZ_DialogueGraph;




UCLASS()
class UPZ_DialogueEdGraph : public UEdGraph
{
	GENERATED_BODY()


//c++ public methods
public:

	void RebuildGraph();
	void GenerateEndNodesOnLeafs();


//c++ protected methods
protected:

	virtual void NotifyGraphChanged(const FEdGraphEditAction& Action) override;


//c++ private methods
private:

	void CompileNode(UEdGraphNode* Node);
	void LinkCompiledNodes();


	UPZ_DialogueEdNode_Root* FindRoot();
	void MakeConnection(UEdGraphNode* A, UEdGraphNode* B);
	void UniqueArrayNames(TArray<FName>& Names);




//c++ public values
public:

	UPZ_DialogueEditor* DialogueEditor = nullptr;
	FPZ_DialogueGraph* DialogueGraph = nullptr;


//c++ private values
private:

	TArray<UEdGraphNode*> CompilingVisitedNodes;

};
