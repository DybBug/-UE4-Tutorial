// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "../SkillSystem.h"
#include "Damageable_Interface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDamageable_Interface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TUTORIAL_API IDamageable_Interface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION()
	virtual void OnReceiveDamage(
		float _BaseDamage,
		EDamageTypes _Type,
		TSubclassOf<class ABase_Element> _ElementClass, 
		int _CritChance, AActor* _pAttacker, 
		class ABase_Skill* _pSpell);
	
};
