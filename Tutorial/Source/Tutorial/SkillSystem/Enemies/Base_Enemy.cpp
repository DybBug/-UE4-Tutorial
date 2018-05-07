// Fill out your copyright notice in the Description page of Project Settings.

#include "Base_Enemy.h"
#include "../Characters/SkillCharacter.h"
#include "../Widgets/EnemyWidget.h"
#include "../BlueprintFunctionLibraries/Combat_BlueprintFunctionLibrary.h"

#include <UObject/ConstructorHelpers.h>
#include <Components/WidgetComponent.h>
#include <Components/SphereComponent.h>
#include <GameFrameWork/CharacterMovementComponent.h>
#include <AI/Navigation/NavigationSystem.h>
#include <Perception/AIPerceptionComponent.h>
#include <Perception/AISenseConfig_Sight.h>
#include <Animation/AnimMontage.h>
#include <Kismet/KismetMathLibrary.h>
#include <Kismet/KismetSystemLibrary.h>
#include <TimerManager.h>
#include <Engine.h>
#include <AIController.h>



// Sets default values
ABase_Enemy::ABase_Enemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

	m_pWidgetVisibleDomain = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	m_pWidgetVisibleDomain->SetSphereRadius(1500.f);
	m_pWidgetVisibleDomain->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	m_pWidgetVisibleDomain->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
	m_pWidgetVisibleDomain->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	m_pWidgetVisibleDomain->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	m_pWidgetVisibleDomain->OnComponentBeginOverlap.AddDynamic(this, &ABase_Enemy::_OnComponentBeginOverlap);
	m_pWidgetVisibleDomain->OnComponentEndOverlap.AddDynamic(this, &ABase_Enemy::_OnComponentEndOverlap);
	m_pWidgetVisibleDomain->bGenerateOverlapEvents = true;
	m_pWidgetVisibleDomain->SetupAttachment(RootComponent);

	m_pAIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComp"));
	UAISenseConfig_Sight* pConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("AISenseConfig_Sight"));
	pConfig->SightRadius = 800.f;
	pConfig->LoseSightRadius = 2000.f;
	pConfig->DetectionByAffiliation.bDetectEnemies = true;
	pConfig->DetectionByAffiliation.bDetectFriendlies = true;
	pConfig->DetectionByAffiliation.bDetectNeutrals = true;
	m_pAIPerception->ConfigureSense(*pConfig);
	m_pAIPerception->SetDominantSense(UAISense_Sight::StaticClass());
	m_pAIPerception->OnPerceptionUpdated.AddDynamic(this, &ABase_Enemy::_OnPerceptionUpdated);	

	m_pWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComp"));
	m_pWidget->SetWidgetSpace(EWidgetSpace::Screen);
	m_pWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_pWidget->bGenerateOverlapEvents = false;
	m_pWidget->SetDrawAtDesiredSize(true);
	m_pWidget->SetupAttachment(RootComponent);

	
}

// Called when the game starts or when spawned
void ABase_Enemy::BeginPlay()
{
	Super::BeginPlay();

	m_StartingLocation = GetActorLocation();
	GetCharacterMovement()->MaxWalkSpeed = m_PatrolWalkSpeed;
	m_CurrHealth = m_MaxHealth;	

	_Patrol();

	if (m_pWidgetVisibleDomain->IsOverlappingActor(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)))
	{
		m_pWidget->GetUserWidgetObject()->SetVisibility(ESlateVisibility::Hidden);
	}
}


void ABase_Enemy::NotifyHit()
{
	FVector StartLoc = GetActorLocation();
	FVector EndLoc = GetActorLocation() + (GetActorForwardVector() * m_AttackTraceDistance);

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjTypeQueries;
	ObjTypeQueries.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	FHitResult HitResult;

	if (UKismetSystemLibrary::LineTraceSingleForObjects(this, StartLoc, EndLoc, ObjTypeQueries, true, TArray<AActor*>(), EDrawDebugTrace::None, HitResult, true))
	{
		// 액터에 인터페이스가 있는가??
		if (UKismetSystemLibrary::DoesImplementInterface(HitResult.GetActor(), UDamageable_Interface::StaticClass()))
		{
			if (Cast<ASkillCharacter>(HitResult.GetActor()))
			{
				Cast<ASkillCharacter>(HitResult.GetActor())->OnReceiveDamage(m_AttackDamage, m_DamageType, m_ElementClass, m_CriticalChance, this, nullptr);
			}
		}
	
	}
}

