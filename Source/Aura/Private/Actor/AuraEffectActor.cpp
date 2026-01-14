


#include "Actor/AuraEffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"


// Sets default values
AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;
	
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}

// Called when the game starts or when spawned
void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();

}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayeffectClass)
{
#pragma region 직접 ASCInterface 캐스팅후 ASC 가져오는 방법
	//IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(Target);
	//if (ASCInterface)
	//{
	//	ASCInterface->GetAbilitySystemComponent();
	//}
#pragma endregion
#pragma region UAbilitySystemBlueprintLibrary를 사용한 ASC 가져오기
	// UAbilitySystemBlueprintLibrary를 사용해서 AActor의 ASComponent를 가져오는 방법
	// 내부 타고들어가보면 우리가하는 것처럼 동일하게 ASCInterface로 캐스팅해서 가져옴
	// 다만 추가적으로 nullptr 체크, interface 구현안된경우 actor의 ASC를 직접 뒤지는 작업을해서 리턴해줌
	// 만약 모든 팀원이 ASCinterface를 잘 구현한다면 이걸 안써도돼, 그게아니라면 이게더 확실해
#pragma endregion 
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	
	if(nullptr == TargetASC) 
		return;
	
	check(GameplayeffectClass);
	
	// GameplayEffectspec을 Apply하는 예제 코드
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayeffectClass, ActorLevel, EffectContextHandle);
	const FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
	
	// 교육 목적? 
	const bool bIsInfinite = EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;
	if (bIsInfinite && EEffectRemovalPolicy::RemoveOnEndOverlap == InfiniteEffectRemovalPolicy)
	{
		ActiveEffectHandles.Add(ActiveEffectHandle, TargetASC);
	}
}

void AAuraEffectActor::OnOverlap(AActor* TargetActor)
{
	if (EEffectApplicationPolicy::ApplyOnOverlap == InstantEffectApplicationPolicy)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}
	if (EEffectApplicationPolicy::ApplyOnOverlap == DurationEffectApplicationPolicy)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}
	if (EEffectApplicationPolicy::ApplyOnOverlap == InfiniteEffectApplicationPolicy)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}

}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
	if (EEffectApplicationPolicy::ApplyOnEndOverlap == InstantEffectApplicationPolicy)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}
	if (EEffectApplicationPolicy::ApplyOnEndOverlap == DurationEffectApplicationPolicy)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}
	if (EEffectApplicationPolicy::ApplyOnEndOverlap == InfiniteEffectApplicationPolicy)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}
	
	// 삭제 정책
	if (EEffectRemovalPolicy::RemoveOnEndOverlap == InfiniteEffectRemovalPolicy)
	{
		// TODO:: 전반적으로 이 방식은 교육 목적이 강해보여 데이터 컨테이너 선택부터 일일이 순회하며 키 탐색을 잘 활용못해
		// 이후에 리팩토링 여지가 있어보임 ASC*를 키로 쓰면 안되나?
		// TODO:: 버그1: FireArea_A 과 FireArea_B가 겹쳐있는경우 A 진입 -> B 진입 -> A탈출 -> A,B 효과 모두 제거 (결과적으로 FireArea_B의 영향을  받아야하는데 안받음)
		// 여기선 이 버그를 Stacking으로 해결했어 -> 각각의 infinite effect가 스택을추가하고, endoverlap시 스택을 1개만 제거하는식으로
		// 근데 이건 firearea가 스태킹 방식으로 중첩데미지를 준다는 가정하에 가능한 솔루션이야
		// 만약 스태킹은 1이 고정이면?
		// 아래의 무한 이펙트 제거 정책은 반드시 변경하자 -> 본 프로젝트 할 때에는
		// 45강 사이드 퀘스트: Arrays of each Duration Type
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		
		if (!IsValid(TargetASC)) return;
		
		TArray<FActiveGameplayEffectHandle> HandlesToRemove;
		for (auto [Handle, ASC] : ActiveEffectHandles)
		{
			if (TargetASC == ASC)
			{
				// 실제 해당 gameplay Effect를 제거 -> 무한 gameplay effect 실행 종료
				TargetASC->RemoveActiveGameplayEffect(Handle, 1);
				HandlesToRemove.Add(Handle);
			}
		}
		
		for (auto Handle : HandlesToRemove)
		{
			// 별도 관리하던 stay 상태의 gameplayeffecthandle 제거
			ActiveEffectHandles.FindAndRemoveChecked(Handle);
		}
	}
}
