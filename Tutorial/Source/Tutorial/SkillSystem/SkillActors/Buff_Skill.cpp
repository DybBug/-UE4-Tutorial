// Fill out your copyright notice in the Description page of Project Settings.

#include "Buff_Skill.h"
#include "../Characters/SkillCharacter.h"

#include <Components/TimelineComponent.h>
#include <Kismet/KismetTextLibrary.h>
#include <Materials/MaterialInstanceDynamic.h>
#include <TimerManager.h>


ABuff_Skill::ABuff_Skill()
{
	m_pDurationTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("TimelineComp"));
	m_pDurationTimeline->SetTimelineLengthMode(ETimelineLengthMode::TL_TimelineLength);
}

void ABuff_Skill::OnSkillNotify()
{
	ABase_Skill::OnSkillNotify();

	bool bHasBuff;
	ABuff_Skill* pBuff;

	m_pPlayer->HasBuff(this->GetClass(), bHasBuff, pBuff);

	// 이미 버프가 적용되어있는가? / 예.
	if (bHasBuff)
	{
		// 버프 리셋.
		ResetBuff();
	}	
	else
	{
		// 버프 적용.
		OnActivated();
	}
}

void ABuff_Skill::ApplyEffect()
{
	//
	// 버퍼관련 스킬들이 이 클래스를 상속받는다.
	// 버퍼의 효과는 여기에다가 재정의 한다.
	// 지금은 버퍼스킬이 하나이기 때문에 여기에다가 정의함.
	//
	GetWorldTimerManager().SetTimer(m_hBuffTimer, [&] {
		// 마나 재생.
		m_pPlayer->ModifyStat(EStats::Mana, 30, true);
	}, 2.f, true);
	
}

void ABuff_Skill::RemoveEffect()
{
	GetWorldTimerManager().ClearTimer(m_hBuffTimer);
}

void ABuff_Skill::ResetBuff()
{
	m_DurationTime = GetCurrStage().Duration;
	m_PassedTime = 0.f;

	//
	// 버퍼 위젯 업데이트.
	//
	m_pBuffWidget->GetDurationText()->SetVisibility(ESlateVisibility::Visible);
	m_pBuffWidget->GetDurationMat()->SetVisibility(ESlateVisibility::Visible);

	//
	// DurationTimeline 실행.
	//
	FOnTimelineEvent Event;
	Event.BindUFunction(this, "OnDeactivated");
	m_pDurationTimeline->SetTimelineFinishedFunc(Event);

	Event.BindUFunction(this, "_DurationTimelineUpdate");
	m_pDurationTimeline->SetTimelinePostUpdateFunc(Event);
	m_pDurationTimeline->SetTimelineLength(m_DurationTime);
	m_pDurationTimeline->PlayFromStart();
}

void ABuff_Skill::OnActivated()
{
	ApplyEffect();

	m_pBuffWidget = m_pPlayer->AddBuff(this);
	m_DurationTime = GetCurrStage().Duration;
	m_PassedTime = 0.f;

	//
	// 버퍼 위젯 업데이트.
	//
	m_pBuffWidget->GetDurationText()->SetVisibility(ESlateVisibility::Visible);
	m_pBuffWidget->GetDurationMat()->SetVisibility(ESlateVisibility::Visible);

	//
	// DurationTimeline 실행.
	//
	FOnTimelineEvent Event;
	Event.BindUFunction(this, "OnDeactivated");
	m_pDurationTimeline->SetTimelineFinishedFunc(Event);

	Event.BindUFunction(this, "_DurationTimelineUpdate");
	m_pDurationTimeline->SetTimelinePostUpdateFunc(Event);
	m_pDurationTimeline->SetTimelineLength(m_DurationTime);
	m_pDurationTimeline->PlayFromStart();


}

void ABuff_Skill::OnDeactivated()
{
	RemoveEffect();

	m_pPlayer->RemoveBuff(this);
	m_pBuffWidget = nullptr;
}

/* Set */
void ABuff_Skill::SetBuffIcon(UTexture2D * _pIcon)
{
	m_pBuffIcon = _pIcon;
}

void ABuff_Skill::SetBuffWidget(UBuffWidget * _pWidget)
{
	m_pBuffWidget = _pWidget;
}

void ABuff_Skill::_DurationTimelineUpdate()
{	
	m_PassedTime = m_pDurationTimeline->GetPlaybackPosition();

	// 남은 시간.
	float RemainingTime = m_DurationTime - m_PassedTime;

	// 남은시간 비율.
	float Percent = m_PassedTime / m_DurationTime;

	//
	// 위젯 업데이트.
	//
	m_pBuffWidget->GetDurationText()->SetText(UKismetTextLibrary::Conv_FloatToText(RemainingTime, ERoundingMode::HalfToZero, false, false, 1, 324, 0, 0));
	//m_pBuffWidget->GetDurationText()->SetText(UKismetTextLibrary::Conv_FloatToText(RemainingTime, ERoundingMode::HalfToZero));

	m_pBuffWidget->GetDynamicDurationMat()->SetScalarParameterValue("Percent", Percent);
}

