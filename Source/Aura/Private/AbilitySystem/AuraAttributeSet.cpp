


#include "AbilitySystem/AuraAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/Character.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

UAuraAttributeSet::UAuraAttributeSet()
{
	// 이처럼 생성자에서 Attribute를 초기화하는 방법은 사실 그리 일반적인 방법은 아니다.
	// GameplayEffect를 사용해 초기화하는 것이 가장 많이 사용되는 방식이라고 한다. 
	// 강의에서는 DT를 사용해 초기화하는 방식도 추가로 설명할 예정인 것 같다.
	InitHealth(50.f);
	InitMaxHealth(100.f);
	InitMana(100.f);
	InitMaxMana(200.f);
}

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	// DOREPLIFETIME
	// DOREPLIFETIME_CONDITION
	// DOREPLIFETIME_CONDITION_NOTIFY
	
	/*
	 *  Primary Attributes
	 */
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Resilience, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Vigor, COND_None, REPNOTIFY_Always);
	
	
	/*
	 * Vital Attributes
	 */
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
}

// Epic은 이 함수를 클램핑 기능에만 사용하길 권장한다.
void UAuraAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	
	// PreAttributeChange 시점에서 NewValue는 CurrentValue이다 -> 연산 모디파이어 연산 과정 중 일시적인 값 -> 그래도 클램핑 해주는 것이 안전?
	//TODO:: MaxHealth가 변경되면 그에 맞게 Health도 한번 더 클램핑 해주는 것이 안전하다.
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue, 0.f, GetMaxHealth());
		//UE_LOG(LogTemp, Warning, TEXT("Health: %f"), NewValue);
	}
	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue, 0.f, GetMaxMana());
		//UE_LOG(LogTemp, Warning, TEXT("Mana: %f"), NewValue);
	}
}

void UAuraAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& EffectData, FEffectProperties& Props) const
{
	// FGameplayEffectContextHandle엔 정말 많은 데이터가 있다 -> 이걸 잘 저장해두면 여러 효과를 구현할때 잘 사용한다.
	// Source = causer of the effect, Target = target of the effect (owner of this AS)
	Props.EffectContextHandle = EffectData.EffectSpec.GetContext(); 
	Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	if (IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo.IsValid() && Props.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.SourceAvatarActor = Props.SourceASC->GetAvatarActor(); // == SourceASC->AbilityActorInfo->AvatarActor.Get(); 
		Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();
		if (nullptr == Props.SourceController && nullptr != Props.SourceAvatarActor)
		{
			if (const APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor))
			{
				Props.SourceController = Pawn->GetController();
			}
		}
		if (Props.SourceController)
		{
			Props.SourceCharacter = Cast<ACharacter>(Props.SourceController->GetPawn());
		}
	}
	
	if ( EffectData.Target.AbilityActorInfo.IsValid() && EffectData.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.TargetAvatarActor = EffectData.Target.AbilityActorInfo->AvatarActor.Get();
		Props.TargetController = EffectData.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
		Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
	}
	
}
void UAuraAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& EffectData)
{
	Super::PostGameplayEffectExecute(EffectData);
	
	FEffectProperties Props;
	SetEffectProperties(EffectData,Props);
	
	// 이제 Props를 가지고 다양한 효과들을 구현할수있다.
	// 그러나 주의할 점은 Props 내의 데이터가 항상 전부 채워져있다는 가정은 하면안된다. nullptr일수있어.
	
	
	// Clamp
	if (EffectData.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}
	if (EffectData.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetHealth(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
	}
}

void UAuraAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Strength, OldStrength);
}

void UAuraAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Intelligence, OldIntelligence);
}

void UAuraAttributeSet::OnRep_Resilience(const FGameplayAttributeData& OldResilience) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Resilience, OldResilience);
}

void UAuraAttributeSet::OnRep_Vigor(const FGameplayAttributeData& OldVigor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Vigor, OldVigor);
}

void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	// 이미 Rep은 완료되었어 서버->클라이언트
	// 나는 클라이언트에서 Rep 되었다는 알림을 받고, 이 사실을 GAS에 공식적으로 알릴거야
	// 아래 코드는 GAS 시스템에 공식적으로 Rep 사실을 알리는 코드야
	// 이 통지를 통해 Attribute Change Delegate가 발생하고,
	// 그 결과 UI, Ability 조건, GameplayEffect 반응 등이 정상적으로 갱신된다.
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Health, OldHealth);
	
	// 26.01.09 디버깅 노트
	// GAMEPLAYATTRIBUTE_REPNOTIFY()가 지금까지 호출이 안되고있었어 선언부에서 ReplicatedUsing 을 Replicated로 써서
	// 그래서 2p에서 포션을 먹어도 제대로 반영이 안되었어 GAMEPLAYATTRIBUTE_REPNOTIFY()가 호출이 안되서
	// 1p에서는 잘 됐던 이유가 리슨서버에서 호스트는 자기 자신이 권한있는 서버라서 곧바로 Attribute변경 이벤이 호출 된거야

}

void UAuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxHealth, OldMaxHealth);
}

void UAuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Mana, OldMana);
}

void UAuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxMana, OldMaxMana);
}

