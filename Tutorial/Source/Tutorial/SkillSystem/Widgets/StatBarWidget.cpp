// Fill out your copyright notice in the Description page of Project Settings.

#include "StatBarWidget.h"

#include "WidgetTree.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Materials/MaterialInstanceDynamic.h"

bool UStatBarWidget::Initialize()
{
	bool Result = Super::Initialize();
	
	m_pStatBar =     WidgetTree->FindWidget<UProgressBar>(FName("StatBar"));
	m_pStatLerpBar = WidgetTree->FindWidget<UImage>(FName("StatLerpBar"));
	m_pStatText =    WidgetTree->FindWidget<UTextBlock>(FName("StatText"));

	m_pDynamicMaterial = m_pStatLerpBar->GetDynamicMaterial();
	m_pDynamicMaterial->SetVectorParameterValue("Color", m_LerpColor);

	return Result;
}


void UStatBarWidget::SetPercent(float _Percent)
{
	m_Percent = _Percent;
}

void UStatBarWidget::SetFillColor(FLinearColor _Color)
{
	m_FillColor = _Color;
}

void UStatBarWidget::SetLerpColor(FLinearColor _Color)
{
	m_LerpColor = _Color;
}

void UStatBarWidget::SetDisplayedValue(float _Value)
{
	m_DisplayedValue = _Value;
}

void UStatBarWidget::SetMaxValue(int _Value)
{
	m_MaxValue = _Value;
}

void UStatBarWidget::SetIsGradiation(bool _Enable)
{
	m_bIsGradiation = _Enable;
}

void UStatBarWidget::SetDynamicMaterial(UMaterialInstanceDynamic * _pDynamicMaterial)
{
	m_pDynamicMaterial = _pDynamicMaterial;
}
