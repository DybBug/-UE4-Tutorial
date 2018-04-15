// Fill out your copyright notice in the Description page of Project Settings.

#include "SkillHotkeyWidget.h"

void USkillHotkeyWidget::SetDynamicMaterial(UMaterialInstanceDynamic* _pMaterial)
{
	m_pDynamicMaterial = _pMaterial;
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
