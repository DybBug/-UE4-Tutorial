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
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "SkillSystemHUD")
	bool GenerateHotkeys(const TArray<FKey>& Key);

	/* Get */
	UFUNCTION(BlueprintPure, Category = "SkillSystemHUD")
	class UStatBarWidget* GetHealthBar() const { return m_pHealthBar; }
	UFUNCTION(BlueprintPure, Category = "SkillSystemHUD")
	class UStatBarWidget* GetManaBar() const { return m_pManaBar; }
	UFUNCTION(BlueprintPure, Category = "SkillSystemHUD")
	class UStatBarWidget* GetExpBar() const { return m_pExpBar; }

	UFUNCTION(BlueprintPure, Category = "SkillSystemHUD")
	const TArray<FKey>& GetHotKeys() const { return m_Hotkeys; }
	UFUNCTION(BlueprintPure, Category = "SkillSystemHUD")
	const int& GetKeysPerRow() const { return m_KeysPerRow; }
	UFUNCTION(BlueprintPure, Category = "SkillSystemHUD")
	const TArray<class USkillHotkeyWidget*>& GetAllHotkeySlots() const { return m_AllHotkeySlots; }



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


	UPROPERTY(EditAnywhere, Category = "SkillSystemHUD")
	TArray<FKey> m_Hotkeys;


	UPROPERTY(EditAnywhere, Category = "SkillSystemHUD")
		int m_KeysPerRow = 0;

	UPROPERTY(BlueprintReadWrite, Category = "SkillSystemHUD")
	TArray<class USkillHotkeyWidget*> m_AllHotkeySlots;


};
