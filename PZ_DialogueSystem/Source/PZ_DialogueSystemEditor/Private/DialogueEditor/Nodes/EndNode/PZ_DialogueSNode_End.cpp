// Fill out your copyright notice in the Description page of Project Settings.


#include "PZ_DialogueSNode_End.h"
#include "PZ_DialogueEdNode_End.h"
#include "Nodes/EndNode/PZ_DialogueREndNode.h"

#include "EditorStyleSet.h"
#include "DialogueEditor/PZ_DialogueEditorStyle.h"




void SPZ_DialogueSNode_End::Construct(const FArguments& InArgs, UPZ_DialogueEdNode_End* InNode)
{
	GraphNode = InNode;
	InNode->SEdNode = this;
	EdEndNode = InNode;
	REndNode = Cast<UPZ_DialogueREndNode>(InNode->RNode);

	this->SetCursor(EMouseCursor::CardinalCross);
	UpdateGraphNode();
}





FText SPZ_DialogueSNode_End::GetNodeName() const
{
	return FText::FromString("End Dialog");
}


FLinearColor SPZ_DialogueSNode_End::GetNodeTitleColor() const
{
	return FLinearColor(0.6f, 0.6f, 0.6f);
}

FLinearColor SPZ_DialogueSNode_End::GetNodeBackgroundColor() const
{
	return FLinearColor(0.35, 0, 0);
}


const FSlateBrush* SPZ_DialogueSNode_End::GetBackgroundImage() const
{
	return FEditorStyle::GetBrush("Graph.StateNode.Body");
}

const FSlateBrush* SPZ_DialogueSNode_End::GetNodeImage() const
{
	return FPZ_DialogueEditorStyle::Get()->GetBrush(FPZ_DialogueEditorStyle::NodeIcon_EndNodeIcon);
}

const FSlateBrush* SPZ_DialogueSNode_End::GetRoleImage() const
{
	if( REndNode->EndEvents.Num() > 0 )
	{
		for( const auto& LEvent : REndNode->EndEvents )
		{
			if( !EdEndNode->CheckParticipantsNames(LEvent.RoleNames) )
			{
				return FPZ_DialogueEditorStyle::Get()->GetBrush("DialogEditor.NodeError");
			}
		}

		if( REndNode->EndEvents.Num() >= 1 )
		{
			return FPZ_DialogueEditorStyle::Get()->GetBrush("DialogEditor.MultiEvents");
		}
	}

	return nullptr;
}

const FLinearColor SPZ_DialogueSNode_End::GetRoleImageColor() const
{
	return FLinearColor::White;
}
