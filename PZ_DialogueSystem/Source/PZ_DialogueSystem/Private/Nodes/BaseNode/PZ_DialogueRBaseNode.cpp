// Fill out your copyright notice in the Description page of Project Settings.


#include "Nodes/BaseNode/PZ_DialogueRBaseNode.h"

#include "Nodes/EdgePoint/PZ_DialogueREdgeNode.h"
#include "PZ_DialogueContext.h"




UPZ_DialogueRBaseNode::UPZ_DialogueRBaseNode()
{
}







UPZ_DialogueRBaseNode* UPZ_DialogueRBaseNode::ChooseNextNode_Implementation(UPZ_DialogueContext* DialogueContext) const
{
	if( !DialogueContext || NextNodes.Num() == 0 ) return nullptr;


	FPrioritySortNodes PrioritySortedNodes;
	FillPrioritySortedNodes(DialogueContext, PrioritySortedNodes);

	UPZ_DialogueRBaseNode* NextNode = nullptr;
	if( PrioritySortedNodes.SucceededNodesWithTheHighestPriority.Num() > 0 )
	{
		NextNode = PrioritySortedNodes.SucceededNodesWithTheHighestPriority[0];
	}

	return NextNode;
}



void UPZ_DialogueRBaseNode::FillPrioritySortedNodes(UPZ_DialogueContext* DialogueContext, FPrioritySortNodes& PrioritySortedNodes) const
{
	if( !DialogueContext ) return;


	for( UPZ_DialogueRBaseNode* ChildNode : NextNodes )
	{
		if( UPZ_DialogueREdgeNode* AsEdgePoint = Cast<UPZ_DialogueREdgeNode>(ChildNode) )
		{
			AsEdgePoint->AddNextNodeToPriorityChoice(DialogueContext, PrioritySortedNodes);
		}
		else
		{
			PrioritySortedNodes.Add(ChildNode, EPrioritySortAddType::Success);
		}
	}


	for( const auto& LPool : PrioritySortedNodes.NeedToBeChosenPool )
	{
		TArray<UPZ_DialogueRBaseNode*> LChosenNodes;
		DialogueContext->RequestParticipantChoseNextActions(LPool.Key, LPool.Value, LChosenNodes);

		for( UPZ_DialogueRBaseNode* LChosenNode : LChosenNodes )
		{
			PrioritySortedNodes.Add(LChosenNode, EPrioritySortAddType::Success);
		}

		for( UPZ_DialogueRBaseNode* LNotChosenNode : LPool.Value )
		{
			if( !LChosenNodes.Contains(LNotChosenNode) )
			{
				PrioritySortedNodes.Add(LNotChosenNode, EPrioritySortAddType::NotSuccess);
			}
		}
	}
}






#if WITH_EDITORONLY_DATA

void UPZ_DialogueRBaseNode::PrepareToCompile()
{
	NextNodes.Empty();
	ParentNodes.Empty();
}

#endif


