


#include "AbilitySystem/AuraAbilitySystemComponent.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	// 여기서 델리게이트 등록절차 수행 
	// ASC가 Character에 대해 알필요 없게 종속성을 단방향으로 유지 Character, Actor 등이 ASC를 소유한 형태
	// OnGameplayEffectAppliedDelegateToSelf.AddUFunction(); -> 리플렉션 FName 기반으로 찾는거라 느려 AddUObject가 권장임
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::EffectApplied);
	
}

void UAuraAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& EffectSpec,
	FActiveGameplayEffectHandle ActiveEffectHandle)
{
	// 간편하게 화면에 디버그 메세지를 출력하는 함수
	// 여긴 일반적으로 Server, Client 모두 호출되는 함수야 그러니까 UI 알림, HUD 업데이트, 간단한 시각 효과 정도를 처리하는 용도로 쓸듯함
	// 본격적인 이펙트나 사운드 등은 GameplayCue를 사용할테니까
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);
	
	EffectAssetTagsDelegate.Broadcast(TagContainer); // AllAsssetTag가 필요한 애들이 이 Delegate를 구독할거야
	
	
}
