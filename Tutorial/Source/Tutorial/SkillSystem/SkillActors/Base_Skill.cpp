// Fill out your copyright notice in the Description page of Project Settings.

#include "Base_Skill.h"
#include "../Characters/SkillCharacter.h"
#include "../Widgets/HotkeyWidget.h"

#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Engine.h"


// Sets default values
ABase_Skill::ABase_Skill()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
	FString String = FString::Printf(TEXT("Casting : %s"), m_SkillInfo.Name);
	GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Green, String);
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

	}
}

void ABase_Skill::OnCooldownExpired()
{
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

