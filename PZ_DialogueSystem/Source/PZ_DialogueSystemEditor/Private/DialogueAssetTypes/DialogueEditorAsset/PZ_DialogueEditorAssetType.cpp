// Fill out your copyright notice in the Description page of Project Settings.


#include "PZ_DialogueEditorAssetType.h"
#include "PZ_DialogueEditor.h"
#include "DialogueEditor/Graph/PZ_DialogueGraph.h"




FPZ_DialogueEditorAssetType::FPZ_DialogueEditorAssetType(EAssetTypeCategories::Type InAssetCategory) :
	MyAssetCategory(InAssetCategory)
{
}





FText FPZ_DialogueEditorAssetType::GetName() const
{
	return FText::FromString("Dialogue Graph");
}

FColor FPZ_DialogueEditorAssetType::GetTypeColor() const
{
	return FColor(129, 196, 115);
}

UClass* FPZ_DialogueEditorAssetType::GetSupportedClass() const
{
	return UPZ_DialogueEditor::StaticClass();
}

void FPZ_DialogueEditorAssetType::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor)
{
	const EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;

	for( auto ObjIt = InObjects.CreateConstIterator(); ObjIt; ++ObjIt )
	{
		if( UPZ_DialogueEditor* LDialogueEditor = Cast<UPZ_DialogueEditor>(*ObjIt) )
		{
			TSharedRef<FPZ_DialogueGraph> NewGraphEditor(new FPZ_DialogueGraph());
			NewGraphEditor->InitDialogueGraph(Mode, EditWithinLevelEditor, LDialogueEditor);
		}
	}
}

uint32 FPZ_DialogueEditorAssetType::GetCategories()
{
	return MyAssetCategory;
}
