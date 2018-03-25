// Fill out your copyright notice in the Description page of Project Settings.

#include "SkillPlayerController.h"
#include "Characters/SkillCharacter.h"
#include "GoalDecal.h"
#include "Characters/SkillCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "GameFramework/SpringArmComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/GameEngine.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"
#include "AI/Navigation/NavigationSystem.h"


ASkillPlayerController::ASkillPlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableTouchEvents = true;
	bEnableMouseOverEvents = true;

	static ConstructorHelpers::FClassFinder<AGoalDecal> BP_GoalDecalClass(TEXT("Blueprint'/Game/TutorialContent/SkillSystem/BP_GoalDecal.BP_GoalDecal_C'"));
	if (BP_GoalDecalClass.Succeeded())
	{
		m_GoalDecalClass = BP_GoalDecalClass.Class;
	}

}

void ASkillPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("RMouseButton", IE_Pressed, this, &ASkillPlayerController::_RMouseButton);
	InputComponent->BindAction("RMouseButton", IE_Released, this, &ASkillPlayerController::_RMouseButton);
	InputComponent->BindAction("LMouseButton", IE_Pressed, this, &ASkillPlayerController::_LMouseButton);

	InputComponent->BindAxis("LookUp", this, &ASkillPlayerController::_LookUp);
	InputComponent->BindAxis("Turn", this, &ASkillPlayerController::_Turn);
	InputComponent->BindAxis("MoveForward", this, &ASkillPlayerController::_MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ASkillPlayerController::_MoveRight);

}

void ASkillPlayerController::CancelMovementCommand()
{
	if (m_pGoalDecal != nullptr)
	{
		Cast<ASkillCharacter>(GetPawn())->GetCharacterMovement()->StopMovementImmediately();
		m_bHasMouseMovementCommand = false;
		m_pGoalDecal->Destroy();
		m_pGoalDecal = nullptr;
	}
}

void ASkillPlayerController::_RMouseButton()
{
	m_bCanTurn = !m_bCanTurn;

}

void ASkillPlayerController::_LMouseButton()
{
	if (GWorld) // GetWorld()도 사용 가능.
	{
		CancelMovementCommand(); // 목표지점 취소.

		FHitResult HitResult;
		
		if (GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery1, true, HitResult))
		{
			GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("Hit!!"));
			FVector GoalDecalLoc = FVector(HitResult.Location.X, HitResult.Location.Y, HitResult.Location.Z);
			m_pGoalDecal = GWorld->SpawnActor<AGoalDecal>(m_GoalDecalClass, GoalDecalLoc, FRotator(0.f, 0.f, 0.f));
			m_bHasMouseMovementCommand = true;
			
			UNavigationSystem::SimpleMoveToLocation(this, GoalDecalLoc);
		}
	}
}

void ASkillPlayerController::_LookUp(float _Value)
{
	if ((_Value != 0.f) && (m_bCanTurn == true))
	{
		if (GetWorld())
		{			

			AddPitchInput(_Value*1.5f);	
			ControlRotation.Pitch = UKismetMathLibrary::ClampAngle(ControlRotation.Pitch, -40.f, 0.f);
		}
	}
}

void ASkillPlayerController::_Turn(float _Value)
{
	if ((_Value != 0.f) && (m_bCanTurn == true))
	{
		if (GetWorld())
		{
			AddYawInput(_Value*2.f);

		}
	}
}

void ASkillPlayerController::_MoveForward(float _Value)
{
	if (_Value != 0.f)
	{
		if (m_bHasMouseMovementCommand == true)
		{
			CancelMovementCommand();
		}
		
		FRotator Rot = GetControlRotation();
		const FVector ForwardVec = FRotationMatrix(Rot).GetScaledAxis(EAxis::X); //회전행렬이 나타내는 어떤(X) 축의 월드 벡터.
		
		
		Cast<ASkillCharacter>(GetPawn())->AddMovementInput(ForwardVec, _Value);
	}

}

void ASkillPlayerController::_MoveRight(float _Value)
{
	if (_Value != 0.f)
	{
		if (m_bHasMouseMovementCommand == true)
		{
			CancelMovementCommand();
		}
		FRotator Rot = GetControlRotation();
		const FVector RightVec = FRotationMatrix(Rot).GetUnitAxis(EAxis::Y);//회전행렬이 나타내는 어떤(Y) 축의 월드 벡터.

		Cast<ASkillCharacter>(GetPawn())->AddMovementInput(RightVec, _Value);
	}
}

