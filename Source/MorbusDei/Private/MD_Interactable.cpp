#include "MD_Interactable.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"
#include "Components/WidgetComponent.h"

AMD_Interactable::AMD_Interactable()
{
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Object"));
	RootComponent = Root;
	Root->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Root->SetCollisionResponseToAllChannels(ECR_Block);

	HighlightedObjects = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HighlightedObjects"));
	HighlightedObjects->SetupAttachment(Root);
	HighlightedObjects->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HighlightedObjects->SetGenerateOverlapEvents(false);

	ToggleableObjects = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ToggleableObjects"));
	ToggleableObjects->SetupAttachment(Root);
	ToggleableObjects->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ToggleableObjects->SetGenerateOverlapEvents(false);

	InteractPromptWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractPromptWidget"));
	InteractPromptWidget->SetupAttachment(RootComponent);
	InteractPromptWidget->SetRelativeLocation(FVector(0.f, 0.f, 120.f));
	InteractPromptWidget->SetWidgetSpace(EWidgetSpace::Screen); // or World
	InteractPromptWidget->SetDrawSize(FVector2D(200.f, 200.f));
	InteractPromptWidget->SetVisibility(false);
	InteractPromptWidget->SetPivot(FVector2D(0, 0));
}

void AMD_Interactable::BeginPlay()
{
	Super::BeginPlay();

	if (!HighlightedObjects) return;

	TArray<USceneComponent*> HighlightChildComponents;
	HighlightedObjects->GetChildrenComponents(true, HighlightChildComponents);
	
	for (USceneComponent* Child : HighlightChildComponents)
	{
		if (UPrimitiveComponent* Primitive = Cast<UPrimitiveComponent>(Child))
		{
			Primitive->SetCustomDepthStencilValue(1);
		}
	}
}

void AMD_Interactable::Interact_Implementation(APawn* Interactor)
{
	UE_LOG(LogTemp, Warning, TEXT("%s interacted with %s"),*GetNameSafe(Interactor),*GetNameSafe(this));
	GEngine->AddOnScreenDebugMessage(-1,2.0f,FColor::Green,FString::Printf(TEXT("Interacted with %s"), *GetNameSafe(this)));

	if (!ToggleableObjects) return;
	TArray<USceneComponent*> ToggleChildComponents;
	ToggleableObjects->GetChildrenComponents(true, ToggleChildComponents);

	for (USceneComponent* Child : ToggleChildComponents)
	{
		Child->ToggleVisibility();
	}
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

void AMD_Interactable::Highlight_Implementation(bool bHighlight)
{
	TArray<USceneComponent*> ChildComponents;
	if (HighlightedObjects)
	{
		HighlightedObjects->GetChildrenComponents(true, ChildComponents);

		for (USceneComponent* Child : ChildComponents)
		{
			if (UPrimitiveComponent* Primitive = Cast<UPrimitiveComponent>(Child))
			{
				Primitive->SetRenderCustomDepth(bHighlight);
			}
		}
	}
}