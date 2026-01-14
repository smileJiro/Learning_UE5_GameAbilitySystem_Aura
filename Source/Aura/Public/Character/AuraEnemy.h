

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

	
public: /** IInteractionInterface */
	void HighlightActor() override;
	void UnHighlightActor() override;
	
protected:
	void BeginPlay() override;
	
private:
	void InitAbilityActorInfo() override;
};
