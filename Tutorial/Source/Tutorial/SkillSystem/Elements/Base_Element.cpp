// Fill out your copyright notice in the Description page of Project Settings.

#include "Base_Element.h"


// Sets default values
ABase_Element::ABase_Element()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABase_Element::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABase_Element::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

