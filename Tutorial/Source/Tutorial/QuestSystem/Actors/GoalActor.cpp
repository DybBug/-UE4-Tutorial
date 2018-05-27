// Fill out your copyright notice in the Description page of Project Settings.

#include "GoalActor.h"

#include <PaperSpriteComponent.h>
#include <Kismet/GameplayStatics.h>
#include <GameFramework/Character.h>
#include <Engine/World.h>


// Sets default values
AGoalActor::AGoalActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	m_pScene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = m_pScene;

	m_pMiniMapIcon = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("MiniMapIcon"));
	m_pMiniMapIcon->SetRelativeRotation(FRotator(0.f, 0.f, -90.f));
	m_pMiniMapIcon->SetRelativeLocation(FVector(0.f, 0.f, 110.f));
	m_pMiniMapIcon->SetRelativeScale3D(FVector(2.5f, 2.5f, 2.5f));
	m_pMiniMapIcon->SetupAttachment(RootComponent);

}

void AGoalActor::BeginPlay()
{
	Super::BeginPlay();

	if (GetWorld())
	{
		SetOwner(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		
		// 소유 플레이어는 메시를 볼 수 없습니다.
		m_pMiniMapIcon->SetOwnerNoSee(true);
	}
}

