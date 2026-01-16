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
	UAbilitySystemComponent* 		SourceASC = nullptr;
	
	UPROPERTY()
	AActor*					 		SourceAvatarActor= nullptr;
	
	UPROPERTY()
	AController*			 		SourceController= nullptr;
	
	UPROPERTY()
	ACharacter*				 		SourceCharacter= nullptr;
	
	UPROPERTY()
	UAbilitySystemComponent* 		TargetASC= nullptr;
	
	UPROPERTY()
	AActor*					 		TargetAvatarActor= nullptr;
	
	UPROPERTY()
	AController*			 		TargetController= nullptr;
	
	UPROPERTY()
	ACharacter*				 		TargetCharacter= nullptr;
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
	
	/*
	 * Attributes 
	 * link: https://forest-bowler-224.notion.site/12-Derived-Attributes-Design-2ea9e6fd93be80968a96c62acbe02dd4?source=copy_link
	 */
	
public: // Primary Attributes
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Strength, Category = "Primary Attributes") 
	FGameplayAttributeData Strength; // 힘
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Strength);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Intelligence, Category = "Primary Attributes") 
	FGameplayAttributeData Intelligence; // 지능
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Intelligence);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Resilience, Category = "Primary Attributes") 
	FGameplayAttributeData Resilience; // 회복력
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Resilience);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Vigor, Category = "Primary Attributes") 
	FGameplayAttributeData Vigor; // 활력
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Vigor);
	
public: // Secondary Attributes
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Armor, Category = "Secondary Attributes") 
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Armor);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ArmorPenetration, Category = "Secondary Attributes") 
	FGameplayAttributeData ArmorPenetration;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ArmorPenetration);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BlockChance, Category = "Secondary Attributes") 
	FGameplayAttributeData BlockChance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, BlockChance);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitChance, Category = "Secondary Attributes") 
	FGameplayAttributeData CriticalHitChance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalHitChance);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitDamage, Category = "Secondary Attributes") 
	FGameplayAttributeData CriticalHitDamage;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalHitDamage);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitResistance, Category = "Secondary Attributes") 
	FGameplayAttributeData CriticalHitResistance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalHitResistance);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HealthRegeneration, Category = "Secondary Attributes") 
	FGameplayAttributeData HealthRegeneration;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, HealthRegeneration);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ManaRegeneration, Category = "Secondary Attributes") 
	FGameplayAttributeData ManaRegeneration;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ManaRegeneration);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Secondary Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxHealth);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "Secondary Attributes")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxMana);

	
public: // Vital Attributes
	// 올바른 Rep을 받기위해서는 Rep에 대한 notify 함수가 필요하고, 이를 uproperty에 명시
	// UE에서의 컨벤션은 "OnRep_" 접두어를 붙인다.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Vital Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Health);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "Vital Attributes")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Mana);

	// Rep Notify 함수는 일반적으로 arg를 0개 혹은 1개만 받을 수 있다.
	// 1개인 경우 그 타입은 리플리케이트된 그 데이터 타입이어야한다(FGameplayAttributeData)
	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldStrength) const;
	
	UFUNCTION()
	void OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const;
	
	UFUNCTION()
	void OnRep_Resilience(const FGameplayAttributeData& OldResilience) const;	
	
	UFUNCTION()
	void OnRep_Vigor(const FGameplayAttributeData& OldVigor) const;

	UFUNCTION()
	void OnRep_Armor(const FGameplayAttributeData& OldArmor) const;
	
	UFUNCTION()
	void OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const;
	
	UFUNCTION()
	void OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const;
	
	UFUNCTION()
	void OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const;
	
	UFUNCTION()
	void OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const;
	
	UFUNCTION()
	void OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const;
	
	UFUNCTION()
	void OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const;
	
	UFUNCTION()
	void OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const;
	
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;
	
	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;
	
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;
	
	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;

private:
	void SetEffectProperties(const FGameplayEffectModCallbackData& EffectData, FEffectProperties& Props) const;
};
