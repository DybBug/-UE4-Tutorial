// Fill out your copyright notice in the Description page of Project Settings.

#include "QuestCharacter.h"
#include "../Widgets/QuestSystemHUD.h"
#include "../Widgets/QuestWidget.h"
#include "../Widgets/SubGoalWidget.h"
#include "../Widgets/QuestJournalWidget.h"
#include "../Widgets/QuestListEntryWidget.h"
#include "../Widgets/SaveLoadWidget.h"
#include "../Actors/QuestManager.h"
#include "../Actors/QuestActors/Quest_Base.h"
#include "../Interfaces/Interactable_Interface.h"
#include "../Npc/Base_Npc.h"
#include "../SaveGames/CharacterSave.h"

#include <UObject/ConstructorHelpers.h>
#include <Camera/CameraComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetMathLibrary.h>
#include <Kismet/KismetTextLibrary.h>
#include <Blueprint/WidgetBlueprintLibrary.h>
#include <Particles/ParticleSystemComponent.h>
#include <Components/ChildActorComponent.h>
#include <Components/CapsuleComponent.h>
#include <PaperSpriteComponent.h>



// Sets default values
AQuestCharacter::AQuestCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	m_pSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	m_pSpringArm->TargetArmLength = 500.f;
	m_pSpringArm->SetRelativeRotation(FRotator(-25.f, 0.f, 0.f));
	m_pSpringArm->SetupAttachment(RootComponent);

	m_pCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	m_pCamera->SetupAttachment(m_pSpringArm);

	m_pMiniMapArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("MiniMapArm"));
	m_pMiniMapArm->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
	m_pMiniMapArm->TargetArmLength = 650.f;
	m_pMiniMapArm->bDoCollisionTest = false;
	m_pMiniMapArm->bInheritPitch = false;
	m_pMiniMapArm->bInheritRoll = false;
	m_pMiniMapArm->bInheritYaw = false;
	m_pMiniMapArm->SetupAttachment(RootComponent);

	m_pMiniMapCamera = CreateDefaultSubobject<UChildActorComponent>(TEXT("MiniMapCamera"));
	m_pMiniMapCamera->SetupAttachment(m_pMiniMapArm);

	m_pPointer = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Pointer"));
	m_pPointer->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_pPointer->bGenerateOverlapEvents = false;
	m_pPointer->bOwnerNoSee = true;
	m_pPointer->SetRelativeLocation(FVector(0.f, 0.f, 120.f));
	m_pPointer->SetRelativeRotation(FRotator(0.f, 90.f, -90.f));
	m_pPointer->SetRelativeScale3D(FVector(2.5f, 2.5f, 2.5f));
	m_pPointer->SetupAttachment(RootComponent);


	m_pLevelUpEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("LevelUpEffect"));
	m_pLevelUpEffect->SetAutoActivate(false);
	m_pLevelUpEffect->SetupAttachment(RootComponent);

	m_pInteractionCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("InteractionCapsule"));
	m_pInteractionCapsule->SetRelativeLocation(FVector(90.f, 0.f, 0.f));
	m_pInteractionCapsule->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	m_pInteractionCapsule->SetCapsuleHalfHeight(240.f);
	m_pInteractionCapsule->SetCapsuleRadius(110.f);
	m_pInteractionCapsule->OnComponentBeginOverlap.AddDynamic(this, &AQuestCharacter::_OnBeginOverlap);
	m_pInteractionCapsule->OnComponentEndOverlap.AddDynamic(this, &AQuestCharacter::_OnEndOverlap);
	m_pInteractionCapsule->SetupAttachment(RootComponent);



	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(TEXT("WidgetBlueprint'/Game/TutorialContent/QuestSystem/Widgets/WB_HUD.WB_HUD_C'"));
	if (WidgetClass.Succeeded())
	{
		m_pHUDClass = WidgetClass.Class;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> Sound(TEXT("SoundWave'/Game/TutorialContent/QuestSystem/Audio/Effects/LevelUpSound.LevelUpSound'"));
	if (Sound.Succeeded())
	{
		m_pLevelUpSound = Sound.Object;
	}

}

