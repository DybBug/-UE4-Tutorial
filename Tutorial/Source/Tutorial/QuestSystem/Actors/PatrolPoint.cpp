// Fill out your copyright notice in the Description page of Project Settings.

#include "PatrolPoint.h"

#include <Components/SceneComponent.h>
#include <Components/BillboardComponent.h>

// Sets default values
APatrolPoint::APatrolPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	m_pScene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = m_pScene;

	m_pBillboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
	m_pBillboard->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void APatrolPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APatrolPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

