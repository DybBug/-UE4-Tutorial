// Fill out your copyright notice in the Description page of Project Settings.

#include "SaveSlotWidget.h"
#include "SaveLoadWidget.h"
#include "../Characters/QuestCharacter.h"
#include "../SaveGames/CharacterSave.h"

#include <Kismet/GameplayStatics.h>
#include <Components/CanvasPanel.h>
#include <Components/Button.h>
#include <Components/TextBlock.h>

void USaveSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	m_pButton->OnClicked.AddDynamic(this, &USaveSlotWidget::_OnButtonClicked);
}


void USaveSlotWidget::Initialize(int _SlotIndex, USaveLoadWidget* _pWidget)
{
	m_SlotIndex = _SlotIndex;
	m_pLoadWidget = _pWidget;
}

void USaveSlotWidget::Update()
{
	FString SlotName = m_pLoadWidget->GetPlayer()->GetDefaultSlotName();
	SlotName.Append(FString::FromInt(m_SlotIndex));

	if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		m_pSaveGame = Cast<UCharacterSave>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
		
		if (m_pSaveGame)
		{
			m_pLevelText->SetText(FText::AsNumber(m_pSaveGame->GetSavedLevel()));

			FString Region = CONVERT_TO_STRING(L"ERegions", m_pSaveGame->GetSavedRegion());
			m_pRegionText->SetText(FText::FromString(Region));

			FString Hour("0");
			if (m_pSaveGame->GetSavedHour() < 10)
			{
				Hour.Append(FString::FromInt(m_pSaveGame->GetSavedHour()));
			}
			else
			{
				Hour = FString::FromInt(m_pSaveGame->GetSavedHour());
			}

			FString Minute("0");
			if (m_pSaveGame->GetSavedMinute() < 10)
			{
				Minute.Append(FString::FromInt(m_pSaveGame->GetSavedMinute()));
			}
			else
			{
				Minute = FString::FromInt(m_pSaveGame->GetSavedMinute());
			}

#define LOCTEXT_NAMESPACE "TimeText"		
			FText TimeText = FText::Format(LOCTEXT("TimeText", "{0}:{1}"), FText::FromString(Hour), FText::FromString(Minute));
#undef LOCTEXT_NAMESPACE
			m_pTimeText->SetText(TimeText);

			m_pButton->SetIsEnabled(true);
			m_pCanvas->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
	}
	else
	{
		m_pButton->SetIsEnabled(!m_pLoadWidget->GetLoad());
		m_pCanvas->SetVisibility(ESlateVisibility::Hidden);
		m_pSaveGame = nullptr;
	}
}

void USaveSlotWidget::_OnButtonClicked()
{
	if (m_pLoadWidget->GetLoad())
	{
		m_pLoadWidget->GetPlayer()->StartGame(true, m_SlotIndex);
	}
	else
	{
		m_pLoadWidget->GetPlayer()->SaveGameToSlot(m_SlotIndex);
		Update();
	}
}

