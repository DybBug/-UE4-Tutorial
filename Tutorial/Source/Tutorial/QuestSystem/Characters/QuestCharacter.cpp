// Fill out your copyright notice in the Description page of Project Settings.

#include "QuestCharacter.h"
#include "../Widgets/QuestSystemHUD.h"
#include "../Actors/QuestManager.h"

#include <UObject/ConstructorHelpers.h>
#include <Camera/CameraComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetMathLibrary.h>
#include <Kismet/KismetTextLibrary.h>
#include <Particles/ParticleSystemComponent.h>
#include <Components/ChildActorComponent.h>
#include <PaperSpriteComponent.h>



// Sets default values
AQuestCharacter::AQuestCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	m_pSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	m_pSpringArm->TargetArmLength = 500.f;
	m_pSpringArm->SetRelativeRotation(FRotator(-25.f, 0.f, 0.f));
	m_pSpringArm->SetupAttachment(RootComponent);

	m_pCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	m_pCamera->SetupAttachment(m_pSpringArm);

	m_pMiniMapArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("MiniMapArm"));
	m_pMiniMapArm->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
	m_pMiniMapArm->TargetArmLength = 650.f;
	m_pMiniMapArm->bDoCollisionTest = false;
	m_pMiniMapArm->bInheritPitch = false;
	m_pMiniMapArm->bInheritRoll = false;
	m_pMiniMapArm->bInheritYaw = false;
	m_pMiniMapArm->SetupAttachment(RootComponent);

	m_pMiniMapCamera = CreateDefaultSubobject<UChildActorComponent>(TEXT("MiniMapCamera"));
	m_pMiniMapCamera->SetupAttachment(m_pMiniMapArm);

	m_pPointer = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Pointer"));
	m_pPointer->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_pPointer->bGenerateOverlapEvents = false;
	m_pPointer->bOwnerNoSee = true;
	m_pPointer->SetRelativeLocation(FVector(0.f, 0.f, 120.f));
	m_pPointer->SetRelativeRotation(FRotator(0.f, 90.f, -90.f));
	m_pPointer->SetRelativeScale3D(FVector(2.5f, 2.5f, 2.5f));
	m_pPointer->SetupAttachment(RootComponent);


	m_pLevelUpEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("LevelUpEffect"));
	m_pLevelUpEffect->SetAutoActivate(false);
	m_pLevelUpEffect->SetupAttachment(RootComponent);


	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(TEXT("WidgetBlueprint'/Game/TutorialContent/QuestSystem/Widgets/WB_HUD.WB_HUD_C'"));
	if (WidgetClass.Succeeded())
	{
		m_pHUDClass = WidgetClass.Class;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> Sound(TEXT("SoundWave'/Game/TutorialContent/QuestSystem/Audio/Effects/LevelUpSound.LevelUpSound'"));
	if (Sound.Succeeded())
	{
		m_pLevelUpSound = Sound.Object;
	}

}

// Called when the game starts or when spawned
void AQuestCharacter::BeginPlay()
{
	Super::BeginPlay();

	// HUD À§Á¬ »ý¼º.
	if (m_pHUDClass)
	{
		m_pHUD = CreateWidget<UQuestSystemHUD>(GetWorld(), m_pHUDClass);
		m_pHUD->AddToViewport();

		FActorSpawnParameters Param;
		Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		m_pQuestManager = GetWorld()->SpawnActor<AQuestManager>(AQuestManager::StaticClass(), Param);

		m_pHUD->SetQuestManager(m_pQuestManager);

		m_pQuestManager->SetHUD(m_pHUD);
		
		UpdateLevel();
		UpdateExp();
	}	
}

// Called to bind functionality to input
void AQuestCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindKey(EKeys::F, IE_Pressed, this, &AQuestCharacter::_FKey);

}

void AQuestCharacter::OnLevelUp()
{
	m_CurrExp         -= m_ExpForNextLevel;
	m_ExpForNextLevel *= m_NextExpMultiplier;	
	m_CurrLevel++;

	UpdateLevel();

	UGameplayStatics::PlaySound2D(GetWorld(), m_pLevelUpSound, 0.5f, 1.2f);
	m_pLevelUpEffect->SetActive(true);
}

void AQuestCharacter::UpdateExp()
{
#define LOCTEXT_NAMESPACE "ExpText"
	FText ExpText = FText::Format(LOCTEXT("ExpText", "{0} / {1}"), m_CurrExp, m_ExpForNextLevel);
#undef LOCTEXT_NAMESPACE

	m_pHUD->GetExpText()->SetText(ExpText);
	m_pHUD->GetExpBar()->SetPercent((float)m_CurrExp / (float)m_ExpForNextLevel);
}

void AQuestCharacter::UpdateLevel()
{
	m_pHUD->GetLevelText()->SetText(UKismetTextLibrary::Conv_IntToText(m_CurrLevel));
}

void AQuestCharacter::AddExpPoints(int _Amount)
{
	m_CurrExp += _Amount;

	CHECK_EXP:
	if (m_CurrExp >= m_ExpForNextLevel)
	{
		OnLevelUp();
		goto CHECK_EXP;
	}
	else
	{
		UpdateExp();
	}
}

void AQuestCharacter::_FKey()
{
	AddExpPoints(100);
}

