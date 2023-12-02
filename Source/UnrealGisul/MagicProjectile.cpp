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
        // ���Ǿ �ܼ� �ݸ��� ǥ������ ����մϴ�.
        CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
        // ���Ǿ��� �ݸ��� �ݰ��� �����մϴ�.
        CollisionComponent->InitSphereRadius(15.0f);
        // ��Ʈ ������Ʈ�� �ݸ��� ������Ʈ�� �ǵ��� �����մϴ�.
        RootComponent = CollisionComponent;
    }

    if (!ProjectileMovementComponent)
    {
        // �� ������Ʈ�� ����Ͽ� �� �߻�ü�� �̵��� �ֵ��մϴ�.
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

    // n�� �� �߻�ü�� �����մϴ�.
    InitialLifeSpan = 1.0f;
}

// Called when the game starts or when spawned
void AMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	
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
    // 3�� �� �߻�ü�� �����մϴ�.
    InitialLifeSpan = 1.0f;
}
