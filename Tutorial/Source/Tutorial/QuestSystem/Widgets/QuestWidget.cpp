// Fill out your copyright notice in the Description page of Project Settings.

#include "QuestWidget.h"
#include "SubGoalWidget.h"
#include "../Actors/QuestActors/Quest_Base.h"

#include <WidgetTree.h>
#include <Components/TextBlock.h>
#include <Components/VerticalBox.h>

void UQuestWidget::NativeConstruct()
{
	Super::NativeConstruct();

	m_pQuestName = WidgetTree->FindWidget<UTextBlock>(TEXT("QuestName"));
	m_pSubGoalBox = WidgetTree->FindWidget<UVerticalBox>(TEXT("SubGoalBox"));

}

void UQuestWidget::Initialize(AQuestManager * _pQuestManager, AQuest_Base* _pAssignedQuest)
{
	m_pQuestManager = _pQuestManager;
	m_pAssignedQuest = _pAssignedQuest;
}

void UQuestWidget::GenerateSubWidgets()
{
	m_SubGoalWidgets.Empty();

	m_pSubGoalBox->ClearChildren();

	for (int i = 0; i < m_pAssignedQuest->GetCurrGoals().Num(); ++i)
	{
		USubGoalWidget* pSubGoalWidget = CreateWidget<USubGoalWidget>(GetWorld(), USubGoalWidget::StaticClass());

		pSubGoalWidget->Initialize(m_pAssignedQuest->GetCurrGoals()[i], m_pAssignedQuest, this);

		m_SubGoalWidgets.Add(pSubGoalWidget);

		m_pSubGoalBox->AddChild(pSubGoalWidget);
	}
}

void UQuestWidget::UpdateQuest()
{
	if (m_pAssignedQuest)
	{
		m_pQuestName->SetText(m_pAssignedQuest->GetQuestInfo().Name);
		
		switch (m_pAssignedQuest->GetQuestInfo().Category)
		{
			case EQuestCategories::Main_Quest :
			{
				m_pQuestName->SetColorAndOpacity(FSlateColor(FColor(0xff5f00ff)));
				break;
			}
			case EQuestCategories::Side_Quest :
			{
				m_pQuestName->SetColorAndOpacity(FSlateColor(FColor(0x747affff)));
				break;
			}
			case EQuestCategories::Events :
			{
				m_pQuestName->SetColorAndOpacity(FSlateColor(FColor(0xff3122ff)));
				break;
			}
			default :
			{
				break;
			}

		}
		GenerateSubWidgets();
	}
}
