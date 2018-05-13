// Fill out your copyright notice in the Description page of Project Settings.

#include "SkillTreeWidget.h"
#include "../Components/SkillTreeComponent.h"
#include "../Characters/SkillCharacter.h"
#include "../Widgets/TreeCategoryWidget.h"
#include "../Widgets/SubTreeWidget.h"

#include <WidgetTree.h>
#include <Components/Button.h>
#include <Components/TextBlock.h>
#include <Components/ScrollBox.h>
#include <Components/WidgetSwitcher.h>
#include <Kismet/KismetTextLibrary.h>

void USkillTreeWidget::NativeConstruct()
{
	m_pCloseButton     = WidgetTree->FindWidget<UButton>("CloseButton");
	m_pSpText          = WidgetTree->FindWidget<UTextBlock>("SpText");
	m_pLevelText       = WidgetTree->FindWidget<UTextBlock>("LevelText");
	m_pCategoriesBox   = WidgetTree->FindWidget<UScrollBox>("CategoriesBox");
	m_pSubTreeSwitcher = WidgetTree->FindWidget<UWidgetSwitcher>("SubTreeSwitcher");

	m_pCloseButton->OnClicked.AddDynamic(this, &USkillTreeWidget::_OnCloseButtonClicked);
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

	for (int i = 0; i < m_TreeCategories.Num(); ++i)
	{
		UTreeCategoryWidget* pTreeCategoryWidget = CreateWidget<UTreeCategoryWidget>(GetWorld(), UTreeCategoryWidget::StaticClass());
		pTreeCategoryWidget->Initialize(FText::FromName(m_TreeCategories[i].Name), this, i);
		m_TreeCategoryWidgets.Add(pTreeCategoryWidget);
		m_pCategoriesBox->AddChild(pTreeCategoryWidget);

		USubTreeWidget* pSubTreeWidget = CreateWidget<USubTreeWidget>(GetWorld(), USubTreeWidget::StaticClass());
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

void USkillTreeWidget::_OnCloseButtonClicked()
{
	m_pSkillTree->HandleShowCommand();
}
