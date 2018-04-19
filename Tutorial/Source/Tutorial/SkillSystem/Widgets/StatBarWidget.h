// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "StatBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class TUTORIAL_API UStatBarWidget : public UUserWidget
{
	GENERATED_BODY()

public :
	virtual void NativeConstruct() override;




	/* Get */
	UFUNCTION(BlueprintPure, Category = "StatBarWidget")
	float GetPercent() const { return m_Percent; }

	UFUNCTION(BlueprintPure, Category = "StatBarWidget")
	FLinearColor GetFillColor() const { return m_FillColor; }

	UFUNCTION(BlueprintPure, Category = "StatBarWidget")
	FLinearColor GetLerpColor() const { return m_LerpColor; }

	UFUNCTION(BlueprintPure, Category = "StatBarWidget")
	float GetDisplayedValue() const { return m_DisplayedValue; }

	UFUNCTION(BlueprintPure, Category = "StatBarWidget")
	float GetMaxValue() const { return m_MaxValue; }

	UFUNCTION(BlueprintPure, Category = "StatBarWidget")
	bool IsGradiation() const { return m_bIsGradiation; }

	UFUNCTION(BlueprintPure, Category = "StatBarWidget")
	UMaterialInstanceDynamic* GetDynamicMaterial() const { return m_pDynamicMaterial; }

	/* Set */
	UFUNCTION(BlueprintCallable, Category = "StatBarWidget")
	void SetPercent(float _Percent);

	UFUNCTION(BlueprintCallable, Category = "StatBarWidget")
	void SetFillColor(FLinearColor _Color);

	UFUNCTION(BlueprintCallable, Category = "StatBarWidget")
	void SetLerpColor(FLinearColor _Color);

	UFUNCTION(BlueprintCallable, Category = "StatBarWidget")
	void SetDisplayedValue(float _Value);

	UFUNCTION(BlueprintCallable, Category = "StatBarWidget")
	void SetMaxValue(int _Value);
	
	UFUNCTION(BlueprintCallable, Category = "StatBarWidget")
	void SetIsGradiation(bool _Enable);

	UFUNCTION(BlueprintCallable, Category = "StatBarWidget")
	void SetDynamicMaterial(UMaterialInstanceDynamic* _pDynamicMaterial);

	
protected :
	UPROPERTY(EditInstanceOnly, Category = "StatBarWidget")
	float m_Percent = 1.f;

	UPROPERTY(EditInstanceOnly, Category = "StatBarWidget")
	FLinearColor m_FillColor = FLinearColor(1.f, 1.f, 1.f, 1.f);

	UPROPERTY(EditInstanceOnly, Category = "StatBarWidget")
	FLinearColor m_LerpColor = FLinearColor(1.f, 1.f, 1.f, 1.f);

	UPROPERTY(EditInstanceOnly, Category = "StatBarWidget")
	float m_DisplayedValue = 0.f;

	UPROPERTY(EditInstanceOnly, Category = "StatBarWidget")
	int m_MaxValue = 0;

	UPROPERTY(EditInstanceOnly, Category = "StatBarWidget")
	bool m_bIsGradiation = false;

	UPROPERTY(EditInstanceOnly, Category = "StatBarWidget")
	UMaterialInstanceDynamic* m_pDynamicMaterial = nullptr;

	/* ÆÈ·¿Æ® º¯¼öµé. */
	UPROPERTY()
	class UProgressBar* m_pStatBar;
	UPROPERTY()
	class UImage* m_pStatLerpBar;
	UPROPERTY()
	class UTextBlock* m_pStatText;
};
