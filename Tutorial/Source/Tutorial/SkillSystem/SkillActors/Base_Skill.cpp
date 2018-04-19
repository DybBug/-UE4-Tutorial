// Fill out your copyright notice in the Description page of Project Settings.

#include "Base_Skill.h"
#include "../Characters/SkillCharacter.h"
#include "../Widgets/SkillHotkeyWidget.h"

#include "Components/TimelineComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Engine.h"


// Sets default values
ABase_Skill::ABase_Skill()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_pCooldownTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("TimelineComp"));
	m_pCooldownTimeline->SetTimelineLengthMode(ETimelineLengthMode::TL_TimelineLength);
}

// Called when the game starts or when spawned
void ABase_Skill::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABase_Skill::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABase_Skill::OnTryCastSpell()
{
	if (!m_bOnCooldown && !m_bCurrentlyCasted)
	{
		ASkillCharacter* pPlayer = Cast<ASkillCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		if (!pPlayer->GetIsCasting())
		{
			InitializeSpellCast();
		}
	}
}

void ABase_Skill::InitializeSpellCast()
{
	ASkillCharacter* pPlayer = Cast<ASkillCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (pPlayer->GetStat(EStats::Mana).CurrValue >= GetCurrStage().ManaCost)
	{
		m_bCurrentlyCasted = true;
		pPlayer->BeginSpellCast(this);
		pPlayer->ModifyStat(EStats::Mana, -GetCurrStage().ManaCost, true);
		OnSpellCast();
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Red, TEXT("Nor enough mana left to cast the spell!!"));
	}
}

void ABase_Skill::OnSpellCast()
{	
	OnCastCompleted();
}

void ABase_Skill::OnCastCompleted()
{
	ASkillCharacter* pPlayer = Cast<ASkillCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	pPlayer->EndSpellCast(this);
	m_bOnCooldown = true;
	m_bCurrentlyCasted = false;

	GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Red, TEXT("Spell on CD!"));


	if (m_pHotkeyWidget)
	{
		m_pHotkeyWidget->GetSkillButton_Widget()->SetIsEnabled(false);
		m_pHotkeyWidget->GetSkillIcon_Widget()->SetColorAndOpacity(FLinearColor(FColor(0x545454FF)));
		m_pHotkeyWidget->GetDynamicMaterial()->SetScalarParameterValue("Percent", 1.0f);
		m_pHotkeyWidget->GetColldownImage_Widget()->SetVisibility(ESlateVisibility::HitTestInvisible);	
		m_pHotkeyWidget->GetCooldownText_Widget()->SetText(FText::FromString(FString::Printf(TEXT("%d"), &GetCurrStage().Cooldown)));
		m_pHotkeyWidget->GetCooldownText_Widget()->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	m_pCooldownTimeline->SetTimelineLength(GetCurrStage().Cooldown);
	m_pCooldownTimeline->PlayFromStart();

	FOnTimelineEvent Event;
	Event.BindUFunction(this, "_OnTimelineUpdate");
	m_pCooldownTimeline->SetTimelinePostUpdateFunc(Event);
	Event.BindUFunction(this, "_OnTimelineFinished");
	m_pCooldownTimeline->SetTimelineFinishedFunc(Event);

}

void ABase_Skill::OnCooldownExpired()
{
	m_bOnCooldown = false;

	if (m_pHotkeyWidget)
	{
		if (!m_pHotkeyWidget->GetDeactivated())
		{
			m_pHotkeyWidget->GetSkillButton_Widget()->SetIsEnabled(true);
			m_pHotkeyWidget->GetSkillIcon_Widget()->SetColorAndOpacity(FLinearColor(FColor(0xFFFFFFFF)));
		}
		
		m_pHotkeyWidget->GetColldownImage_Widget()->SetVisibility(ESlateVisibility::Hidden);
		m_pHotkeyWidget->GetCooldownText_Widget()->SetVisibility(ESlateVisibility::Hidden);
		
	}
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

void ABase_Skill::_OnTimelineUpdate()
{
	if (m_pHotkeyWidget)
	{
		float Percent = m_pCooldownTimeline->GetPlaybackPosition() / m_pCooldownTimeline->GetTimelineLength();
		m_pHotkeyWidget->GetDynamicMaterial()->SetScalarParameterValue("Percent", 1.0f - Percent);

		float Cooldown = m_pCooldownTimeline->GetTimelineLength() - m_pCooldownTimeline->GetPlaybackPosition();
		
		m_pHotkeyWidget->GetCooldownText_Widget()->SetText(FText::FromString(FString::Printf(TEXT("%d"), &Cooldown)));
	}

}

void ABase_Skill::_OnTimelineFinished()
{
	OnCooldownExpired();
}

