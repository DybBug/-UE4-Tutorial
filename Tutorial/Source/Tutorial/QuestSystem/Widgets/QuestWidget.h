// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestWidget.generated.h"

class UTextBlock;
class UVerticalBox;
class AQuestManager;
class AQuest_Base;
class USubGoalWidget;
/**
 * 
 */
UCLASS()
class TUTORIAL_API UQuestWidget : public UUserWidget
{
	GENERATED_BODY()

protected :
	virtual void NativeConstruct() override;

public :
	void Initialize(AQuestManager* _pQuestManager, AQuest_Base* _pAssignedQuest);
	void GenerateSubWidgets();
	void UpdateQuest();

protected :
	UTextBlock* m_pQuestName;
	UVerticalBox* m_pSubGoalBox;

	AQuestManager* m_pQuestManager;
	AQuest_Base* m_pAssignedQuest;

	TArray<USubGoalWidget*> m_SubGoalWidgets;
	USubGoalWidget* m_pSelectedSubGoalWidget;
	
};
