// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../QuestSystem.h"
#include "GoalEntryWidget.generated.h"

class UQuestJournalWidget;
class UImage;
class UTextBlock;
/**
 * 
 */
UCLASS()
class TUTORIAL_API UGoalEntryWidget : public UUserWidget
{
	GENERATED_BODY()

public :
	void Initialize(const FGoalInfo& _Info, const EGoalState& _State, UQuestJournalWidget* _pWidget, int _HuntIndex);

protected :
	virtual void NativeConstruct() override;

protected :
	void _Update();
	
protected :
	UImage* m_pGoalState;
	UTextBlock* m_pGoalText;

	FGoalInfo m_GoalInfo;
	EGoalState m_State;

	UQuestJournalWidget* m_pJournalWidget;

	int m_HuntIndex;
	
};
