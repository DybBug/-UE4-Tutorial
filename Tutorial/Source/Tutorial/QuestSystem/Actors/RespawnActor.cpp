// Fill out your copyright notice in the Description page of Project Settings.

#include "RespawnActor.h"
#include "../Enemies/Base2_Enemy.h"

#include <Components/SphereComponent.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/KismetMathLibrary.h>
#include <AI/Navigation/NavigationSystem.h>

// Sets default values
ARespawnActor::ARespawnActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_pRespawnRadius = CreateDefaultSubobject<USphereComponent>(TEXT("RespawnRadius"));
	RootComponent = m_pRespawnRadius;
}

// Called when the game starts or when spawned
void ARespawnActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARespawnActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARespawnActor::RespawnEnemy(FEnemyRespawn _RespawnInfo)
{
	m_RespawnList.Add(_RespawnInfo);

	if (m_hRespawnTimer.IsValid())
	{

	}
	else
	{
		GetWorldTimerManager().SetTimer(m_hRespawnTimer, this, &ARespawnActor::RespawnTick, 0.5f, true);
	}
}

void ARespawnActor::RespawnTick()
{
	for (int i = 0 ; i < m_RespawnList.Num(); ++i)
	{
		if ((m_RespawnList[i].Time - 0.5f) > 0.f)
		{
			m_RespawnList[i].Time -= 0.5f;
		}
		else
		{
			m_IndicesToSpawn.Add(i);
		}
	}

	for (int i = 0; i < m_IndicesToSpawn.Num(); ++i)
	{
		FVector Loc = UNavigationSystem::GetRandomReachablePointInRadius(GetWorld(), GetActorLocation(), m_pRespawnRadius->GetUnscaledSphereRadius());
		FTransform Trans;
		Trans.SetLocation(Loc);

		FActorSpawnParameters Param;
		Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		
		AActor* pSpawnedActor = GetWorld()->SpawnActor(m_RespawnList[m_IndicesToSpawn[i]].EnemyClass, &Trans, Param);
		Cast<ABase2_Enemy>(pSpawnedActor)->SetRespawnActor(this);

		m_RespawnList.RemoveAt(m_IndicesToSpawn[i]);

		for (int index = m_IndicesToSpawn[i]; index < m_IndicesToSpawn.Num(); ++index)
		{			
			m_IndicesToSpawn[index] = m_IndicesToSpawn[index] - 1;
		}
	}

	m_IndicesToSpawn.Empty();

	if (m_RespawnList.Num() > 0)
	{

	}
	else
	{
		UKismetSystemLibrary::K2_ClearAndInvalidateTimerHandle(GetWorld(), m_hRespawnTimer);
	}
}

