// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"



class UPZ_DialogueEditor;
class UPZ_DialogueEdGraph;

class IDetailsView;
class SGraphEditor;



class FPZ_DialogueGraph : public FAssetEditorToolkit, public FNotifyHook, public FEditorUndoClient
{


public:

	FPZ_DialogueGraph();
	virtual ~FPZ_DialogueGraph();



public:

	static void Startup();
	static void Shutdown();

	void InitDialogueGraph(const EToolkitMode::Type Mode, const TSharedPtr< IToolkitHost >& InitToolkitHost, UPZ_DialogueEditor* Editor);

private:

	void CreateEdGraph();
	void CreateCommandList();
	void BindCommands();
	void CreateInternalWidgets();

	void CreateToolbarTab();
	void CreateViewportWidget();
	void CreatePropertiesWidget();



public:

	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;


	//..........................FAssetEditorToolkit Interface..........................//
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FText GetToolkitName() const override;
	virtual FText GetToolkitToolTipText() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FString GetDocumentationLink() const override;
	virtual bool OnRequestClose() override;

	virtual bool CanSaveAsset() const override;
	virtual void SaveAsset_Execute() override;
	virtual bool CanSaveAssetAs() const override;
	virtual void SaveAssetAs_Execute() override;
	//.................................................................................//
	
	//..............................FEditorUndoClient Interface........................//
	virtual void PostUndo(bool bSuccess) override;
	virtual void PostRedo(bool bSuccess) override;
	//.................................................................................//



	void Refresh();

	bool GetSelectedNodesBounds(FSlateRect& Bounds, float Padding);


private:

	TSharedRef<SDockTab> SpawnTab_Viewport(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> SpawnTab_Details(const FSpawnTabArgs& Args);
	void FillToolbar(FToolBarBuilder& ToolbarBuilder);


	//...........................Delegates for graph editor commands...................//

	void SelectAllNodes();
	bool CanSelectAllNodes();
	void DeleteSelectedNodes();
	bool CanDeleteNodes();
	void CutSelectedNodes();
	bool CanCutNodes();
	void CopySelectedNodes();
	bool CanCopyNodes();
	void PasteNodes();
	bool CanPasteNodes();
	void DuplicateNodes();
	bool CanDuplicateNodes();
	void OnRenameNode();
	bool CanRenameNodes();
	void CreateComment();
	bool CanCreateComment();
	void OnCommandUndoGraphAction();
	void OnCommandRedoGraphAction();

	void RebuildDialogue();
	bool CanRebuildDialogue();

	void GenerateEndNodesOnLeafs();
	bool CanGenerateEndNodesOnLeafs();

	//.................................................................................//

	
	//..........................Graph editor event.....................................//

	void OnSelectedNodesChanged(const TSet<UObject*>& NewSelection);
	void OnNodeDoubleClicked(UEdGraphNode* Node);
	void OnNodeTitleCommitted(const FText& NewText, ETextCommit::Type CommitInfo, UEdGraphNode* NodeBeingChanged);
	void OnFinishedChangingProperties(const FPropertyChangedEvent& PropertyChangedEvent);
	void OnPackageSaved(const FString& PackageFileName, UObject* Outer);

	//.................................................................................//



	void RefreshViewport();
	void RefreshDetailsView();




private:

	UPZ_DialogueEditor* DialogueEditor = nullptr;
	UPZ_DialogueEdGraph* MyEdGraph = nullptr;



	//..................SubWidgets..........................//

	TSharedPtr<FExtender> ToolbarExtender;
	TSharedPtr<SGraphEditor> ViewportWidget;
	TSharedPtr<IDetailsView> PropertyWidget;

	//......................................................//

	//......................Commands........................//

	TSharedPtr<FUICommandList> GraphEditorCommands;

	//......................................................//



	FDelegateHandle OnPackageSavedDelegateHandle;

};
