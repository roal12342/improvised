// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Mover.h"
#include "TriggerComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CRYPTRAID_API UTriggerComponent : public UBoxComponent
{
	GENERATED_BODY()

public:
	UTriggerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void SetMover(UMover* Mover);


private:
	bool bHasTransferredControl = false;
	bool bHasTriggered;
	bool bHasReturnedToOriginal = false;
	AActor* OriginalPlayerPawn = nullptr;


	UPROPERTY(EditAnywhere)
	FName AcceptableActorTag;

	UMover* Mover;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	AActor* TargetActor;

	UPROPERTY(EditAnywhere)
	AActor* TargetCamera;

	UPROPERTY(EditAnywhere)
	APawn* TargetPawn;

	
	TArray<UMaterialInstanceDynamic*> DynamicMaterials;

	AActor* GetAcceptableActor() const;

};
