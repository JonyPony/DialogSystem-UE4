// Fill out your copyright notice in the Description page of Project Settings.


#include "PZ_DialogueEdNode_RunService.h"




UPZ_DialogueEdNode_RunService::UPZ_DialogueEdNode_RunService()
{
	bCanRenameNode = false;
}





bool UPZ_DialogueEdNode_RunService::CanDuplicateNode() const
{
	return true;
}

bool UPZ_DialogueEdNode_RunService::CanUserDeleteNode() const
{
	return true;
}


EDialogueNodeType UPZ_DialogueEdNode_RunService::GetNodeType() const
{
	return EDialogueNodeType::RunServiceNode;
}




void UPZ_DialogueEdNode_RunService::OnNodeDoubleClicked()
{
	//TODO...
}

void UPZ_DialogueEdNode_RunService::OnChangedProperties(const FName& PropertyName) const
{
	Super::OnChangedProperties(PropertyName);

	//TODO...
}








