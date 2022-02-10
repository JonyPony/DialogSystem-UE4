// Fill out your copyright notice in the Description page of Project Settings.


#include "PZ_DialogueSNode_Delay.h"
#include "PZ_DialogueEdNode_Delay.h"
#include "Nodes/DelayNode/PZ_DialogueRDelayNode.h"

#include "EditorStyleSet.h"
#include "DialogueEditor/PZ_DialogueEditorStyle.h"

#include "DialogueServices/PZ_DialogueService.h"



void SPZ_DialogueSNode_Delay::Construct(const FArguments& InArgs, UPZ_DialogueEdNode_Delay* InNode)
{
	GraphNode = InNode;
	InNode->SEdNode = this;
	EdDelayNode = InNode;
	RDelayNode = Cast<UPZ_DialogueRDelayNode>(InNode->RNode);

	this->SetCursor(EMouseCursor::CardinalCross);
	UpdateGraphNode();
}





FText SPZ_DialogueSNode_Delay::GetNodeName() const
{
	return FText::FromString("Delay");
}

FText SPZ_DialogueSNode_Delay::GetNodeAdditionalText() const
{
	return FText::FromString("Delay time: " + FString::SanitizeFloat(RDelayNode->DelayTime));
}


FLinearColor SPZ_DialogueSNode_Delay::GetNodeTitleColor() const
{
	return FLinearColor(0.6f, 0.6f, 0.6f);
}

FLinearColor SPZ_DialogueSNode_Delay::GetNodeBackgroundColor() const
{
	return FLinearColor(0, 0.33, 0.1);
}


const FSlateBrush* SPZ_DialogueSNode_Delay::GetBackgroundImage() const
{
	return FEditorStyle::GetBrush("Graph.StateNode.Body");
}

const FSlateBrush* SPZ_DialogueSNode_Delay::GetNodeImage() const
{
	return FPZ_DialogueEditorStyle::Get()->GetBrush(FPZ_DialogueEditorStyle::NodeIcon_DelayNodeIcon);
}

const FSlateBrush* SPZ_DialogueSNode_Delay::GetRoleImage() const
{
	if( RDelayNode->DelayServices.Num() <= 0) return nullptr;

	for(UPZ_DialogueService* LDelayService : RDelayNode->DelayServices)
	{
		if(!IsValid(LDelayService)) return FPZ_DialogueEditorStyle::Get()->GetBrush("DialogEditor.NodeError");
	}

	return FPZ_DialogueEditorStyle::Get()->GetBrush(FPZ_DialogueEditorStyle::NodeIcon_ServiceNodeIcon);
}

const FLinearColor SPZ_DialogueSNode_Delay::GetRoleImageColor() const
{
	return FLinearColor::White;
}
