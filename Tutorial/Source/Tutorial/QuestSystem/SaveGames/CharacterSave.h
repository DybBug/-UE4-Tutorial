// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "../QuestSystem.h"
#include "CharacterSave.generated.h"

class AObject_Base;
/**
 * 
 */
UCLASS()
class TUTORIAL_API UCharacterSave : public USaveGame
{
	GENERATED_BODY()

public:
	/* Get */
	const int& GetSavedExp()             const { return m_SavedExp; }
	const int& GetSavedExpForNextLevel() const { return m_SavedExpForNextLevel; }
	const int& GetSavedLevel()           const { return m_SavedLevel; }
	const int& GetSavedHealth()          const { return m_SavedHealth; }
	const int& GetSavedMaxHealth()       const { return m_SavedMaxHealth; }

	const TArray<TSubclassOf<AObject_Base>>& GetSavedObtainedObjects() const { return m_SavedObtainedObjects; }
	const TArray<FRegionPrestige>&           GetSavedPrestigePoints()  const { return m_SavedPrestigePoints; }

	const ERegions& GetSavedRegion()   const { return m_SavedRegion; }
	const FVector&  GetSavedLocation() const { return m_SavedLocation; }
	const int&      GetSavedHour()     const { return m_SavedHour; }
	const int&      GetSavedMinute()   const { return m_SavedMinute; }

	const TArray<FSavedQuest>& GetSavedQuests() const { return m_SavedQuests; }

	/* Set */
	void SetSavedExp(const int& _NewData)             { m_SavedExp = _NewData; }
	void SetSavedExpForNextLevel(const int& _NewData) { m_SavedExpForNextLevel = _NewData; }
	void SetSavedLevel(const int& _NewData)           { m_SavedLevel = _NewData; }
	void SetSavedHealth(const int& _NewData)          { m_SavedHealth = _NewData; }
	void SetSavedMaxHealth(const int& _NewData)       { m_SavedMaxHealth = _NewData; }

	void SetSavedObtainedObjects(const TArray<TSubclassOf<AObject_Base>>& _NewData) { m_SavedObtainedObjects = _NewData; }
	void SetSavedPrestigePoints(const TArray<FRegionPrestige>& _NewData)            { m_SavedPrestigePoints = _NewData; }

	void SetSavedRegion(const ERegions& _NewData)  { m_SavedRegion = _NewData; }
	void SetSavedLocation(const FVector& _NewData) { m_SavedLocation = _NewData; }
	void SetSavedHour(const int& _NewData)         { m_SavedHour = _NewData; }
	void SetSavedMinute(const int& _NewData)       { m_SavedMinute = _NewData; }

	void SetSavedQuests(const TArray<FSavedQuest>& _SavedQuests) { m_SavedQuests = _SavedQuests; }

protected :
	UPROPERTY()
	int m_SavedExp;

	UPROPERTY()
	int m_SavedExpForNextLevel;

	UPROPERTY()
	int m_SavedLevel;

	UPROPERTY()
	int m_SavedHealth;

	UPROPERTY()
	int m_SavedMaxHealth;

	UPROPERTY()
	TArray<TSubclassOf<AObject_Base>> m_SavedObtainedObjects;

	UPROPERTY()
	TArray<FRegionPrestige> m_SavedPrestigePoints;

	UPROPERTY()
	ERegions m_SavedRegion;

	UPROPERTY()
	FVector m_SavedLocation;

	UPROPERTY()
	int m_SavedHour;

	UPROPERTY()
	int m_SavedMinute;	

	UPROPERTY()
	TArray<FSavedQuest> m_SavedQuests;
};
