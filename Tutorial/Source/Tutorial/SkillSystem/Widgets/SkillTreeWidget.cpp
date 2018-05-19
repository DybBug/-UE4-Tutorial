// Fill out your copyright notice in the Description page of Project Settings.

#include "SkillTreeWidget.h"
#include "../Components/SkillTreeComponent.h"
#include "../Characters/SkillCharacter.h"
#include "../Widgets/TreeCategoryWidget.h"
#include "../Widgets/SubTreeWidget.h"
#include "../DragDropOperations/WidgetDragDropOperation.h"

#include <WidgetTree.h>
#include <Components/Button.h>
#include <Components/TextBlock.h>
#include <Components/ScrollBox.h>
#include <Components/WidgetSwitcher.h>
#include <Kismet/KismetTextLibrary.h>
#include <Blueprint/WidgetBlueprintLibrary.h>
#include <Blueprint/SlateBlueprintLibrary.h>


bool USkillTreeWidget::Initialize()
{
	bool Result = Super::Initialize();
	m_pCloseButton     = WidgetTree->FindWidget<UButton>("CloseButton");
	m_pSpText          = WidgetTree->FindWidget<UTextBlock>("SpText");
	m_pLevelText       = WidgetTree->FindWidget<UTextBlock>("LevelText");
	m_pCategoriesBox   = WidgetTree->FindWidget<UScrollBox>("CategoriesBox");
	m_pSubTreeSwitcher = WidgetTree->FindWidget<UWidgetSwitcher>("SubTreeSwitcher");

	m_pCloseButton->OnClicked.AddDynamic(this, &USkillTreeWidget::_OnCloseButtonClicked);

	return Result;
}

void USkillTreeWidget::Initialize(USkillTreeComponent * _pAssignedSkillTree)
{
	m_pSkillTree = _pAssignedSkillTree;
	m_TreeCategories = m_pSkillTree->GetCategories();

	UpdateSP();
	UpdateLevel();
	GenerateCategories();
	OnCategoryClicked(0);
}

void USkillTreeWidget::UpdateSP()
{	
	m_pSpText->SetText(UKismetTextLibrary::Conv_IntToText(m_pSkillTree->GetSkillPoints()));
}

void USkillTreeWidget::UpdateLevel()
{	
	m_pLevelText->SetText(UKismetTextLibrary::Conv_IntToText(m_pSkillTree->GetPlayer()->GetCurrLevel()));
}

void USkillTreeWidget::GenerateCategories()
{
	m_pCategoriesBox->ClearChildren();
	m_pSubTreeSwitcher->ClearChildren();
	m_TreeCategoryWidgets.Empty();
	m_SubTreeWidgets.Empty();

	UClass* TreeCategoryWidgetClass = LoadClass<UTreeCategoryWidget>(nullptr, TEXT("WidgetBlueprint'/Game/TutorialContent/SkillSystem/Widgets/WBP_TreeCategory.WBP_TreeCategory_C'"));
	UClass* SubTreeWidgetClass = LoadClass<USubTreeWidget>(nullptr, TEXT("WidgetBlueprint'/Game/TutorialContent/SkillSystem/Widgets/WBP_SubTree.WBP_SubTree_C'"));
	for (int i = 0; i < m_TreeCategories.Num(); ++i)
	{
		UTreeCategoryWidget* pTreeCategoryWidget = CreateWidget<UTreeCategoryWidget>(GetWorld(), TreeCategoryWidgetClass);
		pTreeCategoryWidget->Initialize(FText::FromName(m_TreeCategories[i].Name), this, i);
		m_TreeCategoryWidgets.Add(pTreeCategoryWidget);
		m_pCategoriesBox->AddChild(pTreeCategoryWidget);

		USubTreeWidget* pSubTreeWidget = CreateWidget<USubTreeWidget>(GetWorld(), SubTreeWidgetClass);
		pSubTreeWidget->Initialize(m_TreeCategories[i].Content, this);
		m_SubTreeWidgets.Add(pSubTreeWidget);
		m_pSubTreeSwitcher->AddChild(pSubTreeWidget);
	}

}

void USkillTreeWidget::OnCategoryClicked(int _Index)
{
	if (_Index != m_CurrSelectedIndex)
	{
		if (m_CurrSelectedIndex == -1)
		{
			m_pSubTreeSwitcher->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		else
		{
			m_TreeCategoryWidgets[m_CurrSelectedIndex]->ChangeState(false);
		}

		m_CurrSelectedIndex = _Index;
		m_TreeCategoryWidgets[m_CurrSelectedIndex]->ChangeState(true);
		m_pSubTreeSwitcher->SetActiveWidgetIndex(m_CurrSelectedIndex);		
	}
}

FReply USkillTreeWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Result = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	FEventReply Event = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
	
	Result = Event.NativeReply;

	return Result;
}

void USkillTreeWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	FVector2D Offset = USlateBlueprintLibrary::AbsoluteToLocal(InGeometry, InMouseEvent.GetScreenSpacePosition());
	UWidgetDragDropOperation* pWidgetOper = Cast<UWidgetDragDropOperation>(UWidgetBlueprintLibrary::CreateDragDropOperation(UWidgetDragDropOperation::StaticClass()));
	pWidgetOper->Initialize(this, Offset);
	pWidgetOper->DefaultDragVisual = this;
	//pWidgetOper->Offset = FVector2D(0.f, 0.f);
	pWidgetOper->Pivot = EDragPivot::MouseDown;

	RemoveFromParent();

	OutOperation = pWidgetOper;	
}


void USkillTreeWidget::_OnCloseButtonClicked()
{
	m_pSkillTree->HandleShowCommand();
}
