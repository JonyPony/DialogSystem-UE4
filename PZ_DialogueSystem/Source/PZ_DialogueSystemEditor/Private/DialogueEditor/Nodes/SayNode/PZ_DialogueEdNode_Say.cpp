// Fill out your copyright notice in the Description page of Project Settings.


#include "PZ_DialogueEdNode_Say.h"




UPZ_DialogueEdNode_Say::UPZ_DialogueEdNode_Say()
{
	bCanRenameNode = false;
}





bool UPZ_DialogueEdNode_Say::CanDuplicateNode() const
{
	return true;
}

bool UPZ_DialogueEdNode_Say::CanUserDeleteNode() const
{
	return true;
}


EDialogueNodeType UPZ_DialogueEdNode_Say::GetNodeType() const
{
	return EDialogueNodeType::SayNode;
}




void UPZ_DialogueEdNode_Say::OnNodeDoubleClicked()
{
	//TODO...
}

void UPZ_DialogueEdNode_Say::OnChangedProperties(const FName& PropertyName) const
{
	Super::OnChangedProperties(PropertyName);

	//TODO...
}


