// Fill out your copyright notice in the Description page of Project Settings.


#include "Nodes/EdgePoint/PZ_DialogueREdgeNode.h"
#include "PZ_DialogueContext.h"

#include "DialogueConditions/PZ_DialogueCondition.h"





UPZ_DialogueREdgeNode::UPZ_DialogueREdgeNode()
{
}





void UPZ_DialogueREdgeNode::AddNextNodeToPriorityChoice(UPZ_DialogueContext* DialogueContext, FPrioritySortNodes& PrioritySortedNodes)
{
	if( CanBeChosenOnlyOnce && DialogueContext->IsEdgeAlreadyChosen(MyIndex) ) return;


	if( AlwaysTrue || !IsValid(Condition) || Condition->Check(DialogueContext) )
	{
		if( NeedToBeChosenBy.IsNone() )
		{
			PrioritySortedNodes.Add(this, EPrioritySortAddType::Success);
		}
		else
		if( DialogueContext->CheckParticipantAtRoleValid(NeedToBeChosenBy) )
		{
			PrioritySortedNodes.Add(this, EPrioritySortAddType::NeedToBeChosen);
		}
		else
		{
			FMessageLog(FName("PIE")).Error(FText::FromString("DialogueSystem Invalid participant name " + NeedToBeChosenBy.ToString()));
		}
	}
	else
	{
		PrioritySortedNodes.Add(this, EPrioritySortAddType::NotSuccess);
	}
}
