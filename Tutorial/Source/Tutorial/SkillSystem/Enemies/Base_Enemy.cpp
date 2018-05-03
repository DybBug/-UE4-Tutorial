// Fill out your copyright notice in the Description page of Project Settings.

#include "Base_Enemy.h"
#include "GameFrameWork/CharacterMovementComponent.h"
#include "AI/Navigation/NavigationSystem.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Animation/AnimMontage.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TimerManager.h"
#include "Engine.h"
#include "AIController.h"

#include "../Characters/SkillCharacter.h"



// Sets default values
ABase_Enemy::ABase_Enemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_pAIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	UAISenseConfig_Sight* pConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("AISenseConfig_Sight"));
	pConfig->SightRadius = 800.f;
	pConfig->LoseSightRadius = 2000.f;
	pConfig->DetectionByAffiliation.bDetectEnemies = true;
	pConfig->DetectionByAffiliation.bDetectFriendlies = true;
	pConfig->DetectionByAffiliation.bDetectNeutrals = true;
	m_pAIPerception->ConfigureSense(*pConfig);
	m_pAIPerception->SetDominantSense(UAISense_Sight::StaticClass());
	m_pAIPerception->OnPerceptionUpdated.AddDynamic(this, &ABase_Enemy::_OnPerceptionUpdated);	
}

// Called when the game starts or when spawned
void ABase_Enemy::BeginPlay()
{
	Super::BeginPlay();

	m_StartingLocation = GetActorLocation();
	GetCharacterMovement()->MaxWalkSpeed = m_PatrolWalkSpeed;

	_Patrol();
}

// Called every frame
void ABase_Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABase_Enemy::NotifyHits()
{
}

void ABase_Enemy::PerformAttack()
{
	if(!m_pTargetActor) return;

	if (!m_bIsDead)
	{
		FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), m_pTargetActor->GetActorLocation());
		SetActorRotation(LookAtRot);
		if(m_AttackAnimMontages.Num() != 0)
		{
			m_pCurrAttackAnimMontage = m_AttackAnimMontages[m_CurrAttackAnimMontageIndex];
			PlayAnimMontage(m_pCurrAttackAnimMontage);
			
			FTimerHandle hTimer;
			GetWorldTimerManager().SetTimer(hTimer, [&]{ 
				m_CurrAttackAnimMontageIndex = GetNextAnimationIndex(); 
				if (GetInAttackRange())
				{
					PerformAttack();
				}
				else
				{
					AAIController* pAICtrl = Cast<AAIController>(GetController());
					//pAICtrl->MoveToActor(m_pTargetActor);
					UNavigationSystem::SimpleMoveToActor(GetController(), m_pTargetActor);
					PerformAttack();					
				}
			}, m_pCurrAttackAnimMontage->SequenceLength, false);
		}
	}
}

void ABase_Enemy::OnAggroedPulled(AActor * _pTarget)
{
	if (!m_bWasAggroed && !m_bIsRunningBack)
	{
		m_bIsPatrolling = false;
		m_bWasAggroed   = true;
		m_pTargetActor  = _pTarget;
		GetCharacterMovement()->MaxWalkSpeed = m_AggroedWalkSpeed;

		GetWorldTimerManager().SetTimer(m_hFollowTargetDistanceTimer, this, &ABase_Enemy::_FollowTargetDistance, 1.f, true);

		if (GetInAttackRange())
		{
			PerformAttack();
		}
		else
		{
			AAIController* pAICtrl = Cast<AAIController>(GetController());
			//pAICtrl->MoveToActor(m_pTargetActor);
			UNavigationSystem::SimpleMoveToActor(GetController(), m_pTargetActor);		
			PerformAttack();		
		}
	}
}

void ABase_Enemy::OnReset()
{
	m_bIsRunningBack = true;

	m_CurrAttackAnimMontageIndex = 0;
	m_pTargetActor = nullptr;
	m_bWasAggroed = false;
	m_bIsPatrolling = true;

//	GetWorldTimerManager().ClearTimer(m_hFollowTargetDistanceTimer);
	UKismetSystemLibrary::K2_ClearAndInvalidateTimerHandle(this, m_hFollowTargetDistanceTimer);

	AAIController* pAICtrl = Cast<AAIController>(GetController());
	//pAICtrl->MoveToLocation(m_StartingLocation);
	UNavigationSystem::SimpleMoveToLocation(pAICtrl, m_StartingLocation);
	GetCharacterMovement()->MaxWalkSpeed = m_PatrolWalkSpeed;

	_Patrol();

	m_bIsRunningBack = false;
}

int ABase_Enemy::GetNextAnimationIndex() 
{
	return (m_CurrAttackAnimMontageIndex + 1) % m_AttackAnimMontages.Num();
}

bool ABase_Enemy::GetInAttackRange() 
{
	bool Result = false;
	if (m_pTargetActor)
	{
		Result = (m_pTargetActor->GetDistanceTo(this) < m_AttackRange) ? true : false;
	}
	return Result;
}

void ABase_Enemy::SetIsDead(bool& _bIsDead)
{
	m_bIsDead = _bIsDead;
}

void ABase_Enemy::SetStartingLocation(FVector & _Location)
{
	m_StartingLocation = _Location;
}

void ABase_Enemy::SetPatrolWalkSpeed(float & _Speed)
{
	m_PatrolWalkSpeed = _Speed;
}

void ABase_Enemy::SetPatrolRadius(float _Radius)
{
	m_PatrolRadius = _Radius;
}

void ABase_Enemy::SetIsPatrolling(bool _bBool)
{
	m_bIsPatrolling = _bBool;
}

void ABase_Enemy::_Patrol()
{
	FVector Point;
	UNavigationSystem::K2_GetRandomReachablePointInRadius(this, m_StartingLocation, Point, m_PatrolRadius);

	AController* pAICtrl = GetController();
	//UNavigationSystem::SimpleMoveToLocation(pAICtrl, Point);
	Cast<AAIController>(pAICtrl)->MoveToLocation(Point);
	
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, FString::Printf(TEXT("%.2f, %.2f, %.2f"), Point.X, Point.Y, Point.Z));

	if (m_bIsPatrolling)
	{
		float Range = UKismetMathLibrary::RandomFloatInRange(1.f, 4.f);

		FTimerHandle hTimer;
		GetWorldTimerManager().SetTimer(hTimer, this, &ABase_Enemy::_DelayFunc, Range, false);
	}
}

void ABase_Enemy::_OnPerceptionUpdated(const TArray<AActor*>& _UpdatedActors)
{
	if (!m_bWasAggroed && m_bAggressive)
	{
		for (int i = 0; i < _UpdatedActors.Num(); ++i)
		{
			OnAggroedPulled(_UpdatedActors[i]);			
		}
	}
}

void ABase_Enemy::_DelayFunc()
{
	if (m_bIsPatrolling)
	{
		_Patrol();
	}
}

void ABase_Enemy::_FollowTargetDistance()
{
	// m_StartingLocation - GetActorLocation()의 거리(길이).
	float Distance = FVector::Distance(GetActorLocation(), m_StartingLocation);
	//float Distance = FVector::Distance(m_StartingLocation, GetActorLocation());

	if (Distance > m_FollowPlayerRadius)
	{
		OnReset();
	}
}

