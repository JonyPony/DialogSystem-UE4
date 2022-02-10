// Fill out your copyright notice in the Description page of Project Settings.


#include "PZ_DialogueEdGraph.h"
#include "PZ_DialogueEditor.h"
#include "PZ_DialogueGraph.h"

#include "DialogueEditor/PZ_EdNodesInclude.h"
#include "PZ_RNodesInclude.h"
#include "DialogueEditor/Nodes/PZ_DialogueNodeInterface.h"





void UPZ_DialogueEdGraph::RebuildGraph()
{
	DialogueEditor->CleanDialog();
	CompilingVisitedNodes.Empty();
	if(DialogueEditor->HasError) return;


	UPZ_DialogueEdNode_Root* RootNode = FindRoot();
	CompileNode(RootNode);
	LinkCompiledNodes();


	DialogueEditor->Participants.ValueSort([](const FParticipantInfo& A, const FParticipantInfo& B) { return A.RolePriority > B.RolePriority; });


	DialogueEditor->NeedToRebuild = false;
}


void UPZ_DialogueEdGraph::CompileNode(UEdGraphNode* Node)
{
	if( !IsValid(Node) || CompilingVisitedNodes.Contains(Node) ) return;


	CompilingVisitedNodes.Add(Node);

	if( IPZ_DialogueNodeInterface* LDialogueNode = Cast<IPZ_DialogueNodeInterface>(Node) )
	{
		LDialogueNode->CompileThisNodeInfo(DialogueEditor);

		if( LDialogueNode->GetOutputPin() == nullptr ) return;
		for( UEdGraphPin* LPin : LDialogueNode->GetOutputPin()->LinkedTo )
		{
			CompileNode(LPin->GetOwningNode());
		}
	}
}

void UPZ_DialogueEdGraph::LinkCompiledNodes()
{
	for( int i = 0; i < CompilingVisitedNodes.Num(); ++i )
	{
		if( IPZ_DialogueNodeInterface* LDialogueNode = Cast<IPZ_DialogueNodeInterface>(CompilingVisitedNodes[i]) )
		{
			if( LDialogueNode->GetOutputPin() == nullptr ) continue;
			for( UEdGraphPin* LPin : LDialogueNode->GetOutputPin()->LinkedTo )
			{
				int32 LinkIndex = -1;
				CompilingVisitedNodes.Find(LPin->GetOwningNode(), LinkIndex);

				DialogueEditor->CompiledNodes[i]->NextNodes.Add(DialogueEditor->CompiledNodes[LinkIndex]);
				DialogueEditor->CompiledNodes[LinkIndex]->ParentNodes.Add(DialogueEditor->CompiledNodes[i]);
			}
		}
	}
}





void UPZ_DialogueEdGraph::GenerateEndNodesOnLeafs()
{
	int NodesCount = Nodes.Num();
	for( int i = 0; i < NodesCount; ++i )
	{
		UEdGraphNode* LNode = Nodes[i];

		if( IPZ_DialogueNodeInterface* LDialogueNode = Cast<IPZ_DialogueNodeInterface>(LNode) )
		{
			if( LDialogueNode->GetOutputPin() == nullptr ) continue;
			if( LDialogueNode->GetOutputPin()->LinkedTo.Num() == 0 )
			{
				FGraphNodeCreator<UPZ_DialogueEdNode_End> EndNodeCreator(*this);
				UPZ_DialogueEdNode_End* LSpawnedNode = EndNodeCreator.CreateNode();
				LSpawnedNode->RNode = NewObject<UPZ_DialogueREndNode>(LSpawnedNode);

				LSpawnedNode->SetPosition(LNode->NodePosX, LNode->NodePosY + 160);

				EndNodeCreator.Finalize();



				FGraphNodeCreator<UPZ_DialogueEdNode_Edge> EdgeNodeCreator(*this);
				UPZ_DialogueEdNode_Edge* LEdgeNode = EdgeNodeCreator.CreateNode();
				LEdgeNode->REdgeNode = NewObject<UPZ_DialogueREdgeNode>(LEdgeNode);
				EdgeNodeCreator.Finalize();


				LEdgeNode->CreateConnections(LNode, LSpawnedNode);
			}
		}
	}
}







void UPZ_DialogueEdGraph::NotifyGraphChanged(const FEdGraphEditAction& Action)
{
	Super::NotifyGraphChanged(Action);

	if( !IsValid(DialogueEditor) || DialogueGraph == nullptr ) return;


	DialogueEditor->NeedToRebuild = true;
	DialogueGraph->RegenerateMenusAndToolbars();
}





UPZ_DialogueEdNode_Root* UPZ_DialogueEdGraph::FindRoot()
{
	TArray<UPZ_DialogueEdNode_Root*> RootNodeList;
	GetNodesOfClass<UPZ_DialogueEdNode_Root>(RootNodeList);
	if( RootNodeList.Num() > 0 )
	{
		return RootNodeList[0];
	}

	return nullptr;
}


void UPZ_DialogueEdGraph::MakeConnection(UEdGraphNode* A, UEdGraphNode* B)
{
	IPZ_DialogueNodeInterface* LDialogueNodeA = Cast<IPZ_DialogueNodeInterface>(A);
	IPZ_DialogueNodeInterface* LDialogueNodeB = Cast<IPZ_DialogueNodeInterface>(B);

	if( LDialogueNodeA->GetOutputPin() == nullptr || LDialogueNodeB->GetInputPin() == nullptr ) return;


	if( LDialogueNodeA && LDialogueNodeB )
	{
		LDialogueNodeA->GetOutputPin()->MakeLinkTo(LDialogueNodeB->GetInputPin());
		LDialogueNodeB->GetInputPin()->MakeLinkTo(LDialogueNodeA->GetOutputPin());
	}
}


void UPZ_DialogueEdGraph::UniqueArrayNames(TArray<FName>& Names)
{
	TArray<FName> LUniqueNames;
	for( const FName LName : Names )
	{
		LUniqueNames.AddUnique(LName);
	}
	Names = LUniqueNames;
}
