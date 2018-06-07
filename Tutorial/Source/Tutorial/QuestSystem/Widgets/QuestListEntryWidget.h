// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestListEntryWidget.generated.h"

class UButton;
class UTextBlock;
class UQuestJournalWidget;
class AQuest_Base;
/**
 * 
 */
UCLASS()
class TUTORIAL_API UQuestListEntryWidget : public UUserWidget
{
	GENERATED_BODY()

protected :
	virtual void NativeConstruct() override;

public :
	void Initialize(UQuestJournalWidget* _pWidget, AQuest_Base* _pAssignedQuest);
	void UpdateLevelColor();
	void Update();

	/* Get */
	UButton* GetQuestButton() const { return m_pQuestButton; }
	AQuest_Base* GetAssignedQuest() const { return m_pAssignedQuest; }

private :
	UFUNCTION()
	void _OnClicked();

protected :
	
	UButton*   m_pQuestButton;
	UTextBlock* m_pQuestName;
	UTextBlock* m_pRegionName;
	UTextBlock* m_pSuggestedLevel;
	
	UQuestJournalWidget* m_pJournalWidget;
	AQuest_Base* m_pAssignedQuest;
};
