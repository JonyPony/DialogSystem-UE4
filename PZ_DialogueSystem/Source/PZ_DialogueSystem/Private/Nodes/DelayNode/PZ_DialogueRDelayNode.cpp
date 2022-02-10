// Fill out your copyright notice in the Description page of Project Settings.


#include "Nodes/DelayNode/PZ_DialogueRDelayNode.h"
#include "PZ_DialogueContext.h"

#include "TimerManager.h"





UPZ_DialogueRDelayNode::UPZ_DialogueRDelayNode()
{
}






bool UPZ_DialogueRDelayNode::Execute_Implementation(UPZ_DialogueContext* DialogueContext)
{
	if( !DialogueContext ) return false;

	DialogueContext->RunServices(DelayServices);

	FTimerDelegate TimerDel;
	FTimerHandle TimerHandle;
	TimerDel.BindUFunction(DialogueContext, FName("IterateDialog"));
	DialogueContext->GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, DelayTime, false);

	return true;
}

bool UPZ_DialogueRDelayNode::StopExecute_Implementation(UPZ_DialogueContext* DialogueContext)
{
	if( !DialogueContext ) return false;

	DialogueContext->StopServices(DelayServices);

	return false;
}
