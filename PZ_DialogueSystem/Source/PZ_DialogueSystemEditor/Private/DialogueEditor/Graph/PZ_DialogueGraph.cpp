// Fill out your copyright notice in the Description page of Project Settings.


#include "PZ_DialogueGraph.h"

#include "PZ_DialogueEditor.h"
#include "PZ_DialogueEdGraph.h"
#include "PZ_DialogueEdGraphSchema.h"
#include "PZ_DialogueGraphCommands.h"
#include "DialogueEditor/PZ_DialogueEditorStyle.h"

#include "DialogueEditor/Nodes/PZ_DialogueNodeInterface.h"
#include "DialogueEditor/Nodes/EdgePoint/PZ_DialogueEdNode_Edge.h"


#include "Toolkits/AssetEditorManager.h"
#include "Toolkits/AssetEditorToolkit.h"

#include "Framework/Commands/GenericCommands.h"
#include "Framework/Docking/TabManager.h"

#include "GraphEditorActions.h"
#include "EdGraphUtilities.h"
#include "Editor/UnrealEd/Public/Kismet2/BlueprintEditorUtils.h"
#include "HAL/PlatformApplicationMisc.h"





const FName DialogueGraphEditorAppName(TEXT("DialogueGraphEditorAppName"));

const FName ViewportID(TEXT("Viewport"));
const FName GraphPropertyID(TEXT("GraphProperty"));






FPZ_DialogueGraph::FPZ_DialogueGraph()
{
	GEditor->RegisterForUndo(this);

	OnPackageSavedDelegateHandle = UPackage::PackageSavedEvent.AddRaw(this, &FPZ_DialogueGraph::OnPackageSaved);
}

FPZ_DialogueGraph::~FPZ_DialogueGraph()
{
	UPackage::PackageSavedEvent.Remove(OnPackageSavedDelegateHandle);

	GEditor->UnregisterForUndo(this);
}






void FPZ_DialogueGraph::Startup()
{
	FGenericCommands::Register();
	FGraphEditorCommands::Register();
	FPZ_DialogueGraphCommands::Register();
}

void FPZ_DialogueGraph::Shutdown()
{
	FPZ_DialogueGraphCommands::Unregister();
}


void FPZ_DialogueGraph::InitDialogueGraph(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UPZ_DialogueEditor* Editor)
{
	DialogueEditor = Editor;

	CreateEdGraph();
	CreateCommandList();
	BindCommands();
	CreateInternalWidgets();


	const TSharedRef<FTabManager::FLayout> StandaloneDefaultLayout = FTabManager::NewLayout("DialogueEditorLayout_v4")
		->AddArea
		(
			FTabManager::NewPrimaryArea()
			->SetOrientation(Orient_Vertical)
			->Split // toolbar at top of window
			(
				FTabManager::NewStack()
				->SetSizeCoefficient(0.1f)
				->SetHideTabWell(true)
				->AddTab(GetToolbarTabId(), ETabState::OpenedTab)
			)
			->Split // Content area
			(
				FTabManager::NewSplitter()
				->SetSizeCoefficient(0.9f)
				->SetOrientation(Orient_Horizontal)
				->Split // Viewport
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.5f)
					->SetHideTabWell(true)
					->AddTab(ViewportID, ETabState::OpenedTab)
				)
				->Split // Properties area
				(
					FTabManager::NewSplitter()->SetOrientation(Orient_Vertical)
					->Split
					(
						FTabManager::NewStack()
						->SetSizeCoefficient(0.7f)
						->SetHideTabWell(true)
						->AddTab(GraphPropertyID, ETabState::OpenedTab)
					)
					
				)
			)
		);



	const bool bCreateDefaultStandaloneMenu = true;
	const bool bCreateDefaultToolbar = true;
	FAssetEditorToolkit::InitAssetEditor(Mode, InitToolkitHost, DialogueGraphEditorAppName, StandaloneDefaultLayout, bCreateDefaultStandaloneMenu, bCreateDefaultToolbar, DialogueEditor, false);



	RegenerateMenusAndToolbars();
}



