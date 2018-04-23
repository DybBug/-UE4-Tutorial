// Fill out your copyright notice in the Description page of Project Settings.

#include "SkillDragDropOperation.h"
#include "../Widgets/SkillHotkeyWidget.h"
#include "../SkillActors/Base_Skill.h"

void USkillDragDropOperation::SetFromHotkey(USkillHotkeyWidget * _pFromHotkey)
{
	m_pFromHotkey = _pFromHotkey;
}

void USkillDragDropOperation::SetSkillActor(ABase_Skill * _pSkillActor)
{
	m_pSkillActor = _pSkillActor;
}
