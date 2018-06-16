// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../Interfaces/Interactable_Interface.h"
#include "Base_Npc.generated.h"

class USpringArmComponent;
class UPaperSpriteComponent;
class UWidgetComponent;
class AQuest_Base;
class APatrolPoint;

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

	/* Interface Functions */
	virtual void OnEnterPlayerRadius(AQuestCharacter* _pPlayer) override;
	virtual void OnLeavePlayerRadius(AQuestCharacter* _pPlayer) override;
	virtual void OnInteractWith(AQuestCharacter* _pPlayer) override;
	
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
};
