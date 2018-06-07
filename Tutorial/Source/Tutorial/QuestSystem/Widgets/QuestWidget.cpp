// Fill out your copyright notice in the Description page of Project Settings.

#include "QuestWidget.h"
#include "SubGoalWidget.h"
#include "../Actors/QuestActors/Quest_Base.h"
#include "../Actors/QuestManager.h"

#include <WidgetTree.h>


void UQuestWidget::NativeConstruct()
{
	Super::NativeConstruct();

	m_pQuestName = WidgetTree->FindWidget<UTextBlock>(TEXT("QuestName"));
	m_pSubGoalBox = WidgetTree->FindWidget<UVerticalBox>(TEXT("SubGoalBox"));

}

void UQuestWidget::Initialize(AQuestManager * _pQuestManager, AQuest_Base* _pAssignedQuest)
{
	m_pQuestManager = _pQuestManager;
	m_pAssignedQuest = _pAssignedQuest;
}

void UQuestWidget::GenerateSubWidgets()
{
	m_SubGoalWidgets.Empty();

	m_pSubGoalBox->ClearChildren();

	UClass* pSubGoaltWidgetClass = LoadClass<USubGoalWidget>(nullptr, TEXT("WidgetBlueprint'/Game/TutorialContent/QuestSystem/Widgets/WB_SubGoal.WB_SubGoal_C'"));

	for (int i = 0; i < m_pAssignedQuest->GetCurrGoals().Num(); ++i)
	{
		USubGoalWidget* pSubGoalWidget = CreateWidget<USubGoalWidget>(GetWorld(), pSubGoaltWidgetClass);

		pSubGoalWidget->Initialize(m_pAssignedQuest->GetCurrGoals()[i], m_pAssignedQuest, this);

		m_SubGoalWidgets.Add(pSubGoalWidget);

		m_pSubGoalBox->AddChild(pSubGoalWidget);
	}
}

void UQuestWidget::UpdateQuest()
{
	if (m_pAssignedQuest)
	{
		m_pQuestName->SetText(m_pAssignedQuest->GetQuestInfo().Name);
		
		switch (m_pAssignedQuest->GetQuestInfo().Category)
		{
			case EQuestCategories::Main_Quest :
			{
				m_pQuestName->SetColorAndOpacity(FSlateColor(FColor(0xffff5f00)));
				break;
			}
			case EQuestCategories::Side_Quest :
			{
				m_pQuestName->SetColorAndOpacity(FSlateColor(FColor(0xffff747a)));
				break;
			}
			case EQuestCategories::Events :
			{
				m_pQuestName->SetColorAndOpacity(FSlateColor(FColor(0xffff3122)));
				break;
			}
			default :
			{
				break;
			}

		}
		GenerateSubWidgets();
	}
}

bool UQuestWidget::IsCurrQuest()
{	
	return (m_pAssignedQuest == m_pQuestManager->GetCurrQuest());
}

void UQuestWidget::SelectSubGoal(USubGoalWidget* _pClickedSubGoal)
{
	if (m_pSelectedSubGoalWidget)
	{
		m_pSelectedSubGoalWidget->GetGoalBorder()->SetContentColorAndOpacity(FLinearColor(FColor(0x7FFFFFFF))); // a = 7F.
		m_pSelectedSubGoalWidget->GetSelectButton()->SetIsEnabled(true);
	}
	
	if (_pClickedSubGoal)
	{
		m_pSelectedSubGoalWidget = _pClickedSubGoal;
		m_pSelectedSubGoalWidget->GetGoalBorder()->SetContentColorAndOpacity(FLinearColor::White);
		m_pSelectedSubGoalWidget->GetSelectButton()->SetIsEnabled(false);		
		
		m_pAssignedQuest->SetSelectedSubGoalIndex(m_pSelectedSubGoalWidget->GetGoalIndex());

		m_pQuestManager->OnSwitchSubQuest();
	}
	
}

void UQuestWidget::OnQuestSelected(USubGoalWidget* _pClickedSubGoal)
{
	m_pQuestName->SetIsEnabled(true);
	SelectSubGoal(_pClickedSubGoal);
}
