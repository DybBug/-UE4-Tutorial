// Fill out your copyright notice in the Description page of Project Settings.

#include "MiniMapWidget.h"

#include <WidgetTree.h>
#include <Components/Image.h>

void UMiniMapWidget::NativeConstruct()
{
	Super::NativeConstruct();

	m_pDirectionArrow = WidgetTree->FindWidget<UImage>(TEXT("DirectionArrow"));

}


