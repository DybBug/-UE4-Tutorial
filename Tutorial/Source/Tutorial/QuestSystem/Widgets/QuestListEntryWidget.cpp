// Fill out your copyright notice in the Description page of Project Settings.

#include "QuestListEntryWidget.h"
#include "QuestJournalWidget.h"
#include "../Characters/QuestCharacter.h"
#include "../Actors/QuestActors/Quest_Base.h"


#include <Components/Button.h>
#include <Components/TextBlock.h>

void UQuestListEntryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	m_pQuestButton->OnClicked.AddDynamic(this, &UQuestListEntryWidget::_OnClicked);

	Update();
}

void UQuestListEntryWidget::Initialize(UQuestJournalWidget* _pWidget, AQuest_Base* _pAssignedQuest)
{
	m_pJournalWidget = _pWidget;
	m_pAssignedQuest = _pAssignedQuest;
}

void UQuestListEntryWidget::UpdateLevelColor()
{
	FLinearColor Color;
	if (m_pJournalWidget->GetPlayer()->GetCurrLevel() > m_pAssignedQuest->GetQuestInfo().SuggestedLevel)
	{
		Color = FLinearColor(FColor(0xff27b700));		
	}
	else
	{		
		if(m_pJournalWidget->GetPlayer()->GetCurrLevel() == m_pAssignedQuest->GetQuestInfo().SuggestedLevel)
		{ 
			Color = FLinearColor::White;
		}
		else
		{
			Color = FLinearColor(FColor(0xffb80a00));

		}
	}
	m_pSuggestedLevel->SetColorAndOpacity(Color);
}

void UQuestListEntryWidget::Update()
{
	const FQuestInfo& QuestInfo =  m_pAssignedQuest->GetQuestInfo();
	// 퀘스트 이름 업데이트.
	FText QuestName = QuestInfo.Name;

	if (QuestName.ToString().Len() > 26)
	{
		FString QuestNameStr = QuestName.ToString().LeftChop(23).Append("...");
		m_pQuestName->SetText(FText::FromString(QuestNameStr));
	}
	else
	{
		m_pQuestName->SetText(QuestName);
	}

	// 지역 이름 업데이트.
	FString RegionName = CONVERT_TO_STRING(L"ERegions" ,QuestInfo.Region);
	m_pRegionName->SetText(FText::FromString(RegionName));

	// 제안 레벨 업데이트.
	m_pSuggestedLevel->SetText(FText::AsNumber(QuestInfo.SuggestedLevel));

	UpdateLevelColor();

}

void UQuestListEntryWidget::_OnClicked()
{
	m_pJournalWidget->OnQuestClicked(this);
}


