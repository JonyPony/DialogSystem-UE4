// Fill out your copyright notice in the Description page of Project Settings.


#include "PZ_DialogueEdNode_Base.h"
#include "PZ_DialogueSNode_Base.h"
#include "Nodes/BaseNode/PZ_DialogueRBaseNode.h"

#include "DialogueEditor/Graph/PZ_DialogueEdGraph.h"
#include "PZ_DialogueEditor.h"

#include "DialogueEditor/PZ_DialogueEditorStyle.h"





UPZ_DialogueEdNode_Base::UPZ_DialogueEdNode_Base()
{
	bCanRenameNode = false;
}





void UPZ_DialogueEdNode_Base::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, "MultipleNodes", FName("In"), TEXT("In"));
	CreatePin(EGPD_Output, "MultipleNodes", FName("Out"), TEXT("Out"));
}


void UPZ_DialogueEdNode_Base::PrepareForCopying()
{
	Super::PrepareForCopying();

	RNode->Rename(nullptr, this, REN_DontCreateRedirectors | REN_DoNotDirty);
}


void UPZ_DialogueEdNode_Base::AutowireNewNode(UEdGraphPin* FromPin)
{
	Super::AutowireNewNode(FromPin);

	if( FromPin )
	{
		if( GetSchema()->TryCreateConnection(FromPin, GetInputPin()) )
		{
			FromPin->GetOwningNode()->NodeConnectionListChanged();
		}
	}
}




//....................................DialogueNode Interface.............................................//

EDialogueNodeType UPZ_DialogueEdNode_Base::GetNodeType() const
{
	return EDialogueNodeType::None;
}


UEdGraphPin* UPZ_DialogueEdNode_Base::GetInputPin() const
{
	for( UEdGraphPin* Pin : Pins )
	{
		if( Pin->Direction == EEdGraphPinDirection::EGPD_Input )
		{
			return Pin;
		}
	}

	return nullptr;
}

UEdGraphPin* UPZ_DialogueEdNode_Base::GetOutputPin() const
{
	for( UEdGraphPin* Pin : Pins )
	{
		if( Pin->Direction == EEdGraphPinDirection::EGPD_Output )
		{
			return Pin;
		}
	}

	return nullptr;
}


void UPZ_DialogueEdNode_Base::CompileThisNodeInfo(UPZ_DialogueEditor* DialogueEditor)
{
	if( DialogueEditor )
	{
		int32 NodeIndex = DialogueEditor->CompiledNodes.Add(RNode);
		if( RNode )
		{
			RNode->PrepareToCompile();
			RNode->MyIndex = NodeIndex;
			RNode->NodeType = GetNodeType();
			RNode->Rename(nullptr, DialogueEditor, REN_DontCreateRedirectors | REN_DoNotDirty);
		}
	}
}


void UPZ_DialogueEdNode_Base::OnNodeDoubleClicked()
{
	// TODO on childs
}

void UPZ_DialogueEdNode_Base::OnChangedProperties(const FName& PropertyName) const
{
	SEdNode->UpdateGraphNode();
}

//.......................................................................................................//



bool UPZ_DialogueEdNode_Base::CanDuplicateNode() const
{
	return true;
}

bool UPZ_DialogueEdNode_Base::CanUserDeleteNode() const
{
	return true;
}

bool UPZ_DialogueEdNode_Base::CanSplitPin(const UEdGraphPin* Pin) const
{
	return false;
}

bool UPZ_DialogueEdNode_Base::CanCreateUnderSpecifiedSchema(const UEdGraphSchema* Schema) const
{
	return true;
}

bool UPZ_DialogueEdNode_Base::CanCreateConnectionTo(const UPZ_DialogueEdNode_Base* ToNode, FText& ErrorMessage, bool& CreateEdge) const
{
	CreateEdge = true;
	return true;
}






void UPZ_DialogueEdNode_Base::SetPosition(int32 X, int32 Y)
{
	NodePosX = X;
	NodePosY = Y;
}

bool UPZ_DialogueEdNode_Base::HasOutputConnectionToNode(const UEdGraphNode* TargetNode) const
{
	if( GetOutputPin() == nullptr ) return false;

	for( UEdGraphPin* ChildInputPin : GetOutputPin()->LinkedTo )
	{
		if( ChildInputPin->GetOwningNode() == TargetNode )
		{
			return true;
		}
	}

	return false;
}





