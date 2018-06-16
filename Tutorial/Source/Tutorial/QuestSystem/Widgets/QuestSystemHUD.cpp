// Fill out your copyright notice in the Description page of Project Settings.

#include "QuestSystemHUD.h"
#include "QuestWidget.h"
#include "QuestJournalWidget.h"

#include <Components/ScrollBoxSlot.h>
#include <MovieScene.h>


void UQuestSystemHUD::NativeConstruct()
{
	Super::NativeConstruct();

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

	// #.이 클래스의 모든 속성을 실행하여 위젯 애니메이션을 찾기.
	while (pProperty != nullptr)
	{
		// #.찾은 속성이 '오브젝트 속성'이라면...
		if (pProperty->GetClass() == UObjectProperty::StaticClass())
		{
			UObjectProperty* pObjProperty = Cast<UObjectProperty>(pProperty);

			// #. '오브젝트 속성'이 위젯 애니메이션이라면...
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
		// #. 다음 속성으로 이동.
		pProperty = pProperty->PropertyLinkNext;
	}
}

