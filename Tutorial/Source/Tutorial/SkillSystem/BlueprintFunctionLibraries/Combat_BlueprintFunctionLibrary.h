// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "../SkillSystem.h"
#include "Combat_BlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class TUTORIAL_API UCombat_BlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public :
	UFUNCTION()
	static bool IsEnemy(AActor* _pActor);

	UFUNCTION()
	static void CalculateFinalDamage(
	float _BaseDamage, 
	int _CritChance, 
	TSubclassOf<class ABase_Element> _AttackerElementClass, 
	TSubclassOf<class ABase_Element> _DefenderElementClass,
	int& out_Damage,
	bool& out_IsCritical,
	EEffectiveness& out_Effectiveness);
	
};
