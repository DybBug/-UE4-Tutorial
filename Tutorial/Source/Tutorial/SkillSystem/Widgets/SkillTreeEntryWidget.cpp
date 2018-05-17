// Fill out your copyright notice in the Description page of Project Settings.

#include "SkillTreeEntryWidget.h"
#include "../SkillActors/Base_Skill.h"
#include "../Widgets/SubTreeWidget.h"
#include "../Widgets/SkillDragWidget.h"
#include "../Components/SkillTreeComponent.h"
#include "../DragDropOperations/SkillDragDropOperation.h"

#include <WidgetTree.h>
#include <Components/TextBlock.h>
#include <Components/Image.h>
#include <Components/Button.h>
#include <Components/VerticalBox.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Blueprint/WidgetBlueprintLibrary.h>


bool USkillTreeEntryWidget::Initialize()
{
	bool Result = Super::Initialize();

	m_pStageText = WidgetTree->FindWidget<UTextBlock>("StageText");
	m_pSkillIcon = WidgetTree->FindWidget<UImage>("SkillIcon");
	m_pPlusButton = WidgetTree->FindWidget<UButton>("PlusButton");
	m_pMinusButton = WidgetTree->FindWidget<UButton>("MinusButton");
	m_pUpgradeBox = WidgetTree->FindWidget<UVerticalBox>("UpgradeBox");

	m_pPlusButton->OnClicked.AddDynamic(this, &USkillTreeEntryWidget::_OnPlusButtonClicked);
	m_pMinusButton->OnClicked.AddDynamic(this, &USkillTreeEntryWidget::_OnMinusButtonClicked);

	return Result;
}

FReply USkillTreeEntryWidget::NativeOnMouseButtonDown(const FGeometry & InGeometry, const FPointerEvent & InMouseEvent)
{
	FReply result = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (m_bSpellLearned)
	{
		FEventReply Event = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);

		return Event.NativeReply;
	}

	return result;
}

void USkillTreeEntryWidget::NativeOnDragDetected(const FGeometry & InGeometry, const FPointerEvent & InMouseEvent, UDragDropOperation *& OutOperation)
{
	USkillHotkeyWidget* pHotkeyWidget;
	bool bIsOnHotkey = m_pSubTreeWidget->GetSkillTreeWidget()->GetSkillTree()->SkillOnHotkey(m_pSkill, pHotkeyWidget);

	if (bIsOnHotkey)
	{
		return;
	}

	UClass* DragWidgetClass = LoadClass<USkillDragWidget>(nullptr, TEXT("WidgetBlueprint'/Game/TutorialContent/SkillSystem/Widgets/WBP_SkillDrag.WBP_SkillDrag_C'"));
	USkillDragWidget* pDragWidget = CreateWidget<USkillDragWidget>(GetWorld(), DragWidgetClass);
	UTexture2D* pIcon = m_pSkill->GetCurrStage().pOverrideIcon ? m_pSkill->GetCurrStage().pOverrideIcon : m_pSkill->GetSkillInfo().pIcon;
	pDragWidget->SetSkillTexture(pIcon);

	UDragDropOperation* pOper = UWidgetBlueprintLibrary::CreateDragDropOperation(USkillDragDropOperation::StaticClass());
	USkillDragDropOperation* pSkillOper = Cast<USkillDragDropOperation>(pOper);
	pSkillOper->DefaultDragVisual = pDragWidget;
	pSkillOper->Pivot = EDragPivot::MouseDown;	
	pSkillOper->SetSkillActor(m_pSkill);

	OutOperation = pOper;
}

void USkillTreeEntryWidget::Initialize(const TSubclassOf<ABase_Skill>& _SkillClass, USubTreeWidget* _pSubTreeWidget)
{
	m_SkillClass = _SkillClass;
	m_pSubTreeWidget = _pSubTreeWidget;

	if (UKismetSystemLibrary::IsValidClass(m_SkillClass))
	{
		m_pSkill = GetWorld()->SpawnActor<ABase_Skill>(m_SkillClass);

		UpdateStageText();
		UpdateIcon();
		UpdateUpgradeBox();
	}
}

int USkillTreeEntryWidget::AmountOfStages()
{
	return m_pSkill->GetSkillInfo().Stages.Num();
}

void USkillTreeEntryWidget::UpdateStageText()
{
#define LOCTEXT_NAMESPACE "StageText"
	FText StageText = FText::Format(LOCTEXT("StageText", "{0} / {1}"), m_pSkill->GetCurrStageIndex() + 1, AmountOfStages());
#undef LOCTEXT_NAMESPACE

	m_pStageText->SetText(StageText);
}

void USkillTreeEntryWidget::UpdateIcon()
{
	UTexture2D* pIcon;
	if (m_pSkill->GetCurrStageIndex() < 0)
	{
		pIcon = m_pSkill->GetSkillInfo().pIcon;
	}
	else
	{
		pIcon = (m_pSkill->GetCurrStage().pOverrideIcon) ? m_pSkill->GetCurrStage().pOverrideIcon : m_pSkill->GetSkillInfo().pIcon;
	}

	m_pSkillIcon->SetBrushFromTexture(pIcon);
}

void USkillTreeEntryWidget::UpdateUpgradeBox()
{
	if (m_pSubTreeWidget->GetSkillTreeWidget()->GetSkillTree()->CanUpgradeSpell(m_pSkill))
	{
		m_pPlusButton->SetVisibility(ESlateVisibility::Visible);
		m_pMinusButton->SetVisibility(ESlateVisibility::Hidden);
		m_pUpgradeBox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		if (m_pSkill->GetCurrStageIndex() >= 1)
		{
			m_pMinusButton->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			SetColorAndOpacity(FLinearColor::White);
		}
	}
	else
	{
		m_pPlusButton->SetVisibility(ESlateVisibility::Hidden);

		if (m_pSkill->GetCurrStageIndex() >= 1)
		{
			m_pMinusButton->SetVisibility(ESlateVisibility::Visible);
			m_pUpgradeBox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

			SetColorAndOpacity(FLinearColor::White);
		}
		else
		{
			bool bSuccess = m_pSubTreeWidget->GetSkillTreeWidget()->GetSkillTree()->PlayerLearntSpell(m_SkillClass);
			if (bSuccess)
			{
				SetColorAndOpacity(FLinearColor::White);
			}
			else
			{
				SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.3f));
			}

			m_pUpgradeBox->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void USkillTreeEntryWidget::OnSpellLearned()
{
	m_bSpellLearned = true;
	m_pSkillIcon->SetColorAndOpacity(FLinearColor::White);
}

void USkillTreeEntryWidget::_OnPlusButtonClicked()
{
	m_pSubTreeWidget->GetSkillTreeWidget()->GetSkillTree()->UpgradeSpell(m_pSkill, this);
}

void USkillTreeEntryWidget::_OnMinusButtonClicked()
{
	m_pSubTreeWidget->GetSkillTreeWidget()->GetSkillTree()->DowngradeSpell(m_pSkill, this);
}
