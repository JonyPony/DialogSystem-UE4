// Fill out your copyright notice in the Description page of Project Settings.


#include "PZ_DialogueEdNode_End.h"




UPZ_DialogueEdNode_End::UPZ_DialogueEdNode_End()
{
	bCanRenameNode = false;
}





void UPZ_DialogueEdNode_End::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, "MultipleNodes", FName("In"), TEXT("In"));
}


bool UPZ_DialogueEdNode_End::CanDuplicateNode() const
{
	return true;
}

bool UPZ_DialogueEdNode_End::CanUserDeleteNode() const
{
	return true;
}


EDialogueNodeType UPZ_DialogueEdNode_End::GetNodeType() const
{
	return EDialogueNodeType::EndNode;
}




void UPZ_DialogueEdNode_End::OnNodeDoubleClicked()
{
	// TODO ....
}

void UPZ_DialogueEdNode_End::OnChangedProperties(const FName& PropertyName) const
{
	Super::OnChangedProperties(PropertyName);

	//TODO....
}


