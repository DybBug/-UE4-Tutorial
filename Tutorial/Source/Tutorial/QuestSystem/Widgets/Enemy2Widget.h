// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Enemy2Widget.generated.h"

class UProgressBar;
class UTextBlock;
/**
 * 
 */
UCLASS()
class TUTORIAL_API UEnemy2Widget : public UUserWidget
{
	GENERATED_BODY()	
public :
	/* Get */
	UProgressBar * GetHealthBar() const { return m_pHealthBar; }
	UTextBlock*   GetNameText()   const { return m_pNameText; }
	UTextBlock*   GetLevelText()  const { return m_pLevelText; }
	
protected :
	UPROPERTY()
	UProgressBar* m_pHealthBar;

	UPROPERTY()
	UTextBlock*   m_pNameText;

	UPROPERTY()
	UTextBlock*   m_pLevelText;
	
};
