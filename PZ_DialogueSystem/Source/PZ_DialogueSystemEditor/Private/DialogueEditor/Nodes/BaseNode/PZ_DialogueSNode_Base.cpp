// Fill out your copyright notice in the Description page of Project Settings.


#include "PZ_DialogueSNode_Base.h"
#include "PZ_DialogueEdNode_Base.h"


#include "SCommentBubble.h"
#include "DialogueEditor/Nodes/PZ_SDialogueGraphPin.h"

#include "EditorStyleSet.h"
#include "DialogueEditor/PZ_DialogueEditorStyle.h"





void SPZ_DialogueSNode_Base::Construct(const FArguments& InArgs, UPZ_DialogueEdNode_Base* InNode)
{
	GraphNode = InNode;
	InNode->SEdNode = this;

	this->SetCursor(EMouseCursor::CardinalCross);
	UpdateGraphNode();
}





void SPZ_DialogueSNode_Base::UpdateGraphNode()
{
	InputPins.Empty();
	OutputPins.Empty();

	RightNodeBox.Reset();
	LeftNodeBox.Reset();
	OutputPinBox.Reset();



	const float IconSize = 32.0f;
	const int32 IconOffsetCoef = 3;

	const float NodeBodyPadding = 20;


	//Create Border
	this->ContentScale.Bind(this, &SGraphNode::GetContentScale);
	this->GetOrAddSlot(ENodeZone::Center)
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SNew(SBorder)
			.BorderImage(GetBackgroundImage())
			.Padding(0)
			.BorderBackgroundColor(GetNodeBackgroundColor())
			[
				SNew(SOverlay)

				

				// INPUT PIN AREA
				+ SOverlay::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Top)
				[
					SAssignNew(LeftNodeBox, SVerticalBox)
				]



				// OUTPUT PIN AREA
				+ SOverlay::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Bottom)
				[

					SAssignNew(RightNodeBox, SVerticalBox)
					+ SVerticalBox::Slot()
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					.FillHeight(1.0f)
					[
						SAssignNew(OutputPinBox, SHorizontalBox)
					]
				]


				//NodeWidget
				+ SOverlay::Slot()
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				.Padding(NodeBodyPadding)
				[
					SAssignNew(NodeWidget, SOverlay)
				]



				//NodeImage
				+SOverlay::Slot()
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Top)
				.Padding(-IconSize / IconOffsetCoef, -IconSize / IconOffsetCoef, 0, 0)
				[
					SNew(SImage)
					.Image(GetNodeImage())
				]


				//RoleImage
				+ SOverlay::Slot()
				.HAlign(HAlign_Right)
				.VAlign(VAlign_Top)
				.Padding(0, -IconSize / IconOffsetCoef, -IconSize / IconOffsetCoef, 0)
				[
					SNew(SImage)
					.Image(GetRoleImage())
					.ColorAndOpacity(GetRoleImageColor())
				]


			]
		];


	CreateNodeWidget();
	CreateCommentBubble();
	CreatePinWidgets();
}

void SPZ_DialogueSNode_Base::CreateNodeWidget()
{
	TSharedPtr<SNodeTitle> NodeTitle = SNew(SNodeTitle, GraphNode);

	NodeWidget->AddSlot()
		[
			SNew(SBorder)
			.BorderImage(FEditorStyle::GetBrush("Graph.StateNode.ColorSpill"))
			.BorderBackgroundColor(GetNodeTitleColor())
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			.Visibility(EVisibility::SelfHitTestInvisible)
			[
				SNew(SHorizontalBox)

	
				+ SHorizontalBox::Slot()
				.Padding(4.0f, 4.0f)
				[
					SNew(SVerticalBox)

					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(STextBlock)
						.Text(GetNodeName())
						.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Bold.ttf"), 16))
					]


					+ SVerticalBox::Slot()
					[
						SNew(STextBlock)
						.Visibility(GetNodeAdditionalText().IsEmpty() ? EVisibility::Collapsed : EVisibility::Visible)
						.Text(GetNodeAdditionalText())
						.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Bold.ttf"), 8))
					]
				
				]

			]
		];

      

}

