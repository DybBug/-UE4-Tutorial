// Fill out your copyright notice in the Description page of Project Settings.

#include "Base_Enemy.h"
#include "GameFrameWork/CharacterMovementComponent.h"
#include "AI/Navigation/NavigationSystem.h"
#include "AIController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TimerManager.h"
#include "Engine.h"



// Sets default values
ABase_Enemy::ABase_Enemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//AIControllerClass  = AAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

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

void ABase_Enemy::SetIsDead(bool & _bIsDead)
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

	//AAIController* pController = Cast<AAIController>(GetController());
	//Cast<AAIController>(GetController())->MoveToLocation(Point);
	//AController* pController = GetController();
	//UNavigationSystem::SimpleMoveToLocation(pController, Point);
	
	
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, FString::Printf(TEXT("%.2f, %.2f, %.2f"), Point.X, Point.Y, Point.Z));

	if (m_bIsPatrolling)
	{
		float Range = UKismetMathLibrary::RandomFloatInRange(1.f, 4.f);

		FTimerHandle hTimer;
		GetWorldTimerManager().SetTimer(hTimer, this, &ABase_Enemy::_DelayFunc, Range, false);
	}
}

void ABase_Enemy::_DelayFunc()
{
	if (m_bIsPatrolling)
	{
		_Patrol();
	}
}