// Called when the game starts or when spawned
void AQuestCharacter::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::SetGamePaused(GetWorld(), true);

	UClass* pWidgetClass = LoadClass<USaveLoadWidget>(nullptr, TEXT("WidgetBlueprint'/Game/TutorialContent/QuestSystem/Widgets/WB_SaveLoad.WB_SaveLoad_C'"));
	m_pSaveLoadWidget = CreateWidget<USaveLoadWidget>(GetWorld(), pWidgetClass);
	m_pSaveLoadWidget->Initialize(this, m_AmountOfSaveSlots, true);
	m_pSaveLoadWidget->AddToViewport(10);

	APlayerController* pController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	pController->bShowMouseCursor = true;
	m_bWidgetInput = true;
}

float AQuestCharacter::TakeDamage(float _DamageAmount, FDamageEvent const& _DamageEvent, AController* _pEventInstigator, AActor* _pDamageCauser)
{
	m_CurrHealth -= _DamageAmount;
	UpdateHealth();

	return _DamageAmount;
}

// Called to bind functionality to input
void AQuestCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AQuestCharacter::_MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AQuestCharacter::_MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &AQuestCharacter::_Turn);

	PlayerInputComponent->BindKey(EKeys::B,   IE_Pressed, this, &AQuestCharacter::_BKey);
	PlayerInputComponent->BindKey(EKeys::E,   IE_Pressed, this, &AQuestCharacter::_EKey);
	PlayerInputComponent->BindKey(EKeys::H,   IE_Pressed, this, &AQuestCharacter::_HKey);
	PlayerInputComponent->BindKey(EKeys::I,   IE_Pressed, this, &AQuestCharacter::_IKey);
	PlayerInputComponent->BindKey(EKeys::J,   IE_Pressed, this, &AQuestCharacter::_JKey);
	PlayerInputComponent->BindKey(EKeys::M,   IE_Pressed, this, &AQuestCharacter::_MKey);
	PlayerInputComponent->BindKey(EKeys::R,   IE_Pressed, this, &AQuestCharacter::_RKey);
	PlayerInputComponent->BindKey(EKeys::Tab, IE_Pressed, this, &AQuestCharacter::_TabKey);

	PlayerInputComponent->BindKey(EKeys::LeftMouseButton, IE_Pressed, this, &AQuestCharacter::_LeftMouseButton);


}

void AQuestCharacter::StartGame(bool _bLoad, int _LoadSlot)
{
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	m_pSaveLoadWidget->SetVisibility(ESlateVisibility::Hidden);

	if (_bLoad)
	{
		LoadGameToSlot(_LoadSlot);
	}

	// HUD 위젯 생성.
	if (m_pHUDClass)
	{
		SetupPrestigePoints();

		m_pHUD = CreateWidget<UQuestSystemHUD>(GetWorld(), m_pHUDClass);
		m_pHUD->AddToViewport();

		FActorSpawnParameters Param;
		Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		m_pQuestManager = GetWorld()->SpawnActor<AQuestManager>(AQuestManager::StaticClass(), Param);

		m_pHUD->SetQuestManager(m_pQuestManager);

		m_pQuestManager->SetHUD(m_pHUD);

		m_pHUD->GetQuestJournalWidget()->Initialize(m_pQuestManager);

		UpdateLevel();
		UpdateExp();
		UpdateHealth();
		UpdateRegionWidget();

		_ToggleInputMode();

		if (_bLoad)
		{
			m_pQuestManager->LoadQuests();
		}

	//	m_pHUD->AddToViewport();
	}	
}

void AQuestCharacter::ContinueGame()
{
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	m_pSaveLoadWidget->SetVisibility(ESlateVisibility::Hidden);
	_ToggleInputMode();
}

