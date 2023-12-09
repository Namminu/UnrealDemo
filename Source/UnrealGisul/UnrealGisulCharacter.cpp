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

//////////////////////////////////////////////////////////////////////////
// AUnrealGisulCharacter

AUnrealGisulCharacter::AUnrealGisulCharacter()
{
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
	if (!isAttack) // 발사 중이 아닐 때만 발사
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Can Move"));
		PlayAnimMontage(AttackMontage, 1.0f);
		
		// 타이머를 사용하여 원하는 시간이 지난 후에 물체를 생성
		GetWorldTimerManager().SetTimer(FireTimerHandle, this, &AUnrealGisulCharacter::Fire, DelayTime, false);
		isAttack = true;
	}
}


void AUnrealGisulCharacter::Fire()
{

	// 발사체 발사를 시도합니다.
	if (ProjectileClass && !isAttack)
	{
		
		FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 100.0f;

		AMagicProjectile* Projectile = GetWorld()->SpawnActor<AMagicProjectile>(ProjectileClass, SpawnLocation, GetActorRotation());

		if (Projectile)
		{
			// 발사 방향 설정 (캐릭터의 전방 벡터를 사용)
			FVector ShootDirection = GetActorForwardVector();

			// 추가적인 속성 설정 (예: 발사 속도)
			Projectile->FireInDirection(ShootDirection);

			// 프로젝타일 스폰
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

