// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../QuestSystem.h"

#include <Components/Border.h>
#include <Components/Button.h>
#include <Components/TextBlock.h>

#include "SubGoalWidget.generated.h"


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


	/* Get */
	UBorder*    GetGoalBorder()   const { return m_pGoalBorder; }
	UButton*    GetSelectButton() const { return m_pSelectButton; }
	UTextBlock* GetGoalText()     const { return m_pGoalText; }
	
	const int& GetGoalIndex() const { return m_GoalIndex; }

protected :
	UFUNCTION()
	void _OnSelectButtonClicked();

	
protected :
	UPROPERTY()
	UBorder* m_pGoalBorder;

	UPROPERTY()
	UButton* m_pSelectButton;

	UPROPERTY()
	UTextBlock* m_pGoalText;

	FGoalInfo m_GoalInfo;
	AQuest_Base* m_pAssignedQuest; 
	UQuestWidget* m_pQuestWidget;
	
	int m_GoalIndex;
	int m_HuntIndex;
};
