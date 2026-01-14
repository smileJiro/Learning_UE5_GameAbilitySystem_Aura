


#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interaction/InteractionInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
#pragma region Note: bReplicates 
	// 해당 컨트롤러는 네트워크에서 복제 대상임을 명시한다.
	// bReplicates는 해당 Actor가 서버 -> 클라이언트로 네트워크 복제될지 여부를 결정
	// Playercontroller는 서버 + 자기 자신 클라이언트에 존재
	// 서버에는 다른 모든 Playercontroller가 존재
	// 각 클라이언트에는 자기 Playercontroller만 존재
	// 다른 플레이어의 Playercontroller는 보이지 않는다.
	// 해당 코드의 의미: 이 Playercontroller는 서버에 생성되고, 해당 플레이어의 클라에도 복제된다.
	// 사실 bReplicates = true는 기본값이다 -> 그러나 명시적으로 작성해주는 것
#pragma endregion
	bReplicates = true;
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();

}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	// 아래의 코드는 Server PC에서는 필요가 없는 작업들이야. 로컬에서만 실행되면 돼
	//if (!IsLocalController()) return;

	check(AuraContext);
	
	// 1. Subsystem Connect
#pragma region Note: Subsystem
	// UE Subsystem: 엔진이 관리하는 전역/범위 기반 서비스 객체로 이해
	// Subsystem은 수명이 명확한 서비스 객체이다 -> 어디까지 살아있는가로 구분
	// LocalPlayerSubsystem은 수명이 LocalPlayer의 수명을 따르는거지
	// 정리: 지정한 범위(Scope)안에서 하나이며, 엔진이 책임지고 관리해주는 싱글톤의 보완
#pragma endregion
#pragma region Note: ULocalPlayer::GetSubsystem
	// ULocalPlayer::GetSubsystem -> 존재하면 반환, 없으면 생성해서 반환 (일반적인 싱글톤처럼 보이지만 조금 다르다.)
	// 각각의 LocalPlayer마다 SubsystemCollection을 하나씩 개별 소유한다.
	// SubsystemCollection은 서브시스템을 TMap으로 관리하는 컨테이너 객체이다.
#pragma endregion
	UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if(Subsystem)
	{
		Subsystem->AddMappingContext(AuraContext, 0);
	}
	

	// 2. Mouse Cursor Set
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	FInputModeGameAndUI InputmodeData; // 캐릭터 이동 + UI 조작까지 모두 가능한 모드 설정 구조체
	InputmodeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock); // 마우스 커서를 뷰포트안에 가둘것인가? 
	InputmodeData.SetHideCursorDuringCapture(false); // 마우스 클릭 및 화면 조작시 커서를 숨길 것인가 -> false: 드래그 해도 마우스 안사라짐
	SetInputMode(InputmodeData); // 적용
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// CastChecked: 잘못된 형변환 곧바로 assert
	UEnhancedInputComponent* EnhancedInputComponent =
		CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(
		MoveAction, 
		ETriggerEvent::Triggered,
		this,
		&AAuraPlayerController::Move
	);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	// 1. Controller의 Forward, Right Vector 추출
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	FRotationMatrix YawRotationMatrix = FRotationMatrix(YawRotation);
	const FVector ForwardDirection = YawRotationMatrix.GetUnitAxis(EAxis::X);
	const FVector RightDirection = YawRotationMatrix.GetUnitAxis(EAxis::Y);
#pragma region note
	// 과거 dx11에서는 WorldMatrix에서 Forward Vector를 뽑아오고, Up축 값을 0으로 변경
	// 이후 그 벡터를 Normalize 해서 같은 연산을 수행했었다.
	// FVector Forward = GetControlRotation().Vector();
	// Forward.Z = 0.f;
	// Forward.Normalize();
	// const FVector Right = FVector::CrossProduct(FVector::UpVector, Forward);
#pragma endregion

	// 2. AddMovementInput 호출
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}

}

void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit)
		return;

#pragma region pseudo code
	/**
	* Line trace from cursor. There are several scenarios:
	* A. PrevActor is null && CurActor is null
	*		- Do nothing
	* B. PrevActor is null && CurActor is valid
	*		- Highlight CurActor
	* C. PrevActor is valid && CurActor is null
	*		- UnHighlight PrevActor
	* D. Both actors are valid && PrevActor != CurActor
	*		- UnHighlight PrevActor, and Highlight CurActor
	* E. Both actors are valid && PrevActor == CurActor
	*		- Do nothing
	*/
#pragma endregion
	// 이 코드 마저도 Actor의 vaild 상태를 명확히 체크하진 않고있어
	PrevCursorHitActor = CurCursorHitActor;
	CurCursorHitActor = Cast<IInteractionInterface>(CursorHit.GetActor());
	if (nullptr == PrevCursorHitActor)
	{
		if (nullptr != CurCursorHitActor)
		{
			// Case B
			CurCursorHitActor->HighlightActor();
		}
		else
		{
			// Case A
		}
	}
	else // PrevActor is valid
	{
		if (nullptr == CurCursorHitActor)
		{
			// Case C
			PrevCursorHitActor->UnHighlightActor();
		}
		else // Both actor are vaild
		{
			if (PrevCursorHitActor != CurCursorHitActor)
			{
				// Case D
				PrevCursorHitActor->UnHighlightActor();
				CurCursorHitActor->HighlightActor();
			}
			else
			{
				// Case E
			}
		}
	}

#pragma region my code
	// TODO: isVaiid() 체크, Obj->Implements<UInteractionInterface>() 체크 추가할 것
	//PrevCursorHitActor = CurCursorHitActor;
	//
	//if (CursorHit.bBlockingHit)
	//	CurCursorHitActor = CursorHit.GetActor();
	//else
	//{
	//	CurCursorHitActor = nullptr;
	//	return;
	//}
	//if (PrevCursorHitActor == CurCursorHitActor)
	//	return;
	//
	//if (PrevCursorHitActor)
	//{
	//	PrevCursorHitActor->UnHighlightActor();
	//}
	//
	//if (CurCursorHitActor)
	//{
	//	CurCursorHitActor->HighlightActor();
	//}
#pragma endregion
}		
