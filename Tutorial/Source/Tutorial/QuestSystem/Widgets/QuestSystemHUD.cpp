// Fill out your copyright notice in the Description page of Project Settings.

#include "QuestSystemHUD.h"
#include "QuestWidget.h"

#include <WidgetTree.h>
#include <Components/ScrollBoxSlot.h>



void UQuestSystemHUD::NativeConstruct()
{
	Super::NativeConstruct();

	m_pExpBar         = WidgetTree->FindWidget<UProgressBar>(TEXT("ExpBar"));
	m_pCharacterIcon  = WidgetTree->FindWidget<UImage>(TEXT("CharacterIcon"));
	m_pExpText        = WidgetTree->FindWidget<UTextBlock>(TEXT("ExpText"));
	m_pLevelText      = WidgetTree->FindWidget<UTextBlock>(TEXT("LevelText"));
	m_pQuestList      = WidgetTree->FindWidget<UScrollBox>(TEXT("QuestList"));
	m_pDistanceBorder = WidgetTree->FindWidget<UBorder>(TEXT("DistanceBorder"));
	m_pDistanceText   = WidgetTree->FindWidget<UTextBlock>(TEXT("DistanceText"));
	m_pMiniMapWidget  = WidgetTree->FindWidget<UMiniMapWidget>(TEXT("WB_MiniMap"));
}


UQuestWidget* UQuestSystemHUD::AddQuestToList(AQuest_Base* _pQuestActor)
{
	if (_pQuestActor)
	{
		UClass* pQuestWidgetClass = LoadClass<UQuestWidget>(nullptr, TEXT("WidgetBlueprint'/Game/TutorialContent/QuestSystem/Widgets/WB_Quest.WB_Quest_C'"));
		UQuestWidget* pQuestWidget =  CreateWidget<UQuestWidget>(GetWorld(), pQuestWidgetClass);
		
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
