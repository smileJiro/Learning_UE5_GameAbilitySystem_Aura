

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "AuraCharacter.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraCharacter : public ACharacterBase
{
	GENERATED_BODY()
	
public:
	AAuraCharacter();
	
public:
	void PossessedBy(AController* NewController) override; // Server에서 동작 
	void OnRep_PlayerState() override; // Client에서 동작
	
private:
	void InitAbilityActorInfo() override;
};
