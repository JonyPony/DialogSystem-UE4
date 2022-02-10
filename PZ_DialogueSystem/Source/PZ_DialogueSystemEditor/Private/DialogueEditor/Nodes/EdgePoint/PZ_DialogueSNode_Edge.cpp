// Fill out your copyright notice in the Description page of Project Settings.


#include "PZ_DialogueSNode_Edge.h"
#include "PZ_DialogueEdNode_Edge.h"
#include "Nodes/EdgePoint/PZ_DialogueREdgeNode.h"

#include "ConnectionDrawingPolicy.h"




void SPZ_DialogueSNode_Edge::Construct(const FArguments& InArgs, UPZ_DialogueEdNode_Edge* InNode)
{
	GraphNode = InNode;
	EdEdgeNode = InNode;
	REdgeNode = Cast<UPZ_DialogueREdgeNode>(InNode->REdgeNode);
	UpdateGraphNode();
}





bool SPZ_DialogueSNode_Edge::RequiresSecondPassLayout() const
{
	return true;
}

void SPZ_DialogueSNode_Edge::PerformSecondPassLayout(const TMap<UObject*, TSharedRef<SNode>>& NodeToWidgetLookup) const
{
	FGeometry StartGeom;
	FGeometry EndGeom;

	UEdGraphNode* Start = EdEdgeNode->GetStartNode();
	UEdGraphNode* End = EdEdgeNode->GetEndNode();
	if (Start && End)
	{
		const TSharedRef<SNode>* pFromWidget = NodeToWidgetLookup.Find(Start);
		const TSharedRef<SNode>* pToWidget = NodeToWidgetLookup.Find(End);
		if (pFromWidget != nullptr && pToWidget != nullptr)
		{
			const TSharedRef<SNode>& FromWidget = *pFromWidget;
			const TSharedRef<SNode>& ToWidget = *pToWidget;

			StartGeom = FGeometry(FVector2D(Start->NodePosX, Start->NodePosY), FVector2D::ZeroVector, FromWidget->GetDesiredSize(), 1.0f);
			EndGeom = FGeometry(FVector2D(End->NodePosX, End->NodePosY), FVector2D::ZeroVector, ToWidget->GetDesiredSize(), 1.0f);
		}
	}

	PositionBetweenTwoNodesWithOffset(StartGeom, EndGeom, 0, 1);
}



void SPZ_DialogueSNode_Edge::UpdateGraphNode()
{
	InputPins.Empty();
	OutputPins.Empty();

	RightNodeBox.Reset();
	LeftNodeBox.Reset();

	this->ContentScale.Bind(this, &SGraphNode::GetContentScale);
	this->GetOrAddSlot(ENodeZone::Center)
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
			[
				SNew(SImage)
				.Image(FEditorStyle::GetBrush("Graph.TransitionNode.ColorSpill"))
				.ColorAndOpacity(this, &SPZ_DialogueSNode_Edge::GetEdgeColor)
			]

			+ SOverlay::Slot()
			[
				SNew(SImage)
				.Image(FEditorStyle::GetBrush("Graph.TransitionNode.Icon"))
			]
		];
}





void SPZ_DialogueSNode_Edge::PositionBetweenTwoNodesWithOffset(const FGeometry& StartGeom, const FGeometry& EndGeom, int32 NodeIndex, int32 MaxNodes) const
{
	check(NodeIndex >= 0);
	check(MaxNodes > 0);

	// Get a reasonable seed point (halfway between the boxes)
	const FVector2D StartCenter = FGeometryHelper::CenterOf(StartGeom);
	const FVector2D EndCenter = FGeometryHelper::CenterOf(EndGeom);
	const FVector2D SeedPoint = (StartCenter + EndCenter) / 2.0f;

	// Find the (approximate) closest points between the two boxes
	const FVector2D StartAnchorPoint = FGeometryHelper::FindClosestPointOnGeom(StartGeom, SeedPoint);
	const FVector2D EndAnchorPoint = FGeometryHelper::FindClosestPointOnGeom(EndGeom, SeedPoint);

	// Position ourselves halfway along the connecting line between the nodes, elevated away perpendicular to the direction of the line
	static constexpr float Height = 30.0f;
	const FVector2D DesiredNodeSize = GetDesiredSize();

	FVector2D DeltaPos(EndAnchorPoint - StartAnchorPoint);
	if (DeltaPos.IsNearlyZero())
	{
		DeltaPos = FVector2D(10.0f, 0.0f);
	}

	const FVector2D Normal = FVector2D(DeltaPos.Y, -DeltaPos.X).GetSafeNormal();
	const FVector2D NewCenter = StartAnchorPoint + (0.5f * DeltaPos) + (Height * Normal);
	const FVector2D DeltaNormal = DeltaPos.GetSafeNormal();

	// TODO do we need this?
	// Calculate node offset in the case of multiple transitions between the same two nodes
	// MultiNodeOffset: the offset where 0 is the centre of the transition, -1 is 1 <size of node>
	// towards the PrevStateNode and +1 is 1 <size of node> towards the NextStateNode.
	static constexpr float MultiNodeSpace = 0.2f; // Space between multiple edge nodes (in units of <size of node> )
	static constexpr float MultiNodeStep = 1.f + MultiNodeSpace; // Step between node centres (Size of node + size of node spacer)

	const float MultiNodeStart = -((MaxNodes - 1) * MultiNodeStep) / 2.f;
	const float MultiNodeOffset = MultiNodeStart + (NodeIndex * MultiNodeStep);

	// Now we need to adjust the new center by the node size, zoom factor and multi node offset
	const FVector2D NewCorner = NewCenter - (0.5f * DesiredNodeSize) + (DeltaNormal * MultiNodeOffset * DesiredNodeSize.Size());

	GraphNode->NodePosX = NewCorner.X;
	GraphNode->NodePosY = NewCorner.Y;
}


FSlateColor SPZ_DialogueSNode_Edge::GetEdgeColor() const
{
	if (REdgeNode->AlwaysTrue)
	{
		return FLinearColor::Green;
	}

	return FLinearColor(0.9f, 0.9f, 0.9f, 1.0f);
}