void FPZ_DialogueGraph::CreateEdGraph()
{
	if (!IsValid(DialogueEditor->EdGraph))
	{
		MyEdGraph = CastChecked<UPZ_DialogueEdGraph>(FBlueprintEditorUtils::CreateNewGraph(DialogueEditor, NAME_None, UPZ_DialogueEdGraph::StaticClass(), UPZ_DialogueEdGraphSchema::StaticClass()));
	
		DialogueEditor->EdGraph = MyEdGraph;

		// Give the schema a chance to fill out any required nodes (like the results node)
		const UEdGraphSchema* Schema = MyEdGraph->GetSchema();
		Schema->CreateDefaultNodesForGraph(*MyEdGraph);
	}
	else
	{
		MyEdGraph = Cast<UPZ_DialogueEdGraph>(DialogueEditor->EdGraph);
	}


	MyEdGraph->DialogueGraph = this;
	MyEdGraph->DialogueEditor = DialogueEditor;
	MyEdGraph->bAllowDeletion = false;
}

void FPZ_DialogueGraph::CreateCommandList()
{
	if (GraphEditorCommands.IsValid()) return; 


	GraphEditorCommands = MakeShareable(new FUICommandList);


	GraphEditorCommands->MapAction(
		FGenericCommands::Get().SelectAll,
		FExecuteAction::CreateRaw(this, &FPZ_DialogueGraph::SelectAllNodes),
		FCanExecuteAction::CreateRaw(this, &FPZ_DialogueGraph::CanSelectAllNodes)
	);

	GraphEditorCommands->MapAction(
		FGenericCommands::Get().Delete,
		FExecuteAction::CreateRaw(this, &FPZ_DialogueGraph::DeleteSelectedNodes),
		FCanExecuteAction::CreateRaw(this, &FPZ_DialogueGraph::CanDeleteNodes)
	);

	GraphEditorCommands->MapAction(
		FGenericCommands::Get().Copy,
		FExecuteAction::CreateRaw(this, &FPZ_DialogueGraph::CopySelectedNodes),
		FCanExecuteAction::CreateRaw(this, &FPZ_DialogueGraph::CanCopyNodes)
	);

	GraphEditorCommands->MapAction(
		FGenericCommands::Get().Cut,
		FExecuteAction::CreateRaw(this, &FPZ_DialogueGraph::CutSelectedNodes),
		FCanExecuteAction::CreateRaw(this, &FPZ_DialogueGraph::CanCutNodes)
	);

	GraphEditorCommands->MapAction(
		FGenericCommands::Get().Paste,
		FExecuteAction::CreateRaw(this, &FPZ_DialogueGraph::PasteNodes),
		FCanExecuteAction::CreateRaw(this, &FPZ_DialogueGraph::CanPasteNodes)
	);

	GraphEditorCommands->MapAction(
		FGenericCommands::Get().Duplicate,
		FExecuteAction::CreateRaw(this, &FPZ_DialogueGraph::DuplicateNodes),
		FCanExecuteAction::CreateRaw(this, &FPZ_DialogueGraph::CanDuplicateNodes)
	);

	GraphEditorCommands->MapAction(
		FGenericCommands::Get().Rename,
		FExecuteAction::CreateSP(this, &FPZ_DialogueGraph::OnRenameNode),
		FCanExecuteAction::CreateSP(this, &FPZ_DialogueGraph::CanRenameNodes)
	);

	GraphEditorCommands->MapAction(
		FGraphEditorCommands::Get().CreateComment,
		FExecuteAction::CreateSP(this, &FPZ_DialogueGraph::CreateComment),
		FCanExecuteAction::CreateSP(this, &FPZ_DialogueGraph::CanCreateComment)
	);
}

void FPZ_DialogueGraph::BindCommands()
{
	ToolkitCommands->MapAction(
		FGenericCommands::Get().Undo,
		FExecuteAction::CreateSP(this, &FPZ_DialogueGraph::OnCommandUndoGraphAction)
	);

	ToolkitCommands->MapAction(
		FGenericCommands::Get().Redo,
		FExecuteAction::CreateSP(this, &FPZ_DialogueGraph::OnCommandRedoGraphAction)
	);



	ToolkitCommands->MapAction(
		FPZ_DialogueGraphCommands::Get().RebuildDialogue,
		FExecuteAction::CreateSP(this, &FPZ_DialogueGraph::RebuildDialogue),
		FCanExecuteAction::CreateSP(this, &FPZ_DialogueGraph::CanRebuildDialogue)
	);

	ToolkitCommands->MapAction(
		FPZ_DialogueGraphCommands::Get().GenerateEndNodesOnLeafs,
		FExecuteAction::CreateSP(this, &FPZ_DialogueGraph::GenerateEndNodesOnLeafs),
		FCanExecuteAction::CreateSP(this, &FPZ_DialogueGraph::CanGenerateEndNodesOnLeafs)
	);
}

void FPZ_DialogueGraph::CreateInternalWidgets()
{
	CreateToolbarTab();
	CreateViewportWidget();
	CreatePropertiesWidget();
}




void FPZ_DialogueGraph::CreateToolbarTab()
{
	if ( !ToolbarExtender.IsValid() )
	{
		ToolbarExtender = MakeShareable(new FExtender);
	}
	ToolbarExtender->AddToolBarExtension("Asset", EExtensionHook::After, GetToolkitCommands(), FToolBarExtensionDelegate::CreateSP(this, &FPZ_DialogueGraph::FillToolbar));
	AddToolbarExtender(ToolbarExtender);
}

void FPZ_DialogueGraph::CreateViewportWidget()
{
	FGraphAppearanceInfo AppearanceInfo;
	AppearanceInfo.CornerText = FText::FromString("Dialogue Graph");


	SGraphEditor::FGraphEditorEvents InEvents;
	InEvents.OnSelectionChanged = SGraphEditor::FOnSelectionChanged::CreateSP(this, &FPZ_DialogueGraph::OnSelectedNodesChanged);
	InEvents.OnNodeDoubleClicked = FSingleNodeEvent::CreateSP(this, &FPZ_DialogueGraph::OnNodeDoubleClicked);
	InEvents.OnTextCommitted = FOnNodeTextCommitted::CreateSP(this, &FPZ_DialogueGraph::OnNodeTitleCommitted);


	ViewportWidget = SNew(SGraphEditor)
		.AdditionalCommands(GraphEditorCommands)
		.IsEditable(true)
		.Appearance(AppearanceInfo)
		.GraphToEdit(MyEdGraph)
		.GraphEvents(InEvents)
		.AutoExpandActionMenu(true)
		.ShowGraphStateOverlay(false);
}

void FPZ_DialogueGraph::CreatePropertiesWidget()
{
	FDetailsViewArgs Args;
	Args.bHideSelectionTip = true;
	Args.NotifyHook = this;


	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyWidget = PropertyModule.CreateDetailView(Args);
	PropertyWidget->SetObject(DialogueEditor);
	PropertyWidget->OnFinishedChangingProperties().AddSP(this, &FPZ_DialogueGraph::OnFinishedChangingProperties);
}







void FPZ_DialogueGraph::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(FText::FromString("Dialogue Graph Editor"));
	TSharedRef<FWorkspaceItem> WorkspaceMenuCategoryRef = WorkspaceMenuCategory.ToSharedRef();


	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	InTabManager->RegisterTabSpawner(ViewportID, FOnSpawnTab::CreateSP(this, &FPZ_DialogueGraph::SpawnTab_Viewport))
		.SetDisplayName(FText::FromString("Viewport"))
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "GraphEditor.EventGraph_16x"));

	InTabManager->RegisterTabSpawner(GraphPropertyID, FOnSpawnTab::CreateSP(this, &FPZ_DialogueGraph::SpawnTab_Details))
		.SetDisplayName(FText::FromString("Property"))
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.Tabs.Details"));
}

