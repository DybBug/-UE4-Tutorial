// Fill out your copyright notice in the Description page of Project Settings.

#include "Quest_Base.h"

#include <Kismet/KismetMathLibrary.h>

// Sets default values
AQuest_Base::AQuest_Base()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	m_StartingSubGoalIndices.SetNum(1);

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