void AQuestCharacter::ShowSaveWidget()
{
	UGameplayStatics::SetGamePaused(GetWorld(), true);

	if (!m_bWidgetInput)
	{
		_ToggleInputMode();		
	}

	m_pSaveLoadWidget->SetLoad(false);
	m_pSaveLoadWidget->Update();
	m_pSaveLoadWidget->SetVisibility(ESlateVisibility::Visible);
}

void AQuestCharacter::OnLevelUp()
{
	m_CurrExp         -= m_ExpForNextLevel;
	m_ExpForNextLevel *= m_NextExpMultiplier;	
	m_CurrLevel++;

	UpdateLevel();

	TArray<UQuestListEntryWidget*> AllQuestListEntryWidgets = m_pHUD->GetQuestJournalWidget()->GetAllQuestEntryWidgets();
	for (int i = 0; i < AllQuestListEntryWidgets.Num(); ++i)
	{
		AllQuestListEntryWidgets[i]->UpdateLevelColor();
	}

	if (m_pHUD->GetQuestJournalWidget()->GetCurrQuestWidget())
	{
		m_pHUD->GetQuestJournalWidget()->UpdateSuggestedLevelColor();
	}

	UGameplayStatics::PlaySound2D(GetWorld(), m_pLevelUpSound, 0.5f, 1.2f);
	m_pLevelUpEffect->SetActive(true);

	for (auto& pNpc : m_pQuestManager->GetAllNpcsInWorld())
	{
		pNpc->OnPlayerLevelUp(m_CurrLevel);
	}
}

void AQuestCharacter::UpdateExp()
{
#define LOCTEXT_NAMESPACE "ExpText"
	FText ExpText = FText::Format(LOCTEXT("ExpText", "{0} / {1}"), m_CurrExp, m_ExpForNextLevel);
#undef LOCTEXT_NAMESPACE

	m_pHUD->GetExpText()->SetText(ExpText);
	m_pHUD->GetExpBar()->SetPercent((float)m_CurrExp / (float)m_ExpForNextLevel);
}

void AQuestCharacter::UpdateLevel()
{
	m_pHUD->GetLevelText()->SetText(UKismetTextLibrary::Conv_IntToText(m_CurrLevel));
}

void AQuestCharacter::UpdateHealth()
{
#define LOCTEXT_NAMESPACE "HealthText"
	FText HealthText = FText::Format(LOCTEXT("HealthText", "{0} / {1}"), m_CurrHealth, m_MaxHealth);
#undef LOCTEXT_NAMESPACE

	m_pHUD->GetHealthText()->SetText(HealthText);
	m_pHUD->GetHealthBar()->SetPercent((float)m_CurrHealth / (float)m_MaxHealth);
}

void AQuestCharacter::AddExpPoints(int _Amount)
{
	m_CurrExp += _Amount;

	CHECK_EXP:
	if (m_CurrExp >= m_ExpForNextLevel)
	{
		OnLevelUp();
		goto CHECK_EXP;
	}
	else
	{
		UpdateExp();
	}
}

void AQuestCharacter::SetupPrestigePoints()
{
	for (int i = 0; i < (int8)ERegions::Max; ++i)
	{
		m_PrestigePoints.Add(FRegionPrestige((ERegions)i, 0));
	}
}

int AQuestCharacter::GetPrestigeByRegion(ERegions _Region)
{
	if(m_PrestigePoints.Num() == 0)
	{
		return 0;
	}
	return m_PrestigePoints[(int8)_Region].Prestige;
}

void AQuestCharacter::SetPrestigeByRegion(ERegions _Region, int _Value)
{
	m_PrestigePoints[(int8)_Region].Region = _Region;
	m_PrestigePoints[(int8)_Region].Prestige = _Value;

	if (m_CurrRegion == _Region)
	{
		UpdateRegionWidget();
	}

	for (auto& pNpc : m_pQuestManager->GetAllNpcsInWorld())
	{
		pNpc->OnPlayerGainPrestige(this);
	}
}

