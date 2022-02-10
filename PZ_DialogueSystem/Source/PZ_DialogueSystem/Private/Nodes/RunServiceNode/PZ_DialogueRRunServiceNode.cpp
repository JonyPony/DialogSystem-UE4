// Fill out your copyright notice in the Description page of Project Settings.


#include "Nodes/RunServiceNode/PZ_DialogueRRunServiceNode.h"
#include "PZ_DialogueContext.h"






UPZ_DialogueRRunServiceNode::UPZ_DialogueRRunServiceNode()
{
}






bool UPZ_DialogueRRunServiceNode::Execute_Implementation(UPZ_DialogueContext* DialogueContext)
{
	if( !DialogueContext ) return false;

	DialogueContext->RunServices(Services);

	return false;
}

