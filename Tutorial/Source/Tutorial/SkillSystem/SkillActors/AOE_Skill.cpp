// Fill out your copyright notice in the Description page of Project Settings.

#include "AOE_Skill.h"
#include "../Interfaces/Damageable_Interface.h"
#include "../Characters/SkillCharacter.h"
#include "../Enemies/Base_Enemy.h"

#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Engine/World.h>
#include <Engine.h>



AAOE_Skill::AAOE_Skill()
{
	m_SkillInfo.Target = ETargetTypes::Area_around_self;

	FSkillStage Stage;
	Stage.AreaRadius = 500.f;
	m_SkillInfo.Stages.Add(Stage);
}

void AAOE_Skill::OnSkillNotify()
{
	ACharacter* pPlayer = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	FVector Location = pPlayer->GetActorLocation();

	// 이펙트 스폰.
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), m_pParticleEffect, Location);
	
	/* SphereTrace 생성. */
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypeQueries;
	ObjectTypeQueries.Add(EOT_Enemy);

	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(pPlayer);

	TArray<FHitResult> HitResults;

	UKismetSystemLibrary::SphereTraceMultiForObjects(
		GetWorld(),
		Location, 
		Location,
		GetCurrStage().AreaRadius,
		ObjectTypeQueries,
		true, 
		IgnoreActors,
		EDrawDebugTrace::Persistent,
		HitResults, 
		true);

	for (int i = 0; i < HitResults.Num(); ++i)
	{
		if (UKismetSystemLibrary::DoesImplementInterface(HitResults[i].GetActor(), UDamageable_Interface::StaticClass()))
		{			
			FSkillStage CurrStage = GetCurrStage();
			ABase_Enemy* pEnemy = Cast<ABase_Enemy>(HitResults[i].Actor);
		
			if (pEnemy)
			{
				pEnemy->OnReceiveDamage(CurrStage.Damage, CurrStage.DamageType, m_SkillInfo.ElementClass, CurrStage.CriticalChance, pPlayer, this);
			}
		}
	}
}
