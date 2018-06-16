// Fill out your copyright notice in the Description page of Project Settings.

#include "StatBarWidget.h"

#include "WidgetTree.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/KismetMathLibrary.h"




void UStatBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	m_pDynamicMaterial = m_pStatLerpBar->GetDynamicMaterial();
	m_pDynamicMaterial->SetVectorParameterValue("Color", m_LerpColor);

	m_pStatBar->SetPercent(m_Percent);
	m_pStatBar->SetFillColorAndOpacity(m_FillColor);
	m_pStatText->SetText(UpdateStatText());
}

void UStatBarWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	m_pStatBar->SetPercent(m_Percent);
	m_pStatBar->SetFillColorAndOpacity(UpdateStatBarColor());
	m_pStatText->SetText(UpdateStatText());
}

FText UStatBarWidget::UpdateStatText()
{
#define LOCTEXT_NAMESPACE "StatText"
	FText Format = FText::Format(LOCTEXT("StatText", "{0} / {1}"), m_DisplayedValue, m_MaxValue);
#undef LOCTEXT_NAMESPACE

	return Format;
}

FLinearColor UStatBarWidget::UpdateStatBarColor()
{
	if (m_bIsGradiation)
	{
		return UKismetMathLibrary::LinearColorLerpUsingHSV(FLinearColor::Red, m_FillColor, m_Percent);
	}
	else
	{
		return m_FillColor;
	}
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
