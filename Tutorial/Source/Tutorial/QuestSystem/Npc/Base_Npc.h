// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../Interfaces/Interactable_Interface.h"
#include "Base_Npc.generated.h"

class UWidgetComponent;
class UCapsuleComponent;
class AQuest_Base;

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
	/* Interface Functions */
	virtual void OnEnterPlayerRadius(AQuestCharacter* _pPlayer) override;
	virtual void OnLeavePlayerRadius(AQuestCharacter* _pPlayer) override;
	virtual void OnInteractWith(AQuestCharacter* _pPlayer) override;
	
protected :
	UPROPERTY(VisibleDefaultsOnly, Category = "Base_Npc")
	UWidgetComponent* m_pWidget;

	UPROPERTY(EditAnywhere, Category = "Base_Npc")
	FText m_Name;

	TSubclassOf<AQuest_Base> m_QuestTest1Class;
};