const FSlateBrush* UPZ_DialogueEdNode_Base::GetVisualizationIcon(ERoleTypeVisualization VisualizationType)
{
	switch( VisualizationType )
	{
	case ERoleTypeVisualization::None:			return nullptr;
	case ERoleTypeVisualization::Dwarf:			return FPZ_DialogueEditorStyle::Get()->GetBrush("DialogEditor.DwarfIcon");
	case ERoleTypeVisualization::Character:		return FPZ_DialogueEditorStyle::Get()->GetBrush("DialogEditor.CharacterIcon");
	case ERoleTypeVisualization::Viking:		return FPZ_DialogueEditorStyle::Get()->GetBrush("DialogEditor.VikingIcon");
	case ERoleTypeVisualization::VikingHelmet:	return FPZ_DialogueEditorStyle::Get()->GetBrush("DialogEditor.VikingHelmetIcon");
	case ERoleTypeVisualization::Skeleton:		return FPZ_DialogueEditorStyle::Get()->GetBrush("DialogEditor.SkeletonIcon");
	case ERoleTypeVisualization::Orc:			return FPZ_DialogueEditorStyle::Get()->GetBrush("DialogEditor.OrcIcon");
	case ERoleTypeVisualization::SpartanHelmet: return FPZ_DialogueEditorStyle::Get()->GetBrush("DialogEditor.SpartanHelmetIcon");
	case ERoleTypeVisualization::Vampire:		return FPZ_DialogueEditorStyle::Get()->GetBrush("DialogEditor.VampireIcon");
	case ERoleTypeVisualization::Overlord:		return FPZ_DialogueEditorStyle::Get()->GetBrush("DialogEditor.OverlordIcon");
	case ERoleTypeVisualization::Wolf:			return FPZ_DialogueEditorStyle::Get()->GetBrush("DialogEditor.WolfIcon");
	case ERoleTypeVisualization::Warrior:		return FPZ_DialogueEditorStyle::Get()->GetBrush("DialogEditor.WarriorIcon");
	case ERoleTypeVisualization::MountedKnight: return FPZ_DialogueEditorStyle::Get()->GetBrush("DialogEditor.MountedKnightIcon");
	case ERoleTypeVisualization::Farmer:		return FPZ_DialogueEditorStyle::Get()->GetBrush("DialogEditor.FarmerIcon");
	case ERoleTypeVisualization::Caesar:		return FPZ_DialogueEditorStyle::Get()->GetBrush("DialogEditor.CaesarIcon");
	case ERoleTypeVisualization::DaggerRose:	return FPZ_DialogueEditorStyle::Get()->GetBrush("DialogEditor.DaggerRoseIcon");
	case ERoleTypeVisualization::Rogue:			return FPZ_DialogueEditorStyle::Get()->GetBrush("DialogEditor.RogueIcon");
	case ERoleTypeVisualization::Transfuse:		return FPZ_DialogueEditorStyle::Get()->GetBrush("DialogEditor.TransfuseIcon");

	default: return nullptr;
	}
}

const FSlateBrush* UPZ_DialogueEdNode_Base::GetChoseOptionTypeIcon(EChoseOptionType ChoseOptionType)
{
	switch( ChoseOptionType )
	{
	case EChoseOptionType::ToAllParticipants:	return FPZ_DialogueEditorStyle::Get()->GetBrush("DialogEditor.Crowd");
	case EChoseOptionType::ToAllFromList:		return FPZ_DialogueEditorStyle::Get()->GetBrush("DialogEditor.Crowd");
	case EChoseOptionType::ToRandomFromList:	return FPZ_DialogueEditorStyle::Get()->GetBrush("DialogEditor.Random");
	case EChoseOptionType::ToRandomParticipant: return FPZ_DialogueEditorStyle::Get()->GetBrush("DialogEditor.Random");

	default: return nullptr;
	}
}





bool UPZ_DialogueEdNode_Base::CheckParticipantsNames(const TArray<FName>& Names)
{
	for( const FName& LName : Names )
	{
		if( !CheckParticipantName(LName) )
		{
			return false;
		}
	}

	return true;
}

bool UPZ_DialogueEdNode_Base::CheckParticipantName(const FName& Name)
{
	if( UPZ_DialogueEdGraph* LEdGraph = Cast<UPZ_DialogueEdGraph>(GetGraph()) )
	{
		return LEdGraph->DialogueEditor->Participants.Contains(Name);
	}

	return false;
}

bool UPZ_DialogueEdNode_Base::GetParticipantRoleVisualization(const FName& ParticipantName, FRoleVisualization& RoleVisualization)
{
	if( UPZ_DialogueEdGraph* LEdGraph = Cast<UPZ_DialogueEdGraph>(GetGraph()) )
	{
		if( LEdGraph->DialogueEditor->Participants.Contains(ParticipantName) )
		{
			RoleVisualization = LEdGraph->DialogueEditor->Participants[ParticipantName].RoleVisualization;
			return true;
		}
	}

	return false;
}
