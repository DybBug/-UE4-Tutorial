// Fill out your copyright notice in the Description page of Project Settings.

#include "QuestManager.h"
#include "GoalActor.h"
#include "QuestActors/Quest_Base.h"
#include "../Characters/QuestCharacter.h"
#include "../Widgets/QuestWidget.h"
#include "../Widgets/MiniMapWidget.h"
#include "../Widgets/QuestSystemHUD.h"
#include "../Widgets/QuestJournalWidget.h"
#include "../Widgets/SubgoalWidget.h"

#include <Engine/World.h>
#include <Kismet/KismetMathLibrary.h>
#include <Kismet/KismetTextLibrary.h>
#include <Kismet/GameplayStatics.h>
#include <UObject/ConstructorHelpers.h>



// Sets default values
AQuestManager::AQuestManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FClassFinder<AGoalActor> GoalActorClass(TEXT("Blueprint'/Game/TutorialContent/QuestSystem/Actors/BP_GoalActor.BP_GoalActor_C'"));
	if (GoalActorClass.Succeeded())
	{
		m_GoalActorClass = GoalActorClass.Class;
	}

}

void AQuestManager::BeginPlay()
{
	Super::BeginPlay();

	m_pPlayer = Cast<AQuestCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

void AQuestManager::OnSwitchSubQuest()
{
	if (m_pCurrGoalActor)
	{
		m_pCurrGoalActor->Destroy();
	}
	

	int Index = m_pCurrQuest->GetSelectedSubGoalIndex();
	m_CurrSubGoal = m_pCurrQuest->GetQuestInfo().SubGoals[Index];

	if (m_CurrSubGoal.GoalLocation.bHasLocation)
	{
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		m_pCurrGoalActor = GetWorld()->SpawnActor<AGoalActor>(m_GoalActorClass, m_CurrSubGoal.GoalLocation.Location, FRotator(0.f, 0.f, 0.f), Params);
		m_pCurrGoalActor->Initialize(m_CurrSubGoal.bUseRadius, m_CurrSubGoal.Radius, m_CurrSubGoal.CircleColor);
		
		m_CurrDistance = DistanceToGoal();
		m_pHUD->GetDistanceText()->SetText(UKismetTextLibrary::Conv_IntToText(m_CurrDistance));

		UpdateDirectionArrow();	
		
		ESlateVisibility Visibility = (m_CurrDistance > m_ShowDirectionArrowAmount) ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden;
		m_pHUD->GetMiniMapWidget()->GetDirectionArrow()->SetVisibility(Visibility);
		m_pHUD->GetDistanceBorder()->SetVisibility(ESlateVisibility::HitTestInvisible);

		
	}
	else
	{
		m_pCurrGoalActor = nullptr;
		m_pHUD->GetDistanceBorder()->SetVisibility(ESlateVisibility::Hidden);
		m_pHUD->GetMiniMapWidget()->GetDirectionArrow()->SetVisibility(ESlateVisibility::Hidden);
	}

}

void AQuestManager::SelectNewQuest(AQuest_Base * _pQuest, USubGoalWidget * _pSubGoal)
{
	if (m_pCurrQuest)
	{
		m_pCurrQuest->GetQuestWidget()->GetQuestName()->SetIsEnabled(false);
		m_pCurrQuest->GetQuestWidget()->SelectSubGoal(nullptr);
		m_pCurrQuest->GetQuestWidget()->SetSelectedSubGoalWidget(nullptr);	
	}

	m_pCurrQuest = _pQuest;
	m_pCurrQuest->GetQuestWidget()->OnQuestSelected(_pSubGoal);
}

bool AQuestManager::AddNewQuest(TSubclassOf<AQuest_Base> _NewQuestClass, bool _bDirectlyStart)
{
	TSubclassOf<AQuest_Base> LocClass = _NewQuestClass;
	bool bLocStart = _bDirectlyStart;

	if (_NewQuestClass && !m_AllQuestClasses.Contains(_NewQuestClass))
	{
		m_AllQuestClasses.Add(_NewQuestClass);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn; // 충돌 무시하고 항상 충돌.
		AQuest_Base* pSpawnedQuestActor = GetWorld()->SpawnActor<AQuest_Base>(_NewQuestClass, SpawnParams);
		pSpawnedQuestActor->Initialize(this);

		m_CurrQuestActors.Add(pSpawnedQuestActor);

		UQuestWidget* pQuestWidget = m_pHUD->AddQuestToList(pSpawnedQuestActor);
		pSpawnedQuestActor->SetQuestWidget(pQuestWidget);

		pSpawnedQuestActor->SetupStartingGoals();

		pQuestWidget->UpdateQuest();

		m_pHUD->GetQuestJournalWidget()->AddEntry(pSpawnedQuestActor);

		if (_bDirectlyStart || (m_CurrQuestActors.Num() <= 1))
		{
			USubGoalWidget* pSubGoalidget = pSpawnedQuestActor->GetQuestWidget()->GetSubGoalWidgets()[0];
			SelectNewQuest(pSpawnedQuestActor, pSubGoalidget);			
		}

		if (m_pHUD->GetSlideOut())
		{
			UWidgetAnimation* pSlideOutAnim = m_pHUD->GetWidgetAnimation("SlideOut");
			m_pHUD->PlayAnimation(pSlideOutAnim, 0.f, 1, EUMGSequencePlayMode::Reverse);

			m_pHUD->SetSlideOut(false);
			return true;
		}
		return true;
	}

	return false;
}

int AQuestManager::DistanceToGoal()
{
	float Distance = FVector::DistXY(m_pPlayer->GetActorLocation(), m_pCurrGoalActor->GetActorLocation());

	return UKismetMathLibrary::Round(Distance/100.f); // 가장 가까운 정수로 반올림.
}

void AQuestManager::UpdateDirectionArrow()
{
	FRotator Rot = UKismetMathLibrary::FindLookAtRotation(m_pPlayer->GetActorLocation(), m_pCurrGoalActor->GetActorLocation());
	m_pHUD->GetMiniMapWidget()->GetDirectionArrow()->SetRenderAngle(Rot.Yaw);
}

void AQuestManager::OnPlayerMove()
{
	static bool bDoOnce = false;

	if (m_pCurrGoalActor)
	{
		m_CurrDistance = DistanceToGoal();
		m_pHUD->GetDistanceText()->SetText(UKismetTextLibrary::Conv_IntToText(m_CurrDistance, false, false));

		if (m_CurrDistance >= m_ShowDirectionArrowAmount)
		{
			UpdateDirectionArrow();

			// 한번만 실행.
			if (!bDoOnce)
			{
				m_pHUD->GetMiniMapWidget()->GetDirectionArrow()->SetVisibility(ESlateVisibility::HitTestInvisible);
				bDoOnce = true;
			}
		}
		else if (m_CurrDistance < m_ShowDirectionArrowAmount)
		{
			// 한번만 실행.
			if (bDoOnce)
			{
				m_pHUD->GetMiniMapWidget()->GetDirectionArrow()->SetVisibility(ESlateVisibility::Hidden);
				bDoOnce = false;
			}
		}
		
	}

}

void AQuestManager::OnEnemyKilled(TSubclassOf<ABase2_Enemy> _Class)
{
	m_IndicesToCompleteLater.Empty();

	m_KilledEnemyClass = _Class;

	for (int CurrQuestActorIndex = 0; CurrQuestActorIndex < m_CurrQuestActors.Num(); ++CurrQuestActorIndex)
	{
		TArray<FGoalInfo> CurrGoals = m_CurrQuestActors[CurrQuestActorIndex]->GetCurrGoals();
		for (int CurrGoalIndex = 0; CurrGoalIndex < CurrGoals.Num(); ++CurrGoalIndex)
		{
			if (CurrGoals[CurrGoalIndex].Type == EGoalTypes::Hunt &&
				CurrGoals[CurrGoalIndex].GoalClass == m_KilledEnemyClass)
			{
				int CurrHuntedAmount = m_CurrQuestActors[CurrQuestActorIndex]->GetCurrHuntedAmounts()[CurrGoalIndex] + 1;
				int CompleteIndex = m_CurrQuestActors[CurrQuestActorIndex]->GetCurrGoalIndices()[CurrGoalIndex];

				if (CurrHuntedAmount >= CurrGoals[CurrGoalIndex].AmountToHunt)
				{
					if (CurrGoals[CurrGoalIndex].bCompletesQuest)
					{
						m_IndicesToCompleteLater.Add(FIndexToComplete(m_CurrQuestActors[CurrQuestActorIndex], CompleteIndex));
					}
					else
					{						
						m_CurrQuestActors[CurrQuestActorIndex]->CompleteSubGoal(CompleteIndex, false);
					}					
				}
				else
				{					
					m_CurrQuestActors[CurrQuestActorIndex]->GetCurrHuntedAmounts()[CurrGoalIndex] = CurrHuntedAmount;
					
					m_CurrQuestActors[CurrQuestActorIndex]->GetQuestWidget()->GetSubGoalWidgets()[CurrGoalIndex]->Update();
					
					if (m_CurrQuestActors[CurrQuestActorIndex]->SelectedInJournal())
					{
						m_pHUD->GetQuestJournalWidget()->GenerateSubGoals();
					}
				}
			}
		}		
	}

	_CompleteGoals();
}

void AQuestManager::OnObjectFound(TSubclassOf<AObject_Base> _FoundObjectClass)
{
	m_IndicesToCompleteLater.Empty();

	m_FoundObjectClass = _FoundObjectClass;

	for (int CurrQuestActorIndex = 0; CurrQuestActorIndex < m_CurrQuestActors.Num(); ++CurrQuestActorIndex)
	{
		TArray<FGoalInfo> CurrGoals = m_CurrQuestActors[CurrQuestActorIndex]->GetCurrGoals();
		for (int CurrGoalIndex = 0; CurrGoalIndex < CurrGoals.Num(); ++CurrGoalIndex)
		{
			if (CurrGoals[CurrGoalIndex].Type == EGoalTypes::Find &&
				CurrGoals[CurrGoalIndex].GoalClass == m_FoundObjectClass)
			{
				int Index = m_CurrQuestActors[CurrQuestActorIndex]->GetCurrGoalIndices()[CurrGoalIndex];

				if (CurrGoals[CurrGoalIndex].bCompletesQuest)
				{
					m_IndicesToCompleteLater.Add(FIndexToComplete(m_CurrQuestActors[CurrQuestActorIndex], Index));
				}
				else
				{
					m_CurrQuestActors[CurrQuestActorIndex]->CompleteSubGoal(Index, false);
				}
			}
		}
	}

	_CompleteGoals();
}

void AQuestManager::HasCurrentQuest(const TSubclassOf<AQuest_Base>& _QuestClass, bool & out_bIsFound, AQuest_Base* out_pQuestActor)
{
	for (int i = 0; i < m_CurrQuestActors.Num(); ++i)
	{
		if (m_CurrQuestActors[i]->GetClass() == _QuestClass)
		{
			out_bIsFound = true;
			out_pQuestActor = m_CurrQuestActors[i];
			return;
		}
	}

	out_bIsFound = false;
	out_pQuestActor = nullptr;
}

void AQuestManager::OnTalkedToNpc(const TSubclassOf<ABase_Npc>& _NpcClass, int _NpcId)
{
	m_IndicesToCompleteLater.Empty();

	for (int CurrQuestIndex = 0;  CurrQuestIndex < m_CurrQuestActors.Num(); ++CurrQuestIndex)
	{
		AQuest_Base* pCurrQuestActor = m_CurrQuestActors[CurrQuestIndex];

		for (int CurrGoalIndex = 0; CurrGoalIndex < pCurrQuestActor->GetCurrGoals().Num(); ++CurrGoalIndex)
		{
			FGoalInfo CurrGoal = pCurrQuestActor->GetCurrGoals()[CurrGoalIndex];

			if ((CurrGoal.Type == EGoalTypes::Talk) &&
				(CurrGoal.GoalClass == _NpcClass) &&
				(CurrGoal.GoalId == _NpcId))
			{
				int Index = m_CurrQuestActors[CurrQuestIndex]->GetCurrGoalIndices()[CurrGoalIndex];

				if (CurrGoal.bCompletesQuest)
				{
					m_IndicesToCompleteLater.Add(FIndexToComplete(m_CurrQuestActors[CurrQuestIndex], Index));
				}
				else
				{
					pCurrQuestActor->CompleteSubGoal(pCurrQuestActor->GetCurrGoalIndices()[CurrGoalIndex], false);
				}
			}
		}
	}

	_CompleteGoals();
}

void AQuestManager::EndQuest(AQuest_Base * _pQuest)
{
	m_CurrQuestActors.Remove(_pQuest);

	switch (_pQuest->GetCurrState())
	{
		case EQuestStates::Current_Quests:
		{
			m_CurrQuestActors.Add(_pQuest);
			break;
		}
		case EQuestStates::Completed_Quests:
		{
			m_CompletedQuestActors.Add(_pQuest);
			break;
		}
		case EQuestStates::Failed_Quests:
		{		
			m_FailedQuestActors.Add(_pQuest);
			break;
		}
		default:
		{
			break;
		}
	}

	m_pHUD->GetQuestJournalWidget()->AddEntry(_pQuest);

	if (_pQuest == m_pCurrQuest)
	{
		m_pCurrQuest = nullptr;

		if (UKismetSystemLibrary::IsValid(m_pCurrGoalActor))
		{
			m_pCurrGoalActor->Destroy();
			m_pHUD->GetDistanceBorder()->SetVisibility(ESlateVisibility::Hidden);
			m_pHUD->GetMiniMapWidget()->SetVisibility(ESlateVisibility::Hidden);
		}		
	}

	if (m_CurrQuestActors.Num() > 0)
	{
		AQuest_Base*    pCurrQuest     = m_CurrQuestActors[0];
		USubGoalWidget* pSubGoalWidget = m_CurrQuestActors[0]->GetQuestWidget()->GetSubGoalWidgets()[0];
		SelectNewQuest(pCurrQuest, pSubGoalWidget);
	}

	if (_pQuest->SelectedInJournal())
	{
		m_pHUD->GetQuestJournalWidget()->OnQuestClicked(nullptr);
	}

	if (_pQuest->GetCurrState() == EQuestStates::Completed_Quests)
	{
		const FQuestInfo& QuestInfo = _pQuest->GetQuestInfo();
		m_pPlayer->AddExpPoints(QuestInfo.CompletionReward.Experience);

		int Prestige = m_pPlayer->GetPrestigeByRegion(QuestInfo.Region) + QuestInfo.CompletionReward.PrestigePoints;
		m_pPlayer->SetPrestigeByRegion(QuestInfo.Region, Prestige);
	}
}

void AQuestManager::_CompleteGoals()
{
	for (int i = 0; i < m_IndicesToCompleteLater.Num(); ++i)
	{
		m_IndicesToCompleteLater[i].pQuest->CompleteSubGoal(m_IndicesToCompleteLater[i].Index, false);
	}
}