void AQuestCharacter::UpdateRegionWidget()
{
	FString Region = CONVERT_TO_STRING(L"ERegions", m_CurrRegion);
	m_pHUD->GetRegionText()->SetText(FText::FromString(Region));

	int Prestige = GetPrestigeByRegion(m_CurrRegion);
	m_pHUD->GetPrestigeText()->SetText(FText::AsNumber(Prestige));
}

void AQuestCharacter::OnNewRegionEntered(ERegions _Region)
{
	if (m_CurrRegion != _Region)
	{
		m_CurrRegion = _Region;
		UpdateRegionWidget();
	}
}

void AQuestCharacter::SaveGameToSlot(int _ToSlot)
{
	m_pSaveGameObject = Cast<UCharacterSave>(UGameplayStatics::CreateSaveGameObject(UCharacterSave::StaticClass()));

	if (m_pSaveGameObject)
	{
		m_pSaveGameObject->SetSavedExp(m_CurrExp);
		m_pSaveGameObject->SetSavedExpForNextLevel(m_ExpForNextLevel);
		m_pSaveGameObject->SetSavedLevel(m_CurrLevel);
		m_pSaveGameObject->SetSavedHealth(m_CurrHealth);
		m_pSaveGameObject->SetSavedMaxHealth(m_MaxHealth);
		m_pSaveGameObject->SetSavedObtainedObjects(m_ObtainedObjectClasses);
		m_pSaveGameObject->SetSavedPrestigePoints(m_PrestigePoints);
		m_pSaveGameObject->SetSavedLocation(GetActorLocation());
		m_pSaveGameObject->SetSavedHour(FDateTime::Now().GetHour());
		m_pSaveGameObject->SetSavedMinute(FDateTime::Now().GetMinute());

		TArray<FSavedQuest> LocalSavedQuests;
		TArray<AQuest_Base*> LocalQuestActors = m_pQuestManager->GetCurrQuestActors();
		LocalQuestActors.Append(m_pQuestManager->GetCompletedQuestActors());
		LocalQuestActors.Append(m_pQuestManager->GetFailedQuestActors());

		for (auto& pLocalQuestActor : LocalQuestActors)
		{
			FSavedQuest SavedQuest;

			SavedQuest.QuestClass = pLocalQuestActor->GetClass();
			SavedQuest.CurrGoalIndices = pLocalQuestActor->GetCurrGoalIndices();
			SavedQuest.HuntAmounts = pLocalQuestActor->GetCurrHuntedAmounts();
			SavedQuest.SelectedGoalIndex = pLocalQuestActor->GetSelectedSubGoalIndex();
			SavedQuest.CurrGoals = pLocalQuestActor->GetCurrGoals();
			SavedQuest.CompletedGoals = pLocalQuestActor->GetCompletedSubGoals();
			SavedQuest.CurrState = pLocalQuestActor->GetCurrState();
			SavedQuest.CurrDescription = pLocalQuestActor->GetCurrDescription();		

			if(m_pQuestManager->GetCurrQuest())
			{
				SavedQuest.bCurrentlyActive = (pLocalQuestActor == m_pQuestManager->GetCurrQuest());
			}
			else
			{
				SavedQuest.bCurrentlyActive = false;
			}

			LocalSavedQuests.Add(SavedQuest);
		}

		m_pSaveGameObject->SetSavedQuests(LocalSavedQuests);

		FString DefaultSlotName = m_DefaultSlotName;
		UGameplayStatics::SaveGameToSlot(m_pSaveGameObject, DefaultSlotName.Append(FString::FromInt(_ToSlot)), 0);
	}
}

void AQuestCharacter::LoadGameToSlot(int _ToSlot)
{
	FString DefaultSlotName = m_DefaultSlotName;
	m_pSaveGameObject = Cast<UCharacterSave>(UGameplayStatics::LoadGameFromSlot(DefaultSlotName.Append(FString::FromInt(_ToSlot)), 0));

	if (m_pSaveGameObject)
	{
		m_CurrExp               = m_pSaveGameObject->GetSavedExp();
		m_ExpForNextLevel       = m_pSaveGameObject->GetSavedExpForNextLevel();
		m_CurrLevel             = m_pSaveGameObject->GetSavedLevel();
		m_CurrHealth            = m_pSaveGameObject->GetSavedHealth();
		m_MaxHealth             = m_pSaveGameObject->GetSavedMaxHealth();
		m_ObtainedObjectClasses = m_pSaveGameObject->GetSavedObtainedObjects();
		m_PrestigePoints        = m_pSaveGameObject->GetSavedPrestigePoints();
		m_CurrRegion            = m_pSaveGameObject->GetSavedRegion();
		SetActorLocation(m_pSaveGameObject->GetSavedLocation());

		m_LoadedQuests = m_pSaveGameObject->GetSavedQuests();
	}
}

void AQuestCharacter::_ToggleInputMode()
{
	APlayerController* pController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (!m_bWidgetInput)
	{
		UWidgetBlueprintLibrary::SetInputMode_GameAndUI(pController, m_pHUD, false, false);
		pController->bShowMouseCursor = true;
		m_bWidgetInput = true;
	}
	else
	{
		UWidgetBlueprintLibrary::SetInputMode_GameOnly(pController);
		pController->bShowMouseCursor = false;
		m_bWidgetInput = false;
	}
}


void AQuestCharacter::_OnBeginOverlap(
	UPrimitiveComponent* _pOverlappedComponent, 
	AActor* _pOtherActor, 
	UPrimitiveComponent* _pOtherComp, 
	int32 _OtherBodyIndex, 
	bool _bFromSweep,
	const FHitResult & _SweepResult)
{
	// 인터페이스가 있는가?
	if (UKismetSystemLibrary::DoesImplementInterface(_pOtherActor, UInteractable_Interface::StaticClass()))
	{
		Cast<IInteractable_Interface>(_pOtherActor)->OnEnterPlayerRadius(this);
	}
}

void AQuestCharacter::_OnEndOverlap(
	UPrimitiveComponent*  _pOverlappedComponent, 
	AActor*  _pOtherActor, 
	UPrimitiveComponent*  _pOtherComp, 
	int32 _OtherBodyIndex)
{
	// 인터페이스가 있는가?
	if (UKismetSystemLibrary::DoesImplementInterface(_pOtherActor, UInteractable_Interface::StaticClass()))
	{
		Cast<IInteractable_Interface>(_pOtherActor)->OnLeavePlayerRadius(this);
	}
}

void AQuestCharacter::_MoveForward(float _Value)
{
	if (_Value == 0.f)
	{
		return;
	}

	FRotator Rot = GetControlRotation();
	FVector Forward = FRotationMatrix(Rot).GetScaledAxis(EAxis::X);

	AddMovementInput(Forward, _Value);

	m_pQuestManager->OnPlayerMove();
}

void AQuestCharacter::_MoveRight(float _Value)
{
	if (_Value == 0.f)
	{
		return;
	}

	FRotator Rot = GetControlRotation();
	FVector Right = FRotationMatrix(Rot).GetScaledAxis(EAxis::Y);

	AddMovementInput(Right, _Value);

	m_pQuestManager->OnPlayerMove();
}

void AQuestCharacter::_Turn(float _Value)
{
	if (_Value == 0.f)
	{
		return;
	}
	AddControllerYawInput(_Value);
}


void AQuestCharacter::_BKey()
{
	if (m_pHUD->GetQuestWidgets().Num() >= 1)
	{
		UWidgetAnimation* pSlideOutAnim = m_pHUD->GetWidgetAnimation("SlideOut");
		bool bSuccessed = m_pHUD->IsAnimationPlaying(pSlideOutAnim);

		if (bSuccessed)
		{
			m_pHUD->ReverseAnimation(pSlideOutAnim);		
		}
		else
		{
			EUMGSequencePlayMode::Type UMGPlayMode = m_pHUD->GetSlideOut() ? EUMGSequencePlayMode::Reverse : EUMGSequencePlayMode::Forward;
			m_pHUD->PlayAnimation(pSlideOutAnim, 0.f, 1, UMGPlayMode);
		}

		m_pHUD->SetSlideOut(!m_pHUD->GetSlideOut());
	}
}

void AQuestCharacter::_EKey()
{
	TArray<AActor*> OverlappingActors;
	m_pInteractionCapsule->GetOverlappingActors(OverlappingActors);

	for (int i = 0; i < OverlappingActors.Num(); ++i)
	{
		if (UKismetSystemLibrary::DoesImplementInterface(OverlappingActors[i], UInteractable_Interface::StaticClass()))
		{
			Cast<IInteractable_Interface>(OverlappingActors[i])->OnInteractWith(this);
			break;
		}
	}
}

void AQuestCharacter::_HKey()
{
	AQuest_Base* pQuest = m_pQuestManager->GetCurrQuestActors()[0];
	if (pQuest)
	{
		pQuest->CompleteSubGoal(0, true);
	}
}

void AQuestCharacter::_IKey()
{
	_ToggleInputMode();
}

void AQuestCharacter::_JKey()
{
	if (m_bWidgetInput)
	{
		m_pHUD->GetQuestJournalWidget()->SetVisibility(ESlateVisibility::Hidden);			
	}
	else
	{
		m_pHUD->GetQuestJournalWidget()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	_ToggleInputMode();	
}

void AQuestCharacter::_MKey()
{
	ShowSaveWidget();
}

void AQuestCharacter::_RKey()
{
	static bool bSwitch = false;
	bSwitch = !bSwitch;
	if (bSwitch)
	{
		OnNewRegionEntered(ERegions::Johto);		
	}
	else
	{
		OnNewRegionEntered(ERegions::Kanto);
	}

}

void AQuestCharacter::_TabKey()
{
	AQuest_Base* pCurrQuest = m_pQuestManager->GetCurrQuest();
	if (pCurrQuest)
	{
		UQuestWidget* pQuestWidget = pCurrQuest->GetQuestWidget();
		if (pQuestWidget->GetSubGoalWidgets().Num() > 1)
		{
			int Index = pQuestWidget->GetSubGoalWidgets().Find(pQuestWidget->GetSelectedSubGoalWidget());
			Index = (Index < (pQuestWidget->GetSubGoalWidgets().Num() - 1)) ? ++Index : 0;			

			pQuestWidget->SelectSubGoal(pQuestWidget->GetSubGoalWidgets()[Index], false);
		}
	}
}

void AQuestCharacter::_LeftMouseButton()
{
	if (m_bCanAttack)
	{
		m_bCanAttack = false;
		UParticleSystem* pParticle = LoadObject<UParticleSystem>(nullptr, TEXT("ParticleSystem'/Game/TutorialContent/SkillSystem/SkillActors/Effects/FlameNova/P_FlameNova.P_FlameNova'"));
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), pParticle, GetActorLocation());

		TArray<TEnumAsByte<EObjectTypeQuery>> ObjTypes;
		ObjTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

		TArray<AActor*> IgnoreActor;
		IgnoreActor.Add(this);
		TArray<FHitResult> HitResults;
		UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), GetActorLocation(), GetActorLocation(), 300.f, ObjTypes, true, IgnoreActor, EDrawDebugTrace::None, HitResults, true);

		for (int i = 0; i < HitResults.Num(); ++i)
		{
			UGameplayStatics::ApplyDamage(HitResults[i].GetActor(), 80.f, nullptr, this, UDamageType::StaticClass());
		}

		FTimerHandle hTimer;
		GetWorldTimerManager().SetTimer(hTimer, [&]() {
			m_bCanAttack = true;
		},1, false, 1);
	}
}


