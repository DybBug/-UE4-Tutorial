// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include <Components/ProgressBar.h>
#include <Components/Image.h>
#include <Components/TextBlock.h>
#include <Components/ScrollBox.h>
#include <Components/Border.h>

#include "QuestSystemHUD.generated.h"

class AQuestManager;
class UQuestWidget;
class AQuest_Base;
class UMiniMapWidget;
/**
 * 
 */
UCLASS()
class TUTORIAL_API UQuestSystemHUD : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	UQuestWidget * AddQuestToList(AQuest_Base* _pQuestActor);

	/* Get */
	UProgressBar*   GetExpBar()         const { return m_pExpBar; }
	UImage*         GetCharacterIcon()  const { return m_pCharacterIcon; }
	UTextBlock*     GetExpText()        const { return m_pExpText; }
	UTextBlock*     GetLevelText()      const { return m_pLevelText; }
	UScrollBox*     GetQuestList()      const { return m_pQuestList; }
	UBorder*        GetDistanceBorder() const { return m_pDistanceBorder; }
	UTextBlock*     GetDistanceText()   const { return m_pDistanceText; }
	UMiniMapWidget* GetMiniMapWidget()  const { return m_pMiniMapWidget; }

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

	UPROPERTY()
	UBorder* m_pDistanceBorder;

	UPROPERTY()
	UTextBlock* m_pDistanceText;

	UMiniMapWidget* m_pMiniMapWidget;
	
	AQuestManager* m_pQuestManager;

	TArray<UQuestWidget*> m_QuestWidgets;
	
};
