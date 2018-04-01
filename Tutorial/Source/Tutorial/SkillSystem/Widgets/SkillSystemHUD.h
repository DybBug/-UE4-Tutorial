// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StatBarWidget.h"
#include "Blueprint/UserWidget.h"
#include "SkillSystemHUD.generated.h"

/**
 * 
 */
UCLASS()
class TUTORIAL_API USkillSystemHUD : public UUserWidget
{
	GENERATED_BODY()

public :

	/* Get */
	UFUNCTION(BlueprintPure, Category = "SkillSystemHUD")
	class UStatBarWidget* GetHealthBar() const { return m_pHealthBar; }
	UFUNCTION(BlueprintPure, Category = "SkillSystemHUD")
	class UStatBarWidget* GetManaBar() const { return m_pManaBar; }
	UFUNCTION(BlueprintPure, Category = "SkillSystemHUD")
	class UStatBarWidget* GetExpBar() const { return m_pExpBar; }

	/* Set */
	UFUNCTION(BlueprintCallable, Category = "SkillSystemHUD")
	void SetHealthBar(class UStatBarWidget* _Widget) { m_pHealthBar = _Widget; }
	UFUNCTION(BlueprintCallable, Category = "SkillSystemHUD")
	void SetManaBar(class UStatBarWidget* _Widget) { m_pManaBar = _Widget; }
	UFUNCTION(BlueprintCallable, Category = "SkillSystemHUD")
	void SetExpBar(class UStatBarWidget* _Widget) { m_pExpBar = _Widget; }

	
protected :
	UPROPERTY(VisibleAnywhere, Category = "SkillSystemHUD")
	class UStatBarWidget* m_pHealthBar;
	UPROPERTY(VisibleAnywhere, Category = "SkillSystemHUD")
	class UStatBarWidget* m_pManaBar;
	UPROPERTY(VisibleAnywhere, Category = "SkillSystemHUD")
	class UStatBarWidget* m_pExpBar;
	
};
