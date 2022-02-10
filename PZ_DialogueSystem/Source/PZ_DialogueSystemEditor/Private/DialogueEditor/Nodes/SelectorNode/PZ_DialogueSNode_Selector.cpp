// Fill out your copyright notice in the Description page of Project Settings.


#include "PZ_DialogueSNode_Selector.h"
#include "PZ_DialogueEdNode_Selector.h"
#include "Nodes/SelectorNode/PZ_DialogueRSelectorNode.h"

#include "EditorStyleSet.h"
#include "DialogueEditor/PZ_DialogueEditorStyle.h"




void SPZ_DialogueSNode_Selector::Construct(const FArguments& InArgs, UPZ_DialogueEdNode_Selector* InNode)
{
	GraphNode = InNode;
	InNode->SEdNode = this;
	EdSelectorNode = InNode;
	RSelectorNode = Cast<UPZ_DialogueRSelectorNode>(InNode->RNode);

	this->SetCursor(EMouseCursor::CardinalCross);
	UpdateGraphNode();
}





FText SPZ_DialogueSNode_Selector::GetNodeName() const
{
	return FText::FromString("Selector");
}


FLinearColor SPZ_DialogueSNode_Selector::GetNodeTitleColor() const
{
	return FLinearColor(0.6f, 0.6f, 0.6f);
}

FLinearColor SPZ_DialogueSNode_Selector::GetNodeBackgroundColor() const
{
	return FLinearColor(1.0f, 0.35f, 0.0f);
}


const FSlateBrush* SPZ_DialogueSNode_Selector::GetBackgroundImage() const
{
	return FEditorStyle::GetBrush("Graph.StateNode.Body");
}

const FSlateBrush* SPZ_DialogueSNode_Selector::GetNodeImage() const
{
	return FPZ_DialogueEditorStyle::Get()->GetBrush(FPZ_DialogueEditorStyle::NodeIcon_SelectorNodeIcon);
}

const FSlateBrush* SPZ_DialogueSNode_Selector::GetRoleImage() const
{
	return nullptr;
}

const FLinearColor SPZ_DialogueSNode_Selector::GetRoleImageColor() const
{
	return FLinearColor();
}
