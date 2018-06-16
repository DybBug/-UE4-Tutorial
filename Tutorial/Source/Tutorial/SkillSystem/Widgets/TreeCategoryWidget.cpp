// Fill out your copyright notice in the Description page of Project Settings.

#include "TreeCategoryWidget.h"
#include "SkillTreeWidget.h"


#include <Components/TextBlock.h>
#include <Components/Button.h>

void UTreeCategoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	m_pCategoryButton->OnClicked.AddDynamic(this, &UTreeCategoryWidget::_OnClicked);
	m_pCategoryButton->OnHovered.AddDynamic(this, &UTreeCategoryWidget::_OnHovered);
	m_pCategoryButton->OnUnhovered.AddDynamic(this, &UTreeCategoryWidget::_OnUnhovered);

}

void UTreeCategoryWidget::Initialize(FText _NewCategory, USkillTreeWidget* _pSkillTreeWidget, int _Index)
{
	m_Category         = _NewCategory;
	m_pSkillTreeWidget = _pSkillTreeWidget;
	m_Index            = _Index;

	m_pCategoryName->SetText(m_Category);
}

void UTreeCategoryWidget::ChangeState(bool _IsSelected)
{
	if (m_bIsSelected != _IsSelected)
	{
		m_bIsSelected = _IsSelected;

		FColor Color = m_bIsSelected ? FColor::White : FColor(0xFF666a6a);
		m_pCategoryName->SetColorAndOpacity(FLinearColor(Color));

		m_pCategoryButton->SetIsEnabled(!m_bIsSelected);
	}
}

void UTreeCategoryWidget::_OnClicked()
{
	m_pSkillTreeWidget->OnCategoryClicked(m_Index);
}

void UTreeCategoryWidget::_OnHovered()
{
	m_pCategoryName->SetColorAndOpacity(FLinearColor(FColor(0xFFd5d5d5)));
}

void UTreeCategoryWidget::_OnUnhovered()
{
	if (!m_bIsSelected)
	{
		m_pCategoryName->SetColorAndOpacity(FLinearColor(FColor(0xFF666a6a)));
	}	
}
