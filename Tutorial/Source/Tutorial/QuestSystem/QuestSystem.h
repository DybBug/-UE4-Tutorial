// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EngineMinimal.h"
#include "UObject/NoExportTypes.h"
#include "QuestSystem.generated.h"

#define CONVERT_TO_STRING(eType, eValue) ( (FindObject<UEnum>(ANY_PACKAGE, eType, true) != nullptr) ? (FindObject<UEnum>(ANY_PACKAGE, eType, true)->GetNameStringByIndex((uint8)eValue)) : FString("Convert Failed!") );

UENUM(BlueprintType)
enum class EQuestCategories : uint8
{
	Main_Quest UMETA(DisplayName = "MAIN_QUEST"),
	Side_Quest UMETA(DisplayName = "SIDE_QUEST"),
	Events     UMETA(DisplayName = "EVENTS"),
	Max        UMETA(DisplayName = "MAX")
};

UENUM(BlueprintType)
enum class EGoalTypes : uint8
{
	Custom UMETA(DisplayName = "CUSTOM"),
	Hunt   UMETA(DisplayName = "HUNT"),
	Find   UMETA(DisplayName = "FIND"),
	Talk   UMETA(DisplayName = "TALK"),
	Max        UMETA(DisplayName = "MAX")
};

UENUM(BlueprintType)
enum class ERegions : uint8
{
	Kanto  UMETA(DisplayName = "KANTO"),
	Johto  UMETA(DisplayName = "JOHTO"),
	Sinnoh UMETA(DisplayName = "SINNOH"),
	Max    UMETA(DisplayName = "MAX")
};

UENUM(BlueprintType)
enum class EQuestStates : uint8
{
	Current_Quests   UMETA(DisplayName = "CURRENT_QUEST"),
	Completed_Quests UMETA(DisplayName = "COMPLETED_QUEST"),
	Failed_Quests    UMETA(DisplayName = "FAILED_QUEST"),
	Max        UMETA(DisplayName = "MAX")
};

UENUM(BlueprintType)
enum class EGoalState : uint8
{
	Current UMETA(DisplayName = "CURRENT"),
	Success UMETA(DisplayName = "SUCCESS"),
	Failed  UMETA(DisplayName = "FAILED"),
	Max        UMETA(DisplayName = "MAX")
};

USTRUCT(BlueprintType)
struct FQuestReward
{
	GENERATED_BODY()

public :
	UPROPERTY(EditAnywhere, Category = "FQuestReword")
	int Experience = 0;

	UPROPERTY(EditAnywhere, Category = "FQuestReword")
	int PrestigePoints = 0;
};

USTRUCT(BlueprintType)
struct FGoalLocation
{
	GENERATED_BODY()

public :
	UPROPERTY(EditAnywhere, Category = "FGoalLocation")
	bool bHasLocation = false;

	UPROPERTY(EditAnywhere, Category = "FGoalLocation")
	FVector Location;
};

USTRUCT(BlueprintType)
struct FGoalInfo
{
	GENERATED_BODY()

	bool operator ==(const FGoalInfo& rhs) const
	{
		return this->AdditionalName.EqualTo(rhs.AdditionalName);		
	}

public :
	UPROPERTY(EditAnywhere, Category = "FGoalInfo")
	int GoalId;

	UPROPERTY(EditAnywhere, Category = "FGoalInfo")
	EGoalTypes Type = EGoalTypes::Custom;

	UPROPERTY(EditAnywhere, Category = "FGoalInfo")
	bool bCustomGoal = true;

	UPROPERTY(EditAnywhere, Category = "FGoalInfo")
	FText GoalText;

	UPROPERTY(EditAnywhere, Category = "FGoalInfo")
	FText AdditionalName;

	UPROPERTY(EditAnywhere, Category = "FGoalInfo")
	int AmountToHunt;

	UPROPERTY(EditAnywhere, Category = "FGoalInfo")
	FGoalLocation GoalLocation;

	UPROPERTY(EditAnywhere, Category = "FGoalInfo")
	bool bUpdateQuestDescription = false;

	UPROPERTY(EditAnywhere, Category = "FGoalInfo")
	FText UpdateDescription;

	UPROPERTY(EditAnywhere, Category = "FGoalInfo")
	TArray<int> FollowingSubGoalIndices;

	UPROPERTY(EditAnywhere, Category = "FGoalInfo")
	bool bUseRadius = false;

	UPROPERTY(EditAnywhere, Category = "FGoalInfo")
	float Radius = 500.f;

	UPROPERTY(EditAnywhere, Category = "FGoalInfo")
	FLinearColor CircleColor = FLinearColor::White;

	UPROPERTY(EditAnywhere, Category = "FGoalInfo")
	TSubclassOf<AActor> GoalClass;

	UPROPERTY(EditAnywhere, Category = "FGoalInfo")
	bool bFailMeansQuestFail = false;

	UPROPERTY(EditAnywhere, Category = "FGoalInfo")
	bool bCompletesQuest = false;
};

