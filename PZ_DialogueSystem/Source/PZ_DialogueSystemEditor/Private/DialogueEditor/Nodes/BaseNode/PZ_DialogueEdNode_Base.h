// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"
#include "DialogueEditor/Nodes/PZ_DialogueNodeInterface.h"
#include "PZ_DialogueNodesInfo.h"
#include "PZ_DialogueParticipantInfo.h"
#include "PZ_DialogueEdNode_Base.generated.h"



class UPZ_DialogueEditor;

class SPZ_DialogueSNode_Base;
class UPZ_DialogueRBaseNode;



UCLASS()
class UPZ_DialogueEdNode_Base :		public UEdGraphNode,
									public IPZ_DialogueNodeInterface
{
	GENERATED_BODY()

//constructor
public:

	UPZ_DialogueEdNode_Base();



//c++ public methods
public:

	virtual void AllocateDefaultPins() override;
	virtual void PrepareForCopying() override;
	virtual void AutowireNewNode(UEdGraphPin* FromPin) override;
	

	//................DialogueNode Interface.........................//

	virtual EDialogueNodeType GetNodeType() const override;

	virtual UEdGraphPin* GetInputPin() const override;
	virtual UEdGraphPin* GetOutputPin() const override;


	virtual void CompileThisNodeInfo(UPZ_DialogueEditor* DialogueEditor) override;


	virtual void OnNodeDoubleClicked() override;
	virtual void OnChangedProperties(const FName& PropertyName) const override;

	//...............................................................//



	//...............Overridable for other nodes.....................//

	virtual bool CanDuplicateNode() const override;
	virtual bool CanUserDeleteNode() const override;
	virtual bool CanSplitPin(const UEdGraphPin* Pin) const override;
	virtual bool CanCreateUnderSpecifiedSchema(const UEdGraphSchema* Schema) const override;
	virtual bool CanCreateConnectionTo(const UPZ_DialogueEdNode_Base* ToNode, FText& ErrorMessage, bool& CreateEdge) const;
	
	//...............................................................//



	
	void SetPosition(int32 X, int32 Y);
	bool HasOutputConnectionToNode(const UEdGraphNode* TargetNode) const;


	const FSlateBrush* GetVisualizationIcon(ERoleTypeVisualization VisualizationType);
	const FSlateBrush* GetChoseOptionTypeIcon(EChoseOptionType ChoseOptionType);
	
	bool CheckParticipantsNames(const TArray<FName>& Names);
	bool CheckParticipantName(const FName& Name);
	bool GetParticipantRoleVisualization(const FName& ParticipantName, struct FRoleVisualization& RoleVisualization);



//c++ public values
public:

	 SPZ_DialogueSNode_Base* SEdNode;


//>>...................................................................................................................................<<//


 //Blueprint values
public:

	UPROPERTY(VisibleAnywhere, Instanced)
		UPZ_DialogueRBaseNode* RNode = nullptr;

};
