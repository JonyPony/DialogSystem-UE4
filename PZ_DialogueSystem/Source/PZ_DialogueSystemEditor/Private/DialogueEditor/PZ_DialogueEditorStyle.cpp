// Fill out your copyright notice in the Description page of Project Settings.


#include "PZ_DialogueEditorStyle.h"
#include "Styling/SlateTypes.h"
#include "Styling/SlateStyleRegistry.h"
#include "Misc/Paths.h"




static const FVector2D Icon16x16(16.0f, 16.0f);
static const FVector2D Icon20x20(20.0f, 20.0f);
static const FVector2D Icon24x24(24.0f, 24.0f);
static const FVector2D Icon40x40(40.0f, 40.0f);
static const FVector2D Icon32x32(32.0f, 32.0f);
static const FVector2D Icon64x64(64.0f, 64.0f);
static const FVector2D Icon96x96(96.0f, 96.0f);



const FName FPZ_DialogueEditorStyle::NodeIcon_DefaultIcon("NodeIcon.DefaultIcon");
const FName FPZ_DialogueEditorStyle::NodeIcon_RootNodeIcon("NodeIcon.RootIcon");
const FName FPZ_DialogueEditorStyle::NodeIcon_EndNodeIcon("NodeIcon.EndIcon");
const FName FPZ_DialogueEditorStyle::NodeIcon_SayNodeIcon("NodeIcon.SayIcon");
const FName FPZ_DialogueEditorStyle::NodeIcon_SendEventNodeIcon("NodeIcon.SendEventIcon");
const FName FPZ_DialogueEditorStyle::NodeIcon_SelectorNodeIcon("NodeIcon.SelectorIcon");
const FName FPZ_DialogueEditorStyle::NodeIcon_DelayNodeIcon("NodeIcon.DelayIcon");
const FName FPZ_DialogueEditorStyle::NodeIcon_ServiceNodeIcon("NodeIcon.ServiceIcon");

TSharedPtr<FSlateStyleSet> FPZ_DialogueEditorStyle::StyleSet = nullptr;





