// Fill out your copyright notice in the Description page of Project Settings.

#include "SkillTreeComponent.h"
#include "../Characters/SkillCharacter.h"
#include "../SkillActors/Base_Skill.h"
#include "../Widgets/SkillHotkeyWidget.h"
#include "../Widgets/SkillTreeWidget.h"
#include "../Widgets/SubTreeWidget.h"
#include "../Widgets/ConnectionWidget.h"
#include "../Widgets/SkillTreeEntryWidget.h"

#include <Kismet/GameplayStatics.h>
#include <Blueprint/WidgetBlueprintLibrary.h>


// Sets default values for this component's properties
USkillTreeComponent::USkillTreeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USkillTreeComponent::BeginPlay()
{
	Super::BeginPlay();

	m_pPlayer = Cast<ASkillCharacter>(GetOwner());	
}


// Called every frame
void USkillTreeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USkillTreeComponent::SetupTree()
{
	m_pPlayer->GetHUD()->GetSkillTree()->Initialize(this);
	m_bTreeSetUp = true;
}

void USkillTreeComponent::HandleShowCommand()
{
	if (m_bTreeSetUp)
	{
		if (m_bTreeShown)
		{
			m_pSkillTreeWidget->SetVisibility(ESlateVisibility::Hidden);
			m_bTreeShown = false;
		}
		else
		{
			m_pSkillTreeWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			UWidgetBlueprintLibrary::SetInputMode_GameAndUI(UGameplayStatics::GetPlayerController(GetWorld(), 0), m_pSkillTreeWidget);
			m_bTreeShown = true;
		}
	}
}

void USkillTreeComponent::ActivateConnenctions(const TSubclassOf<ABase_Skill>& _ForSpell)
{
	for (int i = 0; i < m_pSkillTreeWidget->GetSubTreeWidget().Num(); ++i)
	{
		TArray<UConnectionWidget*> ConnectionWidgets = (m_pSkillTreeWidget->GetSubTreeWidget()[i])->GetConnectionWidgets();
		for (int j = 0; j < ConnectionWidgets.Num(); ++j)
		{
			if (ConnectionWidgets[j]->GetForSpellClass() == _ForSpell)
			{
				ConnectionWidgets[j]->Active();
			}
		}
	}
	
}

void USkillTreeComponent::UpdateAllEntries()
{
	for (int i = 0; i < m_pSkillTreeWidget->GetSubTreeWidget().Num(); ++i)
	{
		TArray<USkillTreeEntryWidget*> pWidgets = m_pSkillTreeWidget->GetSubTreeWidget()[i]->GetSkillTreeEntryWidgets();
		for (int j = 0; j < pWidgets.Num(); ++j)
		{
			pWidgets[j]->UpdateUpgradeBox();
		}
	}
}

bool USkillTreeComponent::PlayerLearntSpell(const TSubclassOf<ABase_Skill>& _pSpellClass)
{
	bool bIsSuccess = m_pPlayer->GetLearntSpellClasses().Contains(_pSpellClass);

	return bIsSuccess;	
}

void USkillTreeComponent::AddSkillPoints(int _Amount)
{
	m_SkillPoints += _Amount;
	UpdateAllEntries();

	m_pSkillTreeWidget->UpdateSP();
}

void USkillTreeComponent::UpgradeSpell(ABase_Skill * _pSkill, USkillTreeEntryWidget * _pEntryWidget)
{
	m_pUpgradedSpell = _pSkill;
	m_pEntryWidget = _pEntryWidget;

	if (CanUpgradeSpell(_pSkill))
	{
		m_SkillPoints -= 1;
		m_pUpgradedSpell->SetCurrStageIndex(m_pUpgradedSpell->GetCurrStageIndex() + 1);

		if (m_pUpgradedSpell->GetCurrStageIndex() == 0)
		{
			TSet<ABase_Skill> Skills;
			Skills.Add(_pSkill, nullptr);

			m_pPlayer->GetLearntSpellClasses().Add(m_pUpgradedSpell->GetClass());

			m_pEntryWidget->OnSpellLearned();
			ActivateConnenctions(m_pUpgradedSpell->GetClass());		
		}

		m_pEntryWidget->UpdateStageText();
		m_pEntryWidget->UpdateIcon();

		m_pSkillTreeWidget->UpdateSP();

		UpdateAllEntries();
	}
}

void USkillTreeComponent::DowngradeSpell(ABase_Skill * _pSkill, USkillTreeEntryWidget * _pEntryWidget)
{
	m_pUpgradedSpell = _pSkill;
	m_pEntryWidget = _pEntryWidget;

	m_pUpgradedSpell->SetCurrStageIndex(m_pUpgradedSpell->GetCurrStageIndex() - 1);

	m_pEntryWidget->UpdateStageText();
	m_pEntryWidget->UpdateIcon();
	
	AddSkillPoints(1);
	UpdateAllEntries();
}

bool USkillTreeComponent::CanUpgradeSpell(ABase_Skill * _pSkill)
{
	FSkillStage NextSkillStage = _pSkill->GetNextStage();

	if ((m_SkillPoints > 0) &&
		(m_pPlayer->GetCurrLevel() >= NextSkillStage.RequiredLevel) &&
		(_pSkill->GetCurrStageIndex() < (_pSkill->GetSkillInfo().Stages.Num() - 1)))
	{
		for (int i = 0; i < NextSkillStage.RequiredSkills.Num(); ++i)
		{
			if (!PlayerLearntSpell(NextSkillStage.RequiredSkills[i]))
			{
				return false;
			}
		}
		return true;
	}
	return false;
}

bool USkillTreeComponent::SkillOnHotkey(ABase_Skill* _pSkill, USkillHotkeyWidget*& out_ppHotkeyWidget)
{
	TArray<USkillHotkeyWidget*> pHotkeySlots = m_pPlayer->GetHUD()->GetAllHotkeySlots();

	for (int i = 0; i < pHotkeySlots.Num(); ++i)
	{
		if (pHotkeySlots[i]->GetAssignedSpell())
		{
			if (pHotkeySlots[i]->GetAssignedSpell() == _pSkill)
			{
				out_ppHotkeyWidget = pHotkeySlots[i];
				return true;
			}
		}
	}

	return false;
}

void USkillTreeComponent::SetPlayer(ASkillCharacter * _pPlayer)
{
	m_pPlayer = _pPlayer;
}

void USkillTreeComponent::SetSkillPoints(const int& _SkillPoints)
{
	m_SkillPoints = _SkillPoints;
}

