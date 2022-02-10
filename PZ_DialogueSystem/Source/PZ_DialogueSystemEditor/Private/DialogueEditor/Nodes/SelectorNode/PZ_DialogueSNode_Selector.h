// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogueEditor/Nodes/BaseNode/PZ_DialogueSNode_Base.h"


class UPZ_DialogueEdNode_Selector;
class UPZ_DialogueRSelectorNode;



class SPZ_DialogueSNode_Selector : public SPZ_DialogueSNode_Base
{

	typedef SPZ_DialogueSNode_Base Super;
	typedef SPZ_DialogueSNode_Selector Self;


public:

	SLATE_BEGIN_ARGS(SPZ_DialogueSNode_Selector) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UPZ_DialogueEdNode_Selector* InNode);



public:

	virtual FText GetNodeName() const override;

	virtual FLinearColor GetNodeTitleColor()  const override;
	virtual FLinearColor GetNodeBackgroundColor()  const override;

	virtual const FSlateBrush* GetBackgroundImage()  const override;
	virtual const FSlateBrush* GetNodeImage()  const override;
	virtual const FSlateBrush* GetRoleImage() const override;
	virtual const FLinearColor GetRoleImageColor() const override;



public:

	UPZ_DialogueEdNode_Selector* EdSelectorNode = nullptr;
	UPZ_DialogueRSelectorNode* RSelectorNode = nullptr;

};
