// Fill out your copyright notice in the Description page of Project Settings.


#include "PZ_DialogueEdNode_Root.h"



UPZ_DialogueEdNode_Root::UPZ_DialogueEdNode_Root()
{
	bCanRenameNode = false;
}




void UPZ_DialogueEdNode_Root::AllocateDefaultPins()
{
	CreatePin(EGPD_Output, "MultipleNodes", FName("Out"), TEXT("Out"));
}


bool UPZ_DialogueEdNode_Root::CanDuplicateNode() const
{
	return false;
}

bool UPZ_DialogueEdNode_Root::CanUserDeleteNode() const
{
	return false;
}


EDialogueNodeType UPZ_DialogueEdNode_Root::GetNodeType() const
{
	return EDialogueNodeType::RootNode;
}



void UPZ_DialogueEdNode_Root::OnNodeDoubleClicked()
{
	// TODO....
}

void UPZ_DialogueEdNode_Root::OnChangedProperties(const FName& PropertyName) const
{
	Super::OnChangedProperties(PropertyName);

	// TODO....
}


