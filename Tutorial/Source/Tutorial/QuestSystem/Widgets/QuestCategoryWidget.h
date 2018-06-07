// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../QuestSystem.h"


#include "QuestCategoryWidget.generated.h"

class UTextBlock;
class UButton;
class UVerticalBox;
/**
 * 
 */
UCLASS()
class TUTORIAL_API UQuestCategoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected :
	virtual void NativeConstruct() override;

	void Init();
	
public :
	/* Get */
	UTextBlock* GetCategoryName() const { return m_pCategoryName; }
	UVerticalBox* GetQuestBox() const { return m_pQuestBox; }
private :
	UFUNCTION()
	void _OnClicked();

protected :
	UButton*      m_pExpandButton;
	UTextBlock*   m_pCategoryName;
	UVerticalBox* m_pQuestBox;

	UPROPERTY(EditAnywhere, Category = "QuestCategoryWidget")
	EQuestStates m_Category;

	bool m_bExpanded;
	
};
