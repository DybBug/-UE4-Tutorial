// Fill out your copyright notice in the Description page of Project Settings.

#include "Base_Skill.h"
#include "../Characters/SkillCharacter.h"
#include "../Widgets/SkillHotkeyWidget.h"

#include "Components/TimelineComponent.h"
#include "GameFrameWork/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Engine.h"


// Sets default values
ABase_Skill::ABase_Skill()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	m_pCastingTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("TimelineComp_Casting"));
	m_pCastingTimeline->SetTimelineLengthMode(ETimelineLengthMode::TL_TimelineLength);

	m_pCooldownTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("TimelineComp_Cooldown"));
	m_pCooldownTimeline->SetTimelineLengthMode(ETimelineLengthMode::TL_TimelineLength);
}

// Called when the game starts or when spawned
void ABase_Skill::BeginPlay()
{
	Super::BeginPlay();
	m_pPlayer = Cast<ASkillCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	
}

void ABase_Skill::OnSkillNotify()
{
	
}

void ABase_Skill::OnTryCastSpell()
{
	if (!m_bOnCooldown && !m_bCurrentlyCasted)
	{		
		if (!m_pPlayer->GetIsCasting())
		{
			InitializeSpellCast();
		}
	}
}

void ABase_Skill::InitializeSpellCast()
{
	
	if (m_pPlayer->GetStat(EStats::Mana).CurrValue >= GetCurrStage().ManaCost)
	{
		m_bCurrentlyCasted = true;
		m_pPlayer->BeginSpellCast(this);
		m_pPlayer->ModifyStat(EStats::Mana, -GetCurrStage().ManaCost, true);
		OnSpellCast();
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Red, TEXT("Nor enough mana left to cast the spell!!"));
	}
}

void ABase_Skill::OnSpellCast()
{	
	if (m_pSkillAnim)
	{
		float PlayRate = m_pSkillAnim->SequenceLength / GetCurrStage().CastingTime;
		PlayAnimation(m_pSkillAnim, PlayRate);
	
		m_pPlayer->GetHUD()->GetCastingOverlay()->SetVisibility(ESlateVisibility::HitTestInvisible);

#define LOCTEXT_NAMESPACE "ABase_Skill"
		FText SpellName = FText::Format(LOCTEXT("SpellName", "Casting {0}"), FText::FromName(m_SkillInfo.Name));
		m_pPlayer->GetHUD()->GetCastSpellText()->SetText(SpellName);
#undef LOCTEXT_NAMESPACE

		FOnTimelineEvent Event;
		Event.BindUFunction(this, "_OnCastingTimelineUpdate");
		m_pCastingTimeline->SetTimelinePostUpdateFunc(Event);

		Event.BindUFunction(this, "_OnCastingTimelineFinished");
		m_pCastingTimeline->SetTimelineFinishedFunc(Event);

		m_pCastingTimeline->SetTimelineLength(GetCurrStage().CastingTime);
		m_pCastingTimeline->PlayFromStart();
	}
}

void ABase_Skill::OnCastCompleted()
{
	m_pPlayer->EndSpellCast(this);
	m_bOnCooldown = true;
	m_bCurrentlyCasted = false;

	GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Red, TEXT("Spell on CD!"));


	if (m_pHotkeyWidget)
	{
		m_pHotkeyWidget->GetSkillButton()->SetIsEnabled(false);
		m_pHotkeyWidget->GetSkillIcon()->SetColorAndOpacity(FLinearColor(FColor(0x545454FF)));
		m_pHotkeyWidget->GetDynamicMaterial()->SetScalarParameterValue("Percent", 1.0f);
		m_pHotkeyWidget->GetColldownImage()->SetVisibility(ESlateVisibility::HitTestInvisible);	
		//m_pHotkeyWidget->GetCooldownText()->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int)GetCurrStage().Cooldown)));
		m_pHotkeyWidget->GetCooldownText()->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	m_pCooldownTimeline->SetTimelineLength(GetCurrStage().Cooldown);
	m_pCooldownTimeline->PlayFromStart();

	FOnTimelineEvent Event;
	Event.BindUFunction(this, "_OnCooldownTimelineUpdate");
	m_pCooldownTimeline->SetTimelinePostUpdateFunc(Event);
	Event.BindUFunction(this, "_OnCooldownTimelineFinished");
	m_pCooldownTimeline->SetTimelineFinishedFunc(Event);

}

