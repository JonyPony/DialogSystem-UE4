// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PZ_DialogueNodesInfo.h"
#include "PZ_DialogueNodeInterface.generated.h"



class UPZ_DialogueEditor;




// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPZ_DialogueNodeInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};



class IPZ_DialogueNodeInterface
{
	GENERATED_IINTERFACE_BODY()


public:

	virtual EDialogueNodeType GetNodeType() const PURE_VIRTUAL(IPZ_DialogueNodeInterface::GetNodeType(), return EDialogueNodeType::None;);

	virtual UEdGraphPin* GetInputPin() const PURE_VIRTUAL(IPZ_DialogueNodeInterface::GetInputPin(), return nullptr;);
	virtual UEdGraphPin* GetOutputPin() const PURE_VIRTUAL(IPZ_DialogueNodeInterface::GetOutputPin(), return nullptr;);


	virtual void CompileThisNodeInfo(UPZ_DialogueEditor* DialogueEditor) PURE_VIRTUAL(IPZ_DialogueNodeInterface::CompileThisNodeInfo(), return;);


	virtual void OnNodeDoubleClicked() PURE_VIRTUAL(IPZ_DialogueNodeInterface::OnNodeDoubleClicked(), return;);
	virtual void OnChangedProperties(const FName& PropertyName) const PURE_VIRTUAL(IPZ_DialogueNodeInterface::OnChangedProperties(), return;);
};

