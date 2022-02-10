// Fill out your copyright notice in the Description page of Project Settings.


#include "PZ_DialogueEdGraphSchema.h"

#include "PZ_DialogueEditor.h"
#include "PZ_GraphDrawingConnectionPolicy.h"
#include "PZ_DialogueEdGraph.h"
#include "PZ_DialogueGraph.h"

#include "DialogueEditor/Nodes/PZ_DialogueNodeInterface.h"
#include "DialogueEditor/PZ_EdNodesInclude.h"
#include "PZ_RNodesInclude.h"


#include "EdGraph/EdGraphPin.h"
#include "Framework/Commands/GenericCommands.h"
#include "GraphEditorActions.h"
#include "ScopedTransaction.h"

#include "ToolMenu.h"
#include "EdGraphNode_Comment.h"




//.....................................FAssetSchemaAction_DialogueGraph_NewNode...............................................//

UEdGraphNode* FAssetSchemaAction_DialogueGraph_NewNode::PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode)
{
	UEdGraphNode* ResultNode = nullptr;

	if( NodeTemplate != nullptr )
	{
		// set outer to be the graph so it doesn't go away
		NodeTemplate->Rename(NULL, (UObject*)ParentGraph, REN_NonTransactional);
		ParentGraph->AddNode(NodeTemplate, true, bSelectNewNode);


		NodeTemplate->CreateNewGuid();
		NodeTemplate->PostPlacedNewNode();
		NodeTemplate->AllocateDefaultPins();
		NodeTemplate->AutowireNewNode(FromPin);

		NodeTemplate->NodePosX = Location.X;
		NodeTemplate->NodePosY = Location.Y;

		NodeTemplate->SetFlags(RF_Transactional);
		NodeTemplate->RNode->SetFlags(RF_Transactional);

		ResultNode = NodeTemplate;
	}

	return ResultNode;
}

UEdGraphNode* FAssetSchemaAction_DialogueGraph_NewNode::PerformAction(UEdGraph* ParentGraph, TArray<UEdGraphPin*>& FromPins, const FVector2D Location, bool bSelectNewNode)
{
	UEdGraphNode* ResultNode = nullptr;

	if( FromPins.Num() > 0 )
	{
		ResultNode = PerformAction(ParentGraph, FromPins[0], Location, bSelectNewNode);

		// Try autowiring the rest of the pins
		for( int32 Index = 1; Index < FromPins.Num(); ++Index )
		{
			ResultNode->AutowireNewNode(FromPins[Index]);
		}
	}
	else
	{
		ResultNode = PerformAction(ParentGraph, NULL, Location, bSelectNewNode);
	}

	return ResultNode;
}

void FAssetSchemaAction_DialogueGraph_NewNode::AddReferencedObjects(FReferenceCollector& Collector)
{
	FEdGraphSchemaAction::AddReferencedObjects(Collector);
	Collector.AddReferencedObject(NodeTemplate);
}

//............................................................................................................................//



//.................................FNewComment_DialogueGraphSchemaAction......................................................//

UEdGraphNode* FNewComment_DialogueGraphSchemaAction::PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode)
{
	UEdGraphNode_Comment* CommentTemplate = NewObject<UEdGraphNode_Comment>();

	FVector2D SpawnLocation = Location;
	FSlateRect Bounds;
	if( UPZ_DialogueEdGraph* LEdGraph = Cast<UPZ_DialogueEdGraph>(ParentGraph) )
	{
		if( LEdGraph->DialogueGraph->GetSelectedNodesBounds(Bounds, 50.0f) )
		{
			CommentTemplate->SetBounds(Bounds);
			SpawnLocation.X = CommentTemplate->NodePosX;
			SpawnLocation.Y = CommentTemplate->NodePosY;
		}
	}


	return FEdGraphSchemaAction_NewNode::SpawnNodeFromTemplate<UEdGraphNode_Comment>(ParentGraph, CommentTemplate, SpawnLocation, bSelectNewNode);
}

//............................................................................................................................//



//................................FAssetSchemaAction_DialogueGraph_NewEdge....................................................//

