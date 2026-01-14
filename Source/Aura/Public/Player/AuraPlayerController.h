

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"


class UInputMappingContext;
class UInputAction;
class IInteractionInterface;

struct FInputActionValue;
/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AAuraPlayerController();

public:
	void PlayerTick(float DeltaTime) override;

protected:
	void BeginPlay() override;
	// Pawn::SetupPlayerInputComponent(UInputComponent* param) 와의 차이를 인지하자.
	void SetupInputComponent() override; 

private:
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> AuraContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

private:
	void Move(const FInputActionValue& InputActionValue);
	void CursorTrace();

private:
	IInteractionInterface* PrevCursorHitActor = nullptr;
	IInteractionInterface* CurCursorHitActor = nullptr;

};