USTRUCT(BlueprintType)
struct FQuestInfo
{
	GENERATED_BODY()

public :
	UPROPERTY(EditAnywhere, Category = "FQuestInfo")
	FText Name;

	UPROPERTY(EditAnywhere, Category = "FQuestInfo")
	EQuestCategories Category = EQuestCategories::Main_Quest;

	UPROPERTY(EditAnywhere, Category = "FQuestInfo")
	FText Description;

	UPROPERTY(EditAnywhere, Category = "FQuestInfo")
	ERegions Region = ERegions::Kanto;

	UPROPERTY(EditAnywhere, Category = "FQuestInfo")
	FQuestReward CompletionReward;

	UPROPERTY(EditAnywhere, Category = "FQuestInfo")
	int SuggestedLevel;

	UPROPERTY(EditAnywhere, Category = "FQuestInfo")
	float Difficulty;

	UPROPERTY(EditAnywhere, Category = "FQuestInfo")
	TArray<FGoalInfo> SubGoals;

	UPROPERTY(EditAnywhere, Category = "FQuestInfo")
	bool bHasClient = true;

	UPROPERTY(EditAnywhere, Category = "FQuestInfo")
	TSubclassOf<class ABase_Npc> ClientClass;

	UPROPERTY(EditAnywhere, Category = "FQuestInfo")
	int ClientId;
};

USTRUCT(BlueprintType)
struct FCompletedGoal
{
	GENERATED_BODY()

public :
	FCompletedGoal() {};
	FCompletedGoal(const int& _Index, const FGoalInfo& _Info, const bool& _Successful)
		: GoalIndex(_Index), 
		  GoalInfo(_Info), 
		  bSuccessful(_Successful)
	{
		return;
	}

public :
	UPROPERTY(EditAnywhere, Category = "FCompletedGoal")
	int GoalIndex;

	UPROPERTY(EditAnywhere, Category = "FCompletedGoal")
	FGoalInfo GoalInfo;

	UPROPERTY(EditAnywhere, Category = "FCompletedGoal")
	bool bSuccessful;
};

USTRUCT(BlueprintType)
struct FEnemyRespawn
{
	GENERATED_BODY()

public :
	FEnemyRespawn() {};
	FEnemyRespawn(float _Time, TSubclassOf<class ABase2_Enemy> _EnemyClass)
		: Time(_Time),
		  EnemyClass(_EnemyClass)
	{
		return;
	}

public :
	UPROPERTY(EditAnywhere, Category = "FEnemyRespawn")
	float Time;

	UPROPERTY(EditAnywhere, Category = "FEnemyRespawn")
	TSubclassOf<class ABase2_Enemy> EnemyClass;
};

USTRUCT(BlueprintType)
struct FRegionPrestige
{
	GENERATED_BODY()

public :
	FRegionPrestige() {};
	FRegionPrestige(ERegions _Region, int _Prestige)
		: Region(_Region),
		Prestige(_Prestige)
	{
		return;
	}
	 
public :
	UPROPERTY(EditAnywhere, Category = "FRegionPrestige")
	ERegions Region;

	UPROPERTY(EditAnywhere, Category = "FRegionPrestige")
	int Prestige;
};

USTRUCT(BlueprintType)
struct FIndexToComplete
{
	GENERATED_BODY()
public :
	FIndexToComplete() {};
	FIndexToComplete(class AQuest_Base* _pQuest, int _Index)
		: pQuest(_pQuest),
		  Index(_Index)
	{
		return;
	}

public :
	UPROPERTY(EditAnywhere, Category = "FIndexToComplete")
	class AQuest_Base* pQuest;

	UPROPERTY(EditAnywhere, Category = "FIndexToComplete")
	int Index;
};

USTRUCT(BlueprintType)
struct FSavedQuest
{
	GENERATED_BODY()
public :
	UPROPERTY(EditAnywhere, Category = "FSavedQuest")
	TSubclassOf<AQuest_Base> QuestClass;

	UPROPERTY(EditAnywhere, Category = "FSavedQuest")
	bool bCurrentlyActive;

	UPROPERTY(EditAnywhere, Category = "FSavedQuest")
	TArray<int> CurrGoalIndices;

	UPROPERTY(EditAnywhere, Category = "FSavedQuest")
	TArray<int> HuntAmounts;

	UPROPERTY(EditAnywhere, Category = "FSavedQuest")
	int SelectedGoalIndex;

	UPROPERTY(EditAnywhere, Category = "FSavedQuest")
	TArray<FGoalInfo> CurrGoals;

	UPROPERTY(EditAnywhere, Category = "FSavedQuest")
	TArray<FCompletedGoal> CompletedGoals;

	UPROPERTY(EditAnywhere, Category = "FSavedQuest")
	EQuestStates CurrState;

	UPROPERTY(EditAnywhere, Category = "FSavedQuest")
	FText CurrDescription;
};
