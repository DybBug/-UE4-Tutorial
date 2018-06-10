// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractionWidget.h"
#include <WidgetTree.h>


bool UInteractionWidget::Initialize()
{
	Super::Initialize();

	m_pName         = WidgetTree->FindWidget<UTextBlock>(TEXT("Name"));
	m_pInteractText = WidgetTree->FindWidget<UTextBlock>(TEXT("InteractText"));

	return false;
}