// Fill out your copyright notice in the Description page of Project Settings.

#include "SkillSystemGameModeBase.h"

#include "SkillPlayerController.h"


ASkillSystemGameModeBase::ASkillSystemGameModeBase()
{
	PlayerControllerClass = ASkillPlayerController::StaticClass();
}


