// Fill out your copyright notice in the Description page of Project Settings.


#include "PZ_DialogueNodesFactory.h"

#include "DialogueEditor/PZ_EdNodesInclude.h"
#include "DialogueEditor/PZ_SNodesInclude.h"

#include "PZ_SDialogueGraphPin.h"


#include "DialogueEditor/Graph/PZ_DialogueEdGraphSchema.h"






TSharedPtr<class SGraphNode> FPZ_DialogueGraphNodeFactory::CreateNode(UEdGraphNode* InNode) const
{
	if( UPZ_DialogueEdNode_Base* LNodeBase = Cast<UPZ_DialogueEdNode_Base>(InNode) )
	{
		if( UPZ_DialogueEdNode_Root* LNodeRoot = Cast<UPZ_DialogueEdNode_Root>(LNodeBase) )
		{
			return SNew(SPZ_DialogueSNode_Root, LNodeRoot);
		}
		if( UPZ_DialogueEdNode_End* LNodeEnd = Cast<UPZ_DialogueEdNode_End>(LNodeBase) )
		{
			return SNew(SPZ_DialogueSNode_End, LNodeEnd);
		}
		if( UPZ_DialogueEdNode_Say* LNodeSay = Cast<UPZ_DialogueEdNode_Say>(LNodeBase) )
		{
			return SNew(SPZ_DialogueSNode_Say, LNodeSay);
		}
		if( UPZ_DialogueEdNode_SendEvent* LNodeSendEvent = Cast<UPZ_DialogueEdNode_SendEvent>(LNodeBase) )
		{
			return SNew(SPZ_DialogueSNode_SendEvent, LNodeSendEvent);
		}
		if( UPZ_DialogueEdNode_Selector* LNodeSelector = Cast<UPZ_DialogueEdNode_Selector>(LNodeBase) )
		{
			return SNew(SPZ_DialogueSNode_Selector, LNodeSelector);
		}
		if( UPZ_DialogueEdNode_Delay* LNodeDelay = Cast<UPZ_DialogueEdNode_Delay>(LNodeBase) )
		{
			return SNew(SPZ_DialogueSNode_Delay, LNodeDelay);
		}
		if( UPZ_DialogueEdNode_RunService* LNodeRunService = Cast<UPZ_DialogueEdNode_RunService>(LNodeBase) )
		{
			return SNew(SPZ_DialogueSNode_RunService, LNodeRunService);
		}


		return SNew(SPZ_DialogueSNode_Base, LNodeBase);
	}
	else 
	if( UPZ_DialogueEdNode_Edge* LNodeEdge = Cast<UPZ_DialogueEdNode_Edge>(InNode) )
	{
		return SNew(SPZ_DialogueSNode_Edge, LNodeEdge);
	}


	return nullptr;
}




TSharedPtr<class SGraphPin> FPZ_DialogueGraphPinFactory::CreatePin(UEdGraphPin* Pin) const
{
	if( Pin->GetSchema()->IsA<UPZ_DialogueEdGraphSchema>() )
	{
		return SNew(SPZ_SDialogueGraphPin, Pin);
	}

	return nullptr;
}
