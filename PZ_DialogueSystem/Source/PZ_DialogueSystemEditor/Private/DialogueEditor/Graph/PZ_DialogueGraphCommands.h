// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "DialogueEditor/PZ_DialogueEditorStyle.h"




class FPZ_DialogueGraphCommands : public TCommands<FPZ_DialogueGraphCommands>
{

public:

	FPZ_DialogueGraphCommands()
		: TCommands<FPZ_DialogueGraphCommands>("DialogueGraphEditor", FText::FromString("Dialogue Graph Editor"), NAME_None, FEditorStyle::GetStyleSetName())
	{
	};



public:

	virtual void RegisterCommands() override;



public:

	TSharedPtr<FUICommandInfo> RebuildDialogue;
	TSharedPtr<FUICommandInfo> GenerateEndNodesOnLeafs;

};
