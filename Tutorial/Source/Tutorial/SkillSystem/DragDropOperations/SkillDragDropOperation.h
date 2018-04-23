// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "SkillDragDropOperation.generated.h"

/**
 * 
 */
UCLASS()
class TUTORIAL_API USkillDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()
	
public :
	/* Get */
	UFUNCTION(BlueprintPure, Category = "SkillDragDropOperation")
	class USkillHotkeyWidget* GetFromHotkey() const { return m_pFromHotkey; }

	UFUNCTION(BlueprintPure, Category = "SkillDragDropOperation")
	class ABase_Skill* GetSkillActor() const { return m_pSkillActor; }

	/* Set */
	UFUNCTION(BlueprintCallable, Category = "SkillDragDr0pOperation")
	void SetFromHotkey(class USkillHotkeyWidget* _pFromHotkey);

	UFUNCTION(BlueprintCallable, Category = "SkillDragDropOperation")
	void SetSkillActor(class ABase_Skill* _pSkillActor);
protected :
	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn = "true"), Category = "SkillDragDropOperation" )
	class USkillHotkeyWidget* m_pFromHotkey;

	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn = "true"), Category = "SkillDragDropOperation")
	class ABase_Skill* m_pSkillActor;
	
};
