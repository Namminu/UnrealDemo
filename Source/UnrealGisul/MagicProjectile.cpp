// Fill out your copyright notice in the Description page of Project Settings.


#include "MagicProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AMagicProjectile::AMagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    if (!RootComponent)
    {
        RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSceneComponent"));
    }

    if (!CollisionComponent)
    {
        // 스피어를 단순 콜리전 표현으로 사용합니다.
        CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
        // 스피어의 콜리전 반경을 설정합니다.
        CollisionComponent->InitSphereRadius(15.0f);
        // 루트 컴포넌트가 콜리전 컴포넌트가 되도록 설정합니다.
        RootComponent = CollisionComponent;
    }

    if (!ProjectileMovementComponent)
    {
        // 이 컴포넌트를 사용하여 이 발사체의 이동을 주도합니다.
        ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
        ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
        ProjectileMovementComponent->InitialSpeed = 3000.0f;
        ProjectileMovementComponent->MaxSpeed = 3000.0f;
        ProjectileMovementComponent->bRotationFollowsVelocity = true;
        ProjectileMovementComponent->bShouldBounce = true;
        ProjectileMovementComponent->Bounciness = 0.3f;
        ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
    }

    if (!ProjectileMeshComponent)
    {
        ProjectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMeshComponent"));
        static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("/Script/Engine.StaticMesh'/Game/Sphere.Sphere'"));
        if (Mesh.Succeeded())
        {
            ProjectileMeshComponent->SetStaticMesh(Mesh.Object);
        }
    }

    static ConstructorHelpers::FObjectFinder<UMaterial>Material(TEXT("/Script/Engine.Material'/Game/SphereMaterial.SphereMaterial'"));
    if (Material.Succeeded())
    {
        ProjectileMaterialInstance = UMaterialInstanceDynamic::Create(Material.Object, ProjectileMeshComponent);
    }
    ProjectileMeshComponent->SetMaterial(0, ProjectileMaterialInstance);
    ProjectileMeshComponent->SetRelativeScale3D(FVector(0.09f, 0.09f, 0.09f));
    ProjectileMeshComponent->SetupAttachment(RootComponent);

    // n초 후 발사체를 제거합니다.
    InitialLifeSpan = 1.0f;
    
}

// Called when the game starts or when spawned
void AMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
    CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AMagicProjectile::OnOverlapBegin);
}

// Called every frame
void AMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMagicProjectile::FireInDirection(const FVector& ShootDirection)
{
    ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}



void AMagicProjectile::Spawn()
{
    // 3초 후 발사체를 제거합니다.
    InitialLifeSpan = 1.0f;
}

void AMagicProjectile::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
    class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("111"));
    Destroy();
}