

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_MaxHealth.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UMMC_MaxHealth : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	
public:
	UMMC_MaxHealth();
	
public:
	float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
	
private:
	// GameplayEffect가 계산(MMC/Execution)을 수행할 때 “어떤 Attribute를, 누구로부터, 어떤 시점의 값으로 캡처할지”를 정의하는 구조체 
	FGameplayEffectAttributeCaptureDefinition VigorDef;
};
