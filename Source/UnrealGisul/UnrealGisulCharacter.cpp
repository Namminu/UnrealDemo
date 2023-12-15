// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealGisulCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "MagicProjectile.h"
#include <Engine/Classes/Kismet/KismetArrayLibrary.h>
#include "Components/ArrowComponent.h"
#include <Kismet/GameplayStatics.h>
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Components/WidgetComponent.h"
//////////////////////////////////////////////////////////////////////////
// AUnrealGisulCharacter

AUnrealGisulCharacter::AUnrealGisulCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement

	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	player_Hp = 100;

	//Niagara Component
	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiaragaComponent"));
	NiagaraComponent->SetupAttachment(ACharacter::GetMesh());
	NiagaraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem>NiagaraSystemAsset(TEXT("/Game/KTP_Effect/Particles/Bottom/Bottom08-05"));
	if (NiagaraSystemAsset.Succeeded())
	{
		NiagaraComponent->SetAsset(NiagaraSystemAsset.Object);
	}

	//HpBarWidget Component
	HpBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HpBarWZ"));
	//HpBarWidgetComponent->SetWidget(PlayerHPWidget);

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	isAttack = false;
	
}

void AUnrealGisulCharacter::BeginPlay()
{	
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	CharacterMovement = GetCharacterMovement();

	// ó�� Ʈ�������� �迭�� ��簪�� ����
	CharacterTransforms.Init(GetActorTransform(), TransformsSize);
	HPList.Init(player_Hp, TransformsSize);
	GetWorldTimerManager().SetTimer(SaveTimerHandle, this, &AUnrealGisulCharacter::SaveCoordinates, CoolTime / TransformsSize, true);

	NiagaraComponent->Deactivate();    //ó�� �÷��� �ÿ��� ��Ȱ��ȭ �� ���·� ����

	SpawnArrow = GetWorld()->SpawnActor<AActor>(Arrow, GetActorLocation(), GetActorRotation());

}

void AUnrealGisulCharacter::Tick(float DeltaTime)
{
	if (isGoingBack)
	{
		AUnrealGisulCharacter::GoingBack(DeltaTime);
	}
	//�� �����Ӹ��� ĳ���Ͱ� �ð��������� ���ư��� ��ġ Ȯ��

}

//////////////////////////////////////////////////////////////////////////
// Input

void AUnrealGisulCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
			//Jumping
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

			//Moving
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AUnrealGisulCharacter::Move);

			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &AUnrealGisulCharacter::StartFire);
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &AUnrealGisulCharacter::Fire_End);

			EnhancedInputComponent->BindAction(ShiftAction, ETriggerEvent::Triggered, this, &AUnrealGisulCharacter::TimeReversal);

			EnhancedInputComponent->BindAction(DebugAction, ETriggerEvent::Triggered, this, &AUnrealGisulCharacter::debug_MinusHP);

	}
}

void AUnrealGisulCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}


void AUnrealGisulCharacter::StartFire()
{
	if (!isAttack) // �߻� ���� �ƴ� ���� �߻�
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Can Move"));
		PlayAnimMontage(AttackMontage, 1.0f);
		
		// Ÿ�̸Ӹ� ����Ͽ� ���ϴ� �ð��� ���� �Ŀ� ��ü�� ����
		GetWorldTimerManager().SetTimer(FireTimerHandle, this, &AUnrealGisulCharacter::Fire, DelayTime, false);
		isAttack = true;

	}
}


void AUnrealGisulCharacter::Fire()
{
	// �߻�ü �߻縦 �õ��մϴ�.
	if (ProjectileClass && !isAttack && !isGoingBack)
	{
		
		FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 100.0f;

		AMagicProjectile* Projectile = GetWorld()->SpawnActor<AMagicProjectile>(ProjectileClass, SpawnLocation, GetActorRotation());

		if (Projectile)
		{
			// �߻� ���� ���� (ĳ������ ���� ���͸� ���)
			FVector ShootDirection = GetActorForwardVector();

			// �߰����� �Ӽ� ���� (��: �߻� �ӵ�)
			Projectile->FireInDirection(ShootDirection);

			// ������Ÿ�� ����
			Projectile->Spawn();
		}

		/*FTimerHandle TH_Attack_End;
		GetWorldTimerManager().SetTimer(TH_Attack_End, this, &AUnrealGisulCharacter::Fire_End, 1.7f, false);*/
	}
}

void AUnrealGisulCharacter::Fire_End()
{
	isAttack = false;
	//GetWorldTimerManager().ClearTimer(FireTimerHandle);
}

void AUnrealGisulCharacter::TimeReversal()
{
	if (!isShift)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Shift"));
		isShift = true;
		isGoingBack = true;
		// �߷� ��
		CharacterMovement->SetMovementMode(MOVE_Flying);
		GetCapsuleComponent()->SetCollisionProfileName(FName("NoCollision"));
		//SetActorLocation(CharacterTransforms[0].GetLocation());
		GetWorldTimerManager().SetTimer(CoolTimerHandle, this, &AUnrealGisulCharacter::CoolTimeIsBack, CoolTime, false);
		SpawnArrow->SetActorHiddenInGame(true);
		EffectFunc();
	}
}

void AUnrealGisulCharacter::CoolTimeIsBack()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("CoolTimeEnd"));
	SpawnArrow->SetActorHiddenInGame(false);
	isShift = false;
}

void AUnrealGisulCharacter::SaveCoordinates()
{
	//GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, TEXT("123123"));
	for (int i = 0; i < TransformsSize - 1; i++)
	{
		CharacterTransforms[i] = CharacterTransforms[i + 1];
	}
	CharacterTransforms.Insert(GetActorTransform(), TransformsSize - 1);
	SpawnArrow->SetActorLocation(CharacterTransforms[0].GetLocation());

	

}

void AUnrealGisulCharacter::GoingBack(float DeltaTime)
{
	// �̵� ���� ��ġ�� ��ǥ ��ġ ����
	StartLocation = GetActorLocation();
	TargetLocation = CharacterTransforms[0].GetLocation();

	// �̵� �ӵ��� �̵� �ð� ����
	float MoveSpeed = 50.0f;
	float MoveDuration = FVector::Dist(StartLocation, TargetLocation) / MoveSpeed;

	// ��� �ð� ����
	ElapsedTime += DeltaTime;

	// �ð��� ���� ���� ���
	float Alpha = FMath::Clamp(ElapsedTime / MoveDuration, 0.0f, 1.0f);
	FVector NewLocation = FMath::Lerp(StartLocation, TargetLocation, Alpha);

	// ������Ʈ ��ġ ������Ʈ
	SetActorLocation(NewLocation);

	// �̵��� �Ϸ�Ǹ� �̵� ����
	if (ElapsedTime >= MoveDuration)
	{
		isGoingBack = false;
		ElapsedTime = 0;
		// �߷� Ŵ
		CharacterMovement->SetMovementMode(MOVE_Walking);
		GetCapsuleComponent()->SetCollisionProfileName(FName("BlockAll"));
	}
}

//���̾ư��� ����Ʈ Ȱ��ȭ �Լ�
void AUnrealGisulCharacter::EffectFunc()
{
	if (NiagaraComponent)
	{
		NiagaraComponent->Activate();
	}
}

//������ ü�� ��� �Լ�
void AUnrealGisulCharacter::debug_MinusHP()
{
	player_Hp -= 10;
	UE_LOG(LogTemp, Warning, TEXT("Player Hp : %d"), player_Hp);
}