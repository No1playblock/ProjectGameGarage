// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Weapon/WeaponActor.h"
#include "Character/CharacterBase.h"
#include "PlayableCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UUserWidget;
class UMaterialInstance;
enum EScoreType;
struct FInputActionValue;

UCLASS()
class PROJECT_GAMEGARAGE_API APlayableCharacter : public ACharacterBase
{
	GENERATED_BODY()


public:
	// Sets default values for this character's properties

	void MoveCameraToRangedWeaponPosition();

	void MoveCameraToDefaultPosition();

	void TakeDamage(float damage);

	UFUNCTION(Server, Reliable)
	void Server_LoadAndApplySavedData(USkeletalMesh* NewSkin, UMaterialInstance* NewEmotion);

	//Setter

	void SetHasWeapon(bool ISHASWEAPON);

	FORCEINLINE void SetbCanMove(bool isbool) { bCanMove = isbool; }

	FORCEINLINE void SetEmoMaterial(UMaterialInstance* EmoMaterial) { GetMesh()->SetMaterial(1, EmoMaterial); }

	FORCEINLINE void SetSkin(USkeletalMesh* Skin) { GetMesh()->SetSkeletalMeshAsset(Skin); }

	FORCEINLINE void SetBisAttacking(bool isattack) { bIsAttacking = isattack; }

	FORCEINLINE void SetIsLeftPunch(bool ispunch) { bIsLeftPunch = ispunch; }

	FORCEINLINE void SetIsRightPunch(bool ispunch) { bIsRightPunch = ispunch; }

	FORCEINLINE void SetEquippedWeapon(AWeaponActor* weapon) { EquippedWeapon = weapon; }

	//Getter
	
	FORCEINLINE UCapsuleComponent* GetLeftHandCollision() const { return LeftHandCollision; }

	FORCEINLINE UCapsuleComponent* GetRightHandCollision() const { return RightHandCollision; }

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	//¸â¹öº¯¼ö
	FORCEINLINE AWeaponActor* GetEquippedWeapon() const { return EquippedWeapon; }

	FORCEINLINE bool GetHasWepaon() const { return bHasWeapon; }

	FORCEINLINE class UUserWidget* GetCrosshairWidgetInstance() const { return CrosshairWidgetInstance; }

	FORCEINLINE bool GetbCanMove() const { return bCanMove; }	

private:

	//»ý¼ºÀÚ
	APlayableCharacter();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void OnHandCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnRep_SetSkinAndEmotion();

	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

	void Shoot();

	void UpdateScore(float score);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	//LeftPunchFunction
	UFUNCTION()
	void LeftPunch();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_LeftPunch();

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multicast_LeftPunch();

	//RightPunchFunction
	UFUNCTION()
	void RightPunch();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_RightPunch();

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multicast_RightPunch();

	//LeftDodgeFunction
	UFUNCTION()
	void LeftDodge();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_LeftDodge();

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multicast_LeftDodge();

	//RightDodgeFunction
	UFUNCTION()
	void RightDodge();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_RightDodge();

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multicast_RightDodge();


	//ÄÄÆ÷³ÍÆ®
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;


	//¸â¹öº¯¼ö
	UPROPERTY()
	TObjectPtr<UUserWidget> CrosshairWidgetInstance;

	UPROPERTY(EditAnywhere, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> CrossHairWidget;


	UPROPERTY(ReplicatedUsing = OnRep_SetSkinAndEmotion)
	TObjectPtr<USkeletalMesh> PlayerMesh;

	UPROPERTY(ReplicatedUsing = OnRep_SetSkinAndEmotion)
	TObjectPtr<UMaterialInstance> PlayerEmo;

	//MappingContext
	UPROPERTY(EditAnywhere, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> WeaponMappingContext;

	UPROPERTY(EditAnywhere, Category = "Collision", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCapsuleComponent> LeftHandCollision;

	UPROPERTY(EditAnywhere, Category = "Collision", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCapsuleComponent> RightHandCollision;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TObjectPtr<class AWeaponActor> EquippedWeapon;

	//InputAction
	UPROPERTY(EditAnywhere, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LeftClickAction;

	UPROPERTY(EditAnywhere, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> RightClickAction;

	UPROPERTY(EditAnywhere, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LeftDodgeAction;

	UPROPERTY(EditAnywhere, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> RightDodgeAction;

	//AnimMontage
	UPROPERTY(EditAnywhere, Category = Animation, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> LeftPunchMontange;

	UPROPERTY(EditAnywhere, Category = Animation, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> RightPunchMontange;


	UPROPERTY(BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
	bool bIsLeftDodge;

	UPROPERTY(BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
	bool bIsRightDodge;

	bool bIsLeftPunch;

	bool bIsRightPunch;

	bool bIsZoom;

	bool bIsAttacking;

	bool bIshitted;


	uint8 Score;

	bool bHasWeapon;
	
	float PunchAnimSpeed = 2.0f;

	int Count = 0;

	bool bCanMove;
	
	FVector DefaultCameraPosition = FVector(0.f, 0.f, 130.f);

	FVector RangedWeaponCameraPosition = FVector(0.f, 60.f, 80.f);
	
};
