// Fill out your copyright notice in the Description page of Project Settings.


#include "Nodes/EndNode/PZ_DialogueREndNode.h"
#include "PZ_DialogueContext.h"



UPZ_DialogueREndNode::UPZ_DialogueREndNode()
{
}






bool UPZ_DialogueREndNode::Execute_Implementation(UPZ_DialogueContext* DialogueContext)
{
	if( !DialogueContext ) return true;


	DialogueContext->ExecuteEvents(EndEvents);
	DialogueContext->EndDialog();
	if( WillParticipantsAutoLeave ) DialogueContext->CleanParticipants();

	return true;
}
