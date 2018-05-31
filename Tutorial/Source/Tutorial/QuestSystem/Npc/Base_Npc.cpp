// Fill out your copyright notice in the Description page of Project Settings.

#include "Base_Npc.h"
#include "../Widgets/InteractionWidget.h"
#include "../Characters/QuestCharacter.h"
#include "../Actors/QuestManager.h"
#include "../Actors/QuestActors/Quest_Base.h"

#include <WidgetComponent.h>
#include <Components/CapsuleComponent.h>
#include <UObject/ConstructorHelpers.h>
#include <Kismet/KismetSystemLibrary.h>


// Sets default values
ABase_Npc::ABase_Npc()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(TEXT("WidgetBlueprint'/Game/TutorialContent/QuestSystem/Widgets/WB_Interaction.WB_Interaction_C'"));
	if (WidgetClass.Succeeded())
	{
		m_pWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
		m_pWidget->SetWidgetClass(WidgetClass.Class);
		m_pWidget->SetRelativeLocation(FVector(0.f, 0.f, 120.f));
		m_pWidget->SetWidgetSpace(EWidgetSpace::Screen);
		m_pWidget->SetPivot(FVector2D(0.5f, 0.f));
		m_pWidget->bGenerateOverlapEvents = false;
		m_pWidget->SetVisibility(false);
		m_pWidget->SetupAttachment(RootComponent);
	}

	static ConstructorHelpers::FClassFinder<AQuest_Base> BP_QuestTest1Class(TEXT("Blueprint'/Game/TutorialContent/QuestSystem/Actors/BP_Quest_Test1.BP_Quest_Test1_C'"));
	if (BP_QuestTest1Class.Succeeded())
	{
		m_QuestTest1Class = BP_QuestTest1Class.Class;
	}
}

// Called when the game starts or when spawned
void ABase_Npc::BeginPlay()
{
	Super::BeginPlay();
	
	Cast<UInteractionWidget>(m_pWidget->GetUserWidgetObject())->GetName()->SetText(m_Name);
	
}

void ABase_Npc::OnEnterPlayerRadius(AQuestCharacter* _pPlayer)
{
	m_pWidget->SetVisibility(true);
}

void ABase_Npc::OnLeavePlayerRadius(AQuestCharacter* _pPlayer)
{
	m_pWidget->SetVisibility(false);
}

void ABase_Npc::OnInteractWith(AQuestCharacter* _pPlayer)
{
	if (!_pPlayer->GetQuestManager()->GetAllQuestClasses().Contains(m_QuestTest1Class))
	{
		_pPlayer->GetQuestManager()->AddNewQuest(m_QuestTest1Class, true);
	}
}