void FPZ_DialogueGraph::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);

	InTabManager->UnregisterTabSpawner(ViewportID);
	InTabManager->UnregisterTabSpawner(GraphPropertyID);
}




//......................................................FAssetEditorToolkit Interface....................................................//

FName FPZ_DialogueGraph::GetToolkitFName() const
{
	return FName("FDialogueEditor");
}

FText FPZ_DialogueGraph::GetBaseToolkitName() const
{
	return FText::FromString("Dialogue Editor");
}

FText FPZ_DialogueGraph::GetToolkitName() const
{
	return FText::FromString(DialogueEditor->GetName());
}

FText FPZ_DialogueGraph::GetToolkitToolTipText() const
{
	return FAssetEditorToolkit::GetToolTipTextForObject(DialogueEditor);
}

FLinearColor FPZ_DialogueGraph::GetWorldCentricTabColorScale() const
{
	return FLinearColor::White;
}

FString FPZ_DialogueGraph::GetWorldCentricTabPrefix() const
{
	return FString("DialogueEditor");
}

FString FPZ_DialogueGraph::GetDocumentationLink() const
{
	return FString("");
}

bool FPZ_DialogueGraph::OnRequestClose()
{
	//TODO

	return FAssetEditorToolkit::OnRequestClose();
}



bool FPZ_DialogueGraph::CanSaveAsset() const
{
	return true;
}

void FPZ_DialogueGraph::SaveAsset_Execute()
{
	//TODO

	FAssetEditorToolkit::SaveAsset_Execute();
}

bool FPZ_DialogueGraph::CanSaveAssetAs() const
{
	return true;
}

void FPZ_DialogueGraph::SaveAssetAs_Execute()
{
	//TODO

	FAssetEditorToolkit::SaveAssetAs_Execute();
}

//.......................................................................................................................................//

//...............................................FEditorUndoClient Interface.............................................................//

void FPZ_DialogueGraph::PostUndo(bool bSuccess)
{
	if (bSuccess)
	{
		Refresh();

		DialogueEditor->NeedToRebuild = true;
		RegenerateMenusAndToolbars();
	}
}

void FPZ_DialogueGraph::PostRedo(bool bSuccess)
{
	if (bSuccess)
	{
		Refresh();
		
		DialogueEditor->NeedToRebuild = true;
		RegenerateMenusAndToolbars();
	}
}

//.......................................................................................................................................//






void FPZ_DialogueGraph::Refresh()
{
	RefreshViewport();
	RefreshDetailsView();

	FSlateApplication::Get().DismissAllMenus();
}


bool FPZ_DialogueGraph::GetSelectedNodesBounds(FSlateRect& Bounds, float Padding)
{
	return ViewportWidget->GetBoundsForSelectedNodes(Bounds, Padding);
}







TSharedRef<SDockTab> FPZ_DialogueGraph::SpawnTab_Viewport(const FSpawnTabArgs& Args)
{
	check(Args.GetTabId() == ViewportID);


	TSharedRef<SDockTab> SpawnedTab = SNew(SDockTab)
		.Icon(FEditorStyle::GetBrush("GraphEditor.EventGraph_16x"))
		.Label(FText::FromString("Viewport"));

	if (ViewportWidget.IsValid())
	{
		SpawnedTab->SetContent(ViewportWidget.ToSharedRef());
	}

	return SpawnedTab;
}

TSharedRef<SDockTab> FPZ_DialogueGraph::SpawnTab_Details(const FSpawnTabArgs& Args)
{
	check(Args.GetTabId() == GraphPropertyID);


	TSharedRef<SDockTab> SpawnedTab = SNew(SDockTab)
		.Icon(FEditorStyle::GetBrush("LevelEditor.Tabs.Details"))
		.Label(FText::FromString("Property"));
		
	if (PropertyWidget.IsValid())
	{
		SpawnedTab->SetContent(PropertyWidget.ToSharedRef());
	}

	return SpawnedTab;
}


