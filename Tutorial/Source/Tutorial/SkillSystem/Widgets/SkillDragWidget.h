// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "SkillDragWidget.generated.h"

/**
 * 
 */
UCLASS()
class TUTORIAL_API USkillDragWidget : public UUserWidget
{
	GENERATED_BODY()
	
public :
	virtual void NativeConstruct() override;

	/* Get */
	class UImage* GetSkillIcon() const { return m_pSkillIcon; }

	/*Set*/
	void SetSkillTexture(UTexture2D* _pNewTexture);

protected :
	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = "true"), Category = "SkillDragWidget")
	class UTexture2D* m_pSkillTexture;

	UPROPERTY(BlueprintReadWrite, Category="SkillDragWidget")
	class UImage* m_pSkillIcon;


};
