// Fill out your copyright notice in the Description page of Project Settings.

#include "Base_Npc.h"
#include "../Widgets/InteractionWidget.h"
#include "../Characters/QuestCharacter.h"
#include "../Actors/QuestManager.h"
#include "../Actors/QuestActors/Quest_Base.h"
#include "../Actors/PatrolPoint.h"

#include <WidgetComponent.h>
#include <PaperSpriteComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <Components/CapsuleComponent.h>
#include <UObject/ConstructorHelpers.h>
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetSystemLibrary.h>
#include <AI/Navigation/NavigationSystem.h>
#include <AIController.h>


// Sets default values
ABase_Npc::ABase_Npc()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	GetCharacterMovement()->MaxWalkSpeed = 200.f;

	m_pSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	m_pSpringArm->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
	m_pSpringArm->TargetArmLength = 450.f;
	m_pSpringArm->SetupAttachment(RootComponent);

	m_pQuestionMark = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("QuestionMark"));
	m_pQuestionMark->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
	m_pQuestionMark->bGenerateOverlapEvents = false;
	m_pQuestionMark->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_pQuestionMark->SetVisibility(false);
	m_pQuestionMark->bOwnerNoSee = true;
	m_pQuestionMark->SetupAttachment(m_pSpringArm);

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

	SetOwner(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	m_pQuestionMark->SetVisibility(m_bHasQuestion);

	Cast<UInteractionWidget>(m_pWidget->GetUserWidgetObject())->GetName()->SetText(m_Name);

	if (m_bDoesPatrol && (m_PatrollingPoints.Num() > 0))
	{
		MoveToCurrPatrolPoint();
	}
	
}

void ABase_Npc::Initialize(bool _bDoesPatrol, TArray<APatrolPoint*>& _PatrollingPoints, int _CurrPatrolIndex)
{
	m_bDoesPatrol = _bDoesPatrol;
	m_PatrollingPoints = _PatrollingPoints;
	m_CurrPatrolIndex = _CurrPatrolIndex;
}

void ABase_Npc::MoveToCurrPatrolPoint()
{
	FVector Loc = m_PatrollingPoints[m_CurrPatrolIndex]->GetActorLocation();
	
	Cast<AAIController>(GetController())->MoveToLocation(Loc, GetCapsuleComponent()->GetUnscaledCapsuleRadius());	

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
		m_pQuestionMark->SetVisibility(false);
	}
}