void FPZ_DialogueGraph::FillToolbar(FToolBarBuilder& ToolbarBuilder)
{
	FSlateIcon RebuildIcon =  FSlateIcon(FPZ_DialogueEditorStyle::GetStyleSetName(), "DialogEditor.RebuildDialogue");
	if (DialogueEditor->HasError)
	{
		RebuildIcon = FSlateIcon(FPZ_DialogueEditorStyle::GetStyleSetName(), "DialogEditor.ErrorRebuildDialogue");
	}
	else
	{
		if(DialogueEditor->NeedToRebuild)
		{
			RebuildIcon = FSlateIcon(FPZ_DialogueEditorStyle::GetStyleSetName(), "DialogEditor.NeedRebuildDialogue");
		}
	}
	

	ToolbarBuilder.BeginSection("Dialogue Graph");
	{
		ToolbarBuilder.AddToolBarButton
		(
			FPZ_DialogueGraphCommands::Get().RebuildDialogue,
			NAME_None,
			FText::FromString("Rebuild Dialog"),
			FText::FromString("Rebuild Dialog"),
			RebuildIcon
		);

		ToolbarBuilder.AddToolBarButton
		(
			FPZ_DialogueGraphCommands::Get().GenerateEndNodesOnLeafs,
			NAME_None,
			FText::FromString("Generate End Nodes On Leafs"),
			FText::FromString("Generate End Nodes On Leafs"),
			FSlateIcon(FPZ_DialogueEditorStyle::GetStyleSetName(), "DialogEditor.GenerateEndNodesOnLeafs")
		);
	}
	ToolbarBuilder.EndSection();
}







//...............................................Delegates for graph editor commands.....................................................//

void FPZ_DialogueGraph::SelectAllNodes()
{
	ViewportWidget->SelectAllNodes();
}

bool FPZ_DialogueGraph::CanSelectAllNodes()
{
	return true;
}


void FPZ_DialogueGraph::DeleteSelectedNodes()
{
	const FScopedTransaction Transaction(FGenericCommands::Get().Delete->GetDescription());

	ViewportWidget->GetCurrentGraph()->Modify();

	const FGraphPanelSelectionSet SelectedNodes = ViewportWidget->GetSelectedNodes();
	ViewportWidget->ClearSelectionSet();

	for (auto& DeletingNode : SelectedNodes)
	{
		UEdGraphNode* EdNode = Cast<UEdGraphNode>(DeletingNode);
		if (EdNode == nullptr || !EdNode->CanUserDeleteNode())	continue;

		EdNode->GetSchema()->BreakNodeLinks(*EdNode);
		EdNode->Modify();
		EdNode->DestroyNode();
		
	}
}

bool FPZ_DialogueGraph::CanDeleteNodes()
{
	const FGraphPanelSelectionSet SelectedNodes = ViewportWidget->GetSelectedNodes();
	for (const auto& SelectedNode : SelectedNodes)
	{
		UEdGraphNode* Node = Cast<UEdGraphNode>(SelectedNode);
		if (Node && Node->CanUserDeleteNode())
		{
			return true;
		}
	}

	return false;
}



void FPZ_DialogueGraph::CutSelectedNodes()
{
	CopySelectedNodes();
	DeleteSelectedNodes();
}

bool FPZ_DialogueGraph::CanCutNodes()
{
	return CanCopyNodes() && CanDeleteNodes();
}


void FPZ_DialogueGraph::CopySelectedNodes()
{
	FGraphPanelSelectionSet SelectedNodes = ViewportWidget->GetSelectedNodes();
	for( FGraphPanelSelectionSet::TIterator SelectedIter(SelectedNodes); SelectedIter; ++SelectedIter )
	{
		UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter);
		if( Node == nullptr )
		{
			SelectedIter.RemoveCurrent();
			continue;
		}

		if (UPZ_DialogueEdNode_Edge* EdNode_Edge = Cast<UPZ_DialogueEdNode_Edge>(*SelectedIter))
		{
			UEdGraphNode* StartNode = EdNode_Edge->GetStartNode();
			UEdGraphNode* EndNode = EdNode_Edge->GetEndNode();

			if( !SelectedNodes.Contains(StartNode) || !SelectedNodes.Contains(EndNode) )
			{
				SelectedIter.RemoveCurrent();
				continue;
			}
		}

		Node->PrepareForCopying();
	}


	FString ExportedText;
	FEdGraphUtilities::ExportNodesToText(SelectedNodes, /*out*/ ExportedText);
	FPlatformApplicationMisc::ClipboardCopy(*ExportedText);
}

