// Fill out your copyright notice in the Description page of Project Settings.


#include "PZ_DialogueSNode_SendEvent.h"
#include "PZ_DialogueEdNode_SendEvent.h"
#include "Nodes/SendEventNode/PZ_DialogueRSendEventNode.h"

#include "EditorStyleSet.h"
#include "DialogueEditor/PZ_DialogueEditorStyle.h"




void SPZ_DialogueSNode_SendEvent::Construct(const FArguments& InArgs, UPZ_DialogueEdNode_SendEvent* InNode)
{
	GraphNode = InNode;
	InNode->SEdNode = this;
	EdSendEventNode = InNode;
	RSendEventNode = Cast<UPZ_DialogueRSendEventNode>(InNode->RNode);

	this->SetCursor(EMouseCursor::CardinalCross);
	UpdateGraphNode();
}





FText SPZ_DialogueSNode_SendEvent::GetNodeName() const
{
	return FText::FromString("Send Event To");
}


FLinearColor SPZ_DialogueSNode_SendEvent::GetNodeTitleColor() const
{
	return FLinearColor(0.6f, 0.6f, 0.6f);
}

FLinearColor SPZ_DialogueSNode_SendEvent::GetNodeBackgroundColor() const
{
	return FLinearColor(0, 0, 0.35);
}


const FSlateBrush* SPZ_DialogueSNode_SendEvent::GetBackgroundImage() const
{
	return FEditorStyle::GetBrush("Graph.StateNode.Body");
}

const FSlateBrush* SPZ_DialogueSNode_SendEvent::GetNodeImage() const
{
	return FPZ_DialogueEditorStyle::Get()->GetBrush(FPZ_DialogueEditorStyle::NodeIcon_SendEventNodeIcon);
}

const FSlateBrush* SPZ_DialogueSNode_SendEvent::GetRoleImage() const
{
	if( RSendEventNode->SendEvents.Num() > 0 )
	{
		for( const auto& LEvent : RSendEventNode->SendEvents )
		{
			if( !EdSendEventNode->CheckParticipantsNames(LEvent.RoleNames) )
			{
				return FPZ_DialogueEditorStyle::Get()->GetBrush("DialogEditor.NodeError");
			}
		}

		if( RSendEventNode->SendEvents.Num() > 1 )
		{
			return FPZ_DialogueEditorStyle::Get()->GetBrush("DialogEditor.MultiEvents");
		}

		return EdSendEventNode->GetChoseOptionTypeIcon(RSendEventNode->SendEvents[0].SendEventOption);
	}

	return nullptr;
}

const FLinearColor SPZ_DialogueSNode_SendEvent::GetRoleImageColor() const
{
	return FLinearColor::White;
}
