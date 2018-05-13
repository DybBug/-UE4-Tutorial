// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include <Components/Image.h>
#include <Components/TextBlock.h>
#include "BuffWidget.generated.h"

/**
 * 
 */
UCLASS()
class TUTORIAL_API UBuffWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	
	/* Get */
	UFUNCTION(BlueprintPure, Category = "BuffWidget")
	UImage* GetBuffIcon() const { return m_pBuffIcon; }

	UFUNCTION(BlueprintPure, Category = "BuffWidget")
	UImage* GetDurationMat() const { return m_pDurationMat; }

	UFUNCTION(BlueprintPure, Category = "BuffWidget")
	UTextBlock* GetDurationText() const { return m_pDurationText; }

	UFUNCTION(BlueprintPure, Category = "BuffWidget")
	UMaterialInstanceDynamic* GetDynamicDurationMat() const { return m_pDynamicDurationMat; }

	/* Set */
	UFUNCTION(BlueprintCallable, Category = "BuffWidget")
	void SetIcon(UTexture2D* _pIcon);
protected :
	UPROPERTY()
	UImage* m_pBuffIcon;

	UPROPERTY()
	UTextBlock* m_pDurationText;

	UPROPERTY()
	UImage* m_pDurationMat;

	UPROPERTY()
	UTexture2D* m_pIcon;

	UPROPERTY()
	UMaterialInstanceDynamic* m_pDynamicDurationMat;
};
