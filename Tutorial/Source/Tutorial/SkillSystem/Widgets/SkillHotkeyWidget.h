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
	
	/*virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

	virtual bool NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;*/

	UFUNCTION(BlueprintCallable, Category = "SkillHotkeyWidget")
	void ClearAssignedSpell();

	UFUNCTION(BlueprintCallable, Category = "SkillHotkeyWidget")
	void EnableHotkey();

	UFUNCTION(BlueprintCallable, Category = "SkillHotkeyWidget")
	void DisableHotkey();

	UFUNCTION(BlueprintCallable, Category = "SkillHotkeyWidget")
	void AssignSpell(class ABase_Skill* _Spell);

	UFUNCTION(BlueprintCallable, Category = "SkillHotkeyWidget")
	void ResetStyle();

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
	class UTextBlock* GetHotkeyName() const { return m_pHotkeyName; }

	UFUNCTION(BlueprintPure, Category = "SkillHotkeyWidget")
	class UTextBlock* GetCooldownText() const { return m_pCooldownText; }

	UFUNCTION(BlueprintPure, Category = "SkillHotkeyWidget")
	class UButton* GetSkillButton() const { return m_pSkillButton; }

	UFUNCTION(BlueprintPure, Category = "SkillHotkeyWidget")
	class UImage* GetBaseImage() const { return m_pBaseImage; }

	UFUNCTION(BlueprintPure, Category = "SkillHotkeyWidget")
	class UImage* GetSkillIcon() const { return m_pSkillIcon; }

	UFUNCTION(BlueprintPure, Category = "SkillHotkeyWidget")
	class UImage* GetColldownImage() const { return m_pCooldownImage; }


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
	
	UPROPERTY(BlueprintReadWrite, Category = "SkillHotkeyWidget")
	class ABase_Skill* m_pAssignedSpell;

	UPROPERTY(BlueprintReadWrite, Category = "SkillHotkeyWidget")
	class UMaterialInstanceDynamic* m_pDynamicMaterial;	

	UPROPERTY(BlueprintReadWrite, Category = "SkillHotkeyWidget")
	bool m_bDeactivated = false;

	UPROPERTY(BlueprintReadWrite, Category = "SkillHotkeyWidget")
	bool m_bDraggedOver = false;

	UPROPERTY(BlueprintReadWrite, Category = "SkillHotkeyWidget")
	FLinearColor m_DragOverColor = FColor(0x7FFFD100);
	
	UPROPERTY(BlueprintReadWrite, Category = "SkillHotkeyWidget")
	FLinearColor m_DefaultColor = FColor(0x33FFFFFF);

	UPROPERTY(BlueprintReadWrite, Category = "SkillHotkeyWidget")
	class UTimelineComponent* m_pCooldownTimeline;
	
	UPROPERTY(BlueprintReadWrite, Category = "SkillHotkeyWidget")
	class UTextBlock* m_pHotkeyName;

	UPROPERTY(BlueprintReadWrite, Category = "SkillHotkeyWidget")
	class UTextBlock* m_pCooldownText;

	UPROPERTY(BlueprintReadWrite, Category = "SkillHotkeyWidget")
	class UButton* m_pSkillButton;

	UPROPERTY(BlueprintReadWrite, Category = "SkillHotkeyWidget")
	class UImage* m_pBaseImage;

	UPROPERTY(BlueprintReadWrite, Category = "SkillHotkeyWidget")
	class UImage* m_pSkillIcon;

	UPROPERTY(BlueprintReadWrite, Category = "SkillHotkeyWidget")
	class UImage* m_pCooldownImage;
};
