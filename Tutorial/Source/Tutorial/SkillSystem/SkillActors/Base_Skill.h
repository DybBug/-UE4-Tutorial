// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../SkillSystem.h"
#include "Base_Skill.generated.h"

UCLASS()
class TUTORIAL_API ABase_Skill : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABase_Skill();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category ="Base_Skill")
	void OnTryCastSpell();

	UFUNCTION(BlueprintCallable, Category = "Base_Skill")
	void InitializeSpellCast();

	UFUNCTION(BlueprintCallable, Category = "Base_Skill")
	void OnSpellCast();

	UFUNCTION(BlueprintCallable, Category = "Base_Skill")
	void OnCastCompleted();

	UFUNCTION(BlueprintCallable, Category = "Base_Skill")
	void OnCooldownExpired();

	/* Get */
	UFUNCTION(BlueprintPure, Category = "Base_Skill")
	const FSkillInfo& GetSkillInfo() const { return m_SkillInfo; }

	UFUNCTION(BlueprintPure, Category = "Base_Skill")
	class USkillHotkeyWidget* GetHotkeyWidget() const { return m_pHotkeyWidget; }

	UFUNCTION(BlueprintPure, Category = "Base_Skill")
	const bool& GetOnCooldown() const { return m_bOnCooldown; }

	UFUNCTION(BlueprintPure, Category = "Base_Skill")
	const bool& GetCurrentlyCasted() const { return m_bCurrentlyCasted; }

	UFUNCTION(BlueprintPure, Category = "Base_Skill")
	const FSkillStage& GetCurrStage() const { return m_SkillInfo.Stages[m_CurrStageIndex]; }

	/* Set */
	UFUNCTION(BlueprintCallable, Category = "Base_Skill")
	void SetSkillInfo(const FSkillInfo& _SkillInfo);

	UFUNCTION(BlueprintCallable, Category = "Base_Skill")
	void SetHotkeyWidget(class USkillHotkeyWidget* _pWidget);

	UFUNCTION(BlueprintCallable, Category = "Base_Skill")
	void SetOnCooldown(bool _bBool);

	UFUNCTION(BlueprintCallable, Category = "Base_Skill")
	void SetCurrentlyCasted(bool _bBool);



protected :
	UPROPERTY(EditDefaultsOnly, Category = "Base_Skill")
	FSkillInfo m_SkillInfo;

	UPROPERTY(VisibleAnywhere, Category = "Base_Skill")
	class USkillHotkeyWidget* m_pHotkeyWidget = nullptr;
	
	UPROPERTY(VisibleAnywhere, Category = "Base_Skill")
	int m_CurrStageIndex;

	UPROPERTY(VisibleAnywhere, Category = "Base_Skill")
	bool m_bOnCooldown = false;

	UPROPERTY(VisibleAnywhere, Category = "Base_Skill")
	bool m_bCurrentlyCasted = false;


};
