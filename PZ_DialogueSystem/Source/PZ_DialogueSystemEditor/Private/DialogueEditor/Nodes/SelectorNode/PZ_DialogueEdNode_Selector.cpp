// Fill out your copyright notice in the Description page of Project Settings.


#include "PZ_DialogueEdNode_Selector.h"




UPZ_DialogueEdNode_Selector::UPZ_DialogueEdNode_Selector()
{
	bCanRenameNode = false;
}





bool UPZ_DialogueEdNode_Selector::CanDuplicateNode() const
{
	return true;
}

bool UPZ_DialogueEdNode_Selector::CanUserDeleteNode() const
{
	return true;
}


EDialogueNodeType UPZ_DialogueEdNode_Selector::GetNodeType() const
{
	return EDialogueNodeType::SelectorNode;
}




void UPZ_DialogueEdNode_Selector::OnNodeDoubleClicked()
{
	//TODO...
}

void UPZ_DialogueEdNode_Selector::OnChangedProperties(const FName& PropertyName) const
{
	Super::OnChangedProperties(PropertyName);

	//TODO...
}


