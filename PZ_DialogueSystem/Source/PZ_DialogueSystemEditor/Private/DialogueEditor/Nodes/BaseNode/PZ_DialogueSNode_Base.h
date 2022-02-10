// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGraphNode.h"



class UPZ_DialogueEdNode_Base;



class SPZ_DialogueSNode_Base : public SGraphNode
{

	typedef SGraphNode Super;
	typedef SPZ_DialogueSNode_Base Self;


public:

	SLATE_BEGIN_ARGS(SPZ_DialogueSNode_Base) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UPZ_DialogueEdNode_Base* InNode);



public:

	virtual void UpdateGraphNode() override;
	void CreateNodeWidget();
	void CreateCommentBubble();

	virtual void CreatePinWidgets() override;
	virtual TSharedPtr<SGraphPin> CreatePinWidget(UEdGraphPin* Pin) const override;
	virtual void CreateStandardPinWidget(UEdGraphPin* Pin) override;
	virtual void AddPin(const TSharedRef<SGraphPin>& PinToAdd) override;


	virtual bool IsNameReadOnly() const override;



	virtual FText GetNodeName() const;
	virtual FText GetNodeAdditionalText() const;

	virtual FLinearColor GetNodeTitleColor() const;
	virtual FLinearColor GetNodeBackgroundColor() const;

	virtual const FSlateBrush* GetBackgroundImage() const;
	virtual const FSlateBrush* GetNodeImage() const;
	virtual const FSlateBrush* GetRoleImage() const;
	virtual const FLinearColor GetRoleImageColor() const;



protected:

	TSharedPtr<SHorizontalBox> OutputPinBox;
	TSharedPtr<SOverlay> NodeWidget;

};
