// Copyright Epic Games, Inc. All Rights Reserved.

#include "PZ_DialogueSystemEditorModule.h"


#include "DialogueAssetTypes/DialogueEditorAsset/PZ_DialogueEditorAssetType.h"

#include "DialogueEditor/Nodes/PZ_DialogueNodesFactory.h"

#include "DialogueEditor/PZ_DialogueEditorStyle.h"

#include "DialogueEditor/Graph/PZ_DialogueGraph.h"


#include "ISettingsModule.h"




#define LOCTEXT_NAMESPACE "FPZ_DialogueSystemEditorModule"




void FPZ_DialogueSystemEditorModule::StartupModule()
{
	RegisterAssets();
	RegisterFactories();
	RegisterSettings();

	FPZ_DialogueEditorStyle::Initialize();
	FPZ_DialogueGraph::Startup();
}

void FPZ_DialogueSystemEditorModule::ShutdownModule()
{
	UnregisterAssets();
	UnregisterFactories();
	UnregisterSettings();

	FPZ_DialogueEditorStyle::Shutdown();
	FPZ_DialogueGraph::Shutdown();
}




template<class AssetType>
void RegisterAsset(TArray<TSharedPtr<IAssetTypeActions>>& CreatedAssetTypeActions, IAssetTools& AssetTools, FName CategoryName, FText CategoryDisplayName)
{
	auto LAssetCategoryBit = AssetTools.RegisterAdvancedAssetCategory(CategoryName, CategoryDisplayName);
	TSharedRef<IAssetTypeActions> LAction = MakeShareable(new AssetType(LAssetCategoryBit));
	AssetTools.RegisterAssetTypeActions(LAction);
	CreatedAssetTypeActions.Add(LAction);
}

void FPZ_DialogueSystemEditorModule::RegisterAssets()
{
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

	RegisterAsset<FPZ_DialogueEditorAssetType>(CreatedAssetTypeActions, AssetTools, FName("PZ_DialogueSystem"), FText::FromString("PZ_DialogueSystem"));
}

void FPZ_DialogueSystemEditorModule::UnregisterAssets()
{
	if( FModuleManager::Get().IsModuleLoaded("AssetTools") )
	{
		IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
		for( int32 Index = 0; Index < CreatedAssetTypeActions.Num(); ++Index )
		{
			AssetTools.UnregisterAssetTypeActions(CreatedAssetTypeActions[Index].ToSharedRef());
		}
	}
}



void FPZ_DialogueSystemEditorModule::RegisterFactories()
{
	DialogueGraphNodeFactory = MakeShared<FPZ_DialogueGraphNodeFactory>();
	FEdGraphUtilities::RegisterVisualNodeFactory(DialogueGraphNodeFactory);

	DialogueGraphPinFactory = MakeShared<FPZ_DialogueGraphPinFactory>();
	FEdGraphUtilities::RegisterVisualPinFactory(DialogueGraphPinFactory);
}

void FPZ_DialogueSystemEditorModule::UnregisterFactories()
{
	if( DialogueGraphNodeFactory.IsValid() )
	{
		FEdGraphUtilities::UnregisterVisualNodeFactory(DialogueGraphNodeFactory);
	}

	if( DialogueGraphPinFactory.IsValid() )
	{
		FEdGraphUtilities::UnregisterVisualPinFactory(DialogueGraphPinFactory);
	}
}



void FPZ_DialogueSystemEditorModule::RegisterSettings()
{
	if(ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		//TODO
	}
}

void FPZ_DialogueSystemEditorModule::UnregisterSettings()
{
	if(ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		//TODO
	}
}




#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FPZ_DialogueSystemEditorModule, PZ_DialogueSystemEditor)