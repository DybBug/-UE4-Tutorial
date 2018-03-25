// Fill out your copyright notice in the Description page of Project Settings.

#include "SkillCharacter.h"

#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"

#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"



// Sets default values
ASkillCharacter::ASkillCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll= false;

	m_pSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	m_pSpringArm->TargetArmLength = 650.f;
	m_pSpringArm->SetRelativeRotation(FRotator(-40.f, 0.f, 0.f));
	m_pSpringArm->bUsePawnControlRotation = true;
	m_pSpringArm->SetupAttachment(RootComponent);

	m_pCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	m_pCamera->SetupAttachment(m_pSpringArm);

	GetCharacterMovement()->bOrientRotationToMovement = true;
}

// Called when the game starts or when spawned
void ASkillCharacter::BeginPlay()
{
	Super::BeginPlay();	
}

// Called every frame
void ASkillCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	

}

// Called to bind functionality to input
void ASkillCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

