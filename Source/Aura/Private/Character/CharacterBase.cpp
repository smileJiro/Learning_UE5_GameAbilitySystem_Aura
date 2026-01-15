


#include "Character/CharacterBase.h"
#include "AbilitySystemComponent.h"

ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), "WeaponHandSocket");
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

UAbilitySystemComponent* ACharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAttributeSet* ACharacterBase::GetAttributeSet() const
{
	return AttributeSet;
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACharacterBase::InitAbilityActorInfo()
{
}

void ACharacterBase::InitializePrimaryAttributes() const
{
	/*
	 * 1. Attribute 초기화용 GameplayEffect를 BP로 만들어서 해당 클래스를 DefaultPrimaryAttributes 변수에 등록
	 * 2. 해당 GameplayEffect를 객체 초기화 시점에 적용시켜서 Attribute 초기값을 설정
	 * 3. 해당 함수는 AuraCharacter에서 InitAbilityActorInfo 시점에 호출할 것임 -> 서버에서만 호출되도 상관 없겠지 이 함순 -> 어짜피 Attribute는 복제 될테니가
	 */
	check(IsValid(GetAbilitySystemComponent()));
	check(DefaultPrimaryAttributes);
	
	const FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(DefaultPrimaryAttributes, 1.0f, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent()); // self 써도되는데 연습삼아 target으로 함
}

