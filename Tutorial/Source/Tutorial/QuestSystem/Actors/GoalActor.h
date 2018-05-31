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

public :
	void Initialize(bool _bUseRadius, float _Radius, const FLinearColor& _CircleColor);


protected :
	UPROPERTY(VisibleDefaultsOnly, Category = "GoalActor|Components")
	USceneComponent* m_pScene;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "GoalActor|Components")
	UPaperSpriteComponent* m_pMiniMapIcon;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "GoalActor|Components")
	UPaperSpriteComponent* m_pMiniMapRadius;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GoalActor")
	bool m_bUseRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GoalActor")
	float m_Radius = 500;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GoalActor")
	FLinearColor m_CircleColor = FLinearColor::White;
};
