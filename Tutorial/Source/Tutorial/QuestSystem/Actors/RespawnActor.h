// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../QuestSystem.h"
#include "RespawnActor.generated.h"

class USphereComponent;

UCLASS()
class TUTORIAL_API ARespawnActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARespawnActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void RespawnEnemy(FEnemyRespawn _RespawnInfo);

	UFUNCTION()
	void RespawnTick();

protected :
	UPROPERTY(VisibleAnywhere, Category = "Default|Components")
	USphereComponent* m_pRespawnRadius;

	TArray<FEnemyRespawn> m_RespawnList;
	FTimerHandle m_hRespawnTimer;
	TArray<int> m_IndicesToSpawn;
	
};
