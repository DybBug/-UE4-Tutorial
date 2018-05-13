// Fill out your copyright notice in the Description page of Project Settings.

#include "Missile_Skill.h"
#include "../Enemies/Base_Enemy.h"
#include "../Characters/SkillCharacter.h"

#include <Components/SphereComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>
#include <Particles/ParticleSystemComponent.h>
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetMathLibrary.h>
#include <AI/Navigation/NavigationSystem.h>
#include <Engine/world.h>

AMissile_Skill::AMissile_Skill()
{
	PrimaryActorTick.bCanEverTick = false;

	m_pCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	m_pCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_pCollision->SetCollisionObjectType(UEngineTypes::ConvertToCollisionChannel(EOT_Skill));
	m_pCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	m_pCollision->SetCollisionResponseToChannel(UEngineTypes::ConvertToCollisionChannel(EOT_Enemy), ECollisionResponse::ECR_Block);
	//m_pCollision->SetNotifyRigidBodyCollision(true); // Simulation Generates Hit Event = true;
	m_pCollision->OnComponentHit.AddDynamic(this, &AMissile_Skill::_OnComponentHit);
	RootComponent = m_pCollision;

	m_pParticleSys = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComp"));
	m_pParticleSys->SetupAttachment(RootComponent);

	m_pMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
	m_pMovement->InitialSpeed = 0.f;
	m_pMovement->MaxSpeed = 0.f;
	m_pMovement->bRotationFollowsVelocity = true;
	m_pMovement->ProjectileGravityScale = 0.f;
	m_pMovement->Bounciness = 0.f;
	m_pMovement->Friction = 0.3f;
	m_pMovement->Velocity = FVector(0.f, 0.f, 0.f);
	m_pMovement->bIsHomingProjectile = true;
	m_pMovement->HomingAccelerationMagnitude = 100000.f;
}

void AMissile_Skill::BeginPlay()
{
	Super::BeginPlay();	
}

void AMissile_Skill::OnSkillNotify()
{	

	if (!m_pPlayer->GetSelectedEnemy() || m_pPlayer->GetSelectedEnemy()->GetIsDead())
	{
		return;
	}
	AMissile_Skill* pMissile = GetWorld()->SpawnActor<AMissile_Skill>(m_ActorClass, m_pPlayer->GetActorLocation(),FRotator(0.f, 0.f, 0.f));
	pMissile->_OnFire(m_pPlayer->GetSelectedEnemy());
}

void AMissile_Skill::OnTryCastSpell()
{	
	if (!m_bOnCooldown && !m_bCurrentlyCasted && m_pPlayer->GetSelectedEnemy())
	{
		if (!m_pPlayer->GetIsCasting() )
		{
			InitializeSpellCast();
		}
	}
}

void AMissile_Skill::InitializeSpellCast()
{
	if (_InAttackRange())
	{
		_ManaCheck();
	}
	else
	{
		m_hDistanceTimer = UKismetSystemLibrary::K2_SetTimer(this, "_DistanceCheck", 0.2f, true);
	}

}

void AMissile_Skill::OnSpellCast()
{	
	FRotator Rot = UKismetMathLibrary::FindLookAtRotation(m_pPlayer->GetActorLocation(), m_pPlayer->GetSelectedEnemy()->GetActorLocation());
	m_pPlayer->SetActorRotation(Rot);

	ABase_Skill::OnSpellCast();
}

void AMissile_Skill::StopTimer()
{
	UKismetSystemLibrary::K2_ClearAndInvalidateTimerHandle(GetWorld(), m_hDistanceTimer);
}


void AMissile_Skill::_OnComponentHit(
	UPrimitiveComponent* _pHitComponent, 
	AActor* _pOtherActor,
	UPrimitiveComponent* _pOtherComp,
	FVector _NormalImpulse,
	const FHitResult& out_Hit)
{
	if (Cast<ABase_Enemy>(_pOtherActor))
	{
		if (m_pTarget == _pOtherActor)
		{
			UParticleSystemComponent* pParticle = UGameplayStatics::SpawnEmitterAtLocation(this, m_pImpectEffect, GetActorLocation());
			m_pTarget->OnReceiveDamage(GetCurrStage().Damage, GetCurrStage().DamageType, m_SkillInfo.ElementClass, GetCurrStage().CriticalChance, m_pPlayer, this);
			Destroy();
		}
		else
		{
			Destroy();
		}
	}	
}

void AMissile_Skill::_OnFire(ABase_Enemy* _pTarget)
{
	m_pTarget = _pTarget;
	m_pCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	m_pMovement->MaxSpeed = GetCurrStage().MissileSpeed;
	m_pMovement->InitialSpeed = GetCurrStage().MissileSpeed;
	m_pMovement->HomingTargetComponent = m_pTarget->GetRootComponent();
	m_pParticleSys->SetTemplate(m_pMissileEffect);
}


void AMissile_Skill::_ManaCheck()
{
	if (m_pPlayer->GetStat(EStats::Mana).CurrValue >= GetCurrStage().ManaCost)
	{
		m_bCurrentlyCasted = true;
		m_pPlayer->BeginSpellCast(this);
		m_pPlayer->ModifyStat(EStats::Mana, -GetCurrStage().ManaCost, true);
		OnSpellCast();
	}
}

bool AMissile_Skill::_InAttackRange()
{
	float Distance = m_pPlayer->GetDistanceTo(m_pPlayer->GetSelectedEnemy());

	return (Distance <= GetCurrStage().Range);
}


void AMissile_Skill::_DistanceCheck()
{
	if (m_pPlayer->GetSelectedEnemy())
	{
		if (_InAttackRange())
		{
			StopTimer();
			_ManaCheck();
		}
		else
		{
			UNavigationSystem::SimpleMoveToActor(m_pPlayer->GetController(), m_pPlayer->GetSelectedEnemy());
		}
	}
	else
	{
		StopTimer();
	}
}
