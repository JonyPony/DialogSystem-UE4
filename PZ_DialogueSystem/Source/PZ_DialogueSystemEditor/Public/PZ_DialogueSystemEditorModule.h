// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"




class FPZ_DialogueSystemEditorModule : public IModuleInterface
{

public:

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:

	void RegisterAssets();
	void UnregisterAssets();

	void RegisterFactories();
	void UnregisterFactories();

	void RegisterSettings();
	void UnregisterSettings();



private:

	TArray<TSharedPtr<class IAssetTypeActions>> CreatedAssetTypeActions;


	TSharedPtr<struct FPZ_DialogueGraphNodeFactory> DialogueGraphNodeFactory;
	TSharedPtr<struct FPZ_DialogueGraphPinFactory> DialogueGraphPinFactory;

};
