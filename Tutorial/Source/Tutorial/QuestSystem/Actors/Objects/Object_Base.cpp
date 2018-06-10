// Fill out your copyright notice in the Description page of Project Settings.

#include "Object_Base.h"
#include "../../Widgets/InteractionWidget.h"
#include "../../Characters/QuestCharacter.h"
#include "../../Actors/QuestManager.h"

#include <Components/StaticMeshComponent.h>
#include <Components/WidgetComponent.h>
#include <UObject/ConstructorHelpers.h>

// Sets default values
AObject_Base::AObject_Base()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	m_pStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = m_pStaticMesh;

	static ConstructorHelpers::FClassFinder<UUserWidget> BP_Widget(TEXT("WidgetBlueprint'/Game/TutorialContent/QuestSystem/Widgets/WB_Interaction.WB_Interaction_C'"));
	if (BP_Widget.Succeeded())
	{
		m_pWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
		m_pWidget->SetRelativeLocation(FVector(0.f, 0.f, 100.f));
		m_pWidget->SetWidgetSpace(EWidgetSpace::Screen);
		m_pWidget->SetWidgetClass(BP_Widget.Class);
		m_pWidget->bMultiBodyOverlap = false;
		m_pWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		m_pWidget->SetVisibility(false);
		m_pWidget->SetupAttachment(RootComponent);
	}
}

// Called when the game starts or when spawned
void AObject_Base::BeginPlay()
{
	Super::BeginPlay();

	Cast<UInteractionWidget>(m_pWidget->GetUserWidgetObject())->GetName()->SetText(m_Name);
	Cast<UInteractionWidget>(m_pWidget->GetUserWidgetObject())->GetInteractText()->SetText(FText::FromString(TEXT("Press [E] to pick up!")));
}


void AObject_Base::OnEnterPlayerRadius(AQuestCharacter * _pPlayer)
{
	m_pWidget->SetVisibility(true);
}

void AObject_Base::OnLeavePlayerRadius(AQuestCharacter * _pPlayer)
{
	m_pWidget->SetVisibility(false);
}

void AObject_Base::OnInteractWith(AQuestCharacter * _pPlayer)
{
	_pPlayer->GetObtainedObjectClasses().Add(this->GetClass());
	_pPlayer->GetQuestManager()->OnObjectFound(this->GetClass());

	Destroy();
}

