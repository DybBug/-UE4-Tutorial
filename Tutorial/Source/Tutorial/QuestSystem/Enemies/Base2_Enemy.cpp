// Fill out your copyright notice in the Description page of Project Settings.

#include "Base2_Enemy.h"
#include "../Widgets/Enemy2Widget.h"
#include "../Characters/QuestCharacter.h"
#include "../Actors/QuestManager.h"
#include "../Actors/RespawnActor.h"


#include <Components/CapsuleComponent.h>
#include <Components/WidgetComponent.h>
#include <Components/ProgressBar.h>
#include <Components/TextBlock.h>
#include <AIController.h>
#include <BehaviorTree/BlackboardComponent.h>
#include <Perception/PawnSensingComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetMathLibrary.h>
#include <Kismet/KismetSystemLibrary.h>
#include <BehaviorTree/Tasks/BTTask_BlueprintBase.h>
#include <Animation/AnimMontage.h>
#include <UObject/ConstructorHelpers.h>
#include <TimerManager.h>

// Sets default values
ABase2_Enemy::ABase2_Enemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// #.PawnSensingComponent.
	m_pSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Sensing"));
	m_pSensing->HearingThreshold = 0.f;
	m_pSensing->LOSHearingThreshold = 0.f;
	m_pSensing->SightRadius = 1000.f;
	m_pSensing->SensingInterval = 0.5f;
	m_pSensing->HearingMaxSoundAge = 0.f;
	m_pSensing->bEnableSensingUpdates = true;
	m_pSensing->bOnlySensePlayers = true;
	m_pSensing->bSeePawns = true;
	m_pSensing->bHearNoises = false;
	m_pSensing->SetPeripheralVisionAngle(45.f);
	m_pSensing->OnSeePawn.AddDynamic(this, &ABase2_Enemy::_OnSeePawn);

	// #. WidgetComponent.
	static ConstructorHelpers::FClassFinder<UUserWidget> WB_EnemyWidget(TEXT("WidgetBlueprint'/Game/TutorialContent/QuestSystem/Widgets/WB_Enemy.WB_Enemy_C'"));
	if (WB_EnemyWidget.Succeeded())
	{
		m_pWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
		m_pWidget->SetWidgetClass(WB_EnemyWidget.Class);
		m_pWidget->SetRelativeLocation(FVector(0.f, 0.f, 110.f));
		m_pWidget->SetWidgetSpace(EWidgetSpace::Screen);
		m_pWidget->SetDrawSize(FVector2D(200.f, 74.f));
		m_pWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		m_pWidget->bMultiBodyOverlap = false;
		m_pWidget->SetVisibility(false);
		m_pWidget->SetupAttachment(RootComponent);
	}
}

// Called when the game starts or when spawned
void ABase2_Enemy::BeginPlay()
{
	Super::BeginPlay();

	if (m_pBehaviorTree)
	{
		Cast<AAIController>(GetController())->RunBehaviorTree(m_pBehaviorTree);
		Cast<AAIController>(GetController())->GetBlackboardComponent()->SetValueAsVector("OriginalLocation", GetActorLocation());
		Cast<AAIController>(GetController())->GetBlackboardComponent()->SetValueAsFloat("PatrolRadius", m_PatrolRadius);	
		Cast<AAIController>(GetController())->GetBlackboardComponent()->SetValueAsFloat("AttackRange", m_AttackRange);	
		Cast<AAIController>(GetController())->GetBlackboardComponent()->SetValueAsFloat("MovementRadius", m_MovementRadius);		

	}

	m_CurrHealth = m_MaxHealth;

	_SetupWidget();

}

float ABase2_Enemy::TakeDamage(float _DamageAmount, FDamageEvent const & _DamageEvent, AController * _pEventInstigator, AActor * _pDamageCauser)
{
	if (!m_bIsDead)
	{
		m_CurrHealth -= _DamageAmount;
		m_CurrHealth = FMath::Clamp<int>(m_CurrHealth, 0, m_MaxHealth);
		UpdateHealthBar();

		m_pKilledByActor = _pDamageCauser;

		if (m_CurrHealth <= 0)
		{			
			OnDeath();
		}
		else
		{
			_OnActorNoticed(_pDamageCauser);
		}
	}
	return 0.0f;
}

void ABase2_Enemy::AttackNotify()
{
	FVector StartVec = GetActorLocation();

	FVector EndVec = StartVec + (GetActorForwardVector() * m_AttackDistance);

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjTypes;
	ObjTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	TArray<AActor*> IgnorActors;
	IgnorActors.Add(this);

	FHitResult Result;
	UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), StartVec, EndVec, ObjTypes, true, IgnorActors, EDrawDebugTrace::None, Result, true);
	UGameplayStatics::ApplyDamage(Result.GetActor(), m_AttackDamage, nullptr, this, UDamageType::StaticClass());

}

void ABase2_Enemy::OnDeath()
{
	if (GetCurrentMontage())
	{
		StopAnimMontage(GetCurrentMontage());
	}
	m_bIsDead = true;

	Cast<AAIController>(GetController())->GetBlackboardComponent()->SetValueAsBool("IsDead", m_bIsDead);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	m_pWidget->DestroyComponent();

	AQuestCharacter* pPlayer = Cast<AQuestCharacter>(m_pKilledByActor);
	pPlayer->AddExpPoints(m_ExpForKill);
	pPlayer->GetQuestManager()->OnEnemyKilled(this->GetClass());

	FTimerHandle hTimer;
	GetWorldTimerManager().SetTimer(hTimer, [&] {
		m_pRespawnActor->RespawnEnemy(FEnemyRespawn(m_RespawnTime, GetClass()));
		Destroy();
	}, 1.f, false, 4.f);
}

void ABase2_Enemy::UpdateHealthBar()
{
	m_pEnemyWidget->GetHealthBar()->SetPercent((float)m_CurrHealth / (float)m_MaxHealth);
}

bool ABase2_Enemy::IsPlayingMontage()
{
	return ((GetCurrentMontage()) ? true : false);
}

void ABase2_Enemy::_SetupWidget()
{
	m_pEnemyWidget = Cast<UEnemy2Widget>(m_pWidget->GetUserWidgetObject());
	m_pEnemyWidget->GetNameText()->SetText(m_Name);
	m_pEnemyWidget->GetLevelText()->SetText(FText::AsNumber(m_Level));

	UpdateHealthBar();

	m_pWidget->SetVisibility(true);
}

void ABase2_Enemy::_OnActorNoticed(AActor * _pActor)
{
	if ((UGameplayStatics::GetPlayerPawn(GetWorld(), 0) == _pActor) && !m_bHasSeenPlayer)
	{
		m_bHasSeenPlayer = true;
		Cast<AAIController>(GetController())->GetBlackboardComponent()->SetValueAsBool("HasSeenPlayer?", m_bHasSeenPlayer);
		Cast<AAIController>(GetController())->GetBlackboardComponent()->SetValueAsObject("Player", _pActor);
		GetCharacterMovement()->MaxWalkSpeed = 500.f;
	}
}

void ABase2_Enemy::_OnSeePawn(APawn * _pPawn)
{
	_OnActorNoticed(_pPawn);
}

