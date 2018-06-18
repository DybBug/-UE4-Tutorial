// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SaveLoadWidget.generated.h"

class UTextBlock;
class UScrollBox;
class UButton;
class AQuestCharacter;
class USaveSlotWidget;
/**
 * 
 */
UCLASS()
class TUTORIAL_API USaveLoadWidget : public UUserWidget
{
	GENERATED_BODY()

protected :
	virtual void NativeConstruct() override;
	
public :
	void Initialize(AQuestCharacter* _pPlayer, int _AmountOfSlots, bool _bLoad);
	void GenerateSlots();
	void Update();

	/* Get */
	UScrollBox* GetSlotBox()           const { return m_pSlotBox; }
	UButton*    GetNewContinueButton() const { return m_pNewContinueButton; }

	AQuestCharacter* GetPlayer() const { return m_pPlayer; }
	const bool& GetLoad() const { return m_bLoad; }

	/* Set */
	void SetLoad(bool _bLoad) { m_bLoad = _bLoad; }

private :
	UFUNCTION()
	void _OnNewContinueButtonClicked();

protected :
	UPROPERTY()
	UTextBlock* m_pText;

	UPROPERTY()
	UScrollBox* m_pSlotBox;

	UPROPERTY()
	UButton* m_pNewContinueButton;

	UPROPERTY()
	UTextBlock* m_pButtonText;

	AQuestCharacter* m_pPlayer;
	int m_AmountOfSlots;
	bool m_bLoad = true;
	TArray<USaveSlotWidget*> m_AllSlots;

	
};