void ABase_Enemy::PerformAttack()
{
	if(!m_pTargetActor) return;

	if (!m_bIsDead && m_bWasAggroed && !m_bIsRunningBack)
	{
		FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), m_pTargetActor->GetActorLocation());
		SetActorRotation(LookAtRot);
		if(m_AttackAnimMontages.Num() != 0)
		{
			m_pCurrAttackAnimMontage = m_AttackAnimMontages[m_CurrAttackAnimMontageIndex];
			PlayAnimMontage(m_pCurrAttackAnimMontage);
			
			FTimerHandle hTimer;
			GetWorldTimerManager().SetTimer(hTimer, [&]{ 
				m_CurrAttackAnimMontageIndex = GetNextAnimationIndex(); 
				if (GetInAttackRange())
				{
					PerformAttack();
				}
				else
				{
					AAIController* pAICtrl = Cast<AAIController>(GetController());
					//pAICtrl->MoveToActor(m_pTargetActor);
					UNavigationSystem::SimpleMoveToActor(GetController(), m_pTargetActor);
					PerformAttack();					
				}
			}, m_pCurrAttackAnimMontage->SequenceLength, false);
		}
	}
}

void ABase_Enemy::OnAggroedPulled(AActor * _pTarget)
{
	if (!m_bWasAggroed && !m_bIsRunningBack)
	{
		m_bIsPatrolling = false;
		m_bWasAggroed   = true;
		m_pTargetActor  = _pTarget;
		GetCharacterMovement()->MaxWalkSpeed = m_AggroedWalkSpeed;

		GetWorldTimerManager().SetTimer(m_hFollowTargetDistanceTimer, this, &ABase_Enemy::_FollowTargetDistance, 1.f, true);

		if (GetInAttackRange())
		{
			PerformAttack();
		}
		else
		{
			AAIController* pAICtrl = Cast<AAIController>(GetController());
			//pAICtrl->MoveToActor(m_pTargetActor);
			UNavigationSystem::SimpleMoveToActor(GetController(), m_pTargetActor);		
			PerformAttack();		
		}
	}
}

void ABase_Enemy::OnReset()
{
	m_bIsRunningBack = true;

	m_CurrAttackAnimMontageIndex = 0;
	m_pTargetActor = nullptr;
	m_bWasAggroed = false;
	m_bIsPatrolling = true;

	StopAnimMontage(m_pCurrAttackAnimMontage);

	m_CurrHealth = m_MaxHealth;
	_UpdateHealthBar();


//	GetWorldTimerManager().ClearTimer(m_hFollowTargetDistanceTimer);
	UKismetSystemLibrary::K2_ClearAndInvalidateTimerHandle(this, m_hFollowTargetDistanceTimer);

	AAIController* pAICtrl = Cast<AAIController>(GetController());
	//pAICtrl->MoveToLocation(m_StartingLocation);
	UNavigationSystem::SimpleMoveToLocation(pAICtrl, m_StartingLocation);
	GetCharacterMovement()->MaxWalkSpeed = m_PatrolWalkSpeed;

	_Patrol();

	m_bIsRunningBack = false;
}

void ABase_Enemy::OnReceiveDamage(
	float _BaseDamage, 
	EDamageTypes _Type, 
	TSubclassOf<class ABase_Element> _ElementClass, 
	int _CritChance,
	AActor* _pAttacker,
	ABase_Skill* _pSpell)
{
	int Damage;
	bool IsCritical;
	EEffectiveness Effectiveness;

	if (!m_bIsDead && !UCombat_BlueprintFunctionLibrary::IsEnemy(_pAttacker))
	{
		UCombat_BlueprintFunctionLibrary::CalculateFinalDamage(_BaseDamage, _CritChance, _ElementClass, m_ElementClass, Damage, IsCritical, Effectiveness);

		m_CurrHealth = FMath::Clamp<int>(m_CurrHealth - Damage, 0, m_MaxHealth);

		_UpdateHealthBar();

		if (m_CurrHealth > 0)
		{
			OnAggroedPulled(_pAttacker);
		}
		else
		{
			_OnDeath(_pAttacker);
		}
	}
}

