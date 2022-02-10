// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"
#include "DialogueEditor/Nodes/PZ_DialogueNodeInterface.h"
#include "PZ_DialogueNodesInfo.h"
#include "PZ_DialogueEdNode_Edge.generated.h"



class UPZ_DialogueEditor;

class UPZ_DialogueREdgeNode;



UCLASS()
class UPZ_DialogueEdNode_Edge : public UEdGraphNode,
								public IPZ_DialogueNodeInterface
{
	GENERATED_BODY()


//constructor
public:

	UPZ_DialogueEdNode_Edge();



//c++ public methods
public:

	virtual void AllocateDefaultPins() override;

	virtual void PinConnectionListChanged(UEdGraphPin* Pin) override;

	virtual void PrepareForCopying() override;



	//................DialogueNode Interface.........................//

	virtual EDialogueNodeType GetNodeType() const override;

	virtual UEdGraphPin* GetInputPin() const override;
	virtual UEdGraphPin* GetOutputPin() const override;


	virtual void CompileThisNodeInfo(UPZ_DialogueEditor* DialogueEditor) override;


	virtual void OnNodeDoubleClicked() override;
	virtual void OnChangedProperties(const FName& PropertyName) const override;
	
	//...............................................................//




	UEdGraphNode* GetStartNode();
	UEdGraphNode* GetEndNode();

	void CreateConnections(UEdGraphNode* Start, UEdGraphNode* End);


//>>........................................................................................................................<<//


//Blueprint values
public:

	UPROPERTY(VisibleAnywhere, Instanced)
		UPZ_DialogueREdgeNode* REdgeNode = nullptr;

};
