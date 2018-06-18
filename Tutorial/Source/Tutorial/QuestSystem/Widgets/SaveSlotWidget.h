// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SaveSlotWidget.generated.h"


class UCanvasPanel;
class UButton;
class UTextBlock;
class USaveLoadWidget;
class UCharacterSave;
/**
 * 
 */
UCLASS()
class TUTORIAL_API USaveSlotWidget : public UUserWidget
{
	GENERATED_BODY()

protected :
	virtual void NativeConstruct() override;

public :
	void Initialize(int _SlotIndex, USaveLoadWidget* _pWidget);
	void Update();

	/* Get */
	UButton*    GetButton()     const { return m_pButton; }
	UTextBlock* GetLevelText()  const { return m_pLevelText; }
	UTextBlock* GetRegionText() const { return m_pRegionText; }
	UTextBlock* GetTimeText()   const { return m_pTimeText; }
	
private :
	UFUNCTION()
	void _OnButtonClicked();

protected :
	UPROPERTY()
	UCanvasPanel* m_pCanvas;

	UPROPERTY()
	UButton* m_pButton;

	UPROPERTY()
	UTextBlock* m_pLevelText;

	UPROPERTY()
	UTextBlock* m_pRegionText;

	UPROPERTY()
	UTextBlock* m_pTimeText;	

	int m_SlotIndex;
	USaveLoadWidget* m_pLoadWidget;
	UCharacterSave* m_pSaveGame;
};
