// Fill out your copyright notice in the Description page of Project Settings.

#include "BuffWidget.h"
#include <WidgetTree.h>


void UBuffWidget::NativeConstruct()
{
	Super::NativeConstruct();

	m_pBuffIcon = WidgetTree->FindWidget<UImage>("BuffIcon");
	m_pDurationText = WidgetTree->FindWidget<UTextBlock>("DurationText");
	m_pDurationMat = WidgetTree->FindWidget<UImage>("DurationMat");

	m_pBuffIcon->SetBrushFromTexture(m_pIcon);
	m_pDynamicDurationMat = m_pDurationMat->GetDynamicMaterial();
}

/* Set */
void UBuffWidget::SetIcon(UTexture2D * _pIcon)
{
	m_pIcon = _pIcon;
}
