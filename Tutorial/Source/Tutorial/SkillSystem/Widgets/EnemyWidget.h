// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include <Components/TextBlock.h>
#include <Components/ProgressBar.h>
#include "EnemyWidget.generated.h"

/**
 * 
 */
UCLASS()
class TUTORIAL_API UEnemyWidget : public UUserWidget
{
	GENERATED_BODY()
		
public :

	/* Get */
	UFUNCTION(BlueprintPure, Category = "EnemyWidget/Get")
	class UTextBlock* GetLevelNameText() { return m_pLevelNameText; }

	UFUNCTION(BlueprintPure, Category = "EnemyWidget/Get")
	class UProgressBar* GetHealthBar() { return m_pHealthBar; }

protected :
	UPROPERTY(BlueprintReadWrite, Category = "EnemyWidget")
	class UTextBlock* m_pLevelNameText;

	UPROPERTY(BlueprintReadWrite, Category = "EnemyWidget")
	class UProgressBar* m_pHealthBar;
	
};
