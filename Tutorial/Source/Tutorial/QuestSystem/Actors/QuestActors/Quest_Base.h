// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include  "../../QuestSystem.h"
#include "Quest_Base.generated.h"


class UQuestWidget;
class UQuestListEntryWidget;
class AQuestManager;

UCLASS()
class TUTORIAL_API AQuest_Base : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AQuest_Base();

	void Initialize(AQuestManager* _pQuestManager);
	void UpdateSubGoals();
	void SetupStartingGoals();
	bool GoToNextSubGoals();

	bool CompleteSubGoal(int _SubGoalIndex);
	bool SelectedInJournal();

	/* Get */
	const FQuestInfo&        GetQuestInfo()        const { return m_QuestInfo; }
	const int&               GetCurrHuntedAmount() const { return m_CurrHuntedAmount; }
	const TArray<FGoalInfo>& GetCurrGoals()        const { return m_CurrGoals;}
	UQuestWidget*            GetQuestWidget()      const { return m_pQuestWidget; }
	const FText&             GetCurrDescription()  const { return m_CurrDescription; }
	const EQuestStates&      GetCurrState()        const { return m_CurrState; }

	const int& GetSelectedSubGoalIndex() const { return m_SelectedSubGoalIndex; }
	TArray<FCompletedGoal> GetCompletedSubGoals() { return m_CompletedSubGoals; }


	/* Set */
	void SetSelectedSubGoalIndex(int _Index)                 { m_SelectedSubGoalIndex = _Index; }
	void SetQuestWidget(UQuestWidget* _pNewQuestWidget)      { m_pQuestWidget = _pNewQuestWidget; }
	void SetListEntryWidget(UQuestListEntryWidget* _pWidget) { m_pListEntryWidget = _pWidget; }

protected :
	UPROPERTY(EditAnywhere, Category = "Quest_Base")
	FQuestInfo m_QuestInfo;

	UPROPERTY(EditAnywhere, Category = "Quest_Base")
	TArray<int> m_StartingSubGoalIndices;

	UPROPERTY(VisibleAnywhere, Category = "Quest_Base|DoNotTouch!")
	TArray<int> m_CurrGoalIndices;
	
	UPROPERTY(VisibleAnywhere, Category = "Quest_Base|DoNotTouch!")
	TArray<FGoalInfo> m_CurrGoals;

	UPROPERTY(VisibleAnywhere, Category = "Quest_Base|DoNotTouch!")
	int m_CurrHuntedAmount;

	UPROPERTY(VisibleAnywhere, Category = "Quest_Base|DoNotTouch!")
	int m_SelectedSubGoalIndex;

	UPROPERTY(VisibleAnywhere, Category = "Quest_Base|DoNotTouch!")
	UQuestWidget* m_pQuestWidget;

	UPROPERTY(VisibleAnywhere, Category = "Quest_Base|DoNotTouch!")
	UQuestListEntryWidget* m_pListEntryWidget;

	UPROPERTY(VisibleAnywhere, Category = "Quest_Base|DoNotTouch!")
	TArray<FCompletedGoal> m_CompletedSubGoals;

	UPROPERTY(VisibleAnywhere, Category = "Quest_Base|DoNotTouch!")
	EQuestStates m_CurrState;

	UPROPERTY(VisibleAnywhere, Category = "Quest_Base|DoNotTouch!")
	FText m_CurrDescription;


	AQuestManager* m_pQuestManager;


};
