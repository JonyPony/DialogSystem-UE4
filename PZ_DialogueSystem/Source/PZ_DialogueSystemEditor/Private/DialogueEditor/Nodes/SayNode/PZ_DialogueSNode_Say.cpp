// Fill out your copyright notice in the Description page of Project Settings.


#include "PZ_DialogueSNode_Say.h"
#include "PZ_DialogueEdNode_Say.h"
#include "Nodes/SayNode/PZ_DialogueRSayNode.h"

#include "EditorStyleSet.h"
#include "DialogueEditor/PZ_DialogueEditorStyle.h"

#include "DialogueServices/PZ_DialogueService.h"




void SPZ_DialogueSNode_Say::Construct(const FArguments& InArgs, UPZ_DialogueEdNode_Say* InNode)
{
	GraphNode = InNode;
	InNode->SEdNode = this;
	EdSayNode = InNode;
	RSayNode = Cast<UPZ_DialogueRSayNode>(InNode->RNode);

	this->SetCursor(EMouseCursor::CardinalCross);
	UpdateGraphNode();
}





FText SPZ_DialogueSNode_Say::GetNodeName() const
{
	return FText::FromString("Say");
}

FText SPZ_DialogueSNode_Say::GetNodeAdditionalText() const
{
	return RSayNode->ShortedPhrase;
}


FLinearColor SPZ_DialogueSNode_Say::GetNodeTitleColor() const
{
	return FLinearColor(0.6f, 0.6f, 0.6f);
}

FLinearColor SPZ_DialogueSNode_Say::GetNodeBackgroundColor() const
{
	return FLinearColor(0.2, 0.2, 0.2);
}


const FSlateBrush* SPZ_DialogueSNode_Say::GetBackgroundImage() const
{
	return FEditorStyle::GetBrush("Graph.StateNode.Body");
}

const FSlateBrush* SPZ_DialogueSNode_Say::GetNodeImage() const
{
	return FPZ_DialogueEditorStyle::Get()->GetBrush(FPZ_DialogueEditorStyle::NodeIcon_SayNodeIcon);
}

const FSlateBrush* SPZ_DialogueSNode_Say::GetRoleImage() const
{
	if (
		!EdSayNode->CheckParticipantsNames(RSayNode->SayToRoles) ||
		!EdSayNode->CheckParticipantName(RSayNode->WhoSay) ||
		RSayNode->SayToRoles.Contains(RSayNode->WhoSay)
		)
	{
		return FPZ_DialogueEditorStyle::Get()->GetBrush("DialogEditor.NodeError");
	}

	for(UPZ_DialogueService* LService : RSayNode->Services)
	{
		if(!IsValid(LService)) return FPZ_DialogueEditorStyle::Get()->GetBrush("DialogEditor.NodeError");
	}


	FRoleVisualization RoleVis;
	EdSayNode->GetParticipantRoleVisualization(RSayNode->WhoSay, RoleVis);
	return EdSayNode->GetVisualizationIcon(RoleVis.RoleTypeVisualization);
}

const FLinearColor SPZ_DialogueSNode_Say::GetRoleImageColor() const
{
	
	FRoleVisualization RoleVis;
	if (
		EdSayNode->CheckParticipantsNames(RSayNode->SayToRoles) &&
		EdSayNode->GetParticipantRoleVisualization(RSayNode->WhoSay, RoleVis) &&
		!RSayNode->SayToRoles.Contains(RSayNode->WhoSay)
		)
	{
		return RoleVis.Color;
	}

	return FLinearColor::White;
}
