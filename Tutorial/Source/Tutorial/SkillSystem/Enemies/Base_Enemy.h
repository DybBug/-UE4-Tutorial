// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Base_Enemy.generated.h"

UCLASS()
class TUTORIAL_API ABase_Enemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABase_Enemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	 
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Base_Enemy")
	void NotifyHits();

	UFUNCTION(BlueprintCallable, Category = "Base_Enemy")
	void PerformAttack();

	UFUNCTION(BlueprintCallable, Category = "Base_Enemy")
	void OnAggroedPulled(class AActor* _pTarget);

	UFUNCTION(BlueprintCallable, Category = "Base_Enemy")
	void OnReset();

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

	UFUNCTION()
	void _OnPerceptionUpdated(const TArray<AActor*>& _UpdatedActors);

	/* Timer Bind Functions */
	UFUNCTION()
	void _DelayFunc();

	UFUNCTION()
	void _FollowTargetDistance();

protected :
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base_Enemy", meta = (ExposeOnSpawn = "true"))
	float m_PatrolWalkSpeed = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base_Enemy", meta = (ExposeOnSpawn = "true"))
	float m_AggroedWalkSpeed = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base_Enemy", meta = (ExposeOnSpawn = "true"))
	float m_PatrolRadius = 800.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base_Enemy", meta = (ExponseOnSpawn = "true"))
	float m_FollowPlayerRadius = 2000.f;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base_Enemy", meta = (ExposeOnSpawn = "true"))
	float m_AttackRange = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base_Enemy", meta = (ExposeOnSpawn = "true"))
	TArray<class UAnimMontage*> m_AttackAnimMontages;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Base_Enemy")
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

	/* Timer Handles */
	UPROPERTY()
	FTimerHandle m_hFollowTargetDistanceTimer;
};