void FPZ_DialogueEditorStyle::Initialize()
{
	if (StyleSet.IsValid()) return; 


	StyleSet = MakeShareable(new FSlateStyleSet("DialogueEditorStyle"));
	StyleSet->SetContentRoot(FPaths::ProjectPluginsDir() / TEXT("PZ_DialogueSystem/Resources"));



	//..................................AssetIcons...............................................//

	StyleSet->Set(
		"ClassThumbnail.PZ_DialogueEditor",
		new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("AssetIcons/DialogueEditorIcon_64x"), TEXT(".png")), Icon64x64, FLinearColor::White)
	);

	StyleSet->Set(
		"ClassThumbnail.PZ_DialogueCondition",
		new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("AssetIcons/DialogueConditionIcon_64x"), TEXT(".png")), Icon64x64, FLinearColor::White)
	);

	StyleSet->Set(
		"ClassThumbnail.PZ_DialogueEvent",
		new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("AssetIcons/DialogueEventIcon_64x"), TEXT(".png")), Icon64x64, FLinearColor::White)
	);

	StyleSet->Set(
		"ClassThumbnail.PZ_PhrasePropagation",
		new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("AssetIcons/PhrasePropagationIcon_64x"), TEXT(".png")), Icon64x64, FLinearColor::White)
	);

	StyleSet->Set(
		"ClassThumbnail.PZ_BaseParticipantNickName",
		new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("AssetIcons/BaseParticipantNickName_64x"), TEXT(".png")), Icon64x64, FLinearColor::White)
	);

	StyleSet->Set(
		"ClassThumbnail.PZ_DialogueService",
		new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("AssetIcons/DialogueService_64x"), TEXT(".png")), Icon64x64, FLinearColor::White)
	);

	//...........................................................................................//
	

	//...............................ToolbarIcons................................................//

	StyleSet->Set(
		"DialogEditor.RebuildDialogue",
		new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("ToolbarIcons/RebuildDialogueIcon_32x"), TEXT(".png")), Icon32x32, FLinearColor::Gray)
	);

	StyleSet->Set(
		"DialogEditor.NeedRebuildDialogue",
		new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("ToolbarIcons/RebuildDialogueIcon_32x"), TEXT(".png")), Icon32x32, FLinearColor::Yellow)
	);

	StyleSet->Set(
		"DialogEditor.ErrorRebuildDialogue",
		new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("ToolbarIcons/RebuildDialogueIcon_32x"), TEXT(".png")), Icon32x32, FLinearColor::Red)
	);


	StyleSet->Set(
		"DialogEditor.GenerateEndNodesOnLeafs",
		new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("ToolbarIcons/GenerateEndNodesOnLeafsIcon_32x"), TEXT(".png")), Icon32x32, FLinearColor::Red)
	);
	
	//...........................................................................................//


	//..............................NodeSettingIcons.............................................//
	
	StyleSet->Set(
		"DialogEditor.NodeError",
		new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("NodeSettingIcons/ErrorSimbol_32x"), TEXT(".png")), Icon32x32, FLinearColor::Yellow)
	);

	StyleSet->Set(
		"DialogEditor.MultiEvents",
		new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("NodeSettingIcons/MultiEventsIcon_32x"), TEXT(".png")), Icon32x32, FLinearColor::White)
	);


	StyleSet->Set(
		"DialogEditor.Crowd",
		new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("NodeSettingIcons/CrowdIcon_32x"), TEXT(".png")), Icon32x32, FLinearColor::White)
	);

	StyleSet->Set(
		"DialogEditor.Random",
		new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("NodeSettingIcons/RandomIcon_32x"), TEXT(".png")), Icon32x32, FLinearColor::White)
	);

	//...........................................................................................//


	//.........................NodeVisualizationIcons............................................//

	StyleSet->Set(
		"DialogEditor.DwarfIcon",
		new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("NodeVisualizationIcons/DwarfIcon_32x"), TEXT(".png")), Icon32x32, FLinearColor::White)
	);

	StyleSet->Set(
		"DialogEditor.CharacterIcon",
		new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("NodeVisualizationIcons/CharacterIcon_32x"), TEXT(".png")), Icon32x32, FLinearColor::White)
	);

	StyleSet->Set(
		"DialogEditor.VikingIcon",
		new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("NodeVisualizationIcons/VikingIcon_32x"), TEXT(".png")), Icon32x32, FLinearColor::White)
	);

	StyleSet->Set(
		"DialogEditor.VikingHelmetIcon",
		new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("NodeVisualizationIcons/VikingHelmetIcon_32x"), TEXT(".png")), Icon32x32, FLinearColor::White)
	);

	StyleSet->Set(
		"DialogEditor.SkeletonIcon",
		new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("NodeVisualizationIcons/SkeletonIcon_32x"), TEXT(".png")), Icon32x32, FLinearColor::White)
	);

	StyleSet->Set(
		"DialogEditor.OrcIcon",
		new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("NodeVisualizationIcons/OrcIcon_32x"), TEXT(".png")), Icon32x32, FLinearColor::White)
	);

	StyleSet->Set(
		"DialogEditor.SpartanHelmetIcon",
		new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("NodeVisualizationIcons/SpartanHelmetIcon_32x"), TEXT(".png")), Icon32x32, FLinearColor::White)
	);

	StyleSet->Set(
		"DialogEditor.VampireIcon",
		new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("NodeVisualizationIcons/VampireIcon_32x"), TEXT(".png")), Icon32x32, FLinearColor::White)
	);

	StyleSet->Set(
		"DialogEditor.OverlordIcon",
		new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("NodeVisualizationIcons/OverlordIcon_32x"), TEXT(".png")), Icon32x32, FLinearColor::White)
	);

	StyleSet->Set(
		"DialogEditor.WolfIcon",
		new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("NodeVisualizationIcons/WolfIcon_32x"), TEXT(".png")), Icon32x32, FLinearColor::White)
	);

	StyleSet->Set(
		"DialogEditor.WarriorIcon",
		new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("NodeVisualizationIcons/WarriorIcon_32x"), TEXT(".png")), Icon32x32, FLinearColor::White)
	);

	StyleSet->Set(
		"DialogEditor.MountedKnightIcon",
		new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("NodeVisualizationIcons/MountedKnightIcon_32x"), TEXT(".png")), Icon32x32, FLinearColor::White)
	);

	StyleSet->Set(
		"DialogEditor.FarmerIcon",
		new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("NodeVisualizationIcons/FarmerIcon_32x"), TEXT(".png")), Icon32x32, FLinearColor::White)
	);

	StyleSet->Set(
		"DialogEditor.CaesarIcon",
		new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("NodeVisualizationIcons/CaesarIcon_32x"), TEXT(".png")), Icon32x32, FLinearColor::White)
	);

	StyleSet->Set(
		"DialogEditor.DaggerRoseIcon",
		new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("NodeVisualizationIcons/DaggerRoseIcon_32x"), TEXT(".png")), Icon32x32, FLinearColor::White)
	);

	StyleSet->Set(
		"DialogEditor.RogueIcon",
		new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("NodeVisualizationIcons/RogueIcon_32x"), TEXT(".png")), Icon32x32, FLinearColor::White)
	);

	StyleSet->Set(
		"DialogEditor.TransfuseIcon",
		new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("NodeVisualizationIcons/TransfuseIcon_32x"), TEXT(".png")), Icon32x32, FLinearColor::White)
	);

	//...........................................................................................//


	//...........................NodeIcons.......................................................//

	StyleSet->Set(
		NodeIcon_DefaultIcon,
		new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("NodeIcons/RootNodeIcon_32x"), TEXT(".png")), Icon32x32, FLinearColor::Gray)
	);

	StyleSet->Set(
		NodeIcon_RootNodeIcon,
		new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("NodeIcons/RootNodeIcon_32x"), TEXT(".png")), Icon32x32, FLinearColor(0, 0.95, 0))
	);

	StyleSet->Set(
		NodeIcon_EndNodeIcon,
		new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("NodeIcons/EndNodeIcon_32x"), TEXT(".png")), Icon32x32, FLinearColor(0.95, 0, 0))
	);

	StyleSet->Set(
		NodeIcon_SayNodeIcon,
		new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("NodeIcons/SayNodeIcon_32x"), TEXT(".png")), Icon32x32, FLinearColor::White)
	);

	StyleSet->Set(
		NodeIcon_SendEventNodeIcon,
		new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("NodeIcons/SendEventNodeIcon_32x"), TEXT(".png")), Icon32x32, FLinearColor::White)
	);

	StyleSet->Set(
		NodeIcon_SelectorNodeIcon,
		new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("NodeIcons/SelectorNodeIcon_32x"), TEXT(".png")), Icon32x32, FLinearColor::Yellow)
	);

	StyleSet->Set(
		NodeIcon_DelayNodeIcon,
		new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("NodeIcons/DelayNodeIcon_32x"), TEXT(".png")), Icon32x32, FLinearColor::White)
	);

	StyleSet->Set(
		NodeIcon_ServiceNodeIcon,
		new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("NodeIcons/ServiceNodeIcon_32x"), TEXT(".png")), Icon32x32, FLinearColor::White)
	);

	//...........................................................................................//



	FSlateStyleRegistry::RegisterSlateStyle(*StyleSet.Get());
}


void FPZ_DialogueEditorStyle::Shutdown()
{
	if (StyleSet.IsValid())
	{
		FSlateStyleRegistry::UnRegisterSlateStyle(*StyleSet.Get());
		ensure(StyleSet.IsUnique());
		StyleSet.Reset();
	}
}
