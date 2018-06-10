// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestSystem/Actors/Objects/Object_Base.h"
#include "Object_Cone.generated.h"

/**
 * 
 */
UCLASS()
class TUTORIAL_API AObject_Cone : public AObject_Base
{
	GENERATED_BODY()
	
protected :
	virtual void BeginPlay() override;

public :
	/* Interface */
	virtual void OnEnterPlayerRadius(AQuestCharacter* _pPlayer) override;
	virtual void OnLeavePlayerRadius(AQuestCharacter* _pPlayer) override;
	virtual void OnInteractWith(AQuestCharacter* _pPlayer) override;	
protected :
	UMaterialInstanceDynamic* m_pMatInstance;
	
};
