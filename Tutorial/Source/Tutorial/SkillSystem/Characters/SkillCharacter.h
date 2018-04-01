// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../SkillSystem.h"
#include "SkillCharacter.generated.h"

UCLASS()
class TUTORIAL_API ASkillCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASkillCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;




	/* Get */
	UFUNCTION(BlueprintPure, Category = "SkillCharacter")
	class USpringArmComponent* GetSpringArm() const { return m_pSpringArm; }

	UFUNCTION(BlueprintPure, Category = "SkillCharacter")
	class UCameraComponent* GetCamera() const { return m_pCamera; }

	UFUNCTION(BlueprintPure, Category = "SkillCharacter")
	FStatData GetStat(EStats _Stat) const { return m_Stats[_Stat]; }

private:
	void _SetupStatBars();
	void _UpdateStat(EStats _Stat); // StatBarWidget의 정보 업데이트.
	void _ModifyStat(EStats _Stat, int _Value, bool _bIsAnimated = false);

	void _SetupRegenerations();
	void _HealthRegenTick();
	void _ManaRegenTick();
	void _HandleRegeneration(EStats _Stat);

	void _OnStatLerpStart(EStats _Stat);
	UFUNCTION()
	void _OnHealthStatLerpEnd();
	void _OnManaStatLerpEnd();
	void _OnExpStatLerpEnd();
	void _LerpStatDisplay(EStats _Stat, float _LerpTime, bool _bInPositive);
	UFUNCTION()
	void _HealthStatLerpTick();
	void _ManaStatLerpTick();
	void _ExpStatLerpTick();

	void _Increase();
	void _Decrease();


protected:
	UPROPERTY(VisibleAnywhere, Category = "SkillCharacter")
	class UCameraComponent* m_pCamera = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "SkillCharacter")
	class USpringArmComponent* m_pSpringArm = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "SkillCharacter")
	class UTimelineComponent* m_pHealthTimeline = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "SkillCharacter")
	class UTimelineComponent* m_pManaTimeline = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "SkillCharacter")
	class UTimelineComponent* m_pExpTimeline = nullptr;

	UPROPERTY()
	TSubclassOf<class UUserWidget> m_HUDClass = nullptr;

	UPROPERTY()
	class USkillSystemHUD* m_pHUD = nullptr;

	UPROPERTY()
	TMap<EStats, FStatData> m_Stats;

	UPROPERTY()
	TMap<EStats, FStatLerp> m_StatLerpData;

	
};