void ABase_Enemy::OnSelected(ASkillCharacter * _pPlayer)
{
	if(!m_bIsDead && !m_bIsSelected)
	{
		GetMesh()->SetRenderCustomDepth(true);
		m_pSelectingPlayer = _pPlayer;
		m_pSelectingPlayer->SetSelectedEnemy(this);
		m_bIsSelected = true;


#define LOCTEXT_NAMESPACE "EnemyName"
		FText EnemyName = FText::Format(LOCTEXT("EnemyName", "{0} (Lv. {1}"),  FText::FromName(m_Name), m_Level);
#undef LOCTEXT_NAMESPACE
		m_pSelectingPlayer->GetHUD()->GetEnemyNameText()->SetText(EnemyName);
		m_pSelectingPlayer->GetHUD()->GetEnemyBorder()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		_UpdateHealthBar();
	}
}

void ABase_Enemy::OnSelectionEnd(ASkillCharacter * _pPlayer)
{
	if (m_bIsSelected)
	{
		GetMesh()->SetRenderCustomDepth(false);
		m_pSelectingPlayer->SetSelectedEnemy(nullptr);
		m_pSelectingPlayer->GetHUD()->GetEnemyBorder()->SetVisibility(ESlateVisibility::Hidden);
		m_pSelectingPlayer = nullptr;		
		m_bIsSelected = false;
		
	}
}

int ABase_Enemy::GetNextAnimationIndex() 
{
	return (m_CurrAttackAnimMontageIndex + 1) % m_AttackAnimMontages.Num();
}

bool ABase_Enemy::GetInAttackRange() 
{
	bool Result = false;
	if (m_pTargetActor)
	{
		Result = (m_pTargetActor->GetDistanceTo(this) < m_AttackRange) ? true : false;
	}
	return Result;
}

void ABase_Enemy::SetIsDead(bool& _bIsDead)
{
	m_bIsDead = _bIsDead;
}

void ABase_Enemy::SetStartingLocation(FVector & _Location)
{
	m_StartingLocation = _Location;
}

void ABase_Enemy::SetPatrolWalkSpeed(float & _Speed)
{
	m_PatrolWalkSpeed = _Speed;
}

void ABase_Enemy::SetPatrolRadius(float _Radius)
{
	m_PatrolRadius = _Radius;
}

void ABase_Enemy::SetIsPatrolling(bool _bBool)
{
	m_bIsPatrolling = _bBool;
}

/*
* private 함수 영역.
*/
void ABase_Enemy::_Patrol()
{
	FVector Point;
	UNavigationSystem::K2_GetRandomReachablePointInRadius(this, m_StartingLocation, Point, m_PatrolRadius);

	AController* pAICtrl = GetController();
	//UNavigationSystem::SimpleMoveToLocation(pAICtrl, Point);
	Cast<AAIController>(pAICtrl)->MoveToLocation(Point);
	
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, FString::Printf(TEXT("%.2f, %.2f, %.2f"), Point.X, Point.Y, Point.Z));

	if (m_bIsPatrolling)
	{
		float Range = UKismetMathLibrary::RandomFloatInRange(1.f, 4.f);

		FTimerHandle hTimer;
		GetWorldTimerManager().SetTimer(hTimer, this, &ABase_Enemy::_DelayFunc, Range, false);
	}
}

void ABase_Enemy::_InitWidgetText()
{
	UEnemyWidget* pWidget = Cast<UEnemyWidget>(m_pWidget->GetUserWidgetObject());

#define LOCTEXT_NAMESPACE "LevelName"
	FText LevelName = FText::Format(LOCTEXT("LevelName", "[Lvl. {0}] {1}"), m_Level, FText::FromName(m_Name));
#undef LOCTEXT_NAMESPACE
	pWidget->GetLevelNameText()->SetText(LevelName);
	pWidget->GetLevelNameText()->SetColorAndOpacity(m_bAggressive ? FSlateColor(FColor::Red) : FSlateColor(FColor::White));
}

void ABase_Enemy::_UpdateHealthBar()
{
	float Percent = (float)m_CurrHealth / (float)m_MaxHealth;
	UEnemyWidget* pWidget = Cast<UEnemyWidget>(m_pWidget->GetUserWidgetObject());
	pWidget->GetHealthBar()->SetPercent(Percent);

	if (m_bIsSelected)
	{
		m_pSelectingPlayer->GetHUD()->GetEnemyHealthBar()->SetPercent(Percent);
#define LOCTEXT_NAMESPACE "EnemyHealth"
		FText EnemyHealth = FText::Format(LOCTEXT("EnemyHealth", "{0} / {1}"), (int)m_CurrHealth, (int)m_MaxHealth);
#undef LOCTEXT_NAMESPACE
		m_pSelectingPlayer->GetHUD()->GetEnemyHealthText()->SetText(EnemyHealth);
	}
}

