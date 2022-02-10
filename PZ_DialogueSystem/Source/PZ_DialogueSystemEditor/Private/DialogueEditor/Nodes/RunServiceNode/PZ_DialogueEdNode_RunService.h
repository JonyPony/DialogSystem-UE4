// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "DialogueEditor/Nodes/BaseNode/PZ_DialogueEdNode_Base.h"
#include "PZ_DialogueEdNode_RunService.generated.h"



UCLASS()
class UPZ_DialogueEdNode_RunService : public UPZ_DialogueEdNode_Base
{

	GENERATED_BODY()

//Constructor
public:

	UPZ_DialogueEdNode_RunService();



//c++ public methods
public:

	virtual bool CanDuplicateNode() const override;
	virtual bool CanUserDeleteNode() const override;

	virtual EDialogueNodeType GetNodeType() const override;


	virtual void OnNodeDoubleClicked() override;
	virtual void OnChangedProperties(const FName& PropertyName) const override;

};