UEdGraphNode* FAssetSchemaAction_DialogueGraph_NewEdge::PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode)
{
	UEdGraphNode* ResultNode = nullptr;

	if( NodeTemplate != nullptr )
	{
		// set outer to be the graph so it doesn't go away
		NodeTemplate->Rename(NULL, (UObject*)ParentGraph, REN_NonTransactional);
		ParentGraph->AddNode(NodeTemplate, true, bSelectNewNode);

		NodeTemplate->CreateNewGuid();
		NodeTemplate->PostPlacedNewNode();
		NodeTemplate->AllocateDefaultPins();
		NodeTemplate->AutowireNewNode(FromPin);

		NodeTemplate->NodePosX = Location.X;
		NodeTemplate->NodePosY = Location.Y;

		NodeTemplate->SetFlags(RF_Transactional);
		NodeTemplate->REdgeNode->SetFlags(RF_Transactional);

		ResultNode = NodeTemplate;
	}

	return ResultNode;
}

void FAssetSchemaAction_DialogueGraph_NewEdge::AddReferencedObjects(FReferenceCollector& Collector)
{
	FEdGraphSchemaAction::AddReferencedObjects(Collector);
	Collector.AddReferencedObject(NodeTemplate);
}

//............................................................................................................................//




template<class T, class R>
void AddAction(const FText& Category, const FText& Title, const FText& Tooltip, TArray<TSharedPtr<FEdGraphSchemaAction>>& OutActions, UEdGraph* OwnerOfTemporaries)
{
	TSharedPtr<FAssetSchemaAction_DialogueGraph_NewNode> NewAction = TSharedPtr<FAssetSchemaAction_DialogueGraph_NewNode>(new FAssetSchemaAction_DialogueGraph_NewNode(Category, Title, Tooltip, 0));
	NewAction->NodeTemplate = NewObject<T>((UObject*)OwnerOfTemporaries);
	NewAction->NodeTemplate->RNode = NewObject<R>(NewAction->NodeTemplate);

	OutActions.Add(NewAction);
}







void UPZ_DialogueEdGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	TArray<TSharedPtr<FEdGraphSchemaAction>> Actions;

	AddAction<UPZ_DialogueEdNode_End, UPZ_DialogueREndNode>(FText::FromString("Nodes"), FText::FromString("Add End Node"), FText::FromString("Ends dialogue"), Actions, ContextMenuBuilder.OwnerOfTemporaries);
	AddAction<UPZ_DialogueEdNode_Say, UPZ_DialogueRSayNode>(FText::FromString("Nodes"), FText::FromString("Add Say Node"), FText::FromString("Say some phrase"), Actions, ContextMenuBuilder.OwnerOfTemporaries);
	AddAction<UPZ_DialogueEdNode_SendEvent, UPZ_DialogueRSendEventNode>(FText::FromString("Nodes"), FText::FromString("Add Send Event Node"), FText::FromString("Send event to the participant"), Actions, ContextMenuBuilder.OwnerOfTemporaries);
	AddAction<UPZ_DialogueEdNode_Selector, UPZ_DialogueRSelectorNode>(FText::FromString("Nodes"), FText::FromString("Add Selector Node"), FText::FromString("If analog"), Actions, ContextMenuBuilder.OwnerOfTemporaries);
	AddAction<UPZ_DialogueEdNode_Delay, UPZ_DialogueRDelayNode>(FText::FromString("Nodes"), FText::FromString("Add Delay Node"), FText::FromString("Delay n seconds"), Actions, ContextMenuBuilder.OwnerOfTemporaries);
	AddAction<UPZ_DialogueEdNode_RunService, UPZ_DialogueRRunServiceNode>(FText::FromString("Nodes"), FText::FromString("Add Run Service Node"), FText::FromString("Start services"), Actions, ContextMenuBuilder.OwnerOfTemporaries);


	for( TSharedPtr<FEdGraphSchemaAction> Action : Actions )
	{
		ContextMenuBuilder.AddAction(Action);
	}
}

void UPZ_DialogueEdGraphSchema::GetContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const
{
	FToolMenuSection& Section = Menu->AddSection("DialogueGraphSchemaNodeActions", FText::FromString("Node Actions"));


	Section.AddMenuEntry(FGenericCommands::Get().Delete);
	Section.AddMenuEntry(FGenericCommands::Get().Cut);
	Section.AddMenuEntry(FGenericCommands::Get().Copy);
	Section.AddMenuEntry(FGenericCommands::Get().Paste);
	Section.AddMenuEntry(FGenericCommands::Get().Duplicate);
	Section.AddMenuEntry(FGenericCommands::Get().Rename);
	Section.AddMenuEntry(FGenericCommands::Get().SelectAll);

	if( Context->Pin )
	{
		Section.AddMenuEntry(FGraphEditorCommands::Get().BreakPinLinks);
	}
	else
	{
		Section.AddMenuEntry(FGraphEditorCommands::Get().BreakNodeLinks);
	}


	Super::GetContextMenuActions(Menu, Context);
}