void ABase_Enemy::_OnDeath(AActor* _pKiller)
{
	StopAnimMontage();
	m_bIsDead = true;

	GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->StopMovementImmediately();

	Cast<ASkillCharacter>(_pKiller)->ModifyStat(EStats::Exp, m_ExpForKill, true);

	m_pWidget->GetUserWidgetObject()->SetVisibility(ESlateVisibility::Hidden);

	if (m_hFollowTargetDistanceTimer.IsValid())
	{
		UKismetSystemLibrary::K2_ClearAndInvalidateTimerHandle(this, m_hFollowTargetDistanceTimer);
	}

	if (m_bIsSelected)
	{
		OnSelectionEnd(m_pSelectingPlayer);
	}

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	FTimerHandle hTimer00;
	GetWorldTimerManager().SetTimer(hTimer00, [&] {
		if (m_bDoesRespawn)
		{
			SetActorHiddenInGame(true);
			FTimerHandle hTimer01;
			GetWorldTimerManager().SetTimer(hTimer01, [&] {
				_OnRespawn();
			}, 1, false, m_RespawnDelay);
		}
		else
		{
			Destroy();
		}
	}, 1, false, m_DeadBodyTime);
}

// 액터를 새로 스폰하지 않고 기존에 있던 적을 재사용 함.
void ABase_Enemy::_OnRespawn()
{
	SetActorLocation(m_StartingLocation);

	m_bIsDead = false;
	m_bWasAggroed = false;
	m_bIsPatrolling = true;
	m_bIsRunningBack = false;
	m_CurrHealth = m_MaxHealth;
	m_pTargetActor = nullptr;

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	GetCharacterMovement()->MaxWalkSpeed = m_PatrolWalkSpeed;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);	

	if (m_pWidgetVisibleDomain->IsOverlappingActor(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		m_pWidget->GetUserWidgetObject()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		m_pWidget->GetUserWidgetObject()->SetVisibility(ESlateVisibility::Hidden);
	}

	SetActorHiddenInGame(false);

	_UpdateHealthBar();
	_Patrol();
}

void ABase_Enemy::_OnPerceptionUpdated(const TArray<AActor*>& _UpdatedActors)
{
	if (!m_bWasAggroed && m_bAggressive)
	{
		for (int i = 0; i < _UpdatedActors.Num(); ++i)
		{
			if (Cast<ASkillCharacter>(_UpdatedActors[i]))
			{
				OnAggroedPulled(_UpdatedActors[i]);
			}
		}
	}
}

void ABase_Enemy::_OnComponentBeginOverlap(
	UPrimitiveComponent* _pOverlappedComponent, 
	AActor* _pOtherActor, 
	UPrimitiveComponent* _pOtherComp, 
	int _OtherBodyIndex, 
	bool _bFromSweep, 
	const FHitResult& out_SweepResult)
{
	if (m_pWidget->GetUserWidgetObject())
	{
		if (!m_pWidget->GetUserWidgetObject()->IsVisible() && !m_bIsDead && Cast<ASkillCharacter>(_pOtherActor))
		{
			m_pWidget->GetUserWidgetObject()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

			_InitWidgetText();
			_UpdateHealthBar();
		}
	}
}

void ABase_Enemy::_OnComponentEndOverlap(
	UPrimitiveComponent* _pOverlappedComponent,
	AActor* _pOtherActor,
	UPrimitiveComponent* _pOtherComp,
	int32 _OtherBodyIndex)
{
	if (m_pWidget->GetUserWidgetObject())
	{		
		if (m_pWidget->GetUserWidgetObject()->IsVisible() && !m_bIsDead && Cast<ASkillCharacter>(_pOtherActor))
		{
			m_pWidget->GetUserWidgetObject()->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void ABase_Enemy::_DelayFunc()
{
	if (m_bIsPatrolling)
	{
		_Patrol();
	}
}

void ABase_Enemy::_FollowTargetDistance()
{
	// m_StartingLocation - GetActorLocation()의 거리(길이).
	float Distance = FVector::Distance(GetActorLocation(), m_StartingLocation);
	//float Distance = FVector::Distance(m_StartingLocation, GetActorLocation());

	if (Distance > m_FollowPlayerRadius)
	{
		OnReset();
	}
}