bool FPZ_DialogueGraph::CanCopyNodes()
{
	const FGraphPanelSelectionSet SelectedNodes = ViewportWidget->GetSelectedNodes();
	for( const auto& SelectedNode : SelectedNodes )
	{
		UEdGraphNode* Node = Cast<UEdGraphNode>(SelectedNode);

		if( Node && Node->CanDuplicateNode() )
		{
			return true;
		}
		
	}

	return false;
}


void FPZ_DialogueGraph::PasteNodes()
{
	FVector2D PasteLocation = ViewportWidget->GetPasteLocation();
	UEdGraph* EdGraph = ViewportWidget->GetCurrentGraph();

	const FScopedTransaction Transaction(FGenericCommands::Get().Paste->GetDescription());

	EdGraph->Modify();
	ViewportWidget->ClearSelectionSet();



	FString TextToImport;
	FPlatformApplicationMisc::ClipboardPaste(TextToImport);

	TSet<UEdGraphNode*> PastedNodes;
	FEdGraphUtilities::ImportNodesFromText(EdGraph, TextToImport, PastedNodes);



	FVector2D AvgNodePosition(0.0f, 0.0f);

	for (const auto&  PastedNode : PastedNodes)
	{
		AvgNodePosition.X += PastedNode->NodePosX;
		AvgNodePosition.Y += PastedNode->NodePosY;
	}

	float InvNumNodes = 1.0f / float(PastedNodes.Num());
	AvgNodePosition.X *= InvNumNodes;
	AvgNodePosition.Y *= InvNumNodes;

	for (const auto& PastedNode : PastedNodes)
	{
		ViewportWidget->SetNodeSelection(PastedNode, true);

		PastedNode->NodePosX = (PastedNode->NodePosX - AvgNodePosition.X) + PasteLocation.X;
		PastedNode->NodePosY = (PastedNode->NodePosY - AvgNodePosition.Y) + PasteLocation.Y;


		// Give new node a different Guid from the old one
		PastedNode->CreateNewGuid();
	}


	RefreshViewport();
	DialogueEditor->PostEditChange();
	DialogueEditor->MarkPackageDirty();

	DialogueEditor->NeedToRebuild = true;
	RegenerateMenusAndToolbars();
}

bool FPZ_DialogueGraph::CanPasteNodes()
{
	FString ClipboardContent;
	FPlatformApplicationMisc::ClipboardPaste(ClipboardContent);

	return FEdGraphUtilities::CanImportNodesFromText(ViewportWidget->GetCurrentGraph(), ClipboardContent);
}


void FPZ_DialogueGraph::DuplicateNodes()
{
	CopySelectedNodes();
	PasteNodes();
}

bool FPZ_DialogueGraph::CanDuplicateNodes()
{
	return CanCopyNodes();
}


void FPZ_DialogueGraph::OnRenameNode()
{
	const FGraphPanelSelectionSet SelectedNodes = ViewportWidget->GetSelectedNodes();
	for( const auto& SelectedNode : SelectedNodes ) // get first
	{
		UEdGraphNode* LSelectedNode = Cast<UEdGraphNode>(SelectedNode);
		ViewportWidget->IsNodeTitleVisible(LSelectedNode, true);
		break;
	}
}

