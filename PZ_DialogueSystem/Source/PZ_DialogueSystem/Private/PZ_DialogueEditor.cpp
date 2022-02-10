// Fill out your copyright notice in the Description page of Project Settings.


#include "PZ_DialogueEditor.h"

#include "DialogueSystemComponent/PZ_BaseParticipantNickName.h"

#include "Nodes/BaseNode/PZ_DialogueRBaseNode.h"
#include "Nodes/RootNode/PZ_DialogueRRootNode.h"



UPZ_DialogueEditor::UPZ_DialogueEditor()
{
}






#if WITH_EDITOR

void UPZ_DialogueEditor::CleanDialog()
{
	CompiledNodes.Empty();
}

#endif






bool UPZ_DialogueEditor::IsParticipantsInfoValid() const
{
	for( const auto& LParticipantInfo : Participants )
	{
		if( LParticipantInfo.Key.IsNone() ) return false;

		for( const auto& LRoleCanBeChosenBe : LParticipantInfo.Value.CanBeChosenBy )
		{
			if( LRoleCanBeChosenBe.Key == nullptr ) return false;
		}
	}


	return true;
}

bool UPZ_DialogueEditor::AreCompiledNodesValid() const
{
	for( UPZ_DialogueRBaseNode* LNode : CompiledNodes )
	{
		if( !IsValid(LNode) ) return false;
	}

	if( !CompiledNodes[0]->IsA(UPZ_DialogueRRootNode::StaticClass()) ) return false;


	return true;
}
