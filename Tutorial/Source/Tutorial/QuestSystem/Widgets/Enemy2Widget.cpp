// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy2Widget.h"

#include <WidgetTree.h>
#include <Components/ProgressBar.h>
#include <Components/TextBlock.h>

bool UEnemy2Widget::Initialize()
{
	bool Result = Super::Initialize();

	m_pHealthBar = WidgetTree->FindWidget<UProgressBar>(TEXT("HealthBar"));
	m_pNameText  = WidgetTree->FindWidget<UTextBlock>(TEXT("NameText"));
	m_pLevelText = WidgetTree->FindWidget<UTextBlock>(TEXT("LevelText"));

	return Result;
}

