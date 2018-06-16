// Fill out your copyright notice in the Description page of Project Settings.

#include "BuffWidget.h"


void UBuffWidget::NativeConstruct()
{
	Super::NativeConstruct();

	m_pBuffIcon->SetBrushFromTexture(m_pIcon);
	m_pDynamicDurationMat = m_pDurationMat->GetDynamicMaterial();
}

/* Set */
void UBuffWidget::SetIcon(UTexture2D * _pIcon)
{
	m_pIcon = _pIcon;
}