void UPZ_DialogueEdGraphSchema::CreateDefaultNodesForGraph(UEdGraph& Graph) const
{
	Super::CreateDefaultNodesForGraph(Graph);

	// This should only be called on empty graphs
	if( Graph.Nodes.Num() > 0 ) return;

	FGraphNodeCreator<UPZ_DialogueEdNode_Root> RootNodeCreator(Graph);
	UPZ_DialogueEdNode_Root* LSpawnedNode = RootNodeCreator.CreateNode();
	LSpawnedNode->RNode = NewObject<UPZ_DialogueRRootNode>(LSpawnedNode);

	LSpawnedNode->SetPosition(0, 0);

	RootNodeCreator.Finalize();
}






const FPinConnectionResponse UPZ_DialogueEdGraphSchema::CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const
{
	if( A == nullptr || B == nullptr )
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, FText::FromString("Invalid pins"));
	}

	// Make sure the pins are not on the same node
	if( A->GetOwningNode() == B->GetOwningNode() )
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, FText::FromString("Both are on the same node"));
	}

	// Compare the directions
	if( (A->Direction == EGPD_Input) && (B->Direction == EGPD_Input) )
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, FText::FromString("Can't connect input node to input node"));
	}
	else 
	if( (A->Direction == EGPD_Output) && (B->Direction == EGPD_Output) )
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, FText::FromString("Can't connect output node to output node"));
	}



	const UPZ_DialogueEdNode_Base* SourceNode = CastChecked<UPZ_DialogueEdNode_Base>(A->GetOwningNode());
	const UPZ_DialogueEdNode_Base* TargetNode = CastChecked<UPZ_DialogueEdNode_Base>(B->GetOwningNode());
	if( !IsValid(SourceNode) || !IsValid(TargetNode) )
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, FText::FromString("Invalid pins"));
	}

	if( SourceNode->HasOutputConnectionToNode(TargetNode) || TargetNode->HasOutputConnectionToNode(SourceNode) )
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, FText::FromString("Connection between nodes already made"));
	}


	FText ErrorMessage;
	bool CreateEdge = false;
	if( SourceNode->CanCreateConnectionTo(TargetNode, ErrorMessage, CreateEdge) )
	{
		if( CreateEdge )
		{
			return FPinConnectionResponse(CONNECT_RESPONSE_MAKE_WITH_CONVERSION_NODE, FText::FromString("Connect nodes with edge"));
		}
		else
		{
			return FPinConnectionResponse(CONNECT_RESPONSE_MAKE, FText::FromString("Connect nodes"));
		}
	}
	else
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, ErrorMessage);
	}
}

bool UPZ_DialogueEdGraphSchema::TryCreateConnection(UEdGraphPin* PinA, UEdGraphPin* PinB) const
{
	if( PinA && PinB )
	{
		return Super::TryCreateConnection(PinA, PinB);
	}

	return false;
}

bool UPZ_DialogueEdGraphSchema::CreateAutomaticConversionNodeAndConnections(UEdGraphPin* PinA, UEdGraphPin* PinB) const
{
	UPZ_DialogueEdNode_Base* NodeA = Cast<UPZ_DialogueEdNode_Base>(PinA->GetOwningNode());
	UPZ_DialogueEdNode_Base* NodeB = Cast<UPZ_DialogueEdNode_Base>(PinB->GetOwningNode());

	if( NodeA == nullptr || NodeB == nullptr ) return false;
	if( NodeA->GetOutputPin() == nullptr || NodeB->GetInputPin() == nullptr ) return false;


	FVector2D InitPos((NodeA->NodePosX + NodeB->NodePosX) / 2, (NodeA->NodePosY + NodeB->NodePosY) / 2);

	FAssetSchemaAction_DialogueGraph_NewEdge Action;
	Action.NodeTemplate = NewObject<UPZ_DialogueEdNode_Edge>(NodeA->GetGraph());
	Action.NodeTemplate->REdgeNode = NewObject<UPZ_DialogueREdgeNode>(Action.NodeTemplate);
	UPZ_DialogueEdNode_Edge* EdgeNode = Cast<UPZ_DialogueEdNode_Edge>(Action.PerformAction(NodeA->GetGraph(), nullptr, InitPos, false));



	if( PinA->Direction == EGPD_Output )
	{
		EdgeNode->CreateConnections(NodeA, NodeB);
	}
	else
	{
		EdgeNode->CreateConnections(NodeB, NodeA);
	}


	return true;
}

