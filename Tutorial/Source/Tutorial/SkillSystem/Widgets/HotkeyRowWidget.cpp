// Fill out your copyright notice in the Description page of Project Settings.

#include "HotkeyRowWidget.h"
#include "SkillHotkeyWidget.h"

#include "WidgetTree.h"
#include "Components/HorizontalBox.h"

void UHotkeyRowWidget::NativeConstruct()
{
	Super::NativeConstruct();

	m_pBox = WidgetTree->FindWidget<UHorizontalBox>(FName("Box"));
}

TArray<USkillHotkeyWidget*> UHotkeyRowWidget::GenerateHotkeyRow()
{
	for (int j = 0; j < m_Hotkeys.Num(); ++j)
	{
		UClass* pWidget = LoadClass<USkillHotkeyWidget>(nullptr, TEXT("WidgetBlueprint'/Game/TutorialContent/SkillSystem/Widgets/WBP_SkillHotkey.WBP_SkillHotkey_C'"));
		USkillHotkeyWidget* pHotkeyWidget = CreateWidget<USkillHotkeyWidget>(GetWorld(), pWidget);
		pHotkeyWidget->SetHotkey(m_Hotkeys[j]);
		m_SkillHotkeyWidgets.Add(pHotkeyWidget);
		m_pBox->AddChildToHorizontalBox(pHotkeyWidget);
	}

	return m_SkillHotkeyWidgets;
}

void UHotkeyRowWidget::SetHotkeys(const TArray<FKey>& _Key)
{
	m_Hotkeys = _Key;
}
