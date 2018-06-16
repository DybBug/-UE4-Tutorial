// Fill out your copyright notice in the Description page of Project Settings.

#include "Quest_Base.h"
#include "../../Widgets/QuestWidget.h"
#include "../../Widgets/SubGoalWidget.h"
#include "../../Widgets/QuestSystemHUD.h"
#include "../../Widgets/QuestJournalWidget.h"
#include "../../Widgets/QuestListEntryWidget.h"
#include "../QuestManager.h"
#include "../../Characters/QuestCharacter.h"


#include <Kismet/KismetMathLibrary.h>
#include <Kismet/KismetArrayLibrary.h>

// Sets default values
AQuest_Base::AQuest_Base()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	m_StartingSubGoalIndices.SetNum(1);

}

void AQuest_Base::Initialize(AQuestManager* _pQuestManager)
{
	m_pQuestManager = _pQuestManager;
}

void AQuest_Base::UpdateSubGoals()
{
	m_CurrGoals.Empty();

	for (int i = 0; i < m_CurrGoalIndices.Num(); ++i)
	{
		m_CurrGoals.Add(m_QuestInfo.SubGoals[m_CurrGoalIndices[i]]);
	}
}

void AQuest_Base::SetupStartingGoals()
{
	m_CurrGoalIndices.Empty();

	m_CurrDescription = m_QuestInfo.Description;

	for(int i = 0; i < m_StartingSubGoalIndices.Num(); ++i)
	{
		AddGoalForIndex(m_StartingSubGoalIndices[i]);
	}

	m_CurrHuntedAmounts.SetNum(m_CurrGoalIndices.Num());

	UpdateSubGoals();	
}

bool AQuest_Base::GoToNextSubGoals()
{
	int IndexOfMaxValue;
	int MaxValue;

	// 배열에서 가장 큰 정수값 가져오기.
	UKismetMathLibrary::MaxOfIntArray(m_CurrGoalIndices, IndexOfMaxValue, MaxValue);
	int NextIndex = MaxValue + 1;

	if (NextIndex <= (m_QuestInfo.SubGoals.Num() - 1))
	{	
		m_CurrGoalIndices.Empty();
		m_CurrGoalIndices.Add(NextIndex);

		UpdateSubGoals();
		return true;
	}

	return false;
}

bool AQuest_Base::CompleteSubGoal(int _SubGoalIndex, bool _bFail)
{
	if (m_CurrGoalIndices.Contains(_SubGoalIndex))
	{
		FGoalInfo CurrGoalInfo = m_QuestInfo.SubGoals[_SubGoalIndex];

		FCompletedGoal CompletedGoal(_SubGoalIndex, CurrGoalInfo, !_bFail);
		m_CompletedSubGoals.Add(CompletedGoal);

		if (CurrGoalInfo.bUpdateQuestDescription && !_bFail)
		{
#define LOCTEXT_NAMESPACE "Description"
			FText Format = FText::Format(LOCTEXT("Description", "{0} \n {1}"), m_CurrDescription, CurrGoalInfo.UpdateDescription);
#undef LOCTEXT_NAMESPACE
			m_CurrDescription = Format;

			if (SelectedInJournal())
			{
				m_pQuestManager->GetHUD()->GetQuestJournalWidget()->UpdateDescription();
			}
		}

		int CurrWidgetIndex = m_CurrGoalIndices.Find(_SubGoalIndex);

		m_CurrHuntedAmounts.RemoveAt(CurrWidgetIndex);

		m_CurrGoalIndices.Remove(_SubGoalIndex);
		m_CurrGoals.RemoveAt(CurrWidgetIndex);

		m_pQuestWidget->GetSubGoalWidgets()[CurrWidgetIndex]->RemoveFromParent();
		m_pQuestWidget->GetSubGoalWidgets().RemoveAt(CurrWidgetIndex);

		if (_bFail)
		{
			OnGoalFailed(_SubGoalIndex);

			if (CurrGoalInfo.bFailMeansQuestFail)
			{
				_EndQuest(_bFail);
				return true;
			}
		}
		else
		{
			OnGoalCompleted(_SubGoalIndex);

			if (CurrGoalInfo.bCompletesQuest)
			{
				_EndQuest(_bFail);
				return true;
			}
			else
			{
				for (int i = 0; i < CurrGoalInfo.FollowingSubGoalIndices.Num(); ++i)
				{
					AddGoalForIndex(CurrGoalInfo.FollowingSubGoalIndices[i]);
				}
			}
		}

		if (SelectedInJournal())
		{
			m_pQuestManager->GetHUD()->GetQuestJournalWidget()->GenerateSubGoals();
		}

		if ((_SubGoalIndex == m_SelectedSubGoalIndex) && (m_pQuestManager->GetCurrQuest() == this))
		{
			m_pQuestWidget->SelectSubGoal(m_pQuestWidget->GetSubGoalWidgets()[0]);
		}

		return true;
	}
	return false;
}

