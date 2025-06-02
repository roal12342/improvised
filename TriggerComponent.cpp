#include "TriggerComponent.h"
#include "Camera/CameraActor.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Components/MeshComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"

UTriggerComponent::UTriggerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bHasReturnedToOriginal = false;
	OriginalPlayerPawn = nullptr;
}

void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();

	// 머티리얼 인스턴스 캐싱 (한 번만)
	if (TargetActor)
	{
		TArray<UMeshComponent*> MeshComponents;
		TargetActor->GetComponents<UMeshComponent>(MeshComponents);

		for (UMeshComponent* MeshComp : MeshComponents)
		{
			int32 NumMats = MeshComp->GetNumMaterials();
			for (int32 i = 0; i < NumMats; ++i)
			{
				UMaterialInstanceDynamic* DynMat = MeshComp->CreateAndSetMaterialInstanceDynamic(i);
				if (DynMat)
				{
					DynamicMaterials.Add(DynMat);
					DynMat->SetScalarParameterValue(TEXT("Opacity"), 0.0f);
				}
			}
		}
	}

	OriginalPlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}

void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bHasReturnedToOriginal)
	{
		return; // 한 번 돌아간 이후로는 트리거 재작동 안 함
	}

	if (TargetPawn && TargetPawn->GetActorLocation().Z < 1100.f)
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PlayerController && OriginalPlayerPawn)
		{
			PlayerController->Possess(Cast<APawn>(OriginalPlayerPawn));
			PlayerController->SetViewTargetWithBlend(OriginalPlayerPawn, 1.0f);
			bHasReturnedToOriginal = true;
		}
		return;
	}

	AActor* Actor = GetAcceptableActor();
	if (Actor != nullptr)
	{
		// 플레이어 조종권 넘기기
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PlayerController && TargetPawn && !bHasReturnedToOriginal)
		{
			if (PlayerController->GetPawn() != TargetPawn)
			{
				PlayerController->SetViewTargetWithBlend(TargetCamera, 1.0f);
				PlayerController->Possess(TargetPawn);
			}
		}

		// 물체 붙이기
		UPrimitiveComponent* Component = Cast<UPrimitiveComponent>(Actor->GetRootComponent());
		if (Component != nullptr)
		{
			Component->SetSimulatePhysics(false);
		}
		Actor->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);

		if (Mover)
		{
			Mover->SetShouldMove(true);
		}

		// 투명도 변경
		for (UMaterialInstanceDynamic* DynMat : DynamicMaterials)
		{
			if (DynMat)
			{
				DynMat->SetScalarParameterValue(TEXT("Opacity"), 1.0f);
			}
		}
		if (PlayerController->GetPawn() == TargetPawn)
		{
			if (TargetCamera && PlayerController->GetViewTarget() != TargetCamera)
			{
				PlayerController->SetViewTargetWithBlend(TargetCamera, 0.0f);
			}
		}
		
	}
	else
	{
		if (Mover)
		{
			Mover->SetShouldMove(false);
		}

		// 투명도 원래대로
		for (UMaterialInstanceDynamic* DynMat : DynamicMaterials)
		{
			if (DynMat)
			{
				DynMat->SetScalarParameterValue(TEXT("Opacity"), 0.0f);
			}
		}
	}
}

void UTriggerComponent::SetMover(UMover* NewMover)
{
	Mover = NewMover;
}

AActor* UTriggerComponent::GetAcceptableActor() const
{
	TArray<AActor*> Actors;
	GetOverlappingActors(Actors);

	for (AActor* Actor : Actors)
	{
		bool HasAcceptableTag = Actor->ActorHasTag(AcceptableActorTag);
		bool IsGrabbed = Actor->ActorHasTag("Grabbed");

		UE_LOG(LogTemp, Warning, TEXT("Overlapping Actor: %s | HasTag: %s | IsGrabbed: %s"),
			*Actor->GetName(),
			HasAcceptableTag ? TEXT("TRUE") : TEXT("FALSE"),
			IsGrabbed ? TEXT("TRUE") : TEXT("FALSE")
		);

		if (HasAcceptableTag && !IsGrabbed)
		{
			return Actor;
		}
	}

	return nullptr;
}
