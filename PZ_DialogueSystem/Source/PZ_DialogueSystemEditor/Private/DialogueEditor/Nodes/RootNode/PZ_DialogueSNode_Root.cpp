// Fill out your copyright notice in the Description page of Project Settings.


#include "PZ_DialogueSNode_Root.h"
#include "PZ_DialogueEdNode_Root.h"
#include "Nodes/RootNode/PZ_DialogueRRootNode.h"

#include "EditorStyleSet.h"
#include "DialogueEditor/PZ_DialogueEditorStyle.h"




void SPZ_DialogueSNode_Root::Construct(const FArguments& InArgs, UPZ_DialogueEdNode_Root* InNode)
{
	GraphNode = InNode;
	InNode->SEdNode = this;
	EdRootNode = InNode;
	RRootNode = Cast<UPZ_DialogueRRootNode>(InNode->RNode);

	this->SetCursor(EMouseCursor::CardinalCross);
	UpdateGraphNode();
}




FText SPZ_DialogueSNode_Root::GetNodeName() const
{
	return FText::FromString("Root");
}


FLinearColor SPZ_DialogueSNode_Root::GetNodeTitleColor() const
{
	return FLinearColor(0.6f, 0.6f, 0.6f);
}

FLinearColor SPZ_DialogueSNode_Root::GetNodeBackgroundColor() const
{
	return FLinearColor(0, 0.35, 0);
}


const FSlateBrush* SPZ_DialogueSNode_Root::GetBackgroundImage() const
{
	return FEditorStyle::GetBrush("Graph.StateNode.Body");
}

const FSlateBrush* SPZ_DialogueSNode_Root::GetNodeImage() const
{
	return FPZ_DialogueEditorStyle::Get()->GetBrush(FPZ_DialogueEditorStyle::NodeIcon_RootNodeIcon);
}

const FSlateBrush* SPZ_DialogueSNode_Root::GetRoleImage() const
{
	if( RRootNode->StartEvents.Num() > 0 )
	{
		for( const auto& LEvent : RRootNode->StartEvents )
		{
			if( !EdRootNode->CheckParticipantsNames(LEvent.RoleNames) )
			{
				return FPZ_DialogueEditorStyle::Get()->GetBrush("DialogEditor.NodeError");
			}
		}

		if( RRootNode->StartEvents.Num() >= 1 )
		{
			return FPZ_DialogueEditorStyle::Get()->GetBrush("DialogEditor.MultiEvents");
		}
	}

	return nullptr;
}

const FLinearColor SPZ_DialogueSNode_Root::GetRoleImageColor() const
{
	return FLinearColor::White;
}
