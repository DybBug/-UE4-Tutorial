// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <UObject/NoExportTypes.h>
#include <Engine/EngineTypes.h>
#include <Slate/WidgetTransform.h>
#include "SkillSystem.generated.h"

#define EOT_Player   EObjectTypeQuery::ObjectTypeQuery1
#define EOT_Enemy    EObjectTypeQuery::ObjectTypeQuery2
#define EOT_Skill    EObjectTypeQuery::ObjectTypeQuery3
#define ETT_Selector ETraceTypeQuery::TraceTypeQuery1
#define ETT_Misc     ETraceTypeQuery::TraceTypeQuery2


/* 스텟 게이지*/
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatData")
	int MinValue = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatData")
	int MaxValue = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatData")
	int CurrValue = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatData")
	float DisplayedValue = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatData")
	float MinLerpTime = 0.2f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatData")
	float MaxLerpTime = 1.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatData")
	class UStatBarWidget* pBarWidget = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatData")
	bool bCurrentlyAnimated = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatData")
	bool bHasRegeneration = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatData")
	float TimeToRegMaxValue = 45.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatData")
	float RegInterval = 0.4f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatData")
	FTimerHandle hRegenerationTimer;
};

USTRUCT(BlueprintType)
struct FStatLerp
{
	GENERATED_BODY()
public :
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatLerp")
	float OriginalValue = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatLerp")
	float ValueToLerpTo = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatLerp")
	bool bPositive = false; // true : 증가, false : 감소.
};


/* 스킬 시스템 */
UENUM(BlueprintType)
enum class ETargetTypes : uint8
{
	Self             UMETA(DisplayName = "SELF"),
	Selected_Enemy   UMETA(DisplayName = "SELECTED_ENEMY"),
	Selected_Area    UMETA(DisplayName = "SELECTED_AREA"),
	Area_around_self UMETA(DisplayName = "AREA_AROUND_SELF"),
	Missile          UMETA(DisplayName = "MISSILE")
};

UENUM(BlueprintType)
enum class EDamageTypes : uint8
{
	Magical  UMETA(DisplayName = "MAGICAL"),
	Physical UMETA(DisplayName = "PHYSICAL"),
	Absolute UMETA(DisplayName = "ABSOLUTE")
};

UENUM(BlueprintType)
enum class EEffectiveness : uint8
{
	Effective       UMETA(DisplayName = "EFFECTIVE"),
	Super_Effective UMETA(DisplayName = "SUPER_EFFECTIVE"),
	Not_Effective   UMETA(DisplayName = "NOT_EFFECTIVE")
};

USTRUCT(BlueprintType)
struct FElementInfo
{
	GENERATED_BODY()

public :
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ElementInfo")
	FName Name = "#Unknown";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ElementInfo")
	class UTexture2D* pIcon = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ElementInfo")
	FLinearColor Color = FLinearColor(1.f, 1.f, 1.f, 1.f);	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ElementInfo")
	TArray<TSubclassOf<class ABase_Element>> WeaknessClasses;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ElementInfo")
	TArray<TSubclassOf<class ABase_Element>> ResistanceClasses;
};

USTRUCT(BlueprintType)
struct FSkillStage
{
	GENERATED_BODY()

public :
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillStage")
	int RequiredLevel = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillStage")
	TArray<TSubclassOf<class ABase_Skill>> RequiredSkills;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillStage")
	class UTexture2D* pOverrideIcon = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillStage")
	float Range = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillStage")
	float Damage = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillStage")
	EDamageTypes DamageType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillStage")
	int CriticalChance = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillStage")
	float Cooldown = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillStage")
	float CastingTime = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillStage")
	float AreaRadius = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillStage")
	float MissileSpeed = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillStage")
	int ManaCost = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillStage")
	float DmgPerTick = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillStage")
	float TickInterval = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillStage")
	float Duration = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillStage")
	int Accuracy = 0;
};

USTRUCT(BlueprintType)
struct FSkillInfo
{
	GENERATED_BODY()

public :
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillInfo")
	FName Name = "#Unknown";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillInfo")
	class UTexture2D* pIcon = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillInfo")
	FText Description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillInfo")
	TSubclassOf<class ABase_Element> ElementClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillInfo")
	ETargetTypes Target;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillInfo")
	TArray<FSkillStage> Stages;
};

/* 스킬트리 */
USTRUCT(BlueprintType)
struct FSubTreeConnection
{
	GENERATED_BODY()

public :
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SubTreeConnection")
	FVector2D Position;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SubTreeConnection")
	FVector2D Size;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SubTreeConnection")
	FWidgetTransform Transform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SubTreeConnection")
	TSubclassOf<class ABase_Skill> ForSkillClass;
};

USTRUCT(BlueprintType)
struct FSubTreeSkill
{
	GENERATED_BODY()

public :
	UPROPERTY(EditAnywhere, Category = "SubTreeSkill")
	FVector2D Position;

	UPROPERTY(EditAnywhere, Category = "SubTreeSkill")
	TSubclassOf<class ABase_Skill> Spell;
};

USTRUCT(BlueprintType)
struct FSubTreeContent
{
	GENERATED_BODY()

public :
	UPROPERTY(EditAnywhere, Category = "SubTreeContent")
	TArray<FSubTreeSkill> Skills;

	UPROPERTY(EditAnywhere, Category = "SubTreeContent")
	TArray<FSubTreeConnection> Connections;
};

USTRUCT(BlueprintType)
struct FTreeCategory
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category="TreeCategory")
	FName Name;

	UPROPERTY(EditAnywhere, Category = "TreeCategory")
	FSubTreeContent Content;
};