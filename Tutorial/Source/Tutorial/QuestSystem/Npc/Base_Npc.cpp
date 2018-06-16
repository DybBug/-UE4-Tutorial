// Fill out your copyright notice in the Description page of Project Settings.

#include "Base_Npc.h"
#include "../Widgets/InteractionWidget.h"
#include "../Widgets/MessageWidget.h"
#include "../Characters/QuestCharacter.h"
#include "../Actors/QuestManager.h"
#include "../Actors/QuestActors/Quest_Base.h"
#include "../Actors/PatrolPoint.h"

#include <WidgetComponent.h>
#include <PaperSpriteComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <Components/CapsuleComponent.h>
#include <Components/TextBlock.h>
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

	static ConstructorHelpers::FClassFinder<UUserWidget> InteractionWidgetClass(TEXT("WidgetBlueprint'/Game/TutorialContent/QuestSystem/Widgets/WB_Interaction.WB_Interaction_C'"));
	if (InteractionWidgetClass.Succeeded())
	{
		m_pInteractionWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionWidget"));
		m_pInteractionWidget->SetWidgetClass(InteractionWidgetClass.Class);
		m_pInteractionWidget->SetRelativeLocation(FVector(0.f, 0.f, 140.f));
		m_pInteractionWidget->SetWidgetSpace(EWidgetSpace::Screen);
		m_pInteractionWidget->SetPivot(FVector2D(0.5f, 0.f));
		m_pInteractionWidget->bGenerateOverlapEvents = false;
		m_pInteractionWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		m_pInteractionWidget->SetVisibility(false);
		m_pInteractionWidget->SetupAttachment(RootComponent);
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> MessageWidgetClass(TEXT("WidgetBlueprint'/Game/TutorialContent/QuestSystem/Widgets/WB_Message.WB_Message_C'"));
	if (MessageWidgetClass.Succeeded())
	{
		m_pMessageWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("MessageWidget"));
		m_pMessageWidget->SetWidgetClass(MessageWidgetClass.Class);
		m_pMessageWidget->SetRelativeLocation(FVector(0.f, 0.f, 140.f));
		m_pMessageWidget->SetWidgetSpace(EWidgetSpace::Screen);
		m_pMessageWidget->SetPivot(FVector2D(0.5f, 0.f));
		m_pMessageWidget->bGenerateOverlapEvents = false;
		m_pMessageWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		m_pMessageWidget->SetVisibility(false);
		m_pMessageWidget->SetupAttachment(RootComponent);
	}
}

// Called when the game starts or when spawned
void ABase_Npc::BeginPlay()
{
	Super::BeginPlay();

	SetOwner(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	m_pQuestionMark->SetVisibility(UKismetSystemLibrary::IsValidClass(m_HasQuestClass));

	Cast<UInteractionWidget>(m_pInteractionWidget->GetUserWidgetObject())->GetName()->SetText(m_Name);

	if (m_bDoesPatrol && (m_PatrollingPoints.Num() > 0))
	{
		MoveToCurrPatrolPoint();
	}
	
}

void ABase_Npc::Initialize(bool _bDoesPatrol, const TArray<APatrolPoint*>& _PatrollingPoints, int _CurrPatrolIndex, int _NpcId)
{
	m_bDoesPatrol = _bDoesPatrol;
	m_PatrollingPoints = _PatrollingPoints;
	m_CurrPatrolIndex = _CurrPatrolIndex;
	m_NpcId = _NpcId;
}

void ABase_Npc::MoveToCurrPatrolPoint()
{
	FVector Loc = m_PatrollingPoints[m_CurrPatrolIndex]->GetActorLocation();
	
	Cast<AAIController>(GetController())->MoveToLocation(Loc, GetCapsuleComponent()->GetUnscaledCapsuleRadius());	

}
void ABase_Npc::OnTalkedTo(AQuestCharacter * _pPlayer)
{
	ShowMessage(m_DefaultMessage, m_DefaultDuration, _pPlayer);
}
void ABase_Npc::ShowMessage(const FText& _Message, float _Duration, AQuestCharacter * _pPlayer)
{
	if (m_bCanBeTalkedTo)
	{
		m_bCanBeTalkedTo = false;
		m_pInteractionWidget->SetVisibility(false);
		Cast<UMessageWidget>(m_pMessageWidget->GetUserWidgetObject())->GetMessage()->SetText(_Message);
		m_pMessageWidget->SetVisibility(true);
		_pPlayer->GetQuestManager()->OnTalkedToNpc(this->GetClass(), m_NpcId);

		FTimerHandle hTimer;
		GetWorldTimerManager().SetTimer(hTimer, [&] {
			m_pMessageWidget->SetVisibility(false);
			m_pInteractionWidget->SetVisibility(m_bInPlayerRadius);
			m_bCanBeTalkedTo = true;

		}, 1, false, _Duration);
	}
}
void ABase_Npc::OnEnterPlayerRadius(AQuestCharacter* _pPlayer)
{
	m_bInPlayerRadius = true;

	if (m_bCanBeTalkedTo)
	{
		m_pInteractionWidget->SetVisibility(true);
	}
}

void ABase_Npc::OnLeavePlayerRadius(AQuestCharacter* _pPlayer)
{
	m_bInPlayerRadius = false;

	m_pInteractionWidget->SetVisibility(false);
}

void ABase_Npc::OnInteractWith(AQuestCharacter* _pPlayer)
{
	if (UKismetSystemLibrary::IsValidClass(m_HasQuestClass))
	{
		if (!_pPlayer->GetQuestManager()->GetAllQuestClasses().Contains(m_HasQuestClass))
		{
			_pPlayer->GetQuestManager()->AddNewQuest(m_HasQuestClass, true);
			m_pQuestionMark->SetVisibility(false);
			return;
		}
	}

	OnTalkedTo(_pPlayer);
}