void ABase_Skill::OnCooldownExpired()
{
	m_bOnCooldown = false;

	if (m_pHotkeyWidget)
	{
		if (!m_pHotkeyWidget->GetDeactivated())
		{
			m_pHotkeyWidget->GetSkillButton()->SetIsEnabled(true);
			m_pHotkeyWidget->GetSkillIcon()->SetColorAndOpacity(FLinearColor(FColor(0xFFFFFFFF)));
		}
		
		m_pHotkeyWidget->GetColldownImage()->SetVisibility(ESlateVisibility::Hidden);
		m_pHotkeyWidget->GetCooldownText()->SetVisibility(ESlateVisibility::Hidden);
		
	}
}

void ABase_Skill::PlayAnimation(UAnimMontage* _pAnimationMontage, float _InPlayRate)
{
	m_pPlayer->GetCharacterMovement()->DisableMovement();
	m_pPlayer->GetCharacterMovement()->StopMovementImmediately();
	m_pPlayer->PlayAnimMontage(_pAnimationMontage, _InPlayRate);
	
	FTimerHandle hTimer;
	float DelayTime = _pAnimationMontage->SequenceLength / _InPlayRate;
	GetWorldTimerManager().SetTimer(hTimer, this, &ABase_Skill::_OnCastCompleted, DelayTime);
}

void ABase_Skill::SetSkillInfo(const FSkillInfo& _SkillInfo)
{
	m_SkillInfo = _SkillInfo;
}

void ABase_Skill::SetHotkeyWidget(USkillHotkeyWidget * _pWidget)
{
	m_pHotkeyWidget = _pWidget;
}

void ABase_Skill::SetOnCooldown(bool _bBool)
{
	m_bOnCooldown = _bBool;
}

void ABase_Skill::SetCurrentlyCasted(bool _bBool)
{
	m_bCurrentlyCasted = _bBool;
}

void ABase_Skill::_OnCooldownTimelineUpdate()
{
	if (m_pHotkeyWidget)
	{
		float Percent = m_pCooldownTimeline->GetPlaybackPosition() / m_pCooldownTimeline->GetTimelineLength();
		m_pHotkeyWidget->GetDynamicMaterial()->SetScalarParameterValue("Percent", 1.0f - Percent);

		float Cooldown = m_pCooldownTimeline->GetTimelineLength() - m_pCooldownTimeline->GetPlaybackPosition();
		
		if (Cooldown > 1.0f)
		{
			m_pHotkeyWidget->GetCooldownText()->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int)Cooldown)));
		}
		else
		{
			m_pHotkeyWidget->GetCooldownText()->SetText(FText::FromString(FString::Printf(TEXT("%.2f"), Cooldown)));
		}
	}

}

void ABase_Skill::_OnCooldownTimelineFinished()
{
	OnCooldownExpired();
}

void ABase_Skill::_OnCastingTimelineUpdate()
{
	float Percent = m_pCastingTimeline->GetPlaybackPosition() / m_pCastingTimeline->GetTimelineLength();
	m_pPlayer->GetHUD()->GetCastingBar()->SetPercent(Percent);
	m_pPlayer->GetHUD()->GetCastTimeText()->SetText(FText::FromString(FString::Printf(TEXT("%.1f / %.1f"),
		m_pCastingTimeline->GetPlaybackPosition(),
		m_pCastingTimeline->GetTimelineLength())));

}

void ABase_Skill::_OnCastingTimelineFinished()
{
	m_pPlayer->GetHUD()->GetCastingOverlay()->SetVisibility(ESlateVisibility::Hidden);
}

void ABase_Skill::_OnCastCompleted()
{
	m_pPlayer->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	OnCastCompleted();
}

