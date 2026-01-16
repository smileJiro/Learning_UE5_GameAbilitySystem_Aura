

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "Interaction/InteractionInterface.h"
#include "AuraEnemy.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public ACharacterBase, public IInteractionInterface
{
	GENERATED_BODY()
	
public:
	AAuraEnemy();

	
public: // IInteractionInterface
	void HighlightActor() override;
	void UnHighlightActor() override;
	
public: // ICombatInterface
	int32 GetPlayerLevel() override;
	
protected:
	void BeginPlay() override;
	
private:
	void InitAbilityActorInfo() override;
	
protected:
	// Enemy Level은 복제하지 않는다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	int32 Level = 1;

};
