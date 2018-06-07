// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestJournalWidget.generated.h"

class AQuest_Base;
class AQuestCharacter;
class AQuestManager;
class UQuestListEntryWidget;
class UQuestCategoryWidget;


class UTextBlock;
class UVerticalBox;
class UHorizontalBox;
class UScrollBox;
class UButton;
/**
 * 
 */
UCLASS()
class TUTORIAL_API UQuestJournalWidget : public UUserWidget
{
	GENERATED_BODY()	

protected :
	virtual void NativeConstruct() override;

public :
	void Initialize(AQuestManager* _pQuestManager);
	void UpdateSuggestedLevelColor();
	void UpdateDescription();
	void GenerateSubGoals();
	void UpdateDetailWindow();
	void AddEntry(AQuest_Base* _pQuestActor);

	void OnQuestClicked(UQuestListEntryWidget* _pWidget);

	/* Get */
	TArray<UQuestListEntryWidget*> GetAllQuestEntryWidgets() { return m_AllQuestEntryWidgets; }
	UQuestListEntryWidget* GetCurrQuestWidget() const { return m_pCurrQuestWidget; }

	AQuest_Base* GetSelectedQuest() const { return m_pSelectedQuest; }
	AQuestCharacter* GetPlayer() const { return m_pPlayer; }

private :
	UFUNCTION()
	void _OnSelectButtonClicked();

	UFUNCTION()
	void _OnCancelButtonClicked();

protected :
	UTextBlock*     m_pQuestName;
	UTextBlock*     m_pQuestCategory;
	UTextBlock*     m_pQuestRegion;
	UTextBlock*     m_pSuggestedLevel;
	UTextBlock*     m_pDescription;
	UVerticalBox*   m_pQuestGoalBox;
	UScrollBox*     m_pQuestDetailsBox;
	UHorizontalBox* m_pExpRewardBox;
	UTextBlock*     m_pExpReward;
	UHorizontalBox* m_pPrestigeRewardBox;
	UTextBlock*     m_pPrestigeReward;
	UVerticalBox*   m_pRewardBox;
	UHorizontalBox* m_pButtonBox;
	UButton*        m_pSelectButton;
	UButton*        m_pCancelButton;

	UScrollBox*           m_pQuestList;
	UQuestCategoryWidget* m_pCatCurrent;
	UQuestCategoryWidget* m_pCatCompleted;
	UQuestCategoryWidget* m_pCatFailed;

	AQuest_Base* m_pSelectedQuest;
	
	AQuestCharacter* m_pPlayer;

	AQuestManager* m_pQuestManager;
	
	TArray<UQuestListEntryWidget*> m_AllQuestEntryWidgets;
	UQuestListEntryWidget* m_pCurrQuestWidget;
};
