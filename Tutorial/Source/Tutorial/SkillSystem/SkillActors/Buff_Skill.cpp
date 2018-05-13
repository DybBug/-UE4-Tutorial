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

	// �̹� ������ ����Ǿ��ִ°�? / ��.
	if (bHasBuff)
	{
		// ���� ����.
		ResetBuff();
	}	
	else
	{
		// ���� ����.
		OnActivated();
	}
}

void ABuff_Skill::ApplyEffect()
{
	//
	// ���۰��� ��ų���� �� Ŭ������ ��ӹ޴´�.
	// ������ ȿ���� ���⿡�ٰ� ������ �Ѵ�.
	// ������ ���۽�ų�� �ϳ��̱� ������ ���⿡�ٰ� ������.
	//
	GetWorldTimerManager().SetTimer(m_hBuffTimer, [&] {
		// ���� ���.
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
	// ���� ���� ������Ʈ.
	//
	m_pBuffWidget->GetDurationText()->SetVisibility(ESlateVisibility::Visible);
	m_pBuffWidget->GetDurationMat()->SetVisibility(ESlateVisibility::Visible);

	//
	// DurationTimeline ����.
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
	// ���� ���� ������Ʈ.
	//
	m_pBuffWidget->GetDurationText()->SetVisibility(ESlateVisibility::Visible);
	m_pBuffWidget->GetDurationMat()->SetVisibility(ESlateVisibility::Visible);

	//
	// DurationTimeline ����.
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

	// ���� �ð�.
	float RemainingTime = m_DurationTime - m_PassedTime;

	// �����ð� ����.
	float Percent = m_PassedTime / m_DurationTime;

	//
	// ���� ������Ʈ.
	//
	m_pBuffWidget->GetDurationText()->SetText(UKismetTextLibrary::Conv_FloatToText(RemainingTime, ERoundingMode::HalfToZero, false, false, 1, 324, 0, 0));
	//m_pBuffWidget->GetDurationText()->SetText(UKismetTextLibrary::Conv_FloatToText(RemainingTime, ERoundingMode::HalfToZero));

	m_pBuffWidget->GetDynamicDurationMat()->SetScalarParameterValue("Percent", Percent);
}

