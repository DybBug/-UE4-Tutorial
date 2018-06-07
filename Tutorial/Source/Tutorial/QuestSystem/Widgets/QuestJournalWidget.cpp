// Fill out your copyright notice in the Description page of Project Settings.

#include "QuestJournalWidget.h"
#include "GoalEntryWidget.h"
#include "QuestListEntryWidget.h"
#include "QuestCategoryWidget.h"
#include "QuestWidget.h"
#include "SubGoalWidget.h"
#include "../Actors/QuestActors/Quest_Base.h"
#include "../Characters/QuestCharacter.h"
#include "../Actors/QuestManager.h"


#include <WidgetTree.h>
#include <Components/TextBlock.h>
#include <Components/VerticalBox.h>
#include <Components/HorizontalBox.h>
#include <Components/ScrollBox.h>
#include <Components/Button.h>
#include <Kismet/GameplayStatics.h>

void UQuestJournalWidget::NativeConstruct()
{
	Super::NativeConstruct();

	m_pQuestName         = WidgetTree->FindWidget<UTextBlock>(TEXT("QuestName"));
	m_pQuestCategory     = WidgetTree->FindWidget<UTextBlock>(TEXT("QuestCategory"));
	m_pQuestRegion       = WidgetTree->FindWidget<UTextBlock>(TEXT("QuestRegion"));
	m_pSuggestedLevel    = WidgetTree->FindWidget<UTextBlock>(TEXT("SuggestedLevel"));
	m_pDescription       = WidgetTree->FindWidget<UTextBlock>(TEXT("Description"));
	m_pQuestGoalBox      = WidgetTree->FindWidget<UVerticalBox>(TEXT("QuestGoalBox"));
	m_pQuestDetailsBox   = WidgetTree->FindWidget<UScrollBox>(TEXT("QuestDetailsBox"));
	m_pExpRewardBox      = WidgetTree->FindWidget<UHorizontalBox>(TEXT("ExpRewardBox"));
	m_pExpReward         = WidgetTree->FindWidget<UTextBlock>(TEXT("ExpReward"));
	m_pPrestigeRewardBox = WidgetTree->FindWidget<UHorizontalBox>(TEXT("PrestigeRewardBox"));
	m_pPrestigeReward    = WidgetTree->FindWidget<UTextBlock>(TEXT("PrestigeReward"));
	m_pRewardBox         = WidgetTree->FindWidget<UVerticalBox>(TEXT("RewardBox"));
	m_pButtonBox         = WidgetTree->FindWidget<UHorizontalBox>(TEXT("ButtonBox"));
	m_pSelectButton      = WidgetTree->FindWidget<UButton>(TEXT("SelectButton"));
	m_pCancelButton      = WidgetTree->FindWidget<UButton>(TEXT("CancelButton"));

	m_pQuestList    = WidgetTree->FindWidget<UScrollBox>(TEXT("QuestList"));
	m_pCatCurrent   = WidgetTree->FindWidget<UQuestCategoryWidget>(TEXT("CatCurrent"));
	m_pCatCompleted = WidgetTree->FindWidget<UQuestCategoryWidget>(TEXT("CatCompleted"));
	m_pCatFailed    = WidgetTree->FindWidget<UQuestCategoryWidget>(TEXT("CatFailed"));

	m_pSelectButton->OnClicked.AddDynamic(this, &UQuestJournalWidget::_OnSelectButtonClicked);
	m_pCancelButton->OnClicked.AddDynamic(this, &UQuestJournalWidget::_OnCancelButtonClicked);
	
}