bool AQuest_Base::SelectedInJournal()
{
	if (m_pQuestManager->GetHUD()->GetQuestJournalWidget()->GetSelectedQuest() != nullptr &&
		m_pQuestManager->GetHUD()->GetQuestJournalWidget()->GetSelectedQuest() == this)
	{
		return true;
	}
	return false;
}

void AQuest_Base::OnGoalCompleted(int _GoalIndex)
{
}

void AQuest_Base::OnGoalFailed(int _GoalIndex)
{
}

FGoalInfo AQuest_Base::GoalAtIndex(int _Index)
{
	return m_QuestInfo.SubGoals[_Index];
}

bool AQuest_Base::GoalAlreadyFound(int _GoalIndex)
{
	bool LoclBool = false;

	if (GoalAtIndex(_GoalIndex).Type == EGoalTypes::Find)
	{
		TArray<TSubclassOf<AObject_Base>> ObtainObjectClasses = m_pQuestManager->GetPlayer()->GetObtainedObjectClasses();
		for (int i = 0; i < ObtainObjectClasses.Num(); ++i)
		{
			if (ObtainObjectClasses[i] == GoalAtIndex(_GoalIndex).GoalClass)
			{
				LoclBool = true;
				break;
			}
		}		

		return LoclBool;
	}

	return false;
}

void AQuest_Base::AddGoalForIndex(int _Index)
{
	if (GoalAlreadyFound(_Index))
	{
		FCompletedGoal CompletedGoal(_Index, GoalAtIndex(_Index), true);
		m_CompletedSubGoals.Add(CompletedGoal);

		OnGoalCompleted(_Index);

		if (GoalAtIndex(_Index).bUpdateQuestDescription)
		{
#define LOCTEXT_NAMESPACE "Description"
			FText Format = FText::Format(LOCTEXT("Description", "{0} \n {1}"), m_CurrDescription, GoalAtIndex(_Index).UpdateDescription);
#undef LOCTEXT_NAMESPACE
			m_CurrDescription = Format;

			if (SelectedInJournal())
			{
				m_pQuestManager->GetHUD()->GetQuestJournalWidget()->UpdateDescription();
			}
		}

		for (int i = 0; i < GoalAtIndex(_Index).FollowingSubGoalIndices.Num(); ++i)
		{
			AddGoalForIndex(GoalAtIndex(_Index).FollowingSubGoalIndices[i]);
		}
	}
	else
	{
		m_CurrGoalIndices.Add(_Index);

		if (m_CurrGoalIndices.Last() > (m_CurrHuntedAmounts.Num() - 1))
		{
			m_CurrHuntedAmounts.Add(0);
		}			

		FGoalInfo SubGoal = GoalAtIndex(_Index);
		m_CurrGoals.Add(SubGoal);

		UClass* pWidget = LoadClass<USubGoalWidget>(nullptr, TEXT("WidgetBlueprint'/Game/TutorialContent/QuestSystem/Widgets/WB_SubGoal.WB_SubGoal_C'"));
		USubGoalWidget* pSubGoalWidget = CreateWidget<USubGoalWidget>(GetWorld(), pWidget);
		pSubGoalWidget->Initialize(SubGoal, this, m_pQuestWidget);

		m_pQuestWidget->GetSubGoalWidgets().Add(pSubGoalWidget);
		m_pQuestWidget->GetSubGoalBox()->AddChild(pSubGoalWidget);
	}
}

void AQuest_Base::_EndQuest(bool _bFail)
{
	for (int i = 0; i < m_CurrGoalIndices.Num(); ++i)
	{
		m_CompletedSubGoals.Add(FCompletedGoal(m_CurrGoalIndices[i], GoalAtIndex(m_CurrGoalIndices[i]), false));
	}

	m_CurrGoalIndices.Empty();
	m_CurrHuntedAmounts.Empty();
	m_CurrGoals.Empty();
	m_CurrState = _bFail ? EQuestStates::Failed_Quests : EQuestStates::Completed_Quests;
	m_pListEntryWidget->RemoveFromParent();
	m_pQuestWidget->RemoveFromParent();
	m_pQuestManager->EndQuest(this);
}
