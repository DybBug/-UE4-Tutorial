// Fill out your copyright notice in the Description page of Project Settings.

#include "ConnectionWidget.h"

#include <WidgetTree.h>
#include <Components/Image.h>


bool UConnectionWidget::Initialize()
{
	bool Result = Super::Initialize();

	m_pLine = WidgetTree->FindWidget<UImage>("Line");

	return Result;
}

void UConnectionWidget::Initialize(const TSubclassOf<ABase_Skill>& _ForSpellClass)
{
	m_ForSpellClass = _ForSpellClass;
}

void UConnectionWidget::Active()
{
	if (m_bIsActive)
	{
		return;
	}

	m_bIsActive = true;
	m_pLine->SetOpacity(0.7f);
}
