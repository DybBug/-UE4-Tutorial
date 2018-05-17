// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <Blueprint/UserWidget.h>
#include "ConnectionWidget.generated.h"

class ABase_Skill;
class UImage;
/**
 * 
 */
UCLASS()
class TUTORIAL_API UConnectionWidget : public UUserWidget
{
	GENERATED_BODY()

public :
	virtual bool Initialize() override;

	void Initialize(const TSubclassOf<ABase_Skill>& _ForSpellClass);

	void Active();

	/* Get */
	UFUNCTION(BlueprintPure, Category = "ConnectionWidget")
	const TSubclassOf<ABase_Skill>& GetForSpellClass() const { return m_ForSpellClass; }
	
protected :
	UPROPERTY()
	UImage* m_pLine;
	
	UPROPERTY()
	TSubclassOf<ABase_Skill> m_ForSpellClass;

	bool m_bIsActive = false;

	
};
