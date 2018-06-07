// Fill out your copyright notice in the Description page of Project Settings.

#include "GoalEntryWidget.h"
#include "QuestJournalWidget.h"
#include "../Actors/QuestActors/Quest_Base.h"

#include <WidgetTree.h>
#include <Components/Image.h>
#include <Components/TextBlock.h>

void UGoalEntryWidget::Initialize(const FGoalInfo& _Info, const EGoalState& _State, UQuestJournalWidget* _pWidget)
{
	m_GoalInfo = _Info;
	m_State = _State;
	m_pJournalWidget = _pWidget;
}

void UGoalEntryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	m_pGoalState = WidgetTree->FindWidget<UImage>(TEXT("GoalState"));
	m_pGoalText = WidgetTree->FindWidget<UTextBlock>(TEXT("GoalText"));

	_Update();
}


void UGoalEntryWidget::_Update()
{
	FText FormatText;

	if (m_GoalInfo.bCustomGoal)
	{
		FormatText = m_GoalInfo.GoalText;
	}
	else
	{
#define LOCTEXT_NAMESPACE "FormatText"
			switch (m_GoalInfo.Type)
			{
				case EGoalTypes::Custom:
				{
					FormatText = m_GoalInfo.GoalText;
					break;
				}
				case EGoalTypes::Hunt:
				{
					FormatText = FText::Format(LOCTEXT("HuntText", "Hunt {0}{1} : {2} / {3}!"),
						m_GoalInfo.AdditionalName,
						m_GoalInfo.AmountToHunt > 1 ? FText::FromString("") : FText::FromString("s"),
						m_pJournalWidget->GetSelectedQuest()->GetCurrHuntedAmount(),
						m_GoalInfo.AmountToHunt);
					break;
				}
				case EGoalTypes::Find:
				{
					FormatText = FText::Format(LOCTEXT("FindText", "Find the {0}!"),
						m_GoalInfo.AdditionalName);
					break;
				}
				case EGoalTypes::Talk:
				{
					FormatText = FText::Format(LOCTEXT("TalkText", "Talk to {0}!"),
						m_GoalInfo.AdditionalName);
					break;
				}
				default:
				{
					break;
				}
			}
#undef LOCTEXT_NAMESPACE
	}

	m_pGoalText->SetText(FormatText);

	UTexture2D* pTexture;
	switch (m_State)
	{
		case EGoalState::Current :
		{
			pTexture = LoadObject<UTexture2D>(nullptr, TEXT("Texture2D'/Game/TutorialContent/QuestSystem/Textures/CurrentGoalIcon.CurrentGoalIcon'"));
			m_pGoalState->SetBrushFromTexture(pTexture);
			break;
		}
		case EGoalState::Success :
		{
			pTexture = LoadObject<UTexture2D>(nullptr, TEXT("Texture2D'/Game/TutorialContent/QuestSystem/Textures/CompletedGoalIcon.CompletedGoalIcon'"));
			m_pGoalState->SetBrushFromTexture(pTexture);
			break;
		}
		case EGoalState::Failed :
		{
			pTexture = LoadObject<UTexture2D>(nullptr, TEXT("Texture2D'/Game/TutorialContent/QuestSystem/Textures/FailedGoalIcon.FailedGoalIcon'"));
			m_pGoalState->SetBrushFromTexture(pTexture);
			break;
		}
		default :
		{
			break;
		}
	}
	
	
}

