

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
	
public: // ICombatInterface
	int32 GetPlayerLevel() override;
	
private:
	void InitAbilityActorInfo() override;
	// 이 AAuraCharacter라는 클래스는 서버에도 할당되고 클라 2p에도 할당되는 그냥 클래스거든? 
};
