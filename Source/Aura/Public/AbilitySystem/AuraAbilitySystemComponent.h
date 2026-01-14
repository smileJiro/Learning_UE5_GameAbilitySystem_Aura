

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

// Dynamic할필요 없어 블프에서 안쓸거니까 
DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer& /*AssetTags*/);



/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	// FOnGameplayEffectAppliedDelegate에 EffectApplied() 함수를 바인딩할 시점이 필요하다. 
	// AuraEnemy, AuraCharacter 등 클래스에서 각각의 시점에 Abilitysystem에 대한 데이터 할당이 끝나면, 해당 함수를 호출한다.
	// 그러면 그 시점에 ASC는 Delegate를 등록할 것이다.
	void AbilityActorInfoSet(); 
	
public:
	FEffectAssetTags EffectAssetTags;

protected:
	void EffectApplied(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);
};
