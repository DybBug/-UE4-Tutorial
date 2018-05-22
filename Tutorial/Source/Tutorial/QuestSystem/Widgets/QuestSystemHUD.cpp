// Fill out your copyright notice in the Description page of Project Settings.

#include "QuestSystemHUD.h"
#include "QuestWidget.h"

#include <WidgetTree.h>
#include <Components/ScrollBoxSlot.h>



void UQuestSystemHUD::NativeConstruct()
{
	Super::NativeConstruct();

	m_pExpBar        = WidgetTree->FindWidget<UProgressBar>(TEXT("ExpBar"));
	m_pCharacterIcon = WidgetTree->FindWidget<UImage>(TEXT("CharacterIcon"));
	m_pExpText       = WidgetTree->FindWidget<UTextBlock>(TEXT("ExpText"));
	m_pLevelText     = WidgetTree->FindWidget<UTextBlock>(TEXT("LevelText"));
}


UQuestWidget* UQuestSystemHUD::AddQuestToList(AQuest_Base* _pQuestActor)
{
	if (_pQuestActor)
	{
		UQuestWidget* pQuestWidget =  CreateWidget<UQuestWidget>(GetWorld(), UQuestWidget::StaticClass());
		pQuestWidget->Initialize(m_pQuestManager, _pQuestActor);

		m_QuestWidgets.Add(pQuestWidget);
		UPanelSlot* pSlot = m_pQuestList->AddChild(pQuestWidget);

		UScrollBoxSlot* pScrollBoxSlot = Cast<UScrollBoxSlot>(pSlot);
		pScrollBoxSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
		pScrollBoxSlot->SetPadding(FMargin(0.f, 8.f, 0.f, 0.f));

		return pQuestWidget;
	}
	return nullptr;
}
