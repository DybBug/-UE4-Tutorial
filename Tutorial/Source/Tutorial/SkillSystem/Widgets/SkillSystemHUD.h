// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StatBarWidget.h"

#include <Blueprint/UserWidget.h>
#include <Components/VerticalBox.h>
#include <Components/HorizontalBox.h>
#include <Components/Overlay.h>
#include <Components/TextBlock.h>
#include <Components/ProgressBar.h>
#include <Components/Border.h>
#include "SkillSystemHUD.generated.h"

/**
 * 
 */
UCLASS()
class TUTORIAL_API USkillSystemHUD : public UUserWidget
{
	GENERATED_BODY()

public :
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, Category = "SkillSystemHUD")
	void GenerateHotkeys(const TArray<FKey>& _Key);

	/* Get */
	UFUNCTION(BlueprintPure, Category = "SkillSystemHUD")
	class UStatBarWidget* GetHealthBar() const { return m_pHealthBar; }

	UFUNCTION(BlueprintPure, Category = "SkillSystemHUD")
	class UStatBarWidget* GetManaBar() const { return m_pManaBar; }

	UFUNCTION(BlueprintPure, Category = "SkillSystemHUD")
	class UStatBarWidget* GetExpBar() const { return m_pExpBar; }

	UFUNCTION(BlueprintPure, Category = "SkillSystemHUD")
	class USkillTreeWidget* GetSkillTree() const { return m_pSkillTree; }

	UFUNCTION(BlueprintPure, Category = "SkillSystemHUD")
	const TArray<FKey>& GetHotKeys() const { return m_Hotkeys; }

	UFUNCTION(BlueprintPure, Category = "SkillSystemHUD")
	const int& GetKeysPerRow() const { return m_KeysPerRow; }

	UFUNCTION(BlueprintPure, Category = "SkillSystemHUD")
	const TArray<class USkillHotkeyWidget*>& GetAllHotkeySlots() const { return m_AllHotkeySlots; }

	UFUNCTION(BlueprintPure, Category = "SkillSystemHUD")
	class UOverlay* GetCastingOverlay() const { return m_pCastingOverlay; }

	UFUNCTION(BlueprintPure, Category = "SkillSystemHUD")
	class UTextBlock* GetCastSpellText() const { return m_pCastedSpellText; }

	UFUNCTION(BlueprintPure, Category = "SkillSystemHUD")
	class UTextBlock* GetCastTimeText() const { return m_pCastTimeText; }

	UFUNCTION(BlueprintPure, Category = "SkillSystemHUD")
	class UProgressBar* GetCastingBar() const { return m_pCastingBar; }

	UFUNCTION(BlueprintPure, Category = "SkillSystemHUD")
	class UBorder* GetEnemyBorder() const { return m_pEnemyBorder; }

	UFUNCTION(BlueprintPure, Category = "SkillSystemHUD")
	class UTextBlock* GetEnemyNameText() const { return m_pEnemyNameText; }

	UFUNCTION(BlueprintPure, Category = "SkillSystemHUD")
	class UTextBlock* GetEnemyHealthText() const { return m_pEnemyHealthText; }

	UFUNCTION(BlueprintPure, Category = "SkillSystemHUD")
	class UProgressBar* GetEnemyHealthBar() const { return m_pEnemyHealthBar; }

	UFUNCTION(BlueprintPure, Category = "SkillSystemHUD")
	class UHorizontalBox* GetBuffContainer() const { return m_pBuffContainer; }

	UFUNCTION(BlueprintPure, Category = "SkillSystemHUD")
	class UTextBlock* GetLevelText() const { return m_pLevelText; }



	/* Set */
	UFUNCTION(BlueprintCallable, Category = "SkillSystemHUD")
	void SetHealthBar(class UStatBarWidget* _Widget) { m_pHealthBar = _Widget; }

	UFUNCTION(BlueprintCallable, Category = "SkillSystemHUD")
	void SetManaBar(class UStatBarWidget* _Widget) { m_pManaBar = _Widget; }

	UFUNCTION(BlueprintCallable, Category = "SkillSystemHUD")
	void SetExpBar(class UStatBarWidget* _Widget) { m_pExpBar = _Widget; }

private :
	virtual bool NativeOnDrop(const FGeometry& _InGeometry, const FDragDropEvent& _InDragDropEvent, UDragDropOperation* _pInOperation) override;

	
protected :
	UPROPERTY(VisibleAnywhere, Category = "SkillSystemHUD")
	class UStatBarWidget* m_pHealthBar;

	UPROPERTY(VisibleAnywhere, Category = "SkillSystemHUD")
	class UStatBarWidget* m_pManaBar;

	UPROPERTY(VisibleAnywhere, Category = "SkillSystemHUD")
	class UStatBarWidget* m_pExpBar;

	UPROPERTY(VisibleAnywhere, Category = "SkillSystemHUD")
	class USkillTreeWidget* m_pSkillTree;

	UPROPERTY(EditAnywhere, Category = "SkillSystemHUD")
	TArray<FKey> m_Hotkeys;


	UPROPERTY(EditAnywhere, Category = "SkillSystemHUD")
	int m_KeysPerRow = 0;

	UPROPERTY(BlueprintReadWrite, Category = "SkillSystemHUD")
	TArray<class USkillHotkeyWidget*> m_AllHotkeySlots;


	/* */
	UPROPERTY()
	class UVerticalBox* m_pHotkeyRowContainer;

	UPROPERTY()
	class UOverlay* m_pCastingOverlay;

	UPROPERTY()
	class UTextBlock* m_pCastedSpellText;

	UPROPERTY()
	class UTextBlock* m_pCastTimeText;

	UPROPERTY()
	class UProgressBar* m_pCastingBar;

	UPROPERTY()
	class UBorder* m_pEnemyBorder;

	UPROPERTY()
	class UTextBlock* m_pEnemyNameText;

	UPROPERTY()
	class UTextBlock* m_pEnemyHealthText;

	UPROPERTY()
	class UProgressBar* m_pEnemyHealthBar;

	UPROPERTY()
	class UHorizontalBox* m_pBuffContainer;

	UPROPERTY()
	class UTextBlock* m_pLevelText;

};
