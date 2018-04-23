// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillSystem/SkillActors/Base_Skill.h"
#include "AOE_Skill.generated.h"

/**
 * 
 */
UCLASS()
class TUTORIAL_API AAOE_Skill : public ABase_Skill
{
	GENERATED_BODY()
public :
	AAOE_Skill();

	UFUNCTION(BlueprintCallable, Category = "AOE_Skill")
	virtual void OnSkillNotify() override;

protected :
	UPROPERTY(EditDefaultsOnly, Category = "AOE_Skill")
	class UParticleSystem* m_pParticleEffect;
	
};
