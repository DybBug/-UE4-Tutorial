// Fill out your copyright notice in the Description page of Project Settings.

#include "SkillHotkeyWidget.h"
#include "../SkillActors/Base_Skill.h"

#include "WidgetTree.h"
#include "Materials/MaterialInstanceDynamic.h "
#include "Kismet/KismetTextLibrary.h"

void USkillHotkeyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	m_pHotkeyName = WidgetTree->FindWidget<UTextBlock>(FName("HotkeyName"));
	m_pCooldownText = WidgetTree->FindWidget<UTextBlock>(FName("CooldownText"));
	m_pSkillButton = WidgetTree->FindWidget<UButton>(FName("SkillButton"));
	m_pSkillIcon = WidgetTree->FindWidget<UImage>(FName("SkillIcon"));
	m_pCooldownImage = WidgetTree->FindWidget<UImage>(FName("CooldownImage"));
	

	m_pHotkeyName->SetText(m_Hotkey.GetDisplayName());
	m_pDynamicMaterial = m_pCooldownImage->GetDynamicMaterial();
}

void USkillHotkeyWidget::SetDynamicMaterial(UMaterialInstanceDynamic* _pMaterial)
{
	m_pDynamicMaterial = _pMaterial;
}

void USkillHotkeyWidget::ClearAssignedSpell()
{
	m_bDeactivated = false;
	if (!m_pAssignedSpell->GetOnCooldown())
	{
		m_pSkillButton->SetIsEnabled(true);
		m_pSkillIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		m_pSkillIcon->SetColorAndOpacity(FLinearColor(FColor(0xFFFFFFFF)));
		m_pCooldownImage->SetVisibility(ESlateVisibility::Hidden);
	}
}

void USkillHotkeyWidget::EnableHotkey()
{
	m_bDeactivated = false;
	if (!m_pAssignedSpell->GetOnCooldown())
	{
		m_pSkillButton->SetIsEnabled(false);
		m_pSkillIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		m_pSkillIcon->SetColorAndOpacity(FLinearColor(FColor(0xFFFFFFFF)));
		m_pCooldownImage->SetVisibility(ESlateVisibility::Hidden);
	}
}

void USkillHotkeyWidget::DisableHotkey()
{
	m_bDeactivated = true;
	if (!m_pAssignedSpell->GetOnCooldown())
	{
		m_pDynamicMaterial->SetScalarParameterValue("Percent", 1.0f);
		m_pSkillButton->SetIsEnabled(false);
		m_pSkillIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		m_pCooldownImage->SetVisibility(ESlateVisibility::Visible);
	}
}

void USkillHotkeyWidget::AssignSpell(ABase_Skill * _Spell)
{
	m_pAssignedSpell = _Spell;

	m_pAssignedSpell->SetHotkeyWidget(this);
	m_pSkillButton->SetIsEnabled(true);
	m_pSkillIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	if (m_pAssignedSpell->GetCurrStage().pOverrideIcon)
	{
		m_pSkillIcon->SetBrushFromTexture(m_pAssignedSpell->GetCurrStage().pOverrideIcon);
	}
	else
	{
 		m_pSkillIcon->SetBrushFromTexture(m_pAssignedSpell->GetSkillInfo().pIcon);
	}	
	
}

void USkillHotkeyWidget::SetHotkey(FKey _Key)
{
	m_Hotkey = _Key;
}

void USkillHotkeyWidget::SetAssignedSpell(ABase_Skill * _pSpell)
{
	m_pAssignedSpell = _pSpell;
}

void USkillHotkeyWidget::SetDeactivated(bool _bBool)
{
	m_bDeactivated = _bBool;
}
