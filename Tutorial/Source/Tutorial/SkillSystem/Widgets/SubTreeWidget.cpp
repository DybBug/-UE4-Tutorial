// Fill out your copyright notice in the Description page of Project Settings.

#include "SubTreeWidget.h"
#include "SkillTreeEntryWidget.h"
#include "ConnectionWidget.h"
#include "SkillTreeWidget.h"

#include <WidgetTree.h>
#include <Components/CanvasPanel.h>
#include <Components/CanvasPanelSlot.h>


bool USubTreeWidget::Initialize()
{
	bool Result = Super::Initialize();
	m_pCanvas = WidgetTree->FindWidget<UCanvasPanel>("Canvas");

	return Result;
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
	UClass* SkillEntryWidgetClass = LoadClass<USkillTreeEntryWidget>(nullptr, TEXT("WidgetBlueprint'/Game/TutorialContent/SkillSystem/Widgets/WBP_SkillTreeEntry.WBP_SkillTreeEntry_C'"));
	for (int i = 0; i < m_Content.Skills.Num(); ++i)
	{
		USkillTreeEntryWidget* pEntryWidget =  CreateWidget<USkillTreeEntryWidget>(GetWorld(), SkillEntryWidgetClass);
		pEntryWidget->Initialize(m_Content.Skills[i].Spell, this);
		m_pSkillTreeEntryWidgets.Add(pEntryWidget);

		UCanvasPanelSlot* pSlot =  m_pCanvas->AddChildToCanvas(pEntryWidget);
		pSlot->SetPosition(m_Content.Skills[i].Position);
		pSlot->SetZOrder(2);
		pSlot->SetAutoSize(true);
	}

	// 牧池记困连 积己.
	UClass* ConnectionWidgetClass = LoadClass<UConnectionWidget>(nullptr, TEXT("WidgetBlueprint'/Game/TutorialContent/SkillSystem/Widgets/WBP_Connection.WBP_Connection_C'"));
	for (int i = 0; i < m_Content.Connections.Num(); ++i)
	{
		UConnectionWidget* pConnectionWidget = CreateWidget<UConnectionWidget>(GetWorld(), ConnectionWidgetClass);
		pConnectionWidget->Initialize(m_Content.Connections[i].ForSkillClass);
		m_pConnectionWidgets.Add(pConnectionWidget);
	
		UCanvasPanelSlot* pSlot = m_pCanvas->AddChildToCanvas(pConnectionWidget);
		pSlot->SetPosition(m_Content.Connections[i].Position);
		pSlot->SetSize(m_Content.Connections[i].Size);
		pConnectionWidget->SetRenderTransform(m_Content.Connections[i].Transform);
	}
}
