// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include  "../../QuestSystem.h"
#include "Quest_Base.generated.h"


class UQuestWidget;

UCLASS()
class TUTORIAL_API AQuest_Base : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AQuest_Base();

	void UpdateSubGoals();
	void SetupStartingGoals();
	bool GoToNextSubGoals();

	/* Get */
	const FQuestInfo& GetQuestInfo() const { return m_QuestInfo; }
	const int& GetCurrHuntedAmount() const { return m_CurrHuntedAmount; }
	const TArray<FGoalInfo>& GetCurrGoals() const {return m_CurrGoals;}


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
};
