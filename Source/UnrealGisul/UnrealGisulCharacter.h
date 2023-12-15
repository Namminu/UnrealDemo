// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "UnrealGisulCharacter.generated.h"


class UNiagaraComponent;
class UWidgetComponent;

UCLASS(config=Game)
class AUnrealGisulCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* FireAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ShiftAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* DebugAction;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Weapon;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Player", meta = (AllowPrivateAccess = "true"))
	int32 player_Hp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TimeEffect", meta = (AllowPrivateAccess = "true"))
	UNiagaraComponent* NiagaraComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Player", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* HpBarWidgetComponent;

	FVector StartLocation;
	FVector TargetLocation;


public:
	AUnrealGisulCharacter();
	

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

		
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

	virtual void Tick(float DeltaTime) override;
	// 스폰할 발사체 클래스입니다.
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AMagicProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = Arrow)
	TSubclassOf<class AActor> Arrow;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	// 발사체 발사를 처리하는 함수입니다.
	UFUNCTION()

	void StartFire();

	void Fire();

	void Fire_End();

	void TimeReversal();

	void SaveCoordinates();

	void CoolTimeIsBack();

	void GoingBack(float DeltaTime);
	// 카메라 위치로부터의 총구 오프셋입니다.


	UPROPERTY(EditDefaultsOnly, Category = Pawn)
	UAnimMontage* AttackMontage;

	bool isAttack;

	UPROPERTY(EditDefaultsOnly, Category = Time)
	float DelayTime = 0.5f;

	UFUNCTION(BluePrintCallable, Category = "Player")
	void debug_MinusHP();

private:

	FTimerHandle FireTimerHandle;

	void EffectFunc();

	TArray<FTransform> CharacterTransforms;
	TArray<int> HPList;

	FTimerHandle SaveTimerHandle;
	FTimerHandle CoolTimerHandle;
	float CoolTime = 6;
	float TransformsSize = 240;

	bool isShift = false;
	bool isGoingBack = false;
	float ElapsedTime = 0.0f;

	AActor* SpawnArrow;

	UCharacterMovementComponent* CharacterMovement;
};

