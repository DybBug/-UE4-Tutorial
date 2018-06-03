// Fill out your copyright notice in the Description page of Project Settings.

#include "QuestCategoryWidget.h"

#include <WidgetTree.h>
#include <Kismet/KismetTextLibrary.h>

void UQuestCategoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	m_pCategoryName = WidgetTree->FindWidget<UTextBlock>(TEXT("CategoryName"));

	FString EnumStr = CONVERT_TO_STRING(L"EQuestStates", m_Category);
	m_pCategoryName->SetText(FText::FromString(EnumStr));
	
}
