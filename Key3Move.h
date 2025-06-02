#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Key3Move.generated.h"

UCLASS()
class CRYPTRAID_API AKey3Move : public APawn
{
	GENERATED_BODY()

public:
	AKey3Move();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void MoveForward(float Value);
	void MoveRight(float Value);

	UPROPERTY(VisibleAnywhere)
	class UFloatingPawnMovement* MovementComponent;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* MeshComponent;
};