// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MD_InteractInterface.h"
#include "MD_Interactable.generated.h"

class UStaticMeshComponent;

UCLASS()
class MORBUSDEI_API AMD_Interactable : public AActor, public IMD_InteractInterface
{
	GENERATED_BODY()

public:
	AMD_Interactable();

protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Highlight")
	bool bCanHighlight = true;

public:
	virtual void Interact_Implementation(APawn* Interactor) override;
};
