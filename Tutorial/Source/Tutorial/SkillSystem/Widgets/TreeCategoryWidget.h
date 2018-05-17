// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <Blueprint/UserWidget.h>
#include "TreeCategoryWidget.generated.h"


class USkillTreeWidget;
class UTextBlock;
class UButton;
/**
 * 
 */
UCLASS()
class TUTORIAL_API UTreeCategoryWidget : public UUserWidget
{
	GENERATED_BODY()

public :
	virtual bool Initialize() override;

	UFUNCTION()
	void Initialize(FText _NewCategory, USkillTreeWidget* _pSkillTreeWidget, int _Index);

	void ChangeState(bool _IsSelected);

private :
	
	UFUNCTION()
	void _OnClicked();

	UFUNCTION()
	void _OnHovered();

	UFUNCTION()
	void _OnUnhovered();

protected :
	UPROPERTY()
	UTextBlock* m_pCategoryName;

	UPROPERTY()
	UButton* m_pCategoryButton;

	UPROPERTY()
	FText m_Category;
	
	UPROPERTY()
	USkillTreeWidget* m_pSkillTreeWidget;

	int m_Index;

	bool m_bIsSelected = false;
};
