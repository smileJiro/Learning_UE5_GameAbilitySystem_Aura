


#include "Character/AuraCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"

AAuraCharacter::AAuraCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	// Init ability actor info for the Sever
	InitAbilityActorInfo();
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	// Init ability actor info for the Client
	InitAbilityActorInfo();
	
	// AbilityActorInfo(OwnerActor, AvatarActor)는 Replication되지 않는 로컬 상태이므로
	// 서버와 클라이언트가 각각 자기 ASC에서 InitAbilityActorInfo를 호출해야 한다.
	// 서버/클라의 Actor는 서로 다른 인스턴스(주소)이다.
}

void AAuraCharacter::InitAbilityActorInfo()
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	
	UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AuraPlayerState->GetAbilitySystemComponent());
	AuraASC->InitAbilityActorInfo(AuraPlayerState, this);
	AuraASC->AbilityActorInfoSet();
	
	AbilitySystemComponent = AuraASC;
	AttributeSet = AuraPlayerState->GetAttributeSet();
	
	if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		if(true == AuraPlayerController->IsLocalController())
		{
			if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(AuraPlayerController->GetHUD())) // 그래서 보면 HUD 이게 UI관련인데 UI는 클라에만 생성이되는거지  
			{
				AuraHUD->InitOverlay(AuraPlayerController, AuraPlayerState,AbilitySystemComponent, AttributeSet);
			}
		}
	}
	
	// server에서만 호출되도 될듯함
	InitializeDefaultAttributes();
}
