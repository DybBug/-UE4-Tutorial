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
	TArray<UQuestListEntryWidget*>& GetAllQuestEntryWidgets() { return m_AllQuestEntryWidgets; }
	UQuestListEntryWidget* GetCurrQuestWidget() const { return m_pCurrQuestWidget; }

	AQuest_Base* GetSelectedQuest() const { return m_pSelectedQuest; }
	AQuestCharacter* GetPlayer() const { return m_pPlayer; }

private :
	UFUNCTION()
	void _OnSelectButtonClicked();

	UFUNCTION()
	void _OnCancelButtonClicked();

protected :
	UPROPERTY()
	UTextBlock* m_pQuestName;

	UPROPERTY()
	UTextBlock* m_pQuestCategory;

	UPROPERTY()
	UTextBlock* m_pQuestRegion;

	UPROPERTY()
	UTextBlock* m_pSuggestedLevel;

	UPROPERTY()
	UTextBlock* m_pDescription;

	UPROPERTY()
	UVerticalBox* m_pQuestGoalBox;

	UPROPERTY()
	UScrollBox* m_pQuestDetailsBox;

	UPROPERTY()
	UHorizontalBox* m_pExpRewardBox;

	UPROPERTY()
	UTextBlock* m_pExpReward;

	UPROPERTY()
	UHorizontalBox* m_pPrestigeRewardBox;

	UPROPERTY()
	UTextBlock* m_pPrestigeReward;

	UPROPERTY()
	UVerticalBox* m_pRewardBox;

	UPROPERTY()
	UHorizontalBox* m_pButtonBox;

	UPROPERTY()
	UButton* m_pSelectButton;

	UPROPERTY()
	UButton* m_pCancelButton;

	UPROPERTY()
	UScrollBox* m_pQuestList;

	UPROPERTY()
	UQuestCategoryWidget* m_pCatCurrent;

	UPROPERTY()
	UQuestCategoryWidget* m_pCatCompleted;

	UPROPERTY()
	UQuestCategoryWidget* m_pCatFailed;

	AQuest_Base* m_pSelectedQuest;
	
	AQuestCharacter* m_pPlayer;

	AQuestManager* m_pQuestManager;
	
	TArray<UQuestListEntryWidget*> m_AllQuestEntryWidgets;
	UQuestListEntryWidget* m_pCurrQuestWidget;
};
