// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../SkillSystem.h"
#include "Base_Element.generated.h"

UCLASS()
class TUTORIAL_API ABase_Element : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABase_Element();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	/* Get */
	const FElementInfo& GetElementInfo() const {return m_ElementInfo;}


protected :
	UPROPERTY(EditDefaultsOnly, Category="Base_Element")
	FElementInfo m_ElementInfo;
	
};
