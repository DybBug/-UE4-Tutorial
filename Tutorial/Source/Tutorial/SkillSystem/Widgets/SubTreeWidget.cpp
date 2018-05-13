// Fill out your copyright notice in the Description page of Project Settings.

#include "SubTreeWidget.h"
#include "SkillTreeEntryWidget.h"
#include "ConnectionWidget.h"
#include "SkillTreeWidget.h"

#include <WidgetTree.h>
#include <Components/CanvasPanel.h>
#include <Components/CanvasPanelSlot.h>


void USubTreeWidget::NativeConstruct()
{
	Super::NativeConstruct();
	m_pCanvas = WidgetTree->FindWidget<UCanvasPanel>("Canvas");
}

void USubTreeWidget::Initialize(const FSubTreeContent& _NewContent, USkillTreeWidget* _pSkillTreeWidget)
{
	m_Content = _NewContent;
	m_pSkillTreeWidget = _pSkillTreeWidget;

	GenerateContent();
}

void USubTreeWidget::GenerateContent()
{
	m_pCanvas->ClearChildren();
	m_pSkillTreeEntryWidgets.Empty();
	m_pConnectionWidgets.Empty();

	// 胶懦飘府浚飘府 困连 积己.
	for (int i = 0; i < m_Content.Skills.Num(); ++i)
	{
		USkillTreeEntryWidget* pEntryWidget =  CreateWidget<USkillTreeEntryWidget>(GetWorld(), USkillTreeEntryWidget::StaticClass());
		pEntryWidget->Initialize(m_Content.Skills[i].Spell, this);
		m_pSkillTreeEntryWidgets.Add(pEntryWidget);

		UCanvasPanelSlot* pSlot =  m_pCanvas->AddChildToCanvas(pEntryWidget);
		pSlot->SetPosition(m_Content.Skills[i].Position);
		pSlot->SetZOrder(2);
		pSlot->SetAutoSize(true);
	}

	// 牧池记困连 积己.
	for (int i = 0; i < m_Content.Connections.Num(); ++i)
	{
		UConnectionWidget* pConnectionWidget = CreateWidget<UConnectionWidget>(GetWorld(), UConnectionWidget::StaticClass());
		pConnectionWidget->Initialize(m_Content.Connections[i].ForSkillClass);
		m_pConnectionWidgets.Add(pConnectionWidget);
	
		UCanvasPanelSlot* pSlot = m_pCanvas->AddChildToCanvas(pConnectionWidget);
		pSlot->SetPosition(m_Content.Connections[i].Position);
		pSlot->SetSize(m_Content.Connections[i].Size);
		pConnectionWidget->SetRenderTransform(m_Content.Connections[i].Transform);
	}
}
