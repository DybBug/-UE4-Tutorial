// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyWidget.h"

#include <WidgetTree.h>


void UEnemyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	m_pLevelNameText = WidgetTree->FindWidget<UTextBlock>(TEXT("LevelNameText"));
	m_pHealthBar = WidgetTree->FindWidget<UProgressBar>(TEXT("HealthBar"));
}

