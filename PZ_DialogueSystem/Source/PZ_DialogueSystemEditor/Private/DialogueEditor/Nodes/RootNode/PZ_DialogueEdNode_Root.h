// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "DialogueEditor/Nodes/BaseNode/PZ_DialogueEdNode_Base.h"
#include "PZ_DialogueEdNode_Root.generated.h"



UCLASS()
class UPZ_DialogueEdNode_Root : public UPZ_DialogueEdNode_Base
{

	GENERATED_BODY()

//Constructor
public:

	UPZ_DialogueEdNode_Root();



//c++ public Methods
public:

	virtual void AllocateDefaultPins() override;

	virtual bool CanDuplicateNode() const override;
	virtual bool CanUserDeleteNode() const override;

	virtual EDialogueNodeType GetNodeType() const override;


	virtual void OnNodeDoubleClicked() override;
	virtual void OnChangedProperties(const FName& PropertyName) const override;

};