void UQuestJournalWidget::Initialize(AQuestManager * _pQuestManager)
{
	m_pQuestManager = _pQuestManager;
	m_pPlayer = Cast<AQuestCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

void UQuestJournalWidget::UpdateSuggestedLevelColor()
{
	FLinearColor Color;
	if (m_pPlayer->GetCurrLevel() > m_pSelectedQuest->GetQuestInfo().SuggestedLevel)
	{
		Color = FLinearColor(FColor(0xff27b700));		
	}
	else
	{		
		if(m_pPlayer->GetCurrLevel() == m_pSelectedQuest->GetQuestInfo().SuggestedLevel)
		{ 
			Color = FLinearColor::White;
		}
		else
		{
			Color = FLinearColor(FColor(0xffb80a00));

		}
	}
	m_pSuggestedLevel->SetColorAndOpacity(Color);
}

void UQuestJournalWidget::UpdateDescription()
{
	FText Desc = m_pSelectedQuest->GetCurrDescription();
	//Desc.ToString().Replace(L"\n", L"엔터?");

	m_pDescription->SetText(Desc);
}

void UQuestJournalWidget::GenerateSubGoals()
{
	m_pQuestGoalBox->ClearChildren();

	UClass* WidgetClass = LoadClass<UGoalEntryWidget>(nullptr, TEXT("WidgetBlueprint'/Game/TutorialContent/QuestSystem/Widgets/WB_GoalEntry.WB_GoalEntry_C'"));
	
	TArray<FCompletedGoal> CompletedGoals = m_pSelectedQuest->GetCompletedSubGoals();
	for (int i = 0; i < CompletedGoals.Num(); ++i)
	{
		UGoalEntryWidget* pGoalEntryWidget = CreateWidget<UGoalEntryWidget>(GetWorld(), WidgetClass);		
		pGoalEntryWidget->Initialize(CompletedGoals[i].GoalInfo, CompletedGoals[i].bSuccessful ? EGoalState::Success : EGoalState::Failed, this);
		m_pQuestGoalBox->AddChildToVerticalBox(pGoalEntryWidget);
	}

	TArray<FGoalInfo> CurrGoals = m_pSelectedQuest->GetCurrGoals();
	for (int i = 0; i < CurrGoals.Num(); ++i)
	{
		UGoalEntryWidget* pGoalEntryWidget = CreateWidget<UGoalEntryWidget>(GetWorld(), WidgetClass);		
		pGoalEntryWidget->Initialize(CurrGoals[i], EGoalState::Current, this);
		m_pQuestGoalBox->AddChildToVerticalBox(pGoalEntryWidget);
	}
}

void UQuestJournalWidget::UpdateDetailWindow()
{
	if (m_pSelectedQuest)
	{
		const FQuestInfo& QuestInfo = m_pSelectedQuest->GetQuestInfo();
		// #.퀘스트 이름.
		m_pQuestName->SetText(QuestInfo.Name);

		// #.퀘스트 카테고리.
		FString Category = CONVERT_TO_STRING(L"EQuestCategories", QuestInfo.Category);
		m_pQuestCategory->SetText(FText::FromString(Category));

		switch (QuestInfo.Category)
		{
			case EQuestCategories::Main_Quest :
			{
				m_pQuestCategory->SetColorAndOpacity(FSlateColor(FColor(0xffff5f00)));
				break;
			}
			case EQuestCategories::Side_Quest :
			{
				m_pQuestCategory->SetColorAndOpacity(FSlateColor(FColor(0xffff747a)));
				break;
			}
			case EQuestCategories::Events :
			{
				m_pQuestCategory->SetColorAndOpacity(FSlateColor(FColor(0xffff3122)));
				break;
			}
			default :
			{
				break;
			}
		}

		// #.퀘스트 지역.
		FString Region = CONVERT_TO_STRING(L"ERegions", QuestInfo.Region);
		m_pQuestRegion->SetText(FText::FromString(Region));

		// #.퀘스트 레벨.
		m_pSuggestedLevel->SetText(FText::AsNumber(QuestInfo.SuggestedLevel));		
		UpdateSuggestedLevelColor();

		// #.퀘스트 경험치 보상.
		if (QuestInfo.CompletionReward.Experience > 0)
		{
			m_pExpRewardBox->SetVisibility(ESlateVisibility::Visible);
#define LOCTEXT_NAMESPACE "ExpReward"
			FText ExpReward = FText::Format(LOCTEXT("ExpReward", "+ {0} Exp"), QuestInfo.CompletionReward.Experience);
#undef LOCTEXT_NAMESPACE
			m_pExpReward->SetText(ExpReward);
		}
		else
		{
			m_pExpRewardBox->SetVisibility(ESlateVisibility::Collapsed);
		}
		
		// #.퀘스트 보상.
		if (QuestInfo.CompletionReward.PrestigePoints > 0)
		{
			m_pPrestigeRewardBox->SetVisibility(ESlateVisibility::Visible);
#define LOCTEXT_NAMESPACE "PrestigeReward"
			FText PrestigeReward = FText::Format(LOCTEXT("PrestigeReward", "+ {0} PP"), QuestInfo.CompletionReward.PrestigePoints);
#undef LOCTEXT_NAMESPACE
			m_pPrestigeReward->SetText(PrestigeReward);
		}
		else
		{
			m_pPrestigeRewardBox->SetVisibility(ESlateVisibility::Collapsed);
		}

		// #.RewardBox.
		if ((QuestInfo.CompletionReward.Experience <= 0) && (QuestInfo.CompletionReward.PrestigePoints <= 0))
		{
			m_pRewardBox->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			m_pRewardBox->SetVisibility(ESlateVisibility::Visible);
		}

		UpdateDescription();
		GenerateSubGoals();

		// #. 버튼 박스.
		if (m_pSelectedQuest->GetCurrState() == EQuestStates::Current_Quests)
		{
			m_pButtonBox->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			m_pButtonBox->SetVisibility(ESlateVisibility::Collapsed);
		}

		m_pQuestDetailsBox->SetVisibility(ESlateVisibility::Visible);
	
	}
	else
	{
		m_pQuestDetailsBox->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UQuestJournalWidget::AddEntry(AQuest_Base * _pQuestActor)
{
	UClass* pWidgetClass = LoadClass<UQuestListEntryWidget>(nullptr, TEXT("WidgetBlueprint'/Game/TutorialContent/QuestSystem/Widgets/WB_QuestListEntry.WB_QuestListEntry_C'"));
	UQuestListEntryWidget* pWidget = CreateWidget<UQuestListEntryWidget>(GetWorld(), pWidgetClass);
	pWidget->Initialize(this, _pQuestActor);

	_pQuestActor->SetListEntryWidget(pWidget);

	m_AllQuestEntryWidgets.Add(pWidget);

	UQuestCategoryWidget* pCategoryWidget = nullptr;
	switch (_pQuestActor->GetCurrState())
	{
		case EQuestStates::Current_Quests:
		{
			pCategoryWidget = m_pCatCurrent;
			break;
		}
		case EQuestStates::Completed_Quests:
		{
			pCategoryWidget = m_pCatCompleted;
			break;
		}
		case EQuestStates::Failed_Quests:
		{
			pCategoryWidget = m_pCatFailed;
			break;
		}
		default:
		{
			pCategoryWidget = nullptr;
			break;
		}
	}
	if (pCategoryWidget)
	{
		pCategoryWidget->GetQuestBox()->AddChild(pWidget);
	}
	
}

void UQuestJournalWidget::OnQuestClicked(UQuestListEntryWidget * _pWidget)
{
	if (m_pCurrQuestWidget)
	{
		m_pCurrQuestWidget->GetQuestButton()->SetIsEnabled(true);
	}

	m_pCurrQuestWidget = _pWidget;

	m_pCurrQuestWidget->GetQuestButton()->SetIsEnabled(false);

	m_pSelectedQuest = m_pCurrQuestWidget->GetAssignedQuest();

	UpdateDetailWindow();
}

void UQuestJournalWidget::_OnSelectButtonClicked()
{
	if (m_pQuestManager->GetCurrQuest() != m_pSelectedQuest)
	{		
		m_pQuestManager->SelectNewQuest(m_pSelectedQuest, m_pSelectedQuest->GetQuestWidget()->GetSubGoalWidgets()[0]);
	}
}

void UQuestJournalWidget::_OnCancelButtonClicked()
{
}


