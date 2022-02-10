// Fill out your copyright notice in the Description page of Project Settings.


#include "PZ_DialogueEdNode_Delay.h"




UPZ_DialogueEdNode_Delay::UPZ_DialogueEdNode_Delay()
{
	bCanRenameNode = false;
}





bool UPZ_DialogueEdNode_Delay::CanDuplicateNode() const
{
	return true;
}

bool UPZ_DialogueEdNode_Delay::CanUserDeleteNode() const
{
	return true;
}


EDialogueNodeType UPZ_DialogueEdNode_Delay::GetNodeType() const
{
	return EDialogueNodeType::DelayNode;
}




void UPZ_DialogueEdNode_Delay::OnNodeDoubleClicked()
{
	//TODO...
}

void UPZ_DialogueEdNode_Delay::OnChangedProperties(const FName& PropertyName) const
{
	Super::OnChangedProperties(PropertyName);

	//TODO...
}








