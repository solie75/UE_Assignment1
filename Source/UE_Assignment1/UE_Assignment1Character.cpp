// Copyright Epic Games, Inc. All Rights Reserved.

#include "UE_Assignment1Character.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
//#include "Math/UnrealMathUtility.h"

AUE_Assignment1Character::AUE_Assignment1Character()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	// Print Init Location
	FVector Location = this->GetActorLocation();
	FString startMessage = FString::Printf(TEXT("Game Start. Player Position : (x : %f, y : %f)"), Location.X, Location.Y);
	UE_LOG(LogTemp, Log, TEXT("%s"), *startMessage);
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, startMessage);
	}

	// Particle
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ExplosionEffect(TEXT("/Script/Engine.ParticleSystem'/Game/StarterContent/Particles/P_Explosion.P_Explosion'"));
	if (ExplosionEffect.Succeeded())
	{
		ExplosionParticle = ExplosionEffect.Object;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Faild to load Explosion Particle System"));
	}
}

void AUE_Assignment1Character::BeginPlay()
{
	Super::BeginPlay();

	// Regist Enhanced Input Subsystem
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Add IMC to Subsystem
			Subsystem->AddMappingContext(IMC, 0);
		}
	}
}

void AUE_Assignment1Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// bind Move()
		EnhancedInputComponent->BindAction(IA_Teleport, ETriggerEvent::Started, this, &AUE_Assignment1Character::Move);
	}
}

void AUE_Assignment1Character::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

void AUE_Assignment1Character::Move()
{
	FIntPoint ip = Step();

	FMath::RandBool() ? CreateEvent() : -1;

	FVector Location = this->GetActorLocation();

	MovedDistance += FMath::Sqrt((float)FMath::Square(100 * ip.X) + (float)FMath::Square(100 * ip.Y));

	Location.X += 100 * ip.X;
	Location.Y += 100 * ip.Y;

	this->SetActorLocation(Location);
	FString positionMessage = FString::Printf(TEXT("Player Position : (x : %f, y : %f)"), Location.X, Location.Y);
	UE_LOG(LogTemp, Log, TEXT("%s"), *positionMessage);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, positionMessage);
	}

	FString distanceMessage = FString::Printf(TEXT("Moved Distance : %f"), MovedDistance);
	UE_LOG(LogTemp, Log, TEXT("%s"), *distanceMessage);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, distanceMessage);
	}

	++MovingCount;
}

FIntPoint AUE_Assignment1Character::Step()
{
	int X = FMath::RandRange(-1, 1);
	int Y = X == 0 ? (FMath::RandBool() ? 1 : -1) : FMath::RandRange(-1, 1);
	return FIntPoint(X, Y);
}

int AUE_Assignment1Character::CreateEvent()
{
	PlayExplosionEffect(this->GetActorLocation());
	return 0;
}

void AUE_Assignment1Character::PlayExplosionEffect(FVector Location)
{
	// Load Explosion Effect.
	
	if (ExplosionParticle)
	{
		// Create ParticleSystemComponent
		UParticleSystemComponent* ParticleComponent = UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			ExplosionParticle,
			Location
		);

		// set particle duration
		if (ParticleComponent)
		{
			ParticleComponent->bAutoDestroy = true;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ExplosionParticle is null! Check if it is properly loaded."));
	}
}
