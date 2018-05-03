// Fill out your copyright notice in the Description page of Project Settings.

#include "Combat_BlueprintFunctionLibrary.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

#include "../Enemies/Base_Enemy.h"
#include "../Elements/Base_Element.h"



bool UCombat_BlueprintFunctionLibrary::IsEnemy(AActor* _pActor)
{
	bool IsEqual = (_pActor->StaticClass() == ABase_Enemy::StaticClass());
	bool IsChild = UKismetMathLibrary::ClassIsChildOf(_pActor->StaticClass(), ABase_Enemy::StaticClass());

	return (IsEqual || IsChild);
}

void UCombat_BlueprintFunctionLibrary::CalculateFinalDamage(
	float _BaseDamage, 
	int _CritChance, 
	TSubclassOf<class ABase_Element> _AttackerElementClass, 
	TSubclassOf<class ABase_Element> _DefenderElementClass, 
	int& out_Damage, 
	bool& out_IsCritical, 
	EEffectiveness& out_Effectiveness)
{
	float CurrDamage = _BaseDamage * UKismetMathLibrary::RandomFloatInRange(0.9f, 1.1f);
	bool IsCritical = false;
	EEffectiveness Effectiveness;

	//
	// Effectiveness ���� �� �׿� ���� ������ ����..
	//
	if (UKismetSystemLibrary::IsValidClass(_AttackerElementClass) &&
		UKismetSystemLibrary::IsValidClass(_DefenderElementClass))
	{
		if (_DefenderElementClass.GetDefaultObject()->GetElementInfo().WeaknessClasses.Contains(_AttackerElementClass))
		{
			Effectiveness = EEffectiveness::Super_Effective;
			CurrDamage *= 1.5f;
		}
		else if (_DefenderElementClass.GetDefaultObject()->GetElementInfo().ResistanceClasses.Contains(_AttackerElementClass))
		{
			Effectiveness = EEffectiveness::Not_Effective;
			CurrDamage *= 0.75f;
		}
		else
		{
			Effectiveness = EEffectiveness::Effective;
			CurrDamage *= 1.f;
		}
	}
	else
	{
		Effectiveness = EEffectiveness::Effective;
		CurrDamage *= 1.f;
	}

	// ũ��Ƽ�ÿ� ���� ������ ����.
	if (UKismetMathLibrary::RandomIntegerInRange(1, 100) <= _CritChance)
	{
		IsCritical = true;
		CurrDamage *= 2.f;
	}
	
	//
	// �ƿ�ǲ �Ű������� ���� �� ����.
	//
	out_Damage        = (int)CurrDamage;
	out_IsCritical    = IsCritical;
	out_Effectiveness = Effectiveness;
}
