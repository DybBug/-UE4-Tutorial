// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include <Components/TextBlock.h>
#include <Components/VerticalBox.h>

#include "QuestWidget.generated.h"


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
	bool IsCurrQuest();
	void SelectSubGoal(USubGoalWidget* _pClickedSubGoal);
	void OnQuestSelected(USubGoalWidget* _pClickedSubGoal);

	/* Get */ 
	UTextBlock*    GetQuestName()    const { return m_pQuestName; }
	UVerticalBox*  GetSubGoalBox()   const { return m_pSubGoalBox; }
	AQuestManager* GetQuestManager() const { return m_pQuestManager; }

	const TArray<USubGoalWidget*>& GetSubGoalWidgets() const { return m_SubGoalWidgets; }
	USubGoalWidget* GetSelectedSubGoalWidget() const { return m_pSelectedSubGoalWidget; }

	/* Set */
	void SetSelectedSubGoalWidget(USubGoalWidget* _pSelectedWidget) { m_pSelectedSubGoalWidget = _pSelectedWidget; }


protected :
	UTextBlock* m_pQuestName;
	UVerticalBox* m_pSubGoalBox;

	AQuestManager* m_pQuestManager;
	AQuest_Base* m_pAssignedQuest;

	TArray<USubGoalWidget*> m_SubGoalWidgets;
	USubGoalWidget* m_pSelectedSubGoalWidget;
	
};
