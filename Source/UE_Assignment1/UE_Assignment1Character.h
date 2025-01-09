// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UE_Assignment1Character.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS(Blueprintable)
class AUE_Assignment1Character : public ACharacter
{
	GENERATED_BODY()

public:
	AUE_Assignment1Character();

	
protected:
	virtual void BeginPlay() override;


public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	UFUNCTION()
	void Move();

	UFUNCTION()
	FIntPoint Step();

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputMappingContext* IMC;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* IA_Teleport;

	//UPROPERTY(VisibleAnywhere, Category = "")
	float MovedDistance = 0.f;

	int MovingCount = 0;

	UFUNCTION()
	int CreateEvent();

	void PlayExplosionEffect(FVector Location);
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	UParticleSystem* ExplosionParticle;
};

