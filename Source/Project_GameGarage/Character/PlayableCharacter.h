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
//enum EScoreType;
struct FInputActionValue;

UCLASS()
class PROJECT_GAMEGARAGE_API APlayableCharacter : public ACharacterBase
{
	GENERATED_BODY()


public:
	// Sets default values for this character's properties

	void MoveCameraToRangedWeaponPosition();

	void MoveCameraToDefaultPosition();

	void TakeDamage(float Damage);

	UFUNCTION(Server, Reliable)
	void Server_LoadAndApplySavedData(USkeletalMesh* NewSkin, UMaterialInstance* NewEmotion);

	//Setter

	void SetHasWeapon(bool bNewHasWeapon);

	FORCEINLINE void SetbCanMove(bool bNewCanMove) { bCanMove = bNewCanMove; }

	FORCEINLINE void SetEmoMaterial(UMaterialInstance* EmoMaterial) { GetMesh()->SetMaterial(1, EmoMaterial); }

	FORCEINLINE void SetSkin(USkeletalMesh* Skin) { GetMesh()->SetSkeletalMeshAsset(Skin); }

	FORCEINLINE void SetBisAttacking(bool bNewIsAttacking) { bIsAttacking = bNewIsAttacking; }

	FORCEINLINE void SetIsLeftPunch(bool bNewIsLeftPunch) { bIsLeftPunch = bNewIsLeftPunch; }

	FORCEINLINE void SetIsRightPunch(bool bNewIsRightPunch) { bIsRightPunch = bNewIsRightPunch; }

	FORCEINLINE void SetEquippedWeapon(AWeaponActor* Weapon) { EquippedWeapon = Weapon; }

	//Getter

	FORCEINLINE UCapsuleComponent* GetLeftHandCollision() const { return LeftHandCollision; }

	FORCEINLINE UCapsuleComponent* GetRightHandCollision() const { return RightHandCollision; }

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	//장착된 무기
	UFUNCTION(BlueprintCallable)
	FORCEINLINE AWeaponActor* GetEquippedWeapon() const { return EquippedWeapon; }

	FORCEINLINE bool GetHasWepaon() const { return bHasWeapon; }

	FORCEINLINE class UUserWidget* GetCrosshairWidgetInstance() const { return CrosshairWidgetInstance; }

	FORCEINLINE bool GetbCanMove() const { return bCanMove; }

	FORCEINLINE bool GetbIsParrying() const { return bIsParrying; }

	FORCEINLINE bool GetbIsStunned() const { return bIsStunned; }

	FORCEINLINE bool GetbParryFailed() const { return bParryFailed; }

	void ApplyStun();

	void EndStun();

private:

	FTimerHandle AutoAttackTimerHandle;

	//내부 함수
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

	UFUNCTION(Server, Reliable)
	void Server_LeftPunch();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_LeftPunch();

	//RightPunchFunction
	UFUNCTION()
	void RightPunch();

	UFUNCTION(Server, Reliable)
	void Server_RightPunch();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_RightPunch();

	//LeftDodgeFunction
	UFUNCTION()
	void LeftDodge();

	UFUNCTION(Server, Reliable)
	void Server_LeftDodge();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_LeftDodge();

	//RightDodgeFunction
	UFUNCTION()
	void RightDodge();

	UFUNCTION(Server, Reliable)
	void Server_RightDodge();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_RightDodge();

	//ParryFunction
	UFUNCTION()
	void Parry();

	UFUNCTION()
	void Local_ParryStart();

	UFUNCTION(Server, Reliable)
	void Server_Parry(float ClientTimestamp);

	void EndParryWindow();

	UFUNCTION()
	void OnRep_bIsParrying();

	UFUNCTION()
	void OnRep_bParryFailed();

	void OnParryMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	void OnExecutionMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ApplyStun();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_EndStun();


	//카메라컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;


	//UI 위젯
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

	UPROPERTY(EditAnywhere, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ParryAction;

	//AnimMontage
	UPROPERTY(EditAnywhere, Category = Animation, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> LeftPunchMontange;

	UPROPERTY(EditAnywhere, Category = Animation, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> RightPunchMontange;

	UPROPERTY(EditAnywhere, Category = Animation, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> ParryMontage;

	UPROPERTY(EditAnywhere, Category = Animation, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> ExecutionMontage;

	UPROPERTY(EditAnywhere, Category = Animation, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> StunMontage;


	UPROPERTY(BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
	bool bIsLeftDodge;

	UPROPERTY(BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
	bool bIsRightDodge;

	UPROPERTY(ReplicatedUsing = OnRep_bIsParrying, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
	bool bIsParrying;

	UPROPERTY(Replicated, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	bool bIsStunned;

	bool bIsLeftPunch;

	bool bIsRightPunch;

	bool bIsZoom;

	bool bIsAttacking;

	bool bIshitted;

	UPROPERTY(ReplicatedUsing = OnRep_bParryFailed)
	bool bParryFailed;

	float ParryGauge;

	UPROPERTY(EditAnywhere, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float MaxParryGauge = 3.0f;

	UPROPERTY(EditAnywhere, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float ParryWindowDuration = 0.4f;

	UPROPERTY(EditAnywhere, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float StunDuration = 5.0f;

	FTimerHandle ParryWindowTimerHandle;

	FTimerHandle StunTimerHandle;

	FTimerHandle StunLogTimerHandle;

	uint8 Score;

	UPROPERTY(Replicated)
	bool bHasWeapon;
	
	float PunchAnimSpeed = 2.0f;

	int Count = 0;

	bool bCanMove;
	
	FVector DefaultCameraPosition = FVector(0.f, 0.f, 130.f);

	FVector RangedWeaponCameraPosition = FVector(0.f, 60.f, 80.f);
	
};
