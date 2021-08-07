// Fill out your copyright notice in the Description page of Project Settings.


#include "BlackHoleActor.h"

// Sets default values
ABlackHoleActor::ABlackHoleActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	DestroySphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("DestroySphereComp"));
	DestroySphereComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	SphereComp->SetupAttachment(MeshComp);
	DestroySphereComp->SetupAttachment(MeshComp);

}

// Called when the game starts or when spawned
void ABlackHoleActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABlackHoleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Get components
	TArray<UPrimitiveComponent*> Components;
	SphereComp->GetOverlappingComponents(Components);

	TArray<UPrimitiveComponent*> ComponentsToDestroy;
	DestroySphereComp->GetOverlappingComponents(ComponentsToDestroy);

	// Iterate over components and add force
	for (UPrimitiveComponent* Component : Components) {
		if (Component && Component->IsSimulatingPhysics()) {
			FVector DirectionToAttract = GetActorLocation() - Component->GetComponentLocation();
			FVector Force = DirectionToAttract * ForceStrength;
			//Component->AddForce(Force);
			Component->AddRadialForce(GetActorLocation(), SphereComp->GetUnscaledSphereRadius(), ForceStrength * -1, RIF_Constant,true);
		}
	}

	// Iterate over components and destroy them
	for (UPrimitiveComponent* Component : ComponentsToDestroy) {
		if (Component && Component->IsSimulatingPhysics()) {
			Component->DestroyComponent(false);
		}
	}


}