FConnectionDrawingPolicy* UPZ_DialogueEdGraphSchema::CreateConnectionDrawingPolicy(int32 InBackLayerID, int32 InFrontLayerID, float InZoomFactor, const FSlateRect& InClippingRect, FSlateWindowElementList& InDrawElements, UEdGraph* InGraphObj) const
{
	return new FPZ_GraphDrawingConnectionPolicy(InBackLayerID, InFrontLayerID, InZoomFactor, InClippingRect, InDrawElements, InGraphObj);
}






FLinearColor UPZ_DialogueEdGraphSchema::GetPinTypeColor(const FEdGraphPinType& PinType) const
{
	return FLinearColor::White;
}

FPinConnectionResponse UPZ_DialogueEdGraphSchema::MovePinLinks(UEdGraphPin& MoveFromPin, UEdGraphPin& MoveToPin, bool bIsIntermediateMove, bool bNotifyLinkedNodes) const
{
	//TODO...
	return FPinConnectionResponse();
}


FPinConnectionResponse UPZ_DialogueEdGraphSchema::CopyPinLinks(UEdGraphPin& CopyFromPin, UEdGraphPin& CopyToPin, bool bIsIntermediateCopy) const
{
	//TODO...
	return FPinConnectionResponse();
}


void UPZ_DialogueEdGraphSchema::BreakNodeLinks(UEdGraphNode& TargetNode) const
{
	const FScopedTransaction Transaction(FText::FromString("BreakNodeLinks"));

	Super::BreakNodeLinks(TargetNode);
}

void UPZ_DialogueEdGraphSchema::BreakPinLinks(UEdGraphPin& TargetPin, bool bSendsNodeNotifcation) const
{
	const FScopedTransaction Transaction(FText::FromString("BreakPinLinks"));

	Super::BreakPinLinks(TargetPin, bSendsNodeNotifcation);
}

void UPZ_DialogueEdGraphSchema::BreakSinglePinLink(UEdGraphPin* SourcePin, UEdGraphPin* TargetPin) const
{
	const FScopedTransaction Transaction(FText::FromString("BreakPinLinks"));

	Super::BreakSinglePinLink(SourcePin, TargetPin);
}


UEdGraphPin* UPZ_DialogueEdGraphSchema::DropPinOnNode(UEdGraphNode* InTargetNode, const FName& InSourcePinName, const FEdGraphPinType& InSourcePinType, EEdGraphPinDirection InSourcePinDirection) const
{
	IPZ_DialogueNodeInterface* LDialogueNode = Cast<IPZ_DialogueNodeInterface>(InTargetNode);

	switch( InSourcePinDirection )
	{
	case EGPD_Input: return LDialogueNode->GetOutputPin();
	case EGPD_Output: return LDialogueNode->GetInputPin();
	default: return nullptr;
	}
}

bool UPZ_DialogueEdGraphSchema::SupportsDropPinOnNode(UEdGraphNode* InTargetNode, const FEdGraphPinType& InSourcePinType, EEdGraphPinDirection InSourcePinDirection, FText& OutErrorMessage) const
{
	//Edges don't support dropping pin on it.
	return Cast<UPZ_DialogueEdNode_Base>(InTargetNode) != nullptr;
}





TSharedPtr<FEdGraphSchemaAction> UPZ_DialogueEdGraphSchema::GetCreateCommentAction() const
{
	return TSharedPtr<FEdGraphSchemaAction>(static_cast<FEdGraphSchemaAction*>(new FNewComment_DialogueGraphSchemaAction));
}





void UPZ_DialogueEdGraphSchema::DroppedAssetsOnGraph(const TArray<FAssetData>& Assets, const FVector2D& GraphPosition, UEdGraph* Graph) const
{
	//TODO...
}

void UPZ_DialogueEdGraphSchema::DroppedAssetsOnNode(const TArray<FAssetData>& Assets, const FVector2D& GraphPosition, UEdGraphNode* Node) const
{
	//TODO...
}
