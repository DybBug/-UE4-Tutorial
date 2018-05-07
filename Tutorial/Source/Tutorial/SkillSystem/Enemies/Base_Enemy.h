// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../SkillSystem.h"
#include "../Interfaces/Damageable_Interface.h"
#include "../Interfaces/Selectable_Interface.h"
#include "Base_Enemy.generated.h"

UCLASS()
class TUTORIAL_API ABase_Enemy : public ACharacter, 
	public IDamageable_Interface, public ISelectable_Interface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABase_Enemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	 
public:	

	UFUNCTION(BlueprintCallable, Category = "Base_Enemy")
	void NotifyHit();

	UFUNCTION(BlueprintCallable, Category = "Base_Enemy")
	void PerformAttack();

	UFUNCTION(BlueprintCallable, Category = "Base_Enemy")
	void OnAggroedPulled(class AActor* _pTarget);

	UFUNCTION(BlueprintCallable, Category = "Base_Enemy")
	void OnReset();

	/* Interface Functions*/
	// Damagable_Interface
	virtual void OnReceiveDamage(
		float _BaseDamage,
		EDamageTypes _Type,
		TSubclassOf<class ABase_Element> _ElementClass,
		int _CritChance,
		AActor* _pAttacker,
		class ABase_Skill* _pSpell) override;

	// Selectable_Interface
	virtual void OnSelected(class ASkillCharacter* _pPlayer) override;
	virtual void OnSelectionEnd(class ASkillCharacter* _pPlayer) override;

	/* Get */
	UFUNCTION(BlueprintPure, Category = "Base_Enemy/GetFunc")
	int GetNextAnimationIndex();

	UFUNCTION(BlueprintPure, Category = "Base_Enemy/GetFunc")
	bool GetInAttackRange();

	UFUNCTION(BlueprintPure, Category = "Base_Enemy/GetFunc")
	const bool& GetIsDead() const { return m_bIsDead; }

	UFUNCTION(BlueprintPure, Category = "Base_Enemy/GetFunc")
	const FVector& GetStartingLocation() const { return m_StartingLocation; }

	UFUNCTION(BlueprintPure, Category = "Base_Enemy/GetFunc")
	const float& GetPatrolWalkSpeed() const { return m_PatrolWalkSpeed; }

	UFUNCTION(BlueprintPure, Category = "Base_Enemy/GetFunc")
	const float& GetPatrolRadius() const {return m_PatrolRadius;}

	UFUNCTION(BlueprintPure, Category = "Base_Enemy/GetFunc")
	const bool& GetIsPatrolling() const {return m_bIsPatrolling;}

	/* Set */
	UFUNCTION(BlueprintCallable, Category = "Base_Enemy/SetFunc")
	void SetIsDead(bool& _bIsDead);

	UFUNCTION(BlueprintCallable, Category = "Base_Enemy/SetFunc")
	void SetStartingLocation(FVector& _Location);

	UFUNCTION(BlueprintCallable, Category = "Base_Enemy/SetFunc")
	void SetPatrolWalkSpeed(float& _Speed);

	UFUNCTION(BlueprintCallable, Category = "Base_Enemy/SetFunc")
	void SetPatrolRadius(float _Radius);

	UFUNCTION(BlueprintCallable, Category ="Base_Enemy/SetFunc")
	void SetIsPatrolling(bool _bBool);

private :
	void _Patrol();
	void _InitWidgetText();
	void _UpdateHealthBar();
	void _OnDeath(AActor* _pKiller);
	void _OnRespawn();


	/* AIPerceptionComponent Bind Function*/
	UFUNCTION()
	void _OnPerceptionUpdated(const TArray<AActor*>& _UpdatedActors);

	/* Collision Bind Function */
	UFUNCTION()
	void _OnComponentBeginOverlap(
		UPrimitiveComponent* _pOverlappedComponent,
		AActor* _pOtherActor,
		UPrimitiveComponent* _pOtherComp,
		int _OtherBodyIndex,
		bool _bFromSweep,
		const FHitResult& out_SweepResult);

	UFUNCTION()
	void _OnComponentEndOverlap(
		UPrimitiveComponent* _pOverlappedComponent, 
		AActor* _pOtherActor, 
		UPrimitiveComponent* _pOtherComp, 
		int32 _OtherBodyIndex);

	/* Timer Bind Functions */
	UFUNCTION()
	void _DelayFunc();

	UFUNCTION()
	void _FollowTargetDistance();

protected :
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base_Enemy")
	FName m_Name = "#Unknown";

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base_Enemy")
	int m_Level = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base_Enemy")
	int m_MaxHealth = 200;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base_Enemy")
	int m_CurrHealth = m_MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base_Enemy")
	int m_ExpForKill = 100;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base_Enemy")
	float m_DeadBodyTime = 4.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base_Enemy")
	float m_RespawnDelay = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base_Enemy")
	TArray<class UAnimMontage*> m_AttackAnimMontages;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base_Enemy")
	float m_PatrolWalkSpeed = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base_Enemy")
	float m_AggroedWalkSpeed = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base_Enemy")
	float m_PatrolRadius = 800.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base_Enemy")
	float m_FollowPlayerRadius = 2000.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base_Enemy")
	float m_AttackRange = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base_Enemy")
	float m_AttackTraceDistance = 90.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base_Enemy")
	float m_AttackDamage = 35.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base_Enemy")
	EDamageTypes m_DamageType = EDamageTypes::Physical;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base_Enemy")
	TSubclassOf<class ABase_Element> m_ElementClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base_Enemy")
	float m_CriticalChance = 25.f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Base_Enemy")
	class UWidgetComponent* m_pWidget;

	// 이 영역에 들어오면 위젯이 보임.
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Base_Enemy")
	class USphereComponent* m_pWidgetVisibleDomain;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Base_Enemy")
	class UAIPerceptionComponent* m_pAIPerception;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Base_Enemy")
	class AActor* m_pTargetActor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Base_Enemy")
	int m_CurrAttackAnimMontageIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Base_Enemy")
	class UAnimMontage* m_pCurrAttackAnimMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Base_Enemy")
	bool m_bIsDead = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Base_Enemy")
	FVector m_StartingLocation;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Base_Enemy")
	bool m_bIsPatrolling = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Base_Enemy")
	bool m_bWasAggroed = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Base_Enemy")
	bool m_bAggressive = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Base_Enemy")
	bool m_bIsRunningBack = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Base_Enemy")
	bool m_bDoesRespawn = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Base_Enemy")
	bool m_bIsSelected = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Base_Enemy")
	class ASkillCharacter* m_pSelectingPlayer = nullptr;

	/* Timer Handles */
	UPROPERTY()
	FTimerHandle m_hFollowTargetDistanceTimer;
};
