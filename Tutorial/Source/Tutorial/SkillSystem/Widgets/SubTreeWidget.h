// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <Blueprint/UserWidget.h>
#include "../SkillSystem.h"
#include "../Widgets/SkillTreeWidget.h"
#include "SubTreeWidget.generated.h"


class USkillTreeEntryWidget;
class UConnectionWidget;
class USkillTreeWidget;
class UCanvasPanel;
/**
 * 
 */
UCLASS()
class TUTORIAL_API USubTreeWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;

	UFUNCTION()
		void Initialize(const FSubTreeContent& _NewContent, USkillTreeWidget* _pSkillTreeWidget);

	void GenerateContent();

	/*Get*/
	UFUNCTION(BlueprintPure, Category = "SubTreeWidget")
	USkillTreeWidget* GetSkillTreeWidget() const { return m_pSkillTreeWidget; }

	UFUNCTION(BlueprintPure, Category = "SubTreeWidget")
	const TArray<USkillTreeEntryWidget*>& GetSkillTreeEntryWidgets() const { return m_pSkillTreeEntryWidgets; }

	UFUNCTION(BlueprintPure, Category = "SubTreeWidget")
	const TArray<UConnectionWidget*>& GetConnectionWidgets() const { return m_pConnectionWidgets; }

protected :
	FSubTreeContent m_Content;

	UPROPERTY()
	TArray<USkillTreeEntryWidget*> m_pSkillTreeEntryWidgets;
	
	UPROPERTY()
	TArray<UConnectionWidget*> m_pConnectionWidgets;

	UPROPERTY()
	USkillTreeWidget* m_pSkillTreeWidget;

	UPROPERTY()
	UCanvasPanel* m_pCanvas;
};
