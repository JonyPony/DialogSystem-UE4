// Fill out your copyright notice in the Description page of Project Settings.


#include "Nodes/RootNode/PZ_DialogueRRootNode.h"
#include "PZ_DialogueContext.h"




UPZ_DialogueRRootNode::UPZ_DialogueRRootNode()
{
}





bool UPZ_DialogueRRootNode::Execute_Implementation(UPZ_DialogueContext* DialogueContext)
{
	if( !DialogueContext ) return false;


	DialogueContext->ExecuteEvents(StartEvents);
	return false;
}
