// Fill out your copyright notice in the Description page of Project Settings.


#include "PZ_DialogueEdNode_SendEvent.h"




UPZ_DialogueEdNode_SendEvent::UPZ_DialogueEdNode_SendEvent()
{
	bCanRenameNode = false;
}





bool UPZ_DialogueEdNode_SendEvent::CanDuplicateNode() const
{
	return true;
}

bool UPZ_DialogueEdNode_SendEvent::CanUserDeleteNode() const
{
	return true;
}


EDialogueNodeType UPZ_DialogueEdNode_SendEvent::GetNodeType() const
{
	return EDialogueNodeType::SendEventNode;
}




void UPZ_DialogueEdNode_SendEvent::OnNodeDoubleClicked()
{
	//TODO...
}

void UPZ_DialogueEdNode_SendEvent::OnChangedProperties(const FName& PropertyName) const
{
	Super::OnChangedProperties(PropertyName);

	//TODO...
}


