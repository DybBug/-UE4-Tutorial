// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillHotkeyWidget.generated.h"

/**
 * 
 */
UCLASS()
class TUTORIAL_API USkillHotkeyWidget : public UUserWidget
{
	GENERATED_BODY()
	
public :
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "SkillHotkeyWidget")
	bool ClearAssignedSpell();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "SkillHotkeyWidget")
	bool EnableHotkey();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "SkillHotkeyWidget")
	bool DisableHotkey();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "SkillHotkeyWidget")
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
	
};
