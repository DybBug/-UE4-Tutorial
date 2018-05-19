// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "WidgetDragDropOperation.generated.h"

class UUserWidget;
/**
 * 
 */
UCLASS()
class TUTORIAL_API UWidgetDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()
	
public :
	void Initialize(UUserWidget* _pDraggedWidget, const FVector2D& _CustomOffset);

	/* Get */
	UFUNCTION(BlueprintPure, Category = "WidgetDragDropOperation")
	UUserWidget* GetDraggedWidget() const { return m_pDraggedWidget; }

	UFUNCTION(BlueprintPure, Category = "WidgetDragDropOperation")
	const FVector2D& GetCustomOffset() const { return m_CustomOffset; }
	
protected :
	UUserWidget* m_pDraggedWidget; 
	FVector2D    m_CustomOffset;   
	
};
