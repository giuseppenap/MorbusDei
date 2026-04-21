#include "MD_Interactable.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"
#include "Components/WidgetComponent.h"

AMD_Interactable::AMD_Interactable()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Object"));
	RootComponent = Mesh;
	
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Mesh->SetCollisionResponseToAllChannels(ECR_Block);
	Mesh->SetRenderCustomDepth(true);
	Mesh->SetCustomDepthStencilValue(1);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeVisualAsset(TEXT("/Engine/BasicShapes/Cube.Cube"));
	Mesh->SetStaticMesh(CubeVisualAsset.Object);
	static ConstructorHelpers::FObjectFinder<UMaterial> DefaultMatAsset(TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));
	Mesh->SetMaterial(0, DefaultMatAsset.Object);

	InteractPromptWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractPromptWidget"));
	InteractPromptWidget->SetupAttachment(RootComponent);
	InteractPromptWidget->SetRelativeLocation(FVector(0.f, 0.f, 120.f));
	InteractPromptWidget->SetWidgetSpace(EWidgetSpace::Screen); // or World
	InteractPromptWidget->SetDrawSize(FVector2D(200.f, 200.f));
	InteractPromptWidget->SetVisibility(false);
	InteractPromptWidget->SetPivot(FVector2D(0, 0));
}

void AMD_Interactable::Interact_Implementation(APawn* Interactor)
{
	UE_LOG(LogTemp, Warning, TEXT("%s interacted with %s"),*GetNameSafe(Interactor),*GetNameSafe(this));
	
	GEngine->AddOnScreenDebugMessage(
		-1,
		2.0f,
		FColor::Green,
		FString::Printf(TEXT("Interacted with %s"), *GetNameSafe(this))
	);
}

void AMD_Interactable::SetInteractPromptVisible_Implementation(bool bVisible)
{
	if (InteractPromptWidget)
	{
		InteractPromptWidget->SetVisibility(bVisible);
	}
}

bool AMD_Interactable::CanInteract_Implementation() const
{
	return bCanInteract;
}