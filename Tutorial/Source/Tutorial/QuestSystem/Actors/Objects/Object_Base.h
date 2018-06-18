// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../../Interfaces/Interactable_Interface.h"
#include "Object_Base.generated.h"

class UStaticMeshComponent;
class UWidgetComponent;
class AQuestManager;

UCLASS()
class TUTORIAL_API AObject_Base : public AActor, public IInteractable_Interface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AObject_Base();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public :
	void OnGameLoaded(AQuestManager* _pManager);

	/* Interface */
	virtual void OnEnterPlayerRadius(AQuestCharacter* _pPlayer) override;
	virtual void OnLeavePlayerRadius(AQuestCharacter* _pPlayer) override;
	virtual void OnInteractWith(AQuestCharacter* _pPlayer) override;

protected :
	UPROPERTY(VisibleDefaultsOnly, Category = "Object_Base|Components")
	UStaticMeshComponent* m_pStaticMesh;

	UPROPERTY(VisibleDefaultsOnly, Category = "Object_Base|Components")
	UWidgetComponent* m_pWidget;

	UPROPERTY(EditAnywhere, Category = "Object_Base")
	FText m_Name;
};
