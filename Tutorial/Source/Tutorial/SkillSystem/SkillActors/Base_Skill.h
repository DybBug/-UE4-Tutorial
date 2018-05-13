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

	// 기술 통보.
	UFUNCTION(BlueprintCallable, Category = "Base_Skill")
	virtual void OnSkillNotify();

	UFUNCTION(BlueprintCallable, Category ="Base_Skill")
	virtual void OnTryCastSpell();

	UFUNCTION(BlueprintCallable, Category = "Base_Skill")
	virtual void InitializeSpellCast();

	UFUNCTION(BlueprintCallable, Category = "Base_Skill")
	virtual void OnSpellCast();

	UFUNCTION(BlueprintCallable, Category = "Base_Skill")
	void OnCastCompleted();

	UFUNCTION(BlueprintCallable, Category = "Base_Skill")
	void OnCooldownExpired();


	UFUNCTION(BlueprintCallable, Category = "Base_Skill")
	void PlayAnimation(class UAnimMontage* _pAnimationMontage, float _InPlayRate = 1.0f);

	UFUNCTION(BlueprintPure, Category = "Base_Skill")
	const FSkillStage& GetCurrStage() const;

	UFUNCTION(BlueprintPure, Category = "Base_Skill")
	const FSkillStage& GetNextStage() const;

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
	const int& GetCurrStageIndex() const { return m_CurrStageIndex; }

	/* Set */
	UFUNCTION(BlueprintCallable, Category = "Base_Skill")
	void SetSkillInfo(const FSkillInfo& _SkillInfo);

	UFUNCTION(BlueprintCallable, Category = "Base_Skill")
	void SetHotkeyWidget(class USkillHotkeyWidget* _pWidget);

	UFUNCTION(BlueprintCallable, Category = "Base_Skill")
	void SetOnCooldown(bool _bBool);

	UFUNCTION(BlueprintCallable, Category = "Base_Skill")
	void SetCurrentlyCasted(bool _bBool);

	UFUNCTION(BlueprintCallable, Category = "Base_Skill")
	void SetCurrStageIndex(int _Index);

protected :
	UFUNCTION()
	void _OnCooldownTimelineUpdate();

	UFUNCTION()
	void _OnCooldownTimelineFinished();

	UFUNCTION()
	void _OnCastingTimelineUpdate();

	UFUNCTION()
	void _OnCastingTimelineFinished();

	UFUNCTION()
	void _OnCastCompleted();



protected :
	UPROPERTY(EditDefaultsOnly, Category = "Base_Skill")
	FSkillInfo m_SkillInfo;

	UPROPERTY(EditDefaultsOnly, Category = "Base_Skill")
	class UAnimMontage* m_pSkillAnim;

	UPROPERTY(VisibleAnywhere, Category = "Base_Skill")
	class ASkillCharacter* m_pPlayer;

	UPROPERTY(VisibleAnywhere, Category = "Base_Skill")
	class USkillHotkeyWidget* m_pHotkeyWidget = nullptr;
	
	UPROPERTY(VisibleAnywhere, Category = "Base_Skill")
	int m_CurrStageIndex = -1;

	UPROPERTY(VisibleAnywhere, Category = "Base_Skill")
	bool m_bOnCooldown = false;

	UPROPERTY(VisibleAnywhere, Category = "Base_Skill")
	bool m_bCurrentlyCasted = false;

	UPROPERTY(VisibleAnywhere, Category = "Base_Skill")
	class UTimelineComponent* m_pCooldownTimeline;

	UPROPERTY(VisibleAnywhere, Category = "Base_Skill")
	class UTimelineComponent* m_pCastingTimeline;
};
