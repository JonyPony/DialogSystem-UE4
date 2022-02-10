// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PZ_DialogueParticipantInfo.generated.h"



class UPZ_DialogueSystemComponent;
class UPZ_BaseParticipantNickName;



UENUM(BlueprintType)
enum class ERoleTypeVisualization : uint8
{
	None			UMETA(DisplayName = "None"),
	Character		UMETA(DisplayName = "Character"),
	Dwarf			UMETA(DisplayName = "Dwarf"),
	Viking			UMETA(DisplayName = "Viking"),
	VikingHelmet	UMETA(DisplayName = "Viking Helmet"),
	SpartanHelmet	UMETA(DisplayName = "Spartan Helmet"),
	Skeleton		UMETA(DisplayName = "Skeleton"),
	Orc				UMETA(DisplayName = "Orc"),
	Vampire			UMETA(DisplayName = "Vampire"),
	Overlord		UMETA(DisplayName = "Overlord"),
	Wolf			UMETA(DisplayName = "Wolf"),
	Warrior			UMETA(DisplayName = "Warrior"),
	MountedKnight	UMETA(DisplayName = "Mounted Knight"),
	Farmer			UMETA(DisplayName = "Farmer"),
	Caesar			UMETA(DisplayName = "Caesar"),
	DaggerRose		UMETA(DisplayName = "DaggerRose"),
	Rogue			UMETA(DisplayName = "Rogue"),
	Transfuse		UMETA(DisplayName = "Transfuse")

};

USTRUCT(BlueprintType)
struct PZ_DIALOGUESYSTEM_API FRoleVisualization
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly)
		ERoleTypeVisualization RoleTypeVisualization;
	UPROPERTY(EditDefaultsOnly)
		FLinearColor Color = FLinearColor(1, 1, 1, 1);
};




USTRUCT(BlueprintType)
struct PZ_DIALOGUESYSTEM_API FRoleChoseInfo
{
	GENERATED_USTRUCT_BODY()

	/*
		A chance to take this role as a ParticipantName.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "1", UIMin = "1"))
		int Priority = 1;
	/*
		If true then all children of NickName class can take this role.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		bool SupportNickNameClassChildren = false;
};

USTRUCT(BlueprintType)
struct PZ_DIALOGUESYSTEM_API FParticipantInfo
{
	GENERATED_USTRUCT_BODY()

	/*
		Priority of this role in dialog.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "1", UIMin = "1"))
		int RolePriority = 1;
	/*
		NickNames of the participants (will be taken from the component) who can take this role.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TMap<TSubclassOf<UPZ_BaseParticipantNickName>, FRoleChoseInfo> CanBeChosenBy;

	/*
		Is it possible to select this role after the dialog starts.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		bool CanJoinToDialogAfterStart = false;
	/*
		Can participant on this role leave from dialog after start.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		bool CanLeaveFromDialog = false;
	/*
		Is this participant necessary to start the dialogue.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		bool IsNecessaryForStart = true; 
	/*
		Will end the dialog if this participant leaves it.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		bool EndDialogWhenLeave = false;



#if WITH_EDITORONLY_DATA

	/*
		Icon of this role for dialog editor.
	*/
	UPROPERTY(EditDefaultsOnly)
		FRoleVisualization RoleVisualization;

#endif

};





USTRUCT(BlueprintType)
struct PZ_DIALOGUESYSTEM_API FParticipantInstanceInfo
{

	GENERATED_USTRUCT_BODY()

public:

	FParticipantInstanceInfo() {}
	FParticipantInstanceInfo(FParticipantInfo* InParticipantInfo) :
		ParticipantInfo(InParticipantInfo)
	{

	}


public:

	FParticipantInfo const* ParticipantInfo = nullptr;



public:

	UPROPERTY(BlueprintReadOnly)
		AActor* ParticipantActor = nullptr;
	UPROPERTY(BlueprintReadOnly)
		UPZ_DialogueSystemComponent* ParticipantDialogueComponent = nullptr;

};