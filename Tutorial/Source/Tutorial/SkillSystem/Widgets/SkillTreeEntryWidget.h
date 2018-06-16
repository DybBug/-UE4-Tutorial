// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <Blueprint/UserWidget.h>
#include "SkillTreeEntryWidget.generated.h"


class USubTreeWidget;
class ABase_Skill;
class UTextBlock;
class UImage;
class UButton;
class UVerticalBox;
/**
 * 
 */
UCLASS()
class TUTORIAL_API USkillTreeEntryWidget : public UUserWidget
{
	GENERATED_BODY()

protected :
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& _InGeometry, const FPointerEvent& _InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

public :
	void Initialize(const TSubclassOf<ABase_Skill>& _SkillClass, USubTreeWidget* _pSubTreeWidget);
	
	int AmountOfStages();
	void UpdateStageText();
	void UpdateIcon();
	void UpdateUpgradeBox();
	void OnSpellLearned();

private :
	/* OnClicked */
	UFUNCTION()
	void _OnPlusButtonClicked();

	UFUNCTION()
	void _OnMinusButtonClicked();

protected :
	UPROPERTY()
	UTextBlock * m_pStageText;

	UPROPERTY()
	UImage* m_pSkillIcon;

	UPROPERTY()
	UButton* m_pPlusButton;

	UPROPERTY()
	UButton* m_pMinusButton;

	UPROPERTY()
	UVerticalBox* m_pUpgradeBox;

	UPROPERTY()
	TSubclassOf<ABase_Skill> m_SkillClass;

	UPROPERTY()
	USubTreeWidget* m_pSubTreeWidget;

	UPROPERTY()
	ABase_Skill* m_pSkill;

	bool m_bSpellLearned = false;
	
};
