// Fill out your copyright notice in the Description page of Project Settings.

#include "SkillSystemHUD.h"
#include "HotkeyRowWidget.h"
#include "StatBarWidget.h"

#include "WidgetTree.h"
#include "Components/VerticalBox.h"
#include "UObject/ConstructorHelpers.h"


void USkillSystemHUD::NativeConstruct()
{
	Super::NativeConstruct();

	m_pHealthBar = WidgetTree->FindWidget<UStatBarWidget>(FName("WBP_HealthBar"));
	m_pManaBar = WidgetTree->FindWidget<UStatBarWidget>(FName("WBP_ManaBar"));
	m_pExpBar = WidgetTree->FindWidget<UStatBarWidget>(FName("WBP_ExpBar"));
	m_pHotkeyRowContainer = WidgetTree->FindWidget<UVerticalBox>(FName("HotkeyRowContainer"));
	
	GenerateHotkeys(m_Hotkeys);
}

void USkillSystemHUD::GenerateHotkeys(const TArray<FKey>& _Key)
{
	TArray<FKey> LocHotkeys = _Key;
	TArray<FKey> LocModifiedHotkeys = _Key;

	if (LocHotkeys.Num() < (m_KeysPerRow - 1))
	{
		UClass* pHotkeyRowWidget = LoadClass<UHotkeyRowWidget>(nullptr, TEXT("WidgetBlueprint'/Game/TutorialContent/SkillSystem/Widgets/WBP_HotkeyRow.WBP_HotkeyRow_C'"));

		UHotkeyRowWidget* pWidget = CreateWidget<UHotkeyRowWidget>(GetWorld(), pHotkeyRowWidget);
		pWidget->SetHotkeys(LocHotkeys);
		m_pHotkeyRowContainer->AddChildToVerticalBox(pWidget);
		m_AllHotkeySlots.Append(pWidget->GenerateHotkeyRow());
	}
	else
	{
		LocModifiedHotkeys.SetNum(m_KeysPerRow);

		UClass* pHotkeyRowWidget = LoadClass<UHotkeyRowWidget>(nullptr, TEXT("WidgetBlueprint'/Game/TutorialContent/SkillSystem/Widgets/WBP_HotkeyRow.WBP_HotkeyRow_C'"));
		
		UHotkeyRowWidget* pWidget = CreateWidget<UHotkeyRowWidget>(GetWorld(), pHotkeyRowWidget);
		pWidget->SetHotkeys(LocModifiedHotkeys);
		m_pHotkeyRowContainer->AddChildToVerticalBox(pWidget);
		TArray<USkillHotkeyWidget*> pHotkeyWidgets = pWidget->GenerateHotkeyRow();
		m_AllHotkeySlots.Append(pHotkeyWidgets);

		for (int i = 0; i < LocModifiedHotkeys.Num(); ++i)
		{
			LocHotkeys.Remove(LocModifiedHotkeys[i]);
		}
		GenerateHotkeys(LocHotkeys);
	}
}


