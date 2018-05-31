// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractionWidget.h"
#include <WidgetTree.h>


bool UInteractionWidget::Initialize()
{
	Super::Initialize();

	m_pName = WidgetTree->FindWidget<UTextBlock>(TEXT("Name"));

	return false;
}