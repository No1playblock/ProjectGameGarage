// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "WeaponActor.h"
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
class PROJECT_GAMEGARAGE_API APlayableCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* WeaponMappingContext;

public:
	// 무기 포인터
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	class AWeaponActor* EquippedWeapon;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LeftClickAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* RightClickAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LeftDodgeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* RightDodgeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ZoomAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess="true"))
	UAnimMontage* LeftPunchMontange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* RightPunchMontange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision", meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* LeftHandCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision", meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* RightHandCollision;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Customizing")
	USkeletalMesh* PlayerMesh;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Customizing")
	UMaterialInstance* PlayerEmo;

public:
	// Sets default values for this character's properties
	APlayableCharacter();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	UFUNCTION()
	void SetPlayerMesh(USkeletalMesh* NewMesh);

	// 서버에서 Mesh를 설정하는 함수
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetPlayerMesh(USkeletalMesh* NewMesh);

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void MulticastSetPlayerMesh(USkeletalMesh* NewMesh);

	// Replicated된 Mesh가 클라이언트에 적용될 때 호출
	UFUNCTION()
	void OnRep_PlayerMesh();

	UFUNCTION()
	void SetPlayerEmo(UMaterialInstance* NewEmo);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetPlayerEmo(UMaterialInstance* NewEmo);

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void MulticastSetPlayerEmo(UMaterialInstance* NewEmo);

	UFUNCTION()
	void OnRep_PlayerEmo();


	UFUNCTION()
	void OnHandCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	
	UFUNCTION(BlueprintCallable)
	void SetHasWeapon(bool ISHASWEAPON);

	UFUNCTION(BlueprintCallable)
	void SetbCanMove(bool isbool);

	UFUNCTION(BlueprintCallable)
	bool GetbCanMove();

	void TakeDamage(float damage);

	void MoveCameraToRangedWeaponPosition();
	void MoveCameraToDefaultPosition();

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

	void LoadAndApplySavedData();

	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void Shoot();

	void Zoom();

	void UpdateScore(float score);
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	bool GetHasWepaon();

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

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
	UUserWidget* CrosshairWidgetInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> CrossHairWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* HPWidget;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "System", meta = (AllowPrivateAccess = "true"))
	uint8 Score;

	bool bHasWeapon;

	float HP;
	float punchAnimSpeed = 2.0f;
	int count = 0;
	bool bCanMove;
	
	FVector DefaultCameraPosition;
	FVector RangedWeaponCameraPosition;
	float CameraDefaultFOV;
	float CameraZoomedFOV;
	int32 ClientID;
};
