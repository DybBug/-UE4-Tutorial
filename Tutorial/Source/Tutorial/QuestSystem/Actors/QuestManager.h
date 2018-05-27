// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../QuestSystem.h"
#include "QuestManager.generated.h"

class AQuestCharacter;
class UQuestSystemHUD;
class AQuest_Base;
class AGoalActor;
class USubGoalWidget;

UCLASS()
class TUTORIAL_API AQuestManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AQuestManager();

protected :
	virtual void BeginPlay() override;

public:	
	void OnSwitchSubQuest();
	void SelectNewQuest(AQuest_Base* _pQuest, USubGoalWidget* _pSubGoal);

	bool AddNewQuest(TSubclassOf<AQuest_Base> _NewQuestClass, bool _bDirectlyStart);

	int DistanceToGoal();
	void UpdateDirectionArrow();

	void OnPlayerMove(); // 플레이어가 움직일때마다 호출.

	/* Get */
	AQuest_Base* GetCurrQuest() const { return m_pCurrQuest; }

	/* Set */
	void SetHUD(UQuestSystemHUD* _pHUD) { m_pHUD = _pHUD; }

protected:
	AQuestCharacter* m_pPlayer;
	
	UQuestSystemHUD* m_pHUD;

	TArray<AQuest_Base*> m_QuestActors;
	AQuest_Base* m_pCurrQuest;

	TArray<TSubclassOf<AQuest_Base>> m_AllQuestClasses;

	FGoalInfo m_CurrSubGoal;

	TSubclassOf<AGoalActor> m_GoalActorClass;
	AGoalActor* m_pCurrGoalActor;

	int m_CurrDistance;
	int m_ShowDirectionArrowAmount = 10;
};
