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
class ABase2_Enemy;
class ABase_Npc;
class AObject_Base;

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
	void SelectNewQuest(AQuest_Base* _pQuest, USubGoalWidget* _pSubGoal, bool _bLoaded);

	bool AddNewQuest(TSubclassOf<AQuest_Base> _NewQuestClass, bool _bDirectlyStart);

	int DistanceToGoal();
	void UpdateDirectionArrow();

	void OnPlayerMove(); // 플레이어가 움직일때마다 호출.

	void OnEnemyKilled(TSubclassOf<ABase2_Enemy> _Class);

	void OnObjectFound(TSubclassOf<AObject_Base> _FoundObjectClass);

	void HasCurrentQuest(const TSubclassOf<AQuest_Base>& _QuestClass, bool& out_bIsFound, AQuest_Base* out_pQuestActor);
	void OnTalkedToNpc(const TSubclassOf<ABase_Npc>& _NpcClass, int _NpcId);

	void EndQuest(AQuest_Base* _pQuest);
	void FindNpcById(TSubclassOf<ABase_Npc> _NpcClass, int _Id, bool& out_bFound, ABase_Npc*& out_pNpc);
	void CancelQuest(AQuest_Base* _pQuestActor);

	void LoadQuests();

	
	/* Get */
	const TArray<AQuest_Base*>& GetCurrQuestActors()      const { return m_CurrQuestActors; }
	const TArray<AQuest_Base*>& GetCompletedQuestActors() const { return m_CompletedQuestActors; }
	const TArray<AQuest_Base*>& GetFailedQuestActors()    const { return m_FailedQuestActors; }

	AQuest_Base* GetCurrQuest() const { return m_pCurrQuest; }
	UQuestSystemHUD* GetHUD() const { return m_pHUD; }
	AQuestCharacter* GetPlayer() const { return m_pPlayer; }

	TArray<TSubclassOf<AQuest_Base>>& GetAllQuestClasses() { return m_AllQuestClasses; }
	TArray<ABase_Npc*>&               GetAllNpcsInWorld() { return m_AllNpcsInWorld; }


	/* Set */
	void SetHUD(UQuestSystemHUD* _pHUD) { m_pHUD = _pHUD; }

protected :
	void _SetupAllObjects();
	void _SetupAllNpcsInWorld();
	void _CompleteGoals();

protected:
	AQuestCharacter* m_pPlayer;
	
	UQuestSystemHUD* m_pHUD;

	TArray<AQuest_Base*> m_CurrQuestActors;
	TArray<AQuest_Base*> m_CompletedQuestActors;
	TArray<AQuest_Base*> m_FailedQuestActors;

	AQuest_Base* m_pCurrQuest;

	TArray<TSubclassOf<AQuest_Base>> m_AllQuestClasses;

	FGoalInfo m_CurrSubGoal;

	TSubclassOf<AGoalActor> m_GoalActorClass;
	AGoalActor* m_pCurrGoalActor;

	int m_CurrDistance;
	int m_ShowDirectionArrowAmount = 10;

	TSubclassOf<ABase2_Enemy> m_KilledEnemyClass;
	TSubclassOf<AObject_Base> m_FoundObjectClass;

	TArray<FIndexToComplete> m_IndicesToCompleteLater;
	TArray<ABase_Npc*> m_AllNpcsInWorld;
	TArray<AObject_Base*> m_AllObjectsInWorld;
};
