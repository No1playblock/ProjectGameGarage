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


protected:

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> WeaponMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCapsuleComponent> LeftHandCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCapsuleComponent> RightHandCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TObjectPtr<class AWeaponActor> EquippedWeapon;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LeftClickAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> RightClickAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LeftDodgeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> RightDodgeAction;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> LeftPunchMontange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> RightPunchMontange;


private:
	UPROPERTY(ReplicatedUsing= OnRep_SetSkinAndEmotion, VisibleAnywhere, Category = "Customizing")
	TObjectPtr<USkeletalMesh> PlayerMesh;

	UPROPERTY(ReplicatedUsing = OnRep_SetSkinAndEmotion, VisibleAnywhere, Category = "Customizing")
	TObjectPtr<UMaterialInstance> PlayerEmo;


protected:
	APlayableCharacter();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	UFUNCTION()
	void OnHandCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	

public:
	// Sets default values for this character's properties
	
	void MoveCameraToRangedWeaponPosition();
	void MoveCameraToDefaultPosition();
	
	UFUNCTION(BlueprintCallable)
	void SetHasWeapon(bool ISHASWEAPON);

	UFUNCTION(BlueprintCallable)
	void SetbCanMove(bool isbool);

	UFUNCTION(BlueprintCallable)
	bool GetbCanMove();

	void TakeDamage(float damage);

	

	UFUNCTION(BlueprintCallable)
	void SetEmoMaterial(UMaterialInstance* EmoMaterial);

	UFUNCTION(BlueprintCallable)
	void SetSkin(USkeletalMesh* Skin);
	//void SetScore(EScoreType scoreToAdd);

private:
	
	UFUNCTION()
	void LeftPunch();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_LeftPunch();

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multicast_LeftPunch();


	UFUNCTION()
	void RightPunch();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_RightPunch();

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multicast_RightPunch();

	UFUNCTION()
	void LeftDodge();
	UFUNCTION()
	void RightDodge();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_LeftDodge();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_RightDodge();

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multicast_LeftDodge();

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multicast_RightDodge();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
	UFUNCTION()
	void OnRep_SetSkinAndEmotion();
	

	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void Shoot();

	void UpdateScore(float score);
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
public:	

	UFUNCTION(Server, Reliable)
	void Server_LoadAndApplySavedData(USkeletalMesh* NewSkin, UMaterialInstance* NewEmotion);

	FORCEINLINE void SetBisAttacking(bool isattack) { bisAttacking = isattack; }

	FORCEINLINE void SetIsLeftPunch(bool ispunch) { isLeftPunch = ispunch; }

	FORCEINLINE void SetIsRightPunch(bool ispunch) { isRightPunch = ispunch; }

	FORCEINLINE void SetEquippedWeapon(AWeaponActor* weapon)  { EquippedWeapon = weapon; }

	FORCEINLINE UCapsuleComponent* GetLeftHandCollision() const { return LeftHandCollision; }

	FORCEINLINE UCapsuleComponent* GetRightHandCollision() const { return RightHandCollision; }

	FORCEINLINE AWeaponActor* GetEquippedWeapon() const { return EquippedWeapon; }

	FORCEINLINE bool GetHasWepaon() const { return bHasWeapon; }

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	FORCEINLINE class UUserWidget* GetCrosshairWidgetInstance() const { return CrosshairWidgetInstance; }

	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
	bool isLeftPunch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
	bool isRightPunch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
	bool isLeftDodge;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
	bool isRightDodge;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
	bool isZoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
	bool bisAttacking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
	bool bishitted;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UUserWidget> CrosshairWidgetInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> CrossHairWidget;

	

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "System", meta = (AllowPrivateAccess = "true"))
	uint8 Score;

	bool bHasWeapon;
	
	
	/*UPROPERTY(Replicated)
	float HP;*/
	
	float punchAnimSpeed = 2.0f;
	int count = 0;
	bool bCanMove;
	
	FVector DefaultCameraPosition = FVector(0.f, 0.f, 130.f);
	FVector RangedWeaponCameraPosition = FVector(0.f, 60.f, 80.f);
	
};
