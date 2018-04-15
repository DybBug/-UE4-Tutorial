// Fill out your copyright notice in the Description page of Project Settings.

#include "SkillCharacter.h"
#include "../Widgets/SkillSystemHUD.h"
#include "../Widgets/SkillHotkeyWidget.h"
#include "../SkillActors/Base_Skill.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"

#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Blueprint/UserWidget.h"
#include "TimerManager.h"



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

	m_pHealthTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("TimelineComp_Health"));
	m_pHealthTimeline->SetTimelineLengthMode(ETimelineLengthMode::TL_TimelineLength);

	m_pManaTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("TimelineComp_Mana"));
	m_pManaTimeline->SetTimelineLengthMode(ETimelineLengthMode::TL_TimelineLength);

	m_pExpTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("TimelineComp_Exp"));
	m_pExpTimeline->SetTimelineLengthMode(ETimelineLengthMode::TL_TimelineLength);


	static ConstructorHelpers::FClassFinder<UUserWidget> WBP_HUD(TEXT("WidgetBlueprint'/Game/TutorialContent/SkillSystem/Widgets/WBP_HUD.WBP_HUD_C'"));
	if (WBP_HUD.Succeeded())
	{
		m_HUDClass = WBP_HUD.Class;
	}

	m_Stats.Add(EStats::Health);
	m_Stats[EStats::Health].MinValue = 0;
	m_Stats[EStats::Health].MaxValue = 300;
	m_Stats[EStats::Health].CurrValue = 300;
	m_Stats[EStats::Health].DisplayedValue = 300.f;
	m_Stats[EStats::Health].MinLerpTime = 0.2f;
	m_Stats[EStats::Health].MaxLerpTime = 1.5f;
	m_Stats[EStats::Health].bHasRegeneration = true;
	m_Stats[EStats::Health].TimeToRegMaxValue = 60.f;
	m_Stats[EStats::Health].RegInterval = 0.5f;

	m_Stats.Add(EStats::Mana);
	m_Stats[EStats::Mana].MinValue = 0;
	m_Stats[EStats::Mana].MaxValue = 200;
	m_Stats[EStats::Mana].CurrValue = 200;
	m_Stats[EStats::Mana].DisplayedValue = 200.f;
	m_Stats[EStats::Mana].MinLerpTime = 0.5f;
	m_Stats[EStats::Mana].MaxLerpTime = 2.5f;
	m_Stats[EStats::Mana].bHasRegeneration = true;
	m_Stats[EStats::Mana].TimeToRegMaxValue = 45.f;
	m_Stats[EStats::Mana].RegInterval = 0.4f;

	m_Stats.Add(EStats::Exp);
	m_Stats[EStats::Exp].MinValue = 0;
	m_Stats[EStats::Exp].MaxValue = 300;
	m_Stats[EStats::Exp].CurrValue = 0;
	m_Stats[EStats::Exp].DisplayedValue = 0.f;
	m_Stats[EStats::Exp].MinLerpTime = 0.4;
	m_Stats[EStats::Exp].MaxLerpTime = 2.0f;


}

