// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GoalActor.generated.h"

class UPaperSpriteComponent;

UCLASS()
class TUTORIAL_API AGoalActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGoalActor();

protected :
	virtual void BeginPlay() override;


protected :
	UPROPERTY(VisibleDefaultsOnly, Category = "GoalActor|Components")
	USceneComponent* m_pScene;

	UPROPERTY(VisibleDefaultsOnly, Category = "GoalActor|Components")
	UPaperSpriteComponent* m_pMiniMapIcon;
	
	
};
