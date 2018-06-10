// Fill out your copyright notice in the Description page of Project Settings.

#include "SubGoalWidget.h"
#include "../Actors/QuestManager.h"
#include "../Actors/QuestActors/Quest_Base.h"
#include "QuestWidget.h"

#include <WidgetTree.h>


void USubGoalWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	m_pGoalBorder   = WidgetTree->FindWidget<UBorder>(TEXT("GoalBorder"));
	m_pSelectButton = WidgetTree->FindWidget<UButton>(TEXT("SelectButton"));
	m_pGoalText     = WidgetTree->FindWidget<UTextBlock>(TEXT("GoalText"));

	m_pSelectButton->OnClicked.AddDynamic(this, &USubGoalWidget::_OnSelectButtonClicked);

	Update();
}

void USubGoalWidget::Initialize(const FGoalInfo& _GoalInfo, AQuest_Base* _Quest, UQuestWidget* _Widget)
{
	m_GoalInfo = _GoalInfo;
	m_pAssignedQuest = _Quest;
	m_pQuestWidget = _Widget;
}

void USubGoalWidget::Update()
{
	m_HuntIndex = m_pAssignedQuest->GetCurrGoals().Find(m_GoalInfo);
	
	if (m_GoalInfo.bCustomGoal)
	{
		m_pGoalText->SetText(m_GoalInfo.GoalText);	
	}
	else
	{
		FText FormatText;

#define LOCTEXT_NAMESPACE "FormatText"
		switch (m_GoalInfo.Type)
		{
			case EGoalTypes::Custom :
			{
				FormatText = m_GoalInfo.GoalText;
				break;
			}
			case EGoalTypes::Hunt :
			{
				FormatText = FText::Format(LOCTEXT("HuntText", "Hunt {0}{1} : {2} / {3}!"), 
						m_GoalInfo.AdditionalName,
						m_GoalInfo.AmountToHunt > 1 ? FText::FromString(""): FText::FromString("s"),
						m_pAssignedQuest->GetCurrHuntedAmounts()[m_HuntIndex],
						m_GoalInfo.AmountToHunt);
				break;
			}
			case EGoalTypes::Find :
			{
				FormatText = FText::Format(LOCTEXT("FindText", "Find the {0}!"),
					m_GoalInfo.AdditionalName);	
				break;
			}
			case EGoalTypes::Talk :
			{
				FormatText = FText::Format(LOCTEXT("TalkText", "Talk to {0}!"),
					m_GoalInfo.AdditionalName);
				break;
			}
			default :
			{
				break;
			}
		}
#undef LOCTEXT_NAMESPACE

		m_pGoalText->SetText(FormatText);
	}

	m_GoalIndex = m_pAssignedQuest->GetQuestInfo().SubGoals.Find(m_GoalInfo);
}

void USubGoalWidget::_OnSelectButtonClicked()
{
	if (m_pQuestWidget->IsCurrQuest())
	{
		m_pQuestWidget->SelectSubGoal(this);
	}
	else
	{
		m_pQuestWidget->GetQuestManager()->SelectNewQuest(m_pAssignedQuest, this);
	}
}
