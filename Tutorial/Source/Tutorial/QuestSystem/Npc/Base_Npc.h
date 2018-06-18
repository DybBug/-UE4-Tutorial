// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../Interfaces/Interactable_Interface.h"
#include "../QuestSystem.h"
#include "Base_Npc.generated.h"

class USpringArmComponent;
class UPaperSpriteComponent;
class UWidgetComponent;
class AQuest_Base;
class APatrolPoint;
class AQuestManager;

UCLASS()
class TUTORIAL_API ABase_Npc : public ACharacter,
							   public IInteractable_Interface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABase_Npc();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public :
	void Initialize(bool _bDoesPatrol, const TArray<APatrolPoint*>& _PatrollingPoints, int _CurrPatrolIndex, int _NpcId);
	void MoveToCurrPatrolPoint();
	virtual void OnTalkedTo(AQuestCharacter* _pPlayer);
	void ShowMessage(const FText& _Message, float _Duration, AQuestCharacter* _pPlayer);
	void OnOwnQuestCancelled(TSubclassOf<AQuest_Base> QuestClass);

	bool OnPlayerLevelUp(int _NewLevel);
	bool OnPlayerGainPrestige(AQuestCharacter* _pPlayer);
	void OnQuestsLoaded(AQuestManager* _pManager);

	/* Interface Functions */
	virtual void OnEnterPlayerRadius(AQuestCharacter* _pPlayer) override;
	virtual void OnLeavePlayerRadius(AQuestCharacter* _pPlayer) override;
	virtual void OnInteractWith(AQuestCharacter* _pPlayer) override;

	/* Get */
	const int& GetNpcId() const { return m_NpcId; }
	
protected :
	UPROPERTY(VisibleDefaultsOnly, Category = "Base_Npc|Components")
	USpringArmComponent* m_pSpringArm;

	UPROPERTY(VisibleDefaultsOnly, Category = "Base_Npc|Components")
	UPaperSpriteComponent* m_pQuestionMark;

	UPROPERTY(VisibleDefaultsOnly, Category = "Base_Npc|Components")
	UWidgetComponent* m_pInteractionWidget;

	UPROPERTY(VisibleDefaultsOnly, Category = "Base_Npc|Components")
	UWidgetComponent* m_pMessageWidget;

	UPROPERTY(EditAnywhere, Category = "Base_Npc")
	FText m_Name;

	UPROPERTY(EditAnywhere, Category = "Base_Npc")
	bool m_bDoesPatrol = false;

	UPROPERTY(EditAnywhere, Category = "Base_Npc")
	TArray<APatrolPoint*> m_PatrollingPoints;

	UPROPERTY(VisibleAnywhere, Category = "Base_Npc")
	int m_CurrPatrolIndex;

	UPROPERTY(EditAnywhere, Category = "Base_Npc")
	TSubclassOf<AQuest_Base> m_HasQuestClass;

	UPROPERTY(VisibleAnywhere, Category = "Base_Npc")
	int m_NpcId;

	UPROPERTY(EditAnywhere, Category = "Base_Npc")
	FText m_DefaultMessage;

	UPROPERTY(EditAnywhere, Category = "Base_Npc")
	float m_DefaultDuration;

	bool m_bInPlayerRadius = false;

	bool m_bCanBeTalkedTo = true;

	UPROPERTY(VisibleAnywhere, Category = "Base_Npc")
	bool m_bCanGiveLevelQuest = false;

	UPROPERTY(VisibleAnywhere, Category = "Base_Npc")
	bool m_bCanGivePrestigeQuest = false;

	UPROPERTY(VisibleAnywhere, Category = "Base_Npc")
	bool m_bHasGivenQuest = false;

	UPROPERTY(EditAnywhere, Category = "Base_Npc")
	int m_RequireLevel;

	UPROPERTY(EditAnywhere, Category = "Base_Npc")
	FRegionPrestige m_RequiredPrestige;

};