// Called when the game starts or when spawned
void ASkillCharacter::BeginPlay()
{
	Super::BeginPlay();	

	if (m_HUDClass)
	{
		m_pHUD = CreateWidget<USkillSystemHUD>(GWorld, m_HUDClass);
		m_pHUD->AddToViewport();

		_SetupStatBars();

		_SetupRegenerations();

		_UpdateStat(EStats::Health);
		_UpdateStat(EStats::Mana);
		_UpdateStat(EStats::Exp);

		GenerateStartingSkills();
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

	PlayerInputComponent->BindKey(EKeys::Equals, IE_Pressed, this, &ASkillCharacter::_Increase);
	PlayerInputComponent->BindKey(EKeys::Hyphen, IE_Pressed, this, &ASkillCharacter::_Decrease);

	PlayerInputComponent->BindKey(EKeys::AnyKey, IE_Pressed, this, &ASkillCharacter::_AnyKey);

}

void ASkillCharacter::ModifyStat(EStats _Stat, int _Value, bool _bIsAnimated)
{
	FStatData& StatRef = m_Stats[_Stat];

	if (_bIsAnimated)
	{
		StatRef.CurrValue = UKismetMathLibrary::Clamp((StatRef.CurrValue + _Value), StatRef.MinValue, StatRef.MaxValue);

		float LerpAlpha = UKismetMathLibrary::Abs(UKismetMathLibrary::Clamp(_Value, -StatRef.CurrValue, StatRef.MaxValue - StatRef.CurrValue)) / StatRef.MaxValue;
		float LerpTime = UKismetMathLibrary::Lerp(StatRef.MinLerpTime, StatRef.MaxLerpTime, LerpAlpha);
		_LerpStatDisplay(_Stat, LerpTime, _Value > 0);

		_HandleRegeneration(_Stat);
	}

	else
	{
		StatRef.DisplayedValue = UKismetMathLibrary::Clamp((StatRef.DisplayedValue + _Value), StatRef.MinValue, StatRef.MaxValue);
		StatRef.CurrValue = UKismetMathLibrary::Clamp((StatRef.CurrValue + _Value), StatRef.MinValue, StatRef.MaxValue);
		_HandleRegeneration(_Stat);
		_UpdateStat(_Stat);
	}
}

void ASkillCharacter::GenerateStartingSkills()
{
	for (int i = 0; i < m_StartingSkillClasses.Num(); ++i)
	{
		ABase_Skill* pSpell = GetWorld()->SpawnActor<ABase_Skill>(m_StartingSkillClasses[i]);

		for (int j = 0; j < m_pHUD->GetAllHotkeySlots().Num(); ++j)
		{
			USkillHotkeyWidget* pSkillHotkeyWidget = m_pHUD->GetAllHotkeySlots()[j];
			if (!pSkillHotkeyWidget->GetAssignedSpell())
			{
				pSkillHotkeyWidget->AssignSpell(pSpell);
				break;
			}
		}
	}
}

void ASkillCharacter::BeginSpellCast(ABase_Skill * _pCastedSkill)
{
	m_bIsCasting = true;
	m_pCurrentSpell = _pCastedSkill;

	for (int i = 0; i < m_pHUD->GetAllHotkeySlots().Num(); ++i)
	{
		USkillHotkeyWidget* pSkillHotkeyWidget = m_pHUD->GetAllHotkeySlots()[i];
		if (pSkillHotkeyWidget->GetAssignedSpell())
		{
			if (pSkillHotkeyWidget->GetAssignedSpell() != _pCastedSkill)
			{
				pSkillHotkeyWidget->DisableHotkey();
			}
		}
	}
}

void ASkillCharacter::EndSpellCast(ABase_Skill * _pCastedSkill)
{
	m_bIsCasting = false;
	m_pCurrentSpell = nullptr;

	for (int i = 0; i < m_pHUD->GetAllHotkeySlots().Num(); ++i)
	{
		USkillHotkeyWidget* pSkillHotkeyWidget = m_pHUD->GetAllHotkeySlots()[i];
		if (pSkillHotkeyWidget->GetAssignedSpell())
		{
			if (pSkillHotkeyWidget->GetAssignedSpell() == _pCastedSkill)
			{
				pSkillHotkeyWidget->EnableHotkey();
			}
		}
	}
}

void ASkillCharacter::_SetupStatBars()
{
	m_Stats[EStats::Health].pBarWidget = m_pHUD->GetHealthBar();
	m_Stats[EStats::Mana].pBarWidget = m_pHUD->GetManaBar();
	m_Stats[EStats::Exp].pBarWidget = m_pHUD->GetExpBar();
}

void ASkillCharacter::_UpdateStat(EStats _Stat)
{
	FStatData Stat = m_Stats[_Stat];

	Stat.pBarWidget->SetDisplayedValue(Stat.DisplayedValue);
	Stat.pBarWidget->SetMaxValue(Stat.MaxValue);
	Stat.pBarWidget->SetPercent(FMath::Clamp(((float)Stat.DisplayedValue / (float)Stat.MaxValue), 0.f, 1.f));

	_HandleRegeneration(_Stat);
}

void ASkillCharacter::_SetupRegenerations()
{
	/* 체력 */
	GetWorldTimerManager().SetTimer(
		m_Stats[EStats::Health].hRegenerationTimer,
		this,
		&ASkillCharacter::_HealthRegenTick,
		m_Stats[EStats::Health].RegInterval, 
		true);

	_HandleRegeneration(EStats::Health);

	/* 마나 */
	GetWorldTimerManager().SetTimer(
		m_Stats[EStats::Mana].hRegenerationTimer,
		this,
		&ASkillCharacter::_ManaRegenTick,
		m_Stats[EStats::Mana].RegInterval,
		true);

	_HandleRegeneration(EStats::Mana);

}

void ASkillCharacter::_HealthRegenTick()
{
	FStatData HealthStat = m_Stats[EStats::Health];
	int Value = FMath::TruncToInt((HealthStat.RegInterval / HealthStat.TimeToRegMaxValue) * HealthStat.MaxValue);
	ModifyStat(EStats::Health, Value);

}

void ASkillCharacter::_ManaRegenTick()
{
	FStatData ManaStat = m_Stats[EStats::Mana];
	int Value = FMath::TruncToInt((ManaStat.RegInterval / ManaStat.TimeToRegMaxValue) * ManaStat.MaxValue);
	ModifyStat(EStats::Mana, Value);

}

void ASkillCharacter::_HandleRegeneration(EStats _Stat)
{
	FStatData Stat = m_Stats[_Stat];

	if (GetWorldTimerManager().IsTimerPaused(Stat.hRegenerationTimer) && 
		(Stat.MaxValue != Stat.CurrValue))
	{
		GetWorldTimerManager().UnPauseTimer(Stat.hRegenerationTimer);
	}
	else if (!GetWorldTimerManager().IsTimerPaused(Stat.hRegenerationTimer) &&
		(Stat.MaxValue == Stat.CurrValue))
	{
		GetWorldTimerManager().PauseTimer(Stat.hRegenerationTimer);
	}
}

void ASkillCharacter::_OnStatLerpStart(EStats _Stat)
{
	m_Stats[_Stat].bCurrentlyAnimated = true;
}

void ASkillCharacter::_OnHealthStatLerpEnd()
{
	m_Stats[EStats::Health].bCurrentlyAnimated = false;
	m_Stats[EStats::Health].pBarWidget->GetWidgetFromName("StatLerpBar")->SetVisibility(ESlateVisibility::Hidden);
}

void ASkillCharacter::_OnManaStatLerpEnd()
{
	m_Stats[EStats::Mana].bCurrentlyAnimated = false;
	m_Stats[EStats::Mana].pBarWidget->GetWidgetFromName("StatLerpBar")->SetVisibility(ESlateVisibility::Hidden);
}

void ASkillCharacter::_OnExpStatLerpEnd()
{
	m_Stats[EStats::Exp].bCurrentlyAnimated = false;
	m_Stats[EStats::Exp].pBarWidget->GetWidgetFromName("StatLerpBar")->SetVisibility(ESlateVisibility::Hidden);
}


void ASkillCharacter::_LerpStatDisplay(EStats _Stat, float _LerpTime, bool _bInPositive)
{
	_OnStatLerpStart(_Stat);

	FStatLerp Data;
	Data.OriginalValue = m_Stats[_Stat].DisplayedValue;
	Data.ValueToLerpTo = m_Stats[_Stat].CurrValue;
	Data.bPositive = _bInPositive;
	m_StatLerpData.Add(_Stat, Data);

	FName Name = _bInPositive ? "PercentRight" : "PercentLeft";
	float Percent = (float)m_Stats[_Stat].CurrValue / (float)m_Stats[_Stat].MaxValue;

	m_Stats[_Stat].pBarWidget->GetDynamicMaterial()->SetScalarParameterValue(Name, Percent);
	m_Stats[_Stat].pBarWidget->GetWidgetFromName("StatLerpBar")->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	if (_Stat == EStats::Health)
	{
		/* 타임라인 Update 함수 설정*/
		FOnTimelineEvent Event;
		Event.BindUFunction(this, "_HealthStatLerpTick");
		m_pHealthTimeline->SetTimelinePostUpdateFunc(Event);

		/* 타임라인 finish 이벤트 설정.*/
		Event.BindUFunction(this, "_OnHealthStatLerpEnd");
		
		m_pHealthTimeline->SetTimelineFinishedFunc(Event);

		/* 타임라인 재생 */
		m_pHealthTimeline->SetTimelineLength(_LerpTime);
		m_pHealthTimeline->PlayFromStart();
	}
	else if (_Stat == EStats::Mana)
	{
		/* 타임라인 Update 함수 설정*/
		FOnTimelineEvent Event;
		Event.BindUFunction(this, "_ManaStatLerpTick");
		m_pManaTimeline->SetTimelinePostUpdateFunc(Event);

		/* 타임라인 finish 이벤트 설정.*/
		Event.BindUFunction(this, "_OnManaStatLerpEnd");
		m_pManaTimeline->SetTimelineFinishedFunc(Event);

		/* 타임라인 재생 */
		m_pManaTimeline->SetTimelineLength(_LerpTime);
		m_pManaTimeline->PlayFromStart();
	}
	else
	{
		/* 타임라인 Update 함수 설정*/
		FOnTimelineEvent Event;
		Event.BindUFunction(this, "_ExpStatLerpTick");
		m_pExpTimeline->SetTimelinePostUpdateFunc(Event);

		/* 타임라인 finish 이벤트 설정.*/
		Event.BindUFunction(this, "_OnExpStatLerpEnd");
		m_pExpTimeline->SetTimelineFinishedFunc(Event);

		/* 타임라인 재생 */
		m_pExpTimeline->SetTimelineLength(_LerpTime);
		m_pExpTimeline->PlayFromStart();
	}

}

void ASkillCharacter::_HealthStatLerpTick()
{
	/* DisplayedValue 설정. */
	float TimelinePlaybackPosition = m_pHealthTimeline->GetPlaybackPosition();
	float TimelineLength = m_pHealthTimeline->GetTimelineLength();
	
	float Alpha = (TimelineLength > 0) ? TimelinePlaybackPosition / TimelineLength : 0.f;

	m_Stats[EStats::Health].DisplayedValue = 
		UKismetMathLibrary::Lerp(m_StatLerpData[EStats::Health].OriginalValue, m_StatLerpData[EStats::Health].ValueToLerpTo, Alpha);

	/* LerpBar의 속성 설정. */
	FName Name = m_StatLerpData[EStats::Health].bPositive ? "PercentLeft" : "PercentRight";
	float Value = m_Stats[EStats::Health].DisplayedValue / m_Stats[EStats::Health].MaxValue;
	m_Stats[EStats::Health].pBarWidget->GetDynamicMaterial()->SetScalarParameterValue(Name, Value);

	_UpdateStat(EStats::Health);
}

void ASkillCharacter::_ManaStatLerpTick()
{
	/* DisplayedValue 설정. */
	float Alpha = m_pManaTimeline->GetPlaybackPosition() / m_pManaTimeline->GetTimelineLength();

	m_Stats[EStats::Mana].DisplayedValue =
		UKismetMathLibrary::Lerp(m_StatLerpData[EStats::Mana].OriginalValue, m_StatLerpData[EStats::Mana].ValueToLerpTo, Alpha);

	/* LerpBar의 속성 설정. */
	FName Name = m_StatLerpData[EStats::Mana].bPositive ? "PercentLeft" : "PercentRight";
	float Value = m_Stats[EStats::Mana].DisplayedValue / m_Stats[EStats::Mana].MaxValue;
	m_Stats[EStats::Mana].pBarWidget->GetDynamicMaterial()->SetScalarParameterValue(Name, Value);

	_UpdateStat(EStats::Mana);
}

void ASkillCharacter::_ExpStatLerpTick()
{
	/* DisplayedValue 설정. */
	float Alpha = m_pExpTimeline->GetPlaybackPosition() / m_pExpTimeline->GetTimelineLength();

	m_Stats[EStats::Exp].DisplayedValue =
		UKismetMathLibrary::Lerp(m_StatLerpData[EStats::Exp].OriginalValue, m_StatLerpData[EStats::Exp].ValueToLerpTo, Alpha);

	/* LerpBar의 속성 설정. */
	FName Name = m_StatLerpData[EStats::Exp].bPositive ? "PercentLeft" : "PercentRight";
	float Value = m_Stats[EStats::Exp].DisplayedValue / m_Stats[EStats::Exp].MaxValue;
	m_Stats[EStats::Exp].pBarWidget->GetDynamicMaterial()->SetScalarParameterValue(Name, Value);

	_UpdateStat(EStats::Exp);
}

void ASkillCharacter::_Increase()
{
	ModifyStat(EStats::Health, 100, true);
}

void ASkillCharacter::_Decrease()
{
	ModifyStat(EStats::Health, -100, true);
}

void ASkillCharacter::_AnyKey()
{
	APlayerController* Controller = UGameplayStatics::GetPlayerController(GWorld, 0);

	for(int i = 0 ; i < m_pHUD->GetAllHotkeySlots().Num(); ++i)
	{
		USkillHotkeyWidget* pSkillHotkeyWidget = m_pHUD->GetAllHotkeySlots()[i];

		if (Controller->WasInputKeyJustPressed(pSkillHotkeyWidget->GetHotKey()) &&
			pSkillHotkeyWidget->GetAssignedSpell())
		{
			pSkillHotkeyWidget->GetAssignedSpell()->OnTryCastSpell();
			break;
		}
	}
}

