// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include <Components/ProgressBar.h>
#include <Components/Image.h>
#include <Components/TextBlock.h>
#include <Components/ScrollBox.h>

#include "QuestSystemHUD.generated.h"

class AQuestManager;
class UQuestWidget;
class AQuest_Base;
/**
 * 
 */
UCLASS()
class TUTORIAL_API UQuestSystemHUD : public UUserWidget
{
	GENERATED_BODY()
	
protected :
	virtual void NativeConstruct() override;

public :
	UQuestWidget* AddQuestToList(AQuest_Base* _pQuestActor);

	/* Get */
	UProgressBar* GetExpBar()  const { return m_pExpBar; }
	UImage* GetCharacterIcon() const { return m_pCharacterIcon; }
	UTextBlock* GetExpText()   const { return m_pExpText; }
	UTextBlock* GetLevelText() const { return m_pLevelText; }

	/* Set */
	void SetQuestManager(AQuestManager* _pQuestManager) { m_pQuestManager = _pQuestManager; }
protected :
	UPROPERTY()
		UProgressBar* m_pExpBar;

	UPROPERTY()
		UImage* m_pCharacterIcon;

	UPROPERTY()
		UTextBlock* m_pExpText;

	UPROPERTY()
	UTextBlock* m_pLevelText;

	UPROPERTY()
	UScrollBox* m_pQuestList;
	
	AQuestManager* m_pQuestManager;

	TArray<UQuestWidget*> m_QuestWidgets;
	
};
