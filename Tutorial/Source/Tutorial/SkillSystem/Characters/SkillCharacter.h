// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <GameFramework/Character.h>
#include "../Widgets/SkillSystemHUD.h"
#include "../SkillSystem.h"
#include "../Interfaces/Damageable_Interface.h"
#include "../SkillActors/Base_Skill.h"

#include "SkillCharacter.generated.h"


class UCameraComponent;
class USpringArmComponent;
class USkillTreeComponent;
class ABase_Skill;

UCLASS()
class TUTORIAL_API ASkillCharacter : public ACharacter, public IDamageable_Interface
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

	UFUNCTION()
	void ModifyStat(EStats _Stat, int _Value, bool _bIsAnimated = false);

	UFUNCTION()
	void BeginSpellCast(class ABase_Skill* _pCastedSkill);

	UFUNCTION()
	void EndSpellCast(class ABase_Skill* _pCastedSkill);

	UFUNCTION()
	void HasBuff(TSubclassOf<class ABuff_Skill> _BuffClass, bool& out_bHasBuff, class ABuff_Skill*& out_pFoundBuff);

	UFUNCTION()
	class UBuffWidget* AddBuff(class ABuff_Skill* _pNewBuff);

	UFUNCTION()
	void RemoveBuff(ABuff_Skill* _pBuff);

	/* Interface */
	virtual void OnReceiveDamage(
		float _BaseDamage,
		EDamageTypes _Type,
		TSubclassOf<class ABase_Element> _ElementClass,
		int _CritChance, AActor* _pAttacker,
		class ABase_Skill* _pSpell) override;


	/* Get */
	UFUNCTION(BlueprintPure, Category = "SkillCharacter")
	USpringArmComponent* GetSpringArm() const { return m_pSpringArm; }

	UFUNCTION(BlueprintPure, Category = "SkillCharacter")
	UCameraComponent* GetCamera() const { return m_pCamera; }

	UFUNCTION(BlueprintPure, Category = "SkillCharacter")
	const FStatData& GetStat(EStats _Stat) const { return m_Stats[_Stat]; }

	UFUNCTION(BlueprintPure, Category = "SkillCharacter")
	ABase_Skill* GetCurrentSpell() const { return m_pCurrentSpell; }

	UFUNCTION(BlueprintPure, Category = "SkillCharacter")
	const bool& GetIsCasting() const { return m_bIsCasting; }

	UFUNCTION(BlueprintPure, Category = "SkillCharacter")
	USkillSystemHUD* GetHUD() const { return m_pHUD; }

	UFUNCTION(BlueprintPure, Category = "SkillCharacter")
	ABase_Enemy* GetSelectedEnemy() const { return m_pSelectedEnemy; }

	UFUNCTION(BlueprintPure, Category = "SkillCharacter")
	const TSet<TSubclassOf<ABase_Skill>>& GetLearntSpellClasses() const { return m_LearntSpellClasses; }

	UFUNCTION(BlueprintPure, Category = "SkillCharacter")
	const int& GetCurrLevel() const { return m_CurrLevel; }

	UFUNCTION(BlueprintPure, Category = "SkillCharacter")
	USkillTreeComponent* GetSkillTreeComponent() const { return m_pSkillTree; }



	/* Set */
	UFUNCTION(BlueprintCallable, Category = "SkillCharacter")
	void SetSelectedEnemy(ABase_Enemy* _pNewEnemy);

private:
	void _SetupStatBars();
	void _UpdateStat(EStats _Stat); // StatBarWidget의 정보 업데이트.
	void _UpdateLevel(); // LevelText 정보 업데이트.

	void _IncreaseLevel();

	void _SetupRegenerations();
	void _HealthRegenTick();
	void _ManaRegenTick();
	void _HandleRegeneration(EStats _Stat);

	UFUNCTION()
	void _OnStatLerpStart(EStats _Stat);

	UFUNCTION()
	void _OnHealthStatLerpEnd();

	UFUNCTION()
	void _OnManaStatLerpEnd();

	UFUNCTION()
	void _OnExpStatLerpEnd();

	UFUNCTION()
	void _LerpStatDisplay(EStats _Stat, float _LerpTime, bool _bInPositive);

	UFUNCTION()
	void _HealthStatLerpTick();

	UFUNCTION()
	void _ManaStatLerpTick();

	UFUNCTION()
	void _ExpStatLerpTick();


	/* 키 입력 관련 함수들. */
	void _AnyKey();

protected:
	UPROPERTY(EditAnywhere, Category = "SkillCharacter")
	TSubclassOf<ABase_Element> m_ElementClass;

	UPROPERTY(VisibleDefaultsOnly, Category = "SkillCharacter")
	UCameraComponent* m_pCamera = nullptr;

	UPROPERTY(VisibleDefaultsOnly, Category = "SkillCharacter")
	USpringArmComponent* m_pSpringArm = nullptr;

	UPROPERTY(VisibleDefaultsOnly, Category = "SkillCharacter")
	USkillTreeComponent* m_pSkillTree;

	UPROPERTY(VisibleDefaultsOnly, Category = "SkillCharacter")
	UTimelineComponent* m_pHealthTimeline = nullptr;

	UPROPERTY(VisibleDefaultsOnly, Category = "SkillCharacter")
	UTimelineComponent* m_pManaTimeline = nullptr;

	UPROPERTY(VisibleDefaultsOnly, Category = "SkillCharacter")
	UTimelineComponent* m_pExpTimeline = nullptr;	

	UPROPERTY(VisibleAnywhere, Category = "SkillCharacter")
	ABase_Skill* m_pCurrentSpell;

	UPROPERTY(VisibleAnywhere, Category = "SkillCharacter")
	TArray<ABuff_Skill*> m_pCurrentBuffs;

	UPROPERTY(VisibleAnywhere, Category = "SkillCharacter")
	ABase_Enemy* m_pSelectedEnemy;

	UPROPERTY()
	TSet<TSubclassOf<ABase_Skill>> m_LearntSpellClasses;

	UPROPERTY()
	TSubclassOf<UUserWidget> m_HUDClass = nullptr;

	UPROPERTY()
	TSubclassOf<UBuffWidget> m_BuffWidgetClass = nullptr;

	UPROPERTY()
	USkillSystemHUD* m_pHUD = nullptr;

	UPROPERTY()
	TMap<EStats, FStatData> m_Stats;

	UPROPERTY()
	TMap<EStats, FStatLerp> m_StatLerpData;

	UPROPERTY()
	bool m_bIsCasting = false;	

	UPROPERTY()
	int m_CurrLevel = 1;

	UPROPERTY()
	int m_RestExp = 0;

	
};
