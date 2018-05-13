// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillSystem/SkillActors/Base_Skill.h"
#include "../Widgets/BuffWidget.h"

#include "Buff_Skill.generated.h"

/**
 * 
 */
UCLASS()
class TUTORIAL_API ABuff_Skill : public ABase_Skill
{
	GENERATED_BODY()

public :
	ABuff_Skill();

public :
	UFUNCTION()
	virtual void OnSkillNotify() override;

	UFUNCTION()
	virtual void ApplyEffect();

	UFUNCTION()
	virtual void RemoveEffect();

	UFUNCTION()
	void ResetBuff();

	UFUNCTION()
	void OnActivated();

	UFUNCTION()
	void OnDeactivated();

	/* Get */
	UFUNCTION(BlueprintPure, Category = "Buff_Skill")
	UTexture2D* GetBuffIcon() const { return m_pBuffIcon; }

	UFUNCTION(BlueprintPure, Category = "Buff_Skill")
	class UBuffWidget* GetBuffWidget() const { return m_pBuffWidget; }

	/* Set */
	UFUNCTION(BlueprintCallable, Category = "Buff_Skill")
	void SetBuffIcon(UTexture2D* _pIcon);

	UFUNCTION(BlueprintCallable, Category = "Buff_Skill")
	void SetBuffWidget(class UBuffWidget* _pWidget);

private :
	UFUNCTION()
	void _DurationTimelineUpdate();

protected :
	UPROPERTY(EditDefaultsOnly, Category = "Buff_Skill")
	UTexture2D* m_pBuffIcon;

	UPROPERTY(VisibleAnywhere, Category = "Buff_Skill")
	class UTimelineComponent* m_pDurationTimeline = nullptr;

	UPROPERTY()
	class UBuffWidget* m_pBuffWidget;
	
	UPROPERTY()
	float m_DurationTime = 0.f; // 지속시간.

	UPROPERTY()
	float m_PassedTime = 0.f;  // 경과시간.

	UPROPERTY()
	FTimerHandle m_hBuffTimer;
	
};
