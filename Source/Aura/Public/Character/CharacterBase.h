

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"

#include "CharacterBase.generated.h" // .generated는 항상 마지막 include 여야한다.
class UAbilitySystemComponent;
class UAttributeSet;
class UGameplayEffect;
UCLASS()
class AURA_API ACharacterBase : public ACharacter,  public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ACharacterBase();
	
public: /* IAbilitySystemInterface */
	UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
public:
	UAttributeSet* GetAttributeSet() const;
	
protected:
	void BeginPlay() override;
	
protected: /* GameplayEffect 실행시 호출되는 델리게이트에 등록하기 위한 가상 함수 */
	virtual void	InitAbilityActorInfo();
	void			InitializeDefaultAttributes() const;
	void			ApplyEffectToSelf(const TSubclassOf<UGameplayEffect> GameplayEffectClass, const float level = 1.0f) const;
	
protected:
#pragma region TObjectPtr Note
	// 원시 포인터와 동일하게 작동하지만 에디터에서 액세스 추적 및 선택 적 지연 로드 등 다양한 기능이 추가
	// 액세스 추적: 얼마나 자주 엑세스 되는지, 얼마나 자주 참조 해제되는지 추적 가능
	// 지연 로딩: 프로그램에서 실제 필요하거나 사용할 때까지 에셋을 로드하지 않음을 의미
#pragma endregion
	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

protected: // AuraEnemy 에서만 쓰는데 왜 이걸 CharacterBase에?
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	
protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;
};
