

#include "Nodes/PZ_DialoguePrioritySortNodes.h"

#include "Nodes/BaseNode/PZ_DialogueRBaseNode.h"
#include "Nodes/EdgePoint/PZ_DialogueREdgeNode.h"




void FPrioritySortNodes::Add(UPZ_DialogueRBaseNode* RNode, EPrioritySortAddType AddType)
{
	if( AddType == EPrioritySortAddType::NotSuccess )
	{
		if( !FoundFirstPriority )
		{
			NodesWithTheHighestPriority.Add(RNode);
		}

		return;
	}



	if( AddType == EPrioritySortAddType::Success )
	{
		SucceededNodes.Add(RNode);


		if( !FoundFirstPriority )
		{
			NodesWithTheHighestPriority.Add(RNode);
			SucceededNodesWithTheHighestPriority.Add(RNode);
		}


		if( UPZ_DialogueREdgeNode* AsEdgePoint = Cast<UPZ_DialogueREdgeNode>(RNode) )
		{
			if( AsEdgePoint->Priority > LHighestPriority )
			{
				FoundFirstPriority = true;
				LHighestPriority = AsEdgePoint->Priority;

				NodesWithTheHighestPriority.Empty();
				NodesWithTheHighestPriority.Add(RNode);

				SucceededNodesWithTheHighestPriority.Empty();
				SucceededNodesWithTheHighestPriority.Add(RNode);
			}
			else if( AsEdgePoint->Priority == LHighestPriority )
			{
				NodesWithTheHighestPriority.Add(RNode);
				SucceededNodesWithTheHighestPriority.Add(RNode);
			}
		}


		return;
	}



	if( AddType == EPrioritySortAddType::NeedToBeChosen )
	{
		if( UPZ_DialogueREdgeNode* AsEdgePoint = Cast<UPZ_DialogueREdgeNode>(RNode) )
		{
			if( NeedToBeChosenPool.Contains(AsEdgePoint->NeedToBeChosenBy) )
			{
				NeedToBeChosenPool[AsEdgePoint->NeedToBeChosenBy].Add(RNode);
			}
			else
			{
				TArray<UPZ_DialogueRBaseNode*> NewRoleArray;
				NewRoleArray.Add(RNode);
				NeedToBeChosenPool.Add(AsEdgePoint->NeedToBeChosenBy, NewRoleArray);
			}
		}

		return;
	}
}