// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Base_Enemy.generated.h"

UCLASS()
class TUTORIAL_API ABase_Enemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABase_Enemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	 
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Base_Enemy")
	void NotifyHits();


	/* Get */
	UFUNCTION(BlueprintPure, Category = "Base_Enemy")
	const bool& GetIsDead() const { return m_bIsDead; }

	UFUNCTION(BlueprintPure, Category = "Base_Enemy")
	const FVector& GetStartingLocation() const { return m_StartingLocation; }

	UFUNCTION(BlueprintPure, Category = "Base_Enemy")
	const float& GetPatrolWalkSpeed() const { return m_PatrolWalkSpeed; }

	UFUNCTION(BlueprintPure, Category = "Base_Enemy")
	const float& GetPatrolRadius() const {return m_PatrolRadius;}

	UFUNCTION(BlueprintPure, Category = "Base_Enemy")
	const bool& GetIsPatrolling() const {return m_bIsPatrolling;}

	/* Set */
	UFUNCTION(BlueprintCallable, Category = "Base_Enemy")
	void SetIsDead(bool& _bIsDead);

	UFUNCTION(BlueprintCallable, Category = "Base_Enemy")
	void SetStartingLocation(FVector& _Location);

	UFUNCTION(BlueprintCallable, Category = "Base_Enemy")
	void SetPatrolWalkSpeed(float& _Speed);

	UFUNCTION(BlueprintCallable, Category = "Base_Enemy")
	void SetPatrolRadius(float _Radius);

	UFUNCTION(BlueprintCallable, Category ="Base_Enemy")
	void SetIsPatrolling(bool _bBool);

private :
	void _Patrol();

	void _DelayFunc();

protected :
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Base_Enemy")
	bool m_bIsDead = false;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Base_Enemy")
	FVector m_StartingLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base_Enemy")
	float m_PatrolWalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base_Enemy")
	float m_PatrolRadius = 800.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base_Enemy")
	bool m_bIsPatrolling = true;
};
