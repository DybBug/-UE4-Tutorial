// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../QuestSystem.h"

#include <Components/TextBlock.h>
#include "QuestCategoryWidget.generated.h"

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

protected :
	UTextBlock* m_pCategoryName;

	UPROPERTY(EditAnywhere, Category = "QuestCategoryWidget")
	EQuestStates m_Category;
	
};
