// Fill out your copyright notice in the Description page of Project Settings.

#include "QuestSystemHUD.h"
#include "QuestWidget.h"

#include <WidgetTree.h>
#include <Components/ScrollBoxSlot.h>
#include <MovieScene.h>



void UQuestSystemHUD::NativeConstruct()
{
	Super::NativeConstruct();

	m_pExpBar         = WidgetTree->FindWidget<UProgressBar>(TEXT("ExpBar"));
	m_pCharacterIcon  = WidgetTree->FindWidget<UImage>(TEXT("CharacterIcon"));
	m_pExpText        = WidgetTree->FindWidget<UTextBlock>(TEXT("ExpText"));
	m_pLevelText      = WidgetTree->FindWidget<UTextBlock>(TEXT("LevelText"));
	m_pQuestList      = WidgetTree->FindWidget<UScrollBox>(TEXT("QuestList"));
	m_pDistanceBorder = WidgetTree->FindWidget<UBorder>(TEXT("DistanceBorder"));
	m_pDistanceText   = WidgetTree->FindWidget<UTextBlock>(TEXT("DistanceText"));
	m_pMiniMapWidget  = WidgetTree->FindWidget<UMiniMapWidget>(TEXT("WB_MiniMap"));

	_SetupWidgetAnimations();
}

UQuestWidget* UQuestSystemHUD::AddQuestToList(AQuest_Base* _pQuestActor)
{
	if (_pQuestActor)
	{
		UClass* pQuestWidgetClass = LoadClass<UQuestWidget>(nullptr, TEXT("WidgetBlueprint'/Game/TutorialContent/QuestSystem/Widgets/WB_Quest.WB_Quest_C'"));
		UQuestWidget* pQuestWidget =  CreateWidget<UQuestWidget>(GetWorld(), pQuestWidgetClass);
		
		pQuestWidget->Initialize(m_pQuestManager, _pQuestActor);

		m_QuestWidgets.Add(pQuestWidget);
		UPanelSlot* pSlot = m_pQuestList->AddChild(pQuestWidget);

		UScrollBoxSlot* pScrollBoxSlot = Cast<UScrollBoxSlot>(pSlot);
		pScrollBoxSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
		pScrollBoxSlot->SetPadding(FMargin(0.f, 8.f, 0.f, 0.f));

		return pQuestWidget;
	}
	return nullptr;
}

void UQuestSystemHUD::_SetupWidgetAnimations()
{
	UProperty* pProperty = GetClass()->PropertyLink;

	// #.�� Ŭ������ ��� �Ӽ��� �����Ͽ� ���� �ִϸ��̼��� ã��.
	while (pProperty != nullptr)
	{
		// #.ã�� �Ӽ��� '������Ʈ �Ӽ�'�̶��...
		if (pProperty->GetClass() == UObjectProperty::StaticClass())
		{
			UObjectProperty* pObjProperty = Cast<UObjectProperty>(pProperty);

			// #. '������Ʈ �Ӽ�'�� ���� �ִϸ��̼��̶��...
			if (pObjProperty->PropertyClass == UWidgetAnimation::StaticClass())
			{
				UObject* pObj = pObjProperty->GetObjectPropertyValue_InContainer(this, 0);

				UWidgetAnimation* pWidgetAnim = Cast<UWidgetAnimation>(pObj);

				if (pWidgetAnim != nullptr && pWidgetAnim->MovieScene != nullptr)
				{
					FName AnimName = pWidgetAnim->MovieScene->GetFName();
					m_WidgetAnimations.Add(AnimName, pWidgetAnim);
				}
			}
		}
		// #. ���� �Ӽ����� �̵�.
		pProperty = pProperty->PropertyLinkNext;
	}
}

