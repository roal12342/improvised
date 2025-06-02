#include "Key3Move.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/StaticMeshComponent.h"

AKey3Move::AKey3Move()
{
	PrimaryActorTick.bCanEverTick = true;

	// 메쉬 생성 및 설정
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;

	// 이동 컴포넌트 추가
	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComponent"));
	MovementComponent->UpdatedComponent = RootComponent;

	// 처음엔 플레이어가 조종 안 함
	AutoPossessPlayer = EAutoReceiveInput::Disabled;
}

void AKey3Move::BeginPlay()
{
	Super::BeginPlay();
}

void AKey3Move::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AKey3Move::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AKey3Move::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AKey3Move::MoveRight);
}

void AKey3Move::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AKey3Move::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorRightVector(), Value);
	}
}