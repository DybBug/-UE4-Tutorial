// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../QuestSystem.h"
#include "SubGoalWidget.generated.h"

class UBorder;
class UButton;
class UTextBlock;
class AQuest_Base;
class UQuestWidget;
/**
 * 
 */
UCLASS()
class TUTORIAL_API USubGoalWidget : public UUserWidget
{
	GENERATED_BODY()

protected :
	virtual void NativeConstruct() override;

public :
	void Initialize(const FGoalInfo& _GoalInfo, AQuest_Base* _Quest, UQuestWidget* _Widget);
	void Update();



	
protected :
	UBorder* m_pGoalBorder;
	UButton* m_pSelectButton;
	UTextBlock* m_pGoalText;

	FGoalInfo m_GoalInfo;
	AQuest_Base* m_pAssignedQuest; 
	UQuestWidget* m_pQuestWidget;
	
	int m_GoalIndex;
};
