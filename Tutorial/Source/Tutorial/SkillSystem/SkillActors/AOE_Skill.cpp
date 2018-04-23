// Fill out your copyright notice in the Description page of Project Settings.

#include "AOE_Skill.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/World.h"
#include "Engine.h"



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
	ObjectTypeQueries.Add(TEnumAsByte<EObjectTypeQuery>(ECC_Pawn));

	TArray<AActor*> Actors;
	Actors.Add(pPlayer);

	TArray<FHitResult> HitResults;

	UKismetSystemLibrary::SphereTraceMultiForObjects(
		GetWorld(),
		Location, 
		Location,
		GetCurrStage().AreaRadius,
		ObjectTypeQueries, 
		true, 
		Actors, 
		EDrawDebugTrace::Persistent,
		HitResults, 
		true);

}
