// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <Blueprint/UserWidget.h>
#include "../SkillSystem.h"
#include "SkillTreeWidget.generated.h"


class USkillTreeComponent;
class UTreeCategoryWidget;
class USubTreeWidget;
class UButton;
class UTextBlock;
class UScrollBox;
class UWidgetSwitcher;
/**
 * 
 */
UCLASS()
class TUTORIAL_API USkillTreeWidget : public UUserWidget
{
	GENERATED_BODY()

protected :
	virtual void NativeConstruct() override;

public :

	void Initialize(USkillTreeComponent* _pAssignedSkillTree);

	void UpdateSP();
	void UpdateLevel();
	void GenerateCategories();

	void OnCategoryClicked(int _Index);

	/* Get */
	UFUNCTION(BlueprintPure, Category = "SkillTreeWidget")
	USkillTreeComponent* GetSkillTree() const { return m_pSkillTree; }

	UFUNCTION(BlueprintPure, Category = "SkillTreeWidget")
	const TArray<USubTreeWidget*>& GetSubTreeWidget() const { return m_SubTreeWidgets; }

protected :
	virtual FReply NativeOnMouseButtonDown( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent ) override;
	virtual void NativeOnDragDetected( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation ) override;


private :
	/* Bind */
	UFUNCTION()
	void _OnCloseButtonClicked();
	
protected :
	UPROPERTY()
	UButton* m_pCloseButton;

	UPROPERTY()
	UTextBlock* m_pSpText;

	UPROPERTY()
	UTextBlock* m_pLevelText;

	UPROPERTY()
	UScrollBox* m_pCategoriesBox;

	UPROPERTY()
	UWidgetSwitcher* m_pSubTreeSwitcher;

	UPROPERTY()
	USkillTreeComponent* m_pSkillTree;

	UPROPERTY()
	TArray<FTreeCategory> m_TreeCategories;

	UPROPERTY()
	TArray<UTreeCategoryWidget*> m_TreeCategoryWidgets;
	
	UPROPERTY()
	TArray<USubTreeWidget*> m_SubTreeWidgets;

	int m_CurrSelectedIndex = -1;



};
