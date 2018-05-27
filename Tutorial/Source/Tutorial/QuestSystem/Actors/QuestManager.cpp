// Fill out your copyright notice in the Description page of Project Settings.

#include "QuestManager.h"
#include "GoalActor.h"
#include "QuestActors/Quest_Base.h"
#include "../Characters/QuestCharacter.h"
#include "../Widgets/QuestWidget.h"
#include "../Widgets/MiniMapWidget.h"
#include "../Widgets/QuestSystemHUD.h"

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

		m_QuestActors.Add(pSpawnedQuestActor);

		pSpawnedQuestActor->SetupStartingGoals();

		UQuestWidget* pQuestWidget = m_pHUD->AddQuestToList(pSpawnedQuestActor);
		pSpawnedQuestActor->SetQuestWidget(pQuestWidget);

		pQuestWidget->UpdateQuest();

		if (_bDirectlyStart || (m_QuestActors.Num() <= 1))
		{
			USubGoalWidget* pSubGoalidget = pSpawnedQuestActor->GetQuestWidget()->GetSubGoalWidgets()[0];
			SelectNewQuest(pSpawnedQuestActor, pSubGoalidget);
		}

		return true;
	}

	return false;
}

int AQuestManager::DistanceToGoal()
{
	float Distance = FVector::DistXY(m_pPlayer->GetActorLocation(), m_pCurrGoalActor->GetActorLocation());

	return UKismetMathLibrary::Round(Distance); // 가장 가까운 정수로 반올림.
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