void SPZ_DialogueSNode_Base::CreateCommentBubble()
{
	TSharedPtr<SCommentBubble> CommentBubble;
	const FSlateColor CommentColor = FLinearColor::White;

	SAssignNew(CommentBubble, SCommentBubble)
		.GraphNode(GraphNode)
		.Text(this, &SGraphNode::GetNodeComment)
		.OnTextCommitted(this, &SGraphNode::OnCommentTextCommitted)
		.ColorAndOpacity(CommentColor)
		.AllowPinning(true)
		.EnableTitleBarBubble(true)
		.EnableBubbleCtrls(true)
		.GraphLOD(this, &SGraphNode::GetCurrentLOD)
		.IsGraphNodeHovered(this, &SGraphNode::IsHovered);

	GetOrAddSlot(ENodeZone::TopCenter)
		.SlotOffset(TAttribute<FVector2D>(CommentBubble.Get(), &SCommentBubble::GetOffset))
		.SlotSize(TAttribute<FVector2D>(CommentBubble.Get(), &SCommentBubble::GetSize))
		.AllowScaling(TAttribute<bool>(CommentBubble.Get(), &SCommentBubble::IsScalingAllowed))
		.VAlign(VAlign_Top)
		[
			CommentBubble.ToSharedRef()
		];
}



void SPZ_DialogueSNode_Base::CreatePinWidgets()
{
	UPZ_DialogueEdNode_Base* StateNode = CastChecked<UPZ_DialogueEdNode_Base>(GraphNode);

	for(const auto& LPin : StateNode->Pins)
	{
		if (!LPin->bHidden)
		{
			TSharedPtr<SGraphPin> NewPin = CreatePinWidget(LPin);
			AddPin(NewPin.ToSharedRef());
		}
	}
}

void SPZ_DialogueSNode_Base::CreateStandardPinWidget(UEdGraphPin* Pin)
{
	Super::CreateStandardPinWidget(Pin);
}

TSharedPtr<SGraphPin> SPZ_DialogueSNode_Base::CreatePinWidget(UEdGraphPin* Pin) const
{
	return SNew(SPZ_SDialogueGraphPin, Pin);
}

void SPZ_DialogueSNode_Base::AddPin(const TSharedRef<SGraphPin>& PinToAdd)
{
	PinToAdd->SetOwner(SharedThis(this));


	const UEdGraphPin* PinObj = PinToAdd->GetPinObj();
	const bool bAdvancedParameter = PinObj && PinObj->bAdvancedView;
	if (bAdvancedParameter)
	{
		PinToAdd->SetVisibility(TAttribute<EVisibility>(PinToAdd, &SGraphPin::IsPinVisibleAsAdvanced));
	}


	if (UPZ_DialogueEdNode_Base* LEdNode = Cast<UPZ_DialogueEdNode_Base>(GraphNode))
	{

		if (PinToAdd->GetDirection() == EEdGraphPinDirection::EGPD_Input)
		{
			LeftNodeBox->AddSlot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				.FillHeight(1.0f)
				.Padding(20.0f, 0.0f)
				[
					PinToAdd
				];
			InputPins.Add(PinToAdd);
			
		}
		else // Direction == EEdGraphPinDirection::EGPD_Output
		{
			OutputPinBox->AddSlot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				.FillWidth(1.0f)
				[
					PinToAdd
				];
			OutputPins.Add(PinToAdd);
		}


	}
}





bool SPZ_DialogueSNode_Base::IsNameReadOnly() const
{
	return false;
}







FText SPZ_DialogueSNode_Base::GetNodeName() const
{
	return FText::FromString("BaseNode");
}

FText SPZ_DialogueSNode_Base::GetNodeAdditionalText() const
{
	return FText::FromString("");
}


FLinearColor SPZ_DialogueSNode_Base::GetNodeTitleColor() const
{
	return FLinearColor(0.6f, 0.6f, 0.6f);
}

FLinearColor SPZ_DialogueSNode_Base::GetNodeBackgroundColor() const
{
	return FLinearColor::White;
}


const FSlateBrush* SPZ_DialogueSNode_Base::GetBackgroundImage() const
{
	return FEditorStyle::GetBrush("Graph.StateNode.Body");
}

const FSlateBrush* SPZ_DialogueSNode_Base::GetNodeImage() const
{
	return FPZ_DialogueEditorStyle::Get()->GetBrush(FPZ_DialogueEditorStyle::NodeIcon_DefaultIcon);
}

const FSlateBrush* SPZ_DialogueSNode_Base::GetRoleImage() const
{
	return nullptr;
}

const FLinearColor SPZ_DialogueSNode_Base::GetRoleImageColor() const
{
	return FLinearColor::White;
}



