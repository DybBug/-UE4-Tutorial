// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Base2_Enemy.generated.h"

class UBehaviorTree;
class UPawnSensingComponent;
class UWidgetComponent;
class UBTTask_BlueprintBase;
class UAnimMontage;
class UEnemy2Widget;

UCLASS()
class TUTORIAL_API ABase2_Enemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABase2_Enemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual float TakeDamage(
			float _DamageAmount,
			FDamageEvent const& _DamageEvent,
			AController* _pEventInstigator,
			AActor* _pDamageCauser) override;

public :
	UFUNCTION(BlueprintCallable, Category = "Base_Enemy")
	void AttackNotify();

	UFUNCTION()
	void OnDeath();

	void UpdateHealthBar();

	UFUNCTION(BlueprintPure, Category = "Base_Enemy")
	bool IsPlayingMontage();

	/* Get */
	UFUNCTION(BlueprintPure, Category = "Base_Enemy")
	const bool& GetIsDead() const { return m_bIsDead; }

	UFUNCTION(BlueprintPure, Category = "Base_Enemy")
	UAnimMontage* GetAttackAnimMontage() const { return m_pAttackAnimMontage; }

	/* Set */
	UFUNCTION(BlueprintCallable, Category = "Base_Enemy")
	void SetHasSeenPlayer(bool _bBool) { m_bHasSeenPlayer = _bBool; }

private :
	void _SetupWidget();
	void _OnActorNoticed(AActor* _pActor);

	UFUNCTION()
	void _OnSeePawn(APawn* _pPawn);

	
protected :
	UPROPERTY(VisibleDefaultsOnly, Category = "Base_Enemy|Components")
	UPawnSensingComponent* m_pSensing;

	UPROPERTY(VisibleDefaultsOnly, Category = "Base_Enemy|Components")
	UWidgetComponent* m_pWidget;

	UPROPERTY(EditAnywhere, Category = "Base_Enemy|AI")
	UBehaviorTree* m_pBehaviorTree;

	UPROPERTY(EditAnywhere, Category = "Base_Enemy|State")
	FText m_Name;

	UPROPERTY(EditAnywhere, Category = "Base_Enemy|State")
	int m_Level = 0;

	UPROPERTY(EditAnywhere, Category = "Base_Enemy|State")
	int m_MaxHealth = 100;

	UPROPERTY(EditAnywhere, Category = "Base_Enemy|State")
	int m_CurrHealth = 100;

	UPROPERTY(EditAnywhere, Category = "Base_Enemy|State")
	int m_ExpForKill = 80;

	UPROPERTY(VisibleInstanceOnly, Category = "Base_Enemy|State")
	bool m_bIsDead;

	UPROPERTY(VisibleInstanceOnly, Category = "Base_Enemy|State")
	bool m_bHasSeenPlayer;

	UPROPERTY(EditAnywhere, Category = "Base_Enemy|State")
	float m_PatrolRadius = 1000.f;

	UPROPERTY(EditAnywhere, Category = "Base_Enemy|State")
	float m_AttackRange = 50.f;

	UPROPERTY(EditAnywhere, Category = "Base_Enemy|State")
	float m_AttackDistance = 200.f;

	UPROPERTY(EditAnywhere, Category = "Base_Enemy|State")
	float m_AttackDamage = 20.f;

	UPROPERTY(EditAnywhere, Category = "Base_Enemy|State")
	float m_MovementRadius = 1500.f;

	UPROPERTY(EditAnywhere, Category = "Base_Enemy|State")
	bool m_bIsRunningBack = false;

	UPROPERTY(EditAnywhere, Category = "Base_Enemy|State")
	UAnimMontage* m_pAttackAnimMontage;
	
	UEnemy2Widget* m_pEnemyWidget;

	AActor* m_pKilledByActor;
};
