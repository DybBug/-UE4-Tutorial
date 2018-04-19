// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "SkillHotkeyWidget.generated.h"

/**
 * 
 */
UCLASS()
class TUTORIAL_API USkillHotkeyWidget : public UUserWidget
{
	GENERATED_BODY()
	
public :
	virtual void NativeConstruct() override;
	

	UFUNCTION(BlueprintCallable, Category = "SkillHotkeyWidget")
	void ClearAssignedSpell();

	UFUNCTION(BlueprintCallable, Category = "SkillHotkeyWidget")
	void EnableHotkey();

	UFUNCTION(BlueprintCallable, Category = "SkillHotkeyWidget")
	void DisableHotkey();

	UFUNCTION(BlueprintCallable, Category = "SkillHotkeyWidget")
	void AssignSpell(class ABase_Skill* _Spell);

	/* Get */
	UFUNCTION(BlueprintPure, Category = "SkillHotkeyWidget")
	const FKey& GetHotKey() const { return m_Hotkey; }

	UFUNCTION(BlueprintPure, Category = "SkillHotkeyWidget")
	class ABase_Skill* GetAssignedSpell() const { return m_pAssignedSpell; }

	UFUNCTION(BlueprintPure, Category = "SkillHotkeyWidget")
	class UMaterialInstanceDynamic* GetDynamicMaterial() const { return m_pDynamicMaterial; }

	UFUNCTION(BlueprintPure, Category = "SkillHotkeyWidget")
	const bool& GetDeactivated() const { return m_bDeactivated; }

	UFUNCTION(BlueprintPure, Category = "SkillHotkeyWidget")
	class UTextBlock* GetHotkeyName_Widget() const { return m_pHotkeyName; }

	UFUNCTION(BlueprintPure, Category = "SkillHotkeyWidget")
	class UTextBlock* GetCooldownText_Widget() const { return m_pCooldownText; }

	UFUNCTION(BlueprintPure, Category = "SkillHotkeyWidget")
	class UButton* GetSkillButton_Widget() const { return m_pSkillButton; }

	UFUNCTION(BlueprintPure, Category = "SkillHotkeyWidget")
	class UImage* GetSkillIcon_Widget() const { return m_pSkillIcon; }

	UFUNCTION(BlueprintPure, Category = "SkillHotkeyWidget")
	class UImage* GetColldownImage_Widget() const { return m_pCooldownImage; }


	/* Set */
	UFUNCTION(BlueprintCallable, Category = "SkillHotkeyWidget")
	void SetHotkey(FKey _Key);

	UFUNCTION(BlueprintCallable, Category = "SkillHotkeyWidget")
	void SetAssignedSpell(class ABase_Skill* _pSpell);

	UFUNCTION(BlueprintCallable, Category = "SkillHotkeyWidget")
	void SetDynamicMaterial(class UMaterialInstanceDynamic* _pMaterial);	
	
	UFUNCTION(BlueprintCallable, Category = "SkillHotkeyWidget")
	void SetDeactivated(bool _bBool);

protected :
	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn = "true"), Category = "SkillHotkeyWidget")
	FKey m_Hotkey;
	
	UPROPERTY()
	class ABase_Skill* m_pAssignedSpell;

	UPROPERTY()
	class UMaterialInstanceDynamic* m_pDynamicMaterial;	

	UPROPERTY()
	bool m_bDeactivated = false;

	UPROPERTY()
	class UTimelineComponent* m_pCooldownTimeline;
	
	UPROPERTY()
	class UTextBlock* m_pHotkeyName;
	UPROPERTY()
	class UTextBlock* m_pCooldownText;
	UPROPERTY()
	class UButton* m_pSkillButton;
	UPROPERTY()
	class UImage* m_pSkillIcon;
	UPROPERTY()
	class UImage* m_pCooldownImage;
};
