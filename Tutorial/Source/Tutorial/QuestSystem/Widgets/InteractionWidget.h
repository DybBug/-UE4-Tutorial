// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include <Components/TextBlock.h>

#include "InteractionWidget.generated.h"

/**
 * 
 */
UCLASS()
class TUTORIAL_API UInteractionWidget : public UUserWidget
{
	GENERATED_BODY()
	
public  :
	virtual bool Initialize() override;
	
public :
	/* Get */
	UTextBlock* GetName() const { return m_pName; }
	UTextBlock* GetInteractText() const { return m_pInteractText; }

protected :
	UTextBlock* m_pName;
	UTextBlock* m_pInteractText;
};
