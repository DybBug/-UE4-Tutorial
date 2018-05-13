// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillSystem/SkillActors/Base_Skill.h"
#include "Missile_Skill.generated.h"


/**
 * 
 */
UCLASS()
class TUTORIAL_API AMissile_Skill : public ABase_Skill
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMissile_Skill();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public :
	UFUNCTION()
	virtual void OnSkillNotify() override;
	UFUNCTION()
	virtual void OnTryCastSpell() override;
	UFUNCTION()
	virtual void InitializeSpellCast() override;
	UFUNCTION()
	virtual void OnSpellCast() override;
	
	UFUNCTION()
	void StopTimer();

	/* Get */
	class ABase_Enemy* GetTarget() const { return m_pTarget; }

private :
	UFUNCTION()
	void _OnComponentHit(
		UPrimitiveComponent* _pHitComponent, 
		AActor* _pOtherActor, 
		UPrimitiveComponent* _pOtherComp, 
		FVector _NormalImpulse, 
		const FHitResult& out_Hit);

	UFUNCTION()
	void _OnFire(ABase_Enemy* _pTarget);
	
	UFUNCTION()
	void _ManaCheck();

	UFUNCTION()
	bool _InAttackRange();

	UFUNCTION()
	void _DistanceCheck();
protected :
	UPROPERTY(VisibleAnywhere, Category = "Missile_Skill")
	class USphereComponent* m_pCollision;

	UPROPERTY(VisibleAnywhere, Category = "Missile_Skill")
	class UParticleSystemComponent* m_pParticleSys;

	UPROPERTY(VisibleAnywhere, Category = "Missile_Skill")
	class UProjectileMovementComponent* m_pMovement;

	UPROPERTY(VisibleAnywhere, Category = "Missile_Skill")
	class ABase_Enemy* m_pTarget;

	UPROPERTY(EditAnywhere, Category = "Missile_Skill")
	class UParticleSystem* m_pMissileEffect;

	UPROPERTY(EditAnywhere, Category = "Missile_Skill")
	class UParticleSystem* m_pImpectEffect;

	UPROPERTY(EditAnywhere, Category = "Missile_Skill")
	TSubclassOf<class AMissile_Skill> m_ActorClass;

	UPROPERTY()
	FTimerHandle m_hDistanceTimer;

};
