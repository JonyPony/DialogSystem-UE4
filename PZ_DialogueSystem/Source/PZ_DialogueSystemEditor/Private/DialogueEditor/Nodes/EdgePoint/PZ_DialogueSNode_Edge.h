// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGraphNode.h"


class UPZ_DialogueEdNode_Edge;
class UPZ_DialogueREdgeNode;



class SPZ_DialogueSNode_Edge : public SGraphNode
{
	typedef SGraphNode Super;
	typedef SPZ_DialogueSNode_Edge Self;


public:

	SLATE_BEGIN_ARGS(SPZ_DialogueSNode_Edge) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UPZ_DialogueEdNode_Edge* InNode);



public:

	virtual bool RequiresSecondPassLayout() const override;
	virtual void PerformSecondPassLayout(const TMap< UObject*, TSharedRef<SNode> >& NodeToWidgetLookup) const override;

	virtual void UpdateGraphNode() override;



	/*
		Calculate position for multiple nodes to be placed between a start and end point, by providing this nodes index and max expected nodes.
	*/
	void PositionBetweenTwoNodesWithOffset(const FGeometry& StartGeom, const FGeometry& EndGeom, int32 NodeIndex, int32 MaxNodes) const;

	FSlateColor GetEdgeColor() const;



public:

	UPZ_DialogueEdNode_Edge* EdEdgeNode = nullptr;
	UPZ_DialogueREdgeNode* REdgeNode = nullptr;

};
