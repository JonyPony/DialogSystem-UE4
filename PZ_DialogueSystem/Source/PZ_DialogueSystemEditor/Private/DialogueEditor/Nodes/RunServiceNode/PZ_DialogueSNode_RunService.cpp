// Fill out your copyright notice in the Description page of Project Settings.


#include "PZ_DialogueSNode_RunService.h"
#include "PZ_DialogueEdNode_RunService.h"
#include "Nodes/RunServiceNode/PZ_DialogueRRunServiceNode.h"

#include "EditorStyleSet.h"
#include "DialogueEditor/PZ_DialogueEditorStyle.h"

#include "DialogueServices/PZ_DialogueService.h"




void SPZ_DialogueSNode_RunService::Construct(const FArguments& InArgs, UPZ_DialogueEdNode_RunService* InNode)
{
	GraphNode = InNode;
	InNode->SEdNode = this;
	EdRunServiceNode = InNode;
	RRunServiceNode = Cast<UPZ_DialogueRRunServiceNode>(InNode->RNode);

	this->SetCursor(EMouseCursor::CardinalCross);
	UpdateGraphNode();
}





FText SPZ_DialogueSNode_RunService::GetNodeName() const
{
	return FText::FromString("Run service");
}


FLinearColor SPZ_DialogueSNode_RunService::GetNodeTitleColor() const
{
	return FLinearColor(0.6f, 0.6f, 0.6f);
}

FLinearColor SPZ_DialogueSNode_RunService::GetNodeBackgroundColor() const
{
	return FLinearColor(0, 1.0f, 0.9);
}


const FSlateBrush* SPZ_DialogueSNode_RunService::GetBackgroundImage() const
{
	return FEditorStyle::GetBrush("Graph.StateNode.Body");
}

const FSlateBrush* SPZ_DialogueSNode_RunService::GetNodeImage() const
{
	return FPZ_DialogueEditorStyle::Get()->GetBrush(FPZ_DialogueEditorStyle::NodeIcon_ServiceNodeIcon);
}

const FSlateBrush* SPZ_DialogueSNode_RunService::GetRoleImage() const
{
	
	for(UPZ_DialogueService* LService : RRunServiceNode->Services)
	{
		if(!IsValid(LService)) return FPZ_DialogueEditorStyle::Get()->GetBrush("DialogEditor.NodeError");
	}

	return nullptr;
}

const FLinearColor SPZ_DialogueSNode_RunService::GetRoleImageColor() const
{
	return FLinearColor::White;
}
