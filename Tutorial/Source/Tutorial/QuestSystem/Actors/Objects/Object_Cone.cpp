// Fill out your copyright notice in the Description page of Project Settings.

#include "Object_Cone.h"

#include <Components/StaticMeshComponent.h>
#include <Kismet/KismetMaterialLibrary.h>
#include <Materials/MaterialInstanceDynamic.h>
#include <Engine/World.h>


void AObject_Cone::BeginPlay()
{
	Super::BeginPlay();

	m_pMatInstance = m_pStaticMesh->CreateDynamicMaterialInstance(0);
}

void AObject_Cone::OnEnterPlayerRadius(AQuestCharacter * _pPlayer)
{
	Super::OnEnterPlayerRadius(_pPlayer);
	m_pMatInstance->SetVectorParameterValue("Color", FColor(0xffff151e));
}

void AObject_Cone::OnLeavePlayerRadius(AQuestCharacter * _pPlayer)
{
	Super::OnLeavePlayerRadius(_pPlayer);
	m_pMatInstance->SetVectorParameterValue("Color", FColor(0xff070707));

}

void AObject_Cone::OnInteractWith(AQuestCharacter * _pPlayer)
{
	Super::OnInteractWith(_pPlayer);
}

