// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include "../SkillSystem.h"
#include <Components/ActorComponent.h>
#include "SkillTreeComponent.generated.h"


class ASkillCharacter;
class ABase_Skill;
class USkillHotkeyWidget;
class USkillTreeEntryWidget;
class USkillTreeWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TUTORIAL_API USkillTreeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USkillTreeComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetupTree();

	void HandleShowCommand();

	void ActivateConnenctions(const TSubclassOf<ABase_Skill>& _ForSpell);

	void UpdateAllEntries();

	UFUNCTION()
	bool PlayerLearntSpell(const TSubclassOf<ABase_Skill>& _pSpellClass);

	UFUNCTION()
	void AddSkillPoints(int _Amount);

	UFUNCTION()
	void UpgradeSpell(ABase_Skill* _pSkill, USkillTreeEntryWidget* _pEntryWidget);

	UFUNCTION()
	void DowngradeSpell(ABase_Skill* _pSkill, USkillTreeEntryWidget* _pEntryWidget);

	UFUNCTION()
	bool CanUpgradeSpell(ABase_Skill* _pSkill);

	UFUNCTION()
	bool SkillOnHotkey(ABase_Skill* _pSkill, USkillHotkeyWidget*& out_ppHotkeyWidget);


	/* Get */
	UFUNCTION(BlueprintPure, Category = "SkillTreeComponent")
	const int& GetSkillPoints() const { return m_SkillPoints; }

	UFUNCTION(BlueprintPure, Category = "SkillTreeComponent")
	ASkillCharacter* GetPlayer() const { return m_pPlayer; }

	const TArray<FTreeCategory>& GetCategories() const { return m_Categories; }

	/* Set */
	UFUNCTION(BlueprintCallable, Category = "SkillTreeComponent")
	void SetSkillPoints(const int& _SkillPoints);

	UFUNCTION(BlueprintCallable, Category = "SkillTreeComponent")
	void SetPlayer(ASkillCharacter* _pPlayer);



protected :
	UPROPERTY(EditAnywhere, Category = "SkillTreeComponent")
	int m_SkillPoints = 2;

	UPROPERTY(EditDefaultsOnly, Category = "SkillTreeComponent")
	TArray<FTreeCategory> m_Categories;

	UPROPERTY()
	ASkillCharacter* m_pPlayer;

	UPROPERTY()
	USkillTreeWidget* m_pSkillTreeWidget;

	UPROPERTY()
	USkillTreeEntryWidget* m_pEntryWidget;

	ABase_Skill* m_pUpgradedSpell;

	bool m_bTreeSetUp = false;
	bool m_bTreeShown = false;
	
};
