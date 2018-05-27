// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MiniMapWidget.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class TUTORIAL_API UMiniMapWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected :
	virtual void NativeConstruct() override;
	
public :

	/* Get */
	UImage* GetDirectionArrow() const { return m_pDirectionArrow; }

protected :
	UImage* m_pDirectionArrow;
	
};
