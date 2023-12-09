// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "UnrealGisulCharacter.generated.h"


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

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Weapon;

public:
	AUnrealGisulCharacter();
	

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

			

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

	// ������ �߻�ü Ŭ�����Դϴ�.
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AMagicProjectile> ProjectileClass;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	// �߻�ü �߻縦 ó���ϴ� �Լ��Դϴ�.
	UFUNCTION()

	void StartFire();

	void Fire();

	void Fire_End();

	// ī�޶� ��ġ�κ����� �ѱ� �������Դϴ�.


	UPROPERTY(EditDefaultsOnly, Category = Pawn)
	UAnimMontage* AttackMontage;

	bool isAttack;

	UPROPERTY(EditDefaultsOnly, Category = Time)
	float DelayTime = 0.5f;

private:

	FTimerHandle FireTimerHandle;

};

