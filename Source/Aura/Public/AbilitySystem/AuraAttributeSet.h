

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AuraAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()
public:
	FEffectProperties(){}
	
public:
	FGameplayEffectContextHandle	EffectContextHandle;
	
	UPROPERTY()
	UAbilitySystemComponent* 		SourceASC;
	
	UPROPERTY()
	AActor*					 		SourceAvatarActor;
	
	UPROPERTY()
	AController*			 		SourceController;
	
	UPROPERTY()
	ACharacter*				 		SourceCharacter;
	
	UPROPERTY()
	UAbilitySystemComponent* 		TargetASC;
	
	UPROPERTY()
	AActor*					 		TargetAvatarActor;
	
	UPROPERTY()
	AController*			 		TargetController;
	
	UPROPERTY()
	ACharacter*				 		TargetCharacter;
};


/**
 * 
 */
UCLASS()
class AURA_API UAuraAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UAuraAttributeSet();
	
public:
	// 변수를 Rep으로 표시할때 반드시 필요한 함수
	// 이 클래스에서 네트워크로 복제(Replication)할 프로퍼티(UPROPERTY) 목록을 엔진에게 등록하는 함수
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	// 값이 변경되기 전 시점에 호출되는 함수 -> 궁극적으로 모디파이어는 이 이후에 동작해서 모디파이어에서 계산되는 변경사항을 잡을 수 없음
	// 그래서 PostGameplayEffectExecute()를 사용해야함
	void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& EffectData) override;
public:
	// 올바른 Rep을 받기위해서는 Rep에 대한 notify 함수가 필요하고, 이를 uproperty에 명시
	// UE에서의 컨벤션은 "OnRep_" 접두어를 붙인다.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Vital Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Health);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Vital Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxHealth);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "Vital Attributes")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Mana);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "Vital Attributes")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxMana);

	// Rep Notify 함수는 일반적으로 arg를 0개 혹은 1개만 받을 수 있다.
	// 1개인 경우 그 타입은 리플리케이트된 그 데이터 타입이어야한다(FGameplayAttributeData)
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;
	
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;
	
	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;
	
	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;

private:
	void SetEffectProperties(const FGameplayEffectModCallbackData& EffectData, FEffectProperties& Props) const;
};
