// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Selectable_Interface.generated.h"

class ASkillCharacter;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USelectable_Interface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TUTORIAL_API ISelectable_Interface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION()
	virtual void OnSelected(ASkillCharacter* _pPlayer) = 0;

	UFUNCTION()
	virtual void OnSelectionEnd(ASkillCharacter* _pPlayer) = 0;
	
};
