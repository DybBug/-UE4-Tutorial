// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include <Components/ProgressBar.h>
#include <Components/Image.h>
#include <Components/TextBlock.h>
#include <Components/ScrollBox.h>
#include <Components/Border.h>
#include <Animation/WidgetAnimation.h>

#include "QuestSystemHUD.generated.h"

class AQuestManager;
class UQuestWidget;
class AQuest_Base;
class UMiniMapWidget;
class UQuestJournalWidget;
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
	UQuestWidget* AddQuestToList(AQuest_Base* _pQuestActor);

	/* Get */
	UProgressBar*        GetExpBar()             const { return m_pExpBar; }
	UTextBlock*          GetExpText()            const { return m_pExpText; }
	UProgressBar*        GetHealthBar()          const { return m_pHealthBar; }
	UTextBlock*          GetHealthText()         const { return m_pHealthText; }
	UImage*              GetCharacterIcon()      const { return m_pCharacterIcon; }
	UTextBlock*          GetLevelText()          const { return m_pLevelText; }
	UScrollBox*          GetQuestList()          const { return m_pQuestList; }
	UBorder*             GetDistanceBorder()     const { return m_pDistanceBorder; }
	UTextBlock*          GetDistanceText()       const { return m_pDistanceText; }
	UMiniMapWidget*      GetMiniMapWidget()      const { return m_pMiniMapWidget; }
	UQuestJournalWidget* GetQuestJournalWidget() const { return m_pJournalWidget; }
	UBorder*             GetRegionBorder()       const { return m_pRegionBorder; }
	UTextBlock*          GetRegionText()         const { return m_pRegionText; }	
	UTextBlock*          GetPrestigeText()       const { return m_pPrestigeText; }



	TArray<UQuestWidget*>&          GetQuestWidgets()     { return m_QuestWidgets; }
	TMap<FName, UWidgetAnimation*>& GetWidgetAnimations() { return m_WidgetAnimations; }
	UWidgetAnimation* GetWidgetAnimation(FName _Name)           const { return m_WidgetAnimations[_Name]; }


	const bool& GetSlideOut() const { return m_bSlideOut; }

	/* Set */
	void SetQuestManager(AQuestManager* _pQuestManager) { m_pQuestManager = _pQuestManager; }
	void SetSlideOut(bool _bBool) { m_bSlideOut = _bBool; }
private :
	void _SetupWidgetAnimations();
protected :
	UPROPERTY()
	UProgressBar* m_pExpBar;

	UPROPERTY()
	UTextBlock* m_pExpText;

	UPROPERTY()
	UProgressBar* m_pHealthBar;

	UPROPERTY()
	UTextBlock* m_pHealthText;

	UPROPERTY()
	UImage* m_pCharacterIcon;	

	UPROPERTY()
	UTextBlock* m_pLevelText;

	UPROPERTY()
	UScrollBox* m_pQuestList;

	UPROPERTY()
	UBorder* m_pDistanceBorder;

	UPROPERTY()
	UTextBlock* m_pDistanceText;

	UPROPERTY()
	UMiniMapWidget* m_pMiniMapWidget;

	UPROPERTY()
	UQuestJournalWidget* m_pJournalWidget;

	UPROPERTY()
	UBorder* m_pRegionBorder;

	UPROPERTY()
	UTextBlock* m_pRegionText;

	UPROPERTY()
	UTextBlock* m_pPrestigeText;
	
	AQuestManager* m_pQuestManager;

	TArray<UQuestWidget*> m_QuestWidgets;

	bool m_bSlideOut = true;

	/* Widget Animations */
	TMap<FName, UWidgetAnimation*> m_WidgetAnimations;
};
