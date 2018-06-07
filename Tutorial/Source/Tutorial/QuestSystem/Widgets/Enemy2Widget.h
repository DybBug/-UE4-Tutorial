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
	virtual bool Initialize() override;
	
public :

	/* Get */
	UProgressBar * GetHealthBar() const { return m_pHealthBar; }
	UTextBlock*   GetNameText()   const { return m_pNameText; }
	UTextBlock*   GetLevelText()  const { return m_pLevelText; }
	
protected :
	UProgressBar* m_pHealthBar;
	UTextBlock*   m_pNameText;
	UTextBlock*   m_pLevelText;
	
};
