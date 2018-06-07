// Fill out your copyright notice in the Description page of Project Settings.

#include "Quest_Base.h"
#include "../../Widgets/QuestWidget.h"
#include "../../Widgets/SubGoalWidget.h"
#include "../../Widgets/QuestSystemHUD.h"
#include "../../Widgets/QuestJournalWidget.h"
#include "../QuestManager.h"


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
		m_CurrGoals.Add(m_QuestInfo.SubGoals[i]);
	}
}

void AQuest_Base::SetupStartingGoals()
{
	m_CurrGoalIndices.Empty();
	m_CurrGoalIndices = m_StartingSubGoalIndices;

	UpdateSubGoals();
	
	m_CurrDescription = m_QuestInfo.Description;

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

bool AQuest_Base::CompleteSubGoal(int _SubGoalIndex)
{
	if (m_CurrGoalIndices.Contains(_SubGoalIndex))
	{
		FGoalInfo CurrGoalInfo = m_QuestInfo.SubGoals[_SubGoalIndex];

		FCompletedGoal CompletedGoal(_SubGoalIndex, CurrGoalInfo, true);
		m_CompletedSubGoals.Add(CompletedGoal);

		if (CurrGoalInfo.bUpdateQuestDescription)
		{
#define LOCTEXT_NAMESPACE "Description"
			FText Format = FText::Format(LOCTEXT("Description", "{0} {1}"), m_CurrDescription, CurrGoalInfo.UpdateDescription);
#undef LOCTEXT_NAMESPACE
			m_CurrDescription = Format;

			if (SelectedInJournal())
			{
				m_pQuestManager->GetHUD()->GetQuestJournalWidget()->UpdateDescription();
			}
		}

		int CurrWidgetIndex = m_CurrGoalIndices.Find(_SubGoalIndex);

		m_CurrGoalIndices.Remove(_SubGoalIndex);

		m_pQuestWidget->GetSubGoalWidgets()[CurrWidgetIndex]->RemoveFromParent();
		m_pQuestWidget->GetSubGoalWidgets().RemoveAt(CurrWidgetIndex);

		for (int i = 0; i < CurrGoalInfo.FollowingSubGoalIndices.Num(); ++i)
		{
			int FollowingSubGoalIndex = CurrGoalInfo.FollowingSubGoalIndices[i];

			m_CurrGoalIndices.Add(FollowingSubGoalIndex);

			FGoalInfo SubGoal = m_QuestInfo.SubGoals[FollowingSubGoalIndex];
			m_CurrGoals.Add(SubGoal);

			UClass* pWidget = LoadClass<USubGoalWidget>(nullptr, TEXT("WidgetBlueprint'/Game/TutorialContent/QuestSystem/Widgets/WB_SubGoal.WB_SubGoal_C'"));
			USubGoalWidget* pSubGoalWidget = CreateWidget<USubGoalWidget>(GetWorld(), pWidget);
			pSubGoalWidget->Initialize(SubGoal, this, m_pQuestWidget);

			m_pQuestWidget->GetSubGoalWidgets().Add(pSubGoalWidget);
			m_pQuestWidget->GetSubGoalBox()->AddChild(pSubGoalWidget);
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
