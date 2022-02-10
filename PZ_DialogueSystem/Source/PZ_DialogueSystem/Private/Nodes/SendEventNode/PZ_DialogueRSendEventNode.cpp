// Fill out your copyright notice in the Description page of Project Settings.


#include "Nodes/SendEventNode/PZ_DialogueRSendEventNode.h"
#include "PZ_DialogueContext.h"



UPZ_DialogueRSendEventNode::UPZ_DialogueRSendEventNode()
{
}






bool UPZ_DialogueRSendEventNode::Execute_Implementation(UPZ_DialogueContext* DialogueContext)
{
	if( !DialogueContext ) return false;


	DialogueContext->ExecuteEvents(SendEvents);

	return false;
}