bool FPZ_DialogueGraph::CanRenameNodes()
{
	const FGraphPanelSelectionSet SelectedNodes = ViewportWidget->GetSelectedNodes();
	bool CanRenameNode = false;
	for( const auto& SelectedNode : SelectedNodes ) // get first
	{
		UEdGraphNode* LSelectedNode = Cast<UEdGraphNode>(SelectedNode);
		CanRenameNode = LSelectedNode->bCanRenameNode;
		break;
	}

	return SelectedNodes.Num() == 1 && CanRenameNode;
}


void FPZ_DialogueGraph::CreateComment()
{
	FNewComment_DialogueGraphSchemaAction CommentAction;
	CommentAction.PerformAction(MyEdGraph, nullptr, ViewportWidget->GetPasteLocation());
}

bool FPZ_DialogueGraph::CanCreateComment()
{
	return true;
}


void FPZ_DialogueGraph::OnCommandUndoGraphAction()
{
	GEditor->UndoTransaction(true);
}

void FPZ_DialogueGraph::OnCommandRedoGraphAction()
{
	ViewportWidget->ClearSelectionSet();
	GEditor->RedoTransaction();
}



void FPZ_DialogueGraph::RebuildDialogue()
{
	MyEdGraph->RebuildGraph();
	SaveAsset_Execute();
	RegenerateMenusAndToolbars();
}

bool FPZ_DialogueGraph::CanRebuildDialogue()
{
	return true;
}


void FPZ_DialogueGraph::GenerateEndNodesOnLeafs()
{
	MyEdGraph->GenerateEndNodesOnLeafs();
}

bool FPZ_DialogueGraph::CanGenerateEndNodesOnLeafs()
{
	return true;
}

//.......................................................................................................................................//


//.....................................................Graph editor event................................................................//

void FPZ_DialogueGraph::OnSelectedNodesChanged(const TSet<class UObject*>& NewSelection)
{
	if( NewSelection.Num() > 0 )
	{
		TArray<UObject*> ViewSelection;
		for( UObject* Selected : NewSelection )
		{
			ViewSelection.Add(Selected);
		}

		PropertyWidget->SetObjects(ViewSelection, true);
	}
	else
	{
		PropertyWidget->SetObject(DialogueEditor);
	}
}

void FPZ_DialogueGraph::OnNodeDoubleClicked(UEdGraphNode* Node)
{
	if( IPZ_DialogueNodeInterface* LDialogueNode = Cast<IPZ_DialogueNodeInterface>(Node) )
	{
		LDialogueNode->OnNodeDoubleClicked();
	}
}

void FPZ_DialogueGraph::OnNodeTitleCommitted(const FText& NewText, ETextCommit::Type CommitInfo, UEdGraphNode* NodeBeingChanged)
{
	if( NodeBeingChanged )
	{
		NodeBeingChanged->Modify();
		NodeBeingChanged->OnRenameNode(NewText.ToString());
	}
}

void FPZ_DialogueGraph::OnFinishedChangingProperties(const FPropertyChangedEvent& PropertyChangedEvent)
{
	for( auto LNode : MyEdGraph->Nodes )
	{
		if( IPZ_DialogueNodeInterface* LDialogueNode = Cast<IPZ_DialogueNodeInterface>(LNode) )
		{
			LDialogueNode->OnChangedProperties(PropertyChangedEvent.GetPropertyName());
		}
		
	}


	DialogueEditor->HasError = !DialogueEditor->IsParticipantsInfoValid();
	DialogueEditor->NeedToRebuild = true;
	RegenerateMenusAndToolbars();
}

void FPZ_DialogueGraph::OnPackageSaved(const FString& PackageFileName, UObject* Outer)
{
	//TODO
}

//.......................................................................................................................................//





void FPZ_DialogueGraph::RefreshViewport()
{
	if(!ViewportWidget.IsValid()) return;

	ViewportWidget->ClearSelectionSet();
	ViewportWidget->NotifyGraphChanged();
}

void FPZ_DialogueGraph::RefreshDetailsView()
{
	if(!PropertyWidget.IsValid()) return;

	PropertyWidget->SetObject(DialogueEditor);
	PropertyWidget->ClearSearch();
}





