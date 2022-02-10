// Fill out your copyright notice in the Description page of Project Settings.


#include "Nodes/SelectorNode/PZ_DialogueRSelectorNode.h"
#include "PZ_DialogueContext.h"




UPZ_DialogueRSelectorNode::UPZ_DialogueRSelectorNode()
{
}






bool UPZ_DialogueRSelectorNode::Execute_Implementation(UPZ_DialogueContext* DialogueContext)
{
	return false;
}


UPZ_DialogueRBaseNode* UPZ_DialogueRSelectorNode::ChooseNextNode_Implementation(UPZ_DialogueContext* DialogueContext) const
{
	if( !DialogueContext || NextNodes.Num() == 0 ) return nullptr;


	FPrioritySortNodes PrioritySortedNodes;
	FillPrioritySortedNodes(DialogueContext, PrioritySortedNodes);

	UPZ_DialogueRBaseNode* NextNode = nullptr;


	switch( ChoosingType )
	{
	case EChoseNextNodeType::FullRandom:
	{
		NextNode = NextNodes[FMath::RandRange(0, NextNodes.Num() - 1)];
		break;
	}


	case EChoseNextNodeType::FirstSuccess:
	{
		if( PrioritySortedNodes.SucceededNodes.Num() > 0 )
		{
			NextNode = PrioritySortedNodes.SucceededNodes[0];
		}
		break;
	}

	case EChoseNextNodeType::RandomSuccess:
	{
		if( PrioritySortedNodes.SucceededNodes.Num() > 0 )
		{
			NextNode = PrioritySortedNodes.SucceededNodes[FMath::RandRange(0, PrioritySortedNodes.SucceededNodes.Num() - 1)];
		}
		break;
	}


	case EChoseNextNodeType::FirstSuccessWithTheHighestPriority:
	{
		if( PrioritySortedNodes.SucceededNodesWithTheHighestPriority.Num() > 0 )
		{
			NextNode = PrioritySortedNodes.SucceededNodesWithTheHighestPriority[0];
		}
		break;
	}


	case EChoseNextNodeType::RandomSuccessWithTheHighestPriority:
	{
		if( PrioritySortedNodes.SucceededNodesWithTheHighestPriority.Num() > 0 )
		{
			NextNode = PrioritySortedNodes.SucceededNodesWithTheHighestPriority[FMath::RandRange(0, PrioritySortedNodes.SucceededNodesWithTheHighestPriority.Num() - 1)];
		}
		break;
	}


	case EChoseNextNodeType::FirstFromTheHighestPriority:
	{
		if( PrioritySortedNodes.NodesWithTheHighestPriority.Num() > 0 )
		{
			NextNode = PrioritySortedNodes.NodesWithTheHighestPriority[0];
		}
		break;
	}

	case EChoseNextNodeType::RandomFromTheHighestPriority:
	{
		if( PrioritySortedNodes.NodesWithTheHighestPriority.Num() > 0 )
		{
			NextNode = PrioritySortedNodes.NodesWithTheHighestPriority[FMath::RandRange(0, PrioritySortedNodes.NodesWithTheHighestPriority.Num() - 1)];
		}
		break;
	}
	}



	return NextNode;
}
