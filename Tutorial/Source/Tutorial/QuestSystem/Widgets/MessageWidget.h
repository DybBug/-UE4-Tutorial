// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MessageWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class TUTORIAL_API UMessageWidget : public UUserWidget
{
	GENERATED_BODY()

public :
	/* Get */
	UTextBlock* GetMessage() const { return m_pMessage; }
	
protected :
	UPROPERTY()
	UTextBlock* m_pMessage;
	
};
