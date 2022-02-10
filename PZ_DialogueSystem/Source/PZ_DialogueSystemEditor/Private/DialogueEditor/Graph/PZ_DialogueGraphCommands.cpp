// Fill out your copyright notice in the Description page of Project Settings.


#include "PZ_DialogueGraphCommands.h"



#define LOCTEXT_NAMESPACE "EditorCommands_DialogueGraph"



void FPZ_DialogueGraphCommands::RegisterCommands()
{
	UI_COMMAND(RebuildDialogue, "Rebuild Dialogue", "Rebuild Dialogue", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(GenerateEndNodesOnLeafs, "Generate end nodes", "Generate end nodes on leafs", EUserInterfaceActionType::Button, FInputChord());
}



#undef LOCTEXT_NAMESPACE