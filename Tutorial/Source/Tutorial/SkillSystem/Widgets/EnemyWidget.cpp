// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyWidget.h"

#include <WidgetTree.h>


bool UEnemyWidget::Initialize()
{
	bool Result = Super::Initialize();

	m_pLevelNameText = WidgetTree->FindWidget<UTextBlock>(TEXT("LevelNameText"));
	m_pHealthBar = WidgetTree->FindWidget<UProgressBar>(TEXT("HealthBar"));

	return Result;
}

