// Fill out your copyright notice in the Description page of Project Settings.

#include "SkillCharacter.h"
#include "../Widgets/SkillSystemHUD.h"

#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"

#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"



// Sets default values
ASkillCharacter::ASkillCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll= false;

	GetCharacterMovement()->bOrientRotationToMovement = true;

	m_pSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	m_pSpringArm->TargetArmLength = 650.f;
	m_pSpringArm->SetRelativeRotation(FRotator(-40.f, 0.f, 0.f));
	m_pSpringArm->bUsePawnControlRotation = true;
	m_pSpringArm->SetupAttachment(RootComponent);

	m_pCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	m_pCamera->SetupAttachment(m_pSpringArm);	

	static ConstructorHelpers::FClassFinder<UUserWidget> WBP_HUD(TEXT("WidgetBlueprint'/Game/TutorialContent/SkillSystem/Widgets/WBP_HUD.WBP_HUD_C'"));
	if (WBP_HUD.Succeeded())
	{
		m_HUDClass = WBP_HUD.Class;
	}
	

}

// Called when the game starts or when spawned
void ASkillCharacter::BeginPlay()
{
	Super::BeginPlay();	

	if (m_HUDClass)
	{
		m_pHUD = CreateWidget<USkillSystemHUD>(GWorld, m_HUDClass);
		m_pHUD->AddToViewport();
	}

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

