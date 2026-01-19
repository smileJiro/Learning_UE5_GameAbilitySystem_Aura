


#include "AbilitySystem/ModMagCalc/MMC_MaxHealth.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"
/*
 * 1. VigorDef.bSnapshot = false; 로 처리해, GE가 적용되는 시점 Modifier가 동작하면, 그때 관련된 속성을 캡처
 * 2. 관련된 속성을 캡처하기 위해 RelevantAttributesToCapture.Add(VigorDef); 해당 코드 작성
 * 3. CalculateBaseMagnitude_Implementation() 함수 내부에서 MMC 구현
 */
UMMC_MaxHealth::UMMC_MaxHealth()
{
	// PROPERTY_GETTER 매크로 덕분에 UAuraAttributeSet::GetVigorAttribute() 호출 가능하다.
	VigorDef.AttributeToCapture = UAuraAttributeSet::GetVigorAttribute();
	VigorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	
	// true	GameplayEffectSpec 생성 시점	한 번 캡처하고 고정
	// false GameplayEffect 적용/계산 시점	그때그때 최신 값 조회
	VigorDef.bSnapshot = false;
	
	// MMC가 사용할 Attribute 목록을 GAS에게 미리 알려주는 등록 코드
	RelevantAttributesToCapture.Add(VigorDef);
	

}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// 계산 목적: Vigor 값과 Level(이게 문제였어 Attribute가 아니라서)을 활용해 MaxHealth를 계산
	
	// Gather tags from source and taget -> 여기선 사용하지 않으나 교육용
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	
	/*
	 * GAS Aggregator : 속성을 캡처하고 해당 속성의 값을 얻으려면 Aggregator(집계하는 사람)가 필요하다
	 * Aggregator는 특정 Attribute에 영향을 주는 모든 Modifier를 모아서,
	 * 규칙에 따라 계산하고, 그 결과를 필요할 때마다 재평가하는 시스템이다.
	 */

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;
	
	float Vigor = 0.0f;
	
	/*
	 * GetCapturedAttributeMagnitude() : 현재 컨텍스트에서 평가된 값을 가져오는 함수
	 * 
	 * VigorDef : 어떤 Attribute를 캡처할지, Source/Target 어디에서 읽을지, Snapshot 여부 등을 알기 위한 입력
	 * Spec : 이 계산이 어떤 상황에서 일어나는가? 를 알기 위한 입력
	 * 어떤 GE가 적용/계산 중인지, Source/Target ASC 및 EffectContext(Instigator/Target, Ability, Level 등),
	 * (Snapshot이면) Spec 내부에 저장된 캡처 값, SetByCaller, 스택 수, 기간/태그 등
	 * EvaluateParameters : Aggregator가 Modifier들을 적용할 때, 태그 요구조건/필터 등을 ‘평가(Evaluate)’하기 위한 입력
	 * Vigor: 성공 시 Vigor에 결과가 담긴다. 실패시 보통 0이거나 false 반환 흐름을 탄다
	 */
	GetCapturedAttributeMagnitude(VigorDef, Spec, EvaluateParameters,Vigor);
	Vigor = FMath::Max<float>(Vigor, 0.0f);
	
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 PlayerLevel = CombatInterface->GetPlayerLevel();

	return 80.f + (Vigor * 2.5f) + (10.f * (float)PlayerLevel);
}
