// Fill out your copyright notice in the Description page of Project Settings.

#include "QuestCategoryWidget.h"


#include <WidgetTree.h>
#include <Components/TextBlock.h>
#include <Components/Button.h>
#include <Components/VerticalBox.h>
#include <Kismet/KismetTextLibrary.h>

void UQuestCategoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	m_pExpandButton = WidgetTree->FindWidget<UButton>(TEXT("ExpandButton"));
	m_pCategoryName = WidgetTree->FindWidget<UTextBlock>(TEXT("CategoryName"));
	m_pQuestBox     = WidgetTree->FindWidget<UVerticalBox>(TEXT("QuestBox"));

	m_pExpandButton->OnClicked.AddDynamic(this, &UQuestCategoryWidget::_OnClicked);

	FString EnumStr = CONVERT_TO_STRING(L"EQuestStates", m_Category);
	m_pCategoryName->SetText(FText::FromString(EnumStr));
	
}

void UQuestCategoryWidget::_OnClicked()
{
	m_bExpanded = !m_bExpanded;
	if (m_bExpanded)
	{
		m_pExpandButton->SetRenderAngle(0.f);
		m_pQuestBox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		m_pExpandButton->SetRenderAngle(-90.f);
		m_pQuestBox->SetVisibility(ESlateVisibility::Collapsed);
	}		
}

