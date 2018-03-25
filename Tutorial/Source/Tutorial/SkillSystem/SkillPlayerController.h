// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SkillPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TUTORIAL_API ASkillPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ASkillPlayerController();

protected:
	virtual void SetupInputComponent()override;

public :
	// ������ ����ϴ� �Լ�.
	void CancelMovementCommand();
	
protected:
	bool m_bCanTurn = false;
	bool m_bHasMouseMovementCommand = false;

	TSubclassOf<class AGoalDecal> m_GoalDecalClass;
	class AGoalDecal* m_pGoalDecal = nullptr;

protected :

	/*Ű ���ε� �Լ�*/
	void _RMouseButton();
	void _LMouseButton();
	void _LookUp(float _Value);
	void _Turn(float _Value);
	void _MoveForward(float _Value);
	void _MoveRight(float _Value);
	
};
