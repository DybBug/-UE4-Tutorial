// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/EngineTypes.h"
#include "SkillSystem.generated.h"

UENUM(BlueprintType)
enum class EStats : uint8
{
	Health UMETA(DisplayName = "HEALTH"),
	Mana   UMETA(DisplayName = "MANA"),
	Exp    UMETA(DisplayName = "EXP")
};

USTRUCT(BlueprintType)
struct FStatData
{
	GENERATED_BODY()
		
public :
	UPROPERTY(BlueprintReadWrite, Category = "StatData")
	int MinValue = 0;
	int MaxValue = 0;
	int CurrValue = 0;
	float DisplayedValue = 0.f;
	float MinLerpTime = 0.2f;
	float MaxLerpTime = 1.5f;
	class UUserWidget* pBarWidget = nullptr;
	bool bCurrentlyAnimated = false;
	bool bHasRegeneration = false;
	float TimeToRegMaxValue = 45.f;
	float RegIntervall = 0.4f;
	FTimerHandle hRegenerationTimer;
};

USTRUCT(BlueprintType)
struct FStatLerp
{
	GENERATED_BODY()
public :
	float OriginalValue = 0.f;
	float ValueToLerpTo = 0.f;
	bool bPositive = false; // true : 증가, false : 감소.
};