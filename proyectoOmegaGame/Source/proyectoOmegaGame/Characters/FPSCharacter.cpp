#include "Characters/FPSCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "InputModifiers.h"

AFPSCharacter::AFPSCharacter()
{
    PrimaryActorTick.bCanEverTick = false;

    GetCapsuleComponent()->InitCapsuleSize(55.0f, 96.0f);

    FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
    FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
    FirstPersonCameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 64.0f));
    FirstPersonCameraComponent->bUsePawnControlRotation = true;

    WeaponMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
    WeaponMeshComponent->SetupAttachment(FirstPersonCameraComponent);
    WeaponMeshComponent->SetRelativeLocation(FVector(30.0f, 15.0f, -15.0f));
    WeaponMeshComponent->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
    WeaponMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    bUseControllerRotationYaw = true;
    bUseControllerRotationPitch = true;
    bUseControllerRotationRoll = false;

    if (UCharacterMovementComponent* Movement = GetCharacterMovement())
    {
        Movement->bOrientRotationToMovement = false;
    }

    InitializeInputMappings();
}

void AFPSCharacter::BeginPlay()
{
    Super::BeginPlay();

    if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
    {
        if (ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
        {
            if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
            {
                if (DefaultMappingContext)
                {
                    Subsystem->AddMappingContext(DefaultMappingContext, 0);
                }
            }
        }
    }
}

void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        if (MoveAction)
        {
            EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFPSCharacter::Move);
        }

        if (LookAction)
        {
            EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFPSCharacter::Look);
        }
    }
}

void AFPSCharacter::InitializeInputMappings()
{
    DefaultMappingContext = NewObject<UInputMappingContext>(this, TEXT("DefaultMappingContext"));
    MoveAction = NewObject<UInputAction>(this, TEXT("MoveAction"));
    LookAction = NewObject<UInputAction>(this, TEXT("LookAction"));

    if (!DefaultMappingContext || !MoveAction || !LookAction)
    {
        return;
    }

    MoveAction->ValueType = EInputActionValueType::Axis2D;
    LookAction->ValueType = EInputActionValueType::Axis2D;

    FEnhancedActionKeyMapping& MoveForward = DefaultMappingContext->MapKey(MoveAction, EKeys::W);
    if (UInputModifierSwizzleAxis* Swizzle = NewObject<UInputModifierSwizzleAxis>(this))
    {
        Swizzle->Order = EInputAxisSwizzle::YXZ;
        MoveForward.Modifiers.Add(Swizzle);
    }

    FEnhancedActionKeyMapping& MoveBackward = DefaultMappingContext->MapKey(MoveAction, EKeys::S);
    if (UInputModifierSwizzleAxis* Swizzle = NewObject<UInputModifierSwizzleAxis>(this))
    {
        Swizzle->Order = EInputAxisSwizzle::YXZ;
        MoveBackward.Modifiers.Add(Swizzle);
    }
    MoveBackward.Modifiers.Add(NewObject<UInputModifierNegate>(this));

    FEnhancedActionKeyMapping& MoveLeft = DefaultMappingContext->MapKey(MoveAction, EKeys::A);
    MoveLeft.Modifiers.Add(NewObject<UInputModifierNegate>(this));

    DefaultMappingContext->MapKey(MoveAction, EKeys::D);
    DefaultMappingContext->MapKey(LookAction, EKeys::Mouse2D);
}

void AFPSCharacter::Move(const FInputActionValue& Value)
{
    const FVector2D MoveValue = Value.Get<FVector2D>();
    if (!MoveValue.IsNearlyZero())
    {
        AddMovementInput(GetActorForwardVector(), MoveValue.Y);
        AddMovementInput(GetActorRightVector(), MoveValue.X);
    }
}

void AFPSCharacter::Look(const FInputActionValue& Value)
{
    const FVector2D LookAxis = Value.Get<FVector2D>();
    AddControllerYawInput(LookAxis.X);
    AddControllerPitchInput(LookAxis.Y);
}
