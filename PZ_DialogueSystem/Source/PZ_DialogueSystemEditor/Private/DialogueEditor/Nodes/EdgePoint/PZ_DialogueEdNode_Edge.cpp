// Fill out your copyright notice in the Description page of Project Settings.


#include "PZ_DialogueEdNode_Edge.h"
#include "Nodes/EdgePoint/PZ_DialogueREdgeNode.h"

#include "PZ_DialogueEditor.h"





UPZ_DialogueEdNode_Edge::UPZ_DialogueEdNode_Edge()
{
	bCanRenameNode = false;
}





void UPZ_DialogueEdNode_Edge::AllocateDefaultPins()
{
	UEdGraphPin* Inputs = CreatePin(EGPD_Input, TEXT("Edge"), FName("In"), TEXT("In"));
	Inputs->bHidden = true;

	UEdGraphPin* Outputs = CreatePin(EGPD_Output, TEXT("Edge"), FName("Out"), TEXT("Out"));
	Outputs->bHidden = true;
}


void UPZ_DialogueEdNode_Edge::PinConnectionListChanged(UEdGraphPin* Pin)
{
	if( Pin->LinkedTo.Num() == 0 )
	{
		// Commit suicide; transitions must always have an input and output connection
		Modify();

		// Our parent graph will have our graph in SubGraphs so needs to be modified to record that.
		if( UEdGraph* ParentGraph = GetGraph() )
		{
			ParentGraph->Modify();
		}


		DestroyNode();
	}
}


void UPZ_DialogueEdNode_Edge::PrepareForCopying()
{
	Super::PrepareForCopying();

	REdgeNode->Rename(nullptr, this, REN_DontCreateRedirectors | REN_DoNotDirty);
}




//....................................DialogueNode Interface.............................................//

EDialogueNodeType UPZ_DialogueEdNode_Edge::GetNodeType() const
{
	return EDialogueNodeType::EdgeNode;
}


UEdGraphPin* UPZ_DialogueEdNode_Edge::GetInputPin() const
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

UEdGraphPin* UPZ_DialogueEdNode_Edge::GetOutputPin() const
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


void UPZ_DialogueEdNode_Edge::CompileThisNodeInfo(UPZ_DialogueEditor* DialogueEditor)
{
	if( DialogueEditor )
	{
		int32 NodeIndex = DialogueEditor->CompiledNodes.Add(REdgeNode);
		if( REdgeNode )
		{
			REdgeNode->PrepareToCompile();
			REdgeNode->MyIndex = NodeIndex;
			REdgeNode->NodeType = GetNodeType();
			REdgeNode->Rename(nullptr, DialogueEditor, REN_DontCreateRedirectors | REN_DoNotDirty);
		}
	}
}


void UPZ_DialogueEdNode_Edge::OnNodeDoubleClicked()
{
	// TODO ....
}

void UPZ_DialogueEdNode_Edge::OnChangedProperties(const FName& PropertyName) const
{
	// TODO ....
}

//.......................................................................................................//






UEdGraphNode* UPZ_DialogueEdNode_Edge::GetStartNode()
{
	if( GetInputPin()->LinkedTo.Num() > 0 )
	{
		return GetInputPin()->LinkedTo[0]->GetOwningNode();
	}

	return nullptr;
}

UEdGraphNode* UPZ_DialogueEdNode_Edge::GetEndNode()
{
	if( GetOutputPin()->LinkedTo.Num() > 0 )
	{
		return GetOutputPin()->LinkedTo[0]->GetOwningNode();
	}

	return nullptr;
}


void UPZ_DialogueEdNode_Edge::CreateConnections(UEdGraphNode* Start, UEdGraphNode* End)
{
	if( !IsValid(Start) || !IsValid(End) ) return;


	IPZ_DialogueNodeInterface* LStartInterface = Cast<IPZ_DialogueNodeInterface>(Start);
	IPZ_DialogueNodeInterface* LEndInterface = Cast<IPZ_DialogueNodeInterface>(End);

	if( LStartInterface == nullptr || LEndInterface == nullptr ) return;

	UEdGraphPin* StartOutputPin = LStartInterface->GetOutputPin();
	UEdGraphPin* EndInputPin = LEndInterface->GetInputPin();

	if( StartOutputPin == nullptr || EndInputPin == nullptr ) return;



	UEdGraphPin* ThisInputPin = GetInputPin();
	UEdGraphPin* ThisOutputPin = GetOutputPin();

	ThisInputPin->Modify();
	ThisInputPin->LinkedTo.Empty();
	ThisOutputPin->Modify();
	ThisOutputPin->LinkedTo.Empty();



	StartOutputPin->Modify();
	ThisInputPin->MakeLinkTo(StartOutputPin);
	StartOutputPin->MakeLinkTo(ThisInputPin);

	EndInputPin->Modify();
	ThisOutputPin->MakeLinkTo(EndInputPin);
	EndInputPin->MakeLinkTo(ThisOutputPin);
}
