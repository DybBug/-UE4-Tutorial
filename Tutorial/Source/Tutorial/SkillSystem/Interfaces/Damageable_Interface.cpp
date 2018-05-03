// Fill out your copyright notice in the Description page of Project Settings.

#include "Damageable_Interface.h"


void IDamageable_Interface::OnReceiveDamage(
	float _BaseDamage, 
	EDamageTypes _Type, 
	TSubclassOf<class ABase_Element> _ElementClass, 
	int _CritChance, 
	AActor* _pAttacker, 
	class ABase_Skill* _pSpell)
{
	return;
}



// Add default functionality here for any IDamageable_Interface functions that are not pure virtual.
