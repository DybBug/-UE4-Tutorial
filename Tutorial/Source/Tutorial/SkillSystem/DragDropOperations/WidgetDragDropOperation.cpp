// Fill out your copyright notice in the Description page of Project Settings.

#include "WidgetDragDropOperation.h"


void UWidgetDragDropOperation::Initialize(UUserWidget* _pDraggedWidget, const FVector2D& _CustomOffset)
{
	m_pDraggedWidget = _pDraggedWidget;
	m_CustomOffset   = _CustomOffset;
}


