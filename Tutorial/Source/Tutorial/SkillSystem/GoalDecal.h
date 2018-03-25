// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SkillPlayerController.h"
#include "GoalDecal.generated.h"

UCLASS()
class TUTORIAL_API AGoalDecal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGoalDecal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere)
	class USceneComponent* m_pScene = nullptr;

	UPROPERTY(VisibleAnywhere)
	class UDecalComponent* m_pDecal = nullptr;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* m_pBox = nullptr;

	UPROPERTY()
	class ASkillPlayerController* m_pController = nullptr;

protected:
	void _OnComponentBeginOverlap(
		UPrimitiveComponent* _pOverlappedComponent,
		AActor* _pOtherActor,
		UPrimitiveComponent* _pOtherComp,
		int _OtherBodyIndex,
		bool _bFromSweep,
		const FHitResult& _SweepResult);
	
};
