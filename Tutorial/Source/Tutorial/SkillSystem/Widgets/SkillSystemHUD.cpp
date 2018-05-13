// Fill out your copyright notice in the Description page of Project Settings.

#include "SkillSystemHUD.h"
#include "HotkeyRowWidget.h"
#include "SkillTreeWidget.h"

#include "WidgetTree.h"
#include "UObject/ConstructorHelpers.h"


void USkillSystemHUD::NativeConstruct()
{
	Super::NativeConstruct();

	m_pHealthBar = WidgetTree->FindWidget<UStatBarWidget>("WBP_HealthBar");
	m_pManaBar = WidgetTree->FindWidget<UStatBarWidget>("WBP_ManaBar");
	m_pExpBar = WidgetTree->FindWidget<UStatBarWidget>("WBP_ExpBar");
	m_pSkillTree = WidgetTree->FindWidget<USkillTreeWidget>("WBP_SkillTree");

	m_pHotkeyRowContainer = WidgetTree->FindWidget<UVerticalBox>("HotkeyRowContainer");

	m_pCastingOverlay = WidgetTree->FindWidget<UOverlay>("CastingOverlay");
	m_pCastingBar = WidgetTree->FindWidget<UProgressBar>("CastingBar");
	m_pCastedSpellText = WidgetTree->FindWidget<UTextBlock>("CastedSpellText");
	m_pCastTimeText = WidgetTree->FindWidget<UTextBlock>("CastTimeText");
	m_pEnemyBorder = WidgetTree->FindWidget<UBorder>("EnemyBorder");
	m_pEnemyNameText = WidgetTree->FindWidget<UTextBlock>("EnemyNameText");
	m_pEnemyHealthText = WidgetTree->FindWidget<UTextBlock>("EnemyHealthText");
	m_pEnemyHealthBar = WidgetTree->FindWidget<UProgressBar>("EnemyHealthBar");
	m_pBuffContainer = WidgetTree->FindWidget<UHorizontalBox>("BuffContainer");
	m_pLevelText = WidgetTree->FindWidget<UTextBlock>("LevelText");

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


