// Fill out your copyright notice in the Description page of Project Settings.

#include "SaveLoadWidget.h"
#include "SaveSlotWidget.h"
#include "../Characters/QuestCharacter.h"

#include <Components/TextBlock.h>
#include <Components/ScrollBox.h>
#include <Components/Button.h>


void USaveLoadWidget::NativeConstruct()
{
	Super::NativeConstruct();

	m_pNewContinueButton->OnClicked.AddDynamic(this, &USaveLoadWidget::_OnNewContinueButtonClicked);

	GenerateSlots();
	Update();
}

void USaveLoadWidget::Initialize(AQuestCharacter * _pPlayer, int _AmountOfSlots, bool _bLoad)
{
	m_pPlayer = _pPlayer;
	m_AmountOfSlots = _AmountOfSlots;
	m_bLoad = _bLoad;
}

void USaveLoadWidget::GenerateSlots()
{
	m_AllSlots.Empty();
	m_pSlotBox->ClearChildren();

	UClass* WidgetClass = LoadClass<USaveSlotWidget>(nullptr, TEXT("WidgetBlueprint'/Game/TutorialContent/QuestSystem/Widgets/WB_SaveSlot.WB_SaveSlot_C'"));
	for (int i = 0; i < m_AmountOfSlots; ++i)
	{
		USaveSlotWidget* pSaveSlotWidget = CreateWidget<USaveSlotWidget>(GetWorld(), WidgetClass);
		pSaveSlotWidget->Initialize(i, this);

		m_AllSlots.Add(pSaveSlotWidget);

		m_pSlotBox->AddChild(pSaveSlotWidget);
	}
}

void USaveLoadWidget::Update()
{
	FString Str;

	Str = m_bLoad ? 
		"Choose a slot to load your game or start a new one!" : 
		"Select a slot to save your game!";
	m_pText->SetText(FText::FromString(Str));

	Str = m_bLoad ? "NewGame" : "Continue";
	m_pButtonText->SetText(FText::FromString(Str));

	for (auto& pSlot : m_AllSlots)
	{
		pSlot->Update();
	}
}

void USaveLoadWidget::_OnNewContinueButtonClicked()
{
	if (m_bLoad)
	{
		m_pPlayer->StartGame(false, 0);
	}
	else
	{
		m_pPlayer->ContinueGame();
	}
}
