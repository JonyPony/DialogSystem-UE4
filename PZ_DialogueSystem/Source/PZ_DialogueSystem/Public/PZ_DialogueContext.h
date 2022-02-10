// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PZ_DialogueNodesInfo.h"
#include "PZ_DialogueParticipantInfo.h"
#include "PZ_DialogueContext.generated.h"



class UPZ_DialogueSystemComponent;
class UPZ_BaseParticipantNickName;

class UPZ_DialogueEditor;
class UPZ_DialogueRBaseNode;

class UPZ_DialogueService;





UENUM(BlueprintType)
enum class EDialogState : uint8
{
	NotStarted	UMETA(DisplayName = "Not started"),
	InProcess	UMETA(DisplayName = "In process"),
	Paused		UMETA(DisplayName = "Paused")
};





DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDialogStart);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDialogPaused);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDialogContinued);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDialogEnd);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSkippedCurrentPhrases);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnParticipantEnter,   AActor*, Participant,  const FName&, ToRole);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnParticipantLeave,   AActor*, Participant,  const FName&, FromRole);




UCLASS(BlueprintType)
class PZ_DIALOGUESYSTEM_API UPZ_DialogueContext : public UObject
{
	GENERATED_BODY()

	friend UPZ_DialogueSystemComponent;


//constructor
public:

	UPZ_DialogueContext() {}
	UPZ_DialogueContext(UPZ_DialogueEditor* Dialog);
	void InitContext(UPZ_DialogueEditor* Dialog);




//c++ public methods
public:

	bool IsEdgeAlreadyChosen(int32 EdgeIndex) const;

//c++ protected methods
protected:

	virtual void BeginDestroy() override;


	UFUNCTION()
		void IterateDialog();
	bool GoToNextNode();


//c++ private methods
private:

	void SetParticipant(AActor* Participant, UPZ_DialogueSystemComponent* DialogueComponent, const FName& RoleName);
	bool HasEmptyRoleFor(TSubclassOf<UPZ_BaseParticipantNickName> NickName, FName& EmptyRoleName) const;
	bool DoesRoleSupportNickName(const FName& RoleName, TSubclassOf<UPZ_BaseParticipantNickName> NickName) const;

	void OnParticipantEnter(AActor* Participant, const FName& ToRole);
	void OnParticipantLeave(AActor* Participant, const FName& FromRole);




//c++ private values
private:

	UPZ_DialogueRBaseNode* CurrentNode = nullptr;
	EDialogState DialodState;
	bool IsDialogIteratingNow = false;
	TArray<int> OnlyOnceChosenEdges;

	UPZ_DialogueEditor* ImplementDialog = nullptr;
	TMap<FName, FParticipantInstanceInfo> ParticipantsInstance;//key = RoleName
	TArray<FName> ParticipantsWhoIsSaying;
	TArray<UPZ_DialogueService*> ActiveServices;


//>>................................................................................................................................<<//



//Blueprint methods
public:

	UFUNCTION(BlueprintCallable, Category = "DialogueSystem|DialogueContext")
		bool RestartDialog();
	UPROPERTY(BlueprintAssignable, Category = "DialogueSystem|DialogueContext|EventsForBind")
		FOnDialogStart OnDialogStartBind;

	UFUNCTION(BlueprintCallable, Category = "DialogueSystem|DialogueContext")
		void EndDialog();
	UPROPERTY(BlueprintAssignable, Category = "DialogueSystem|DialogueContext|EventsForBind")
		FOnDialogEnd OnDialogEndBind;

	UFUNCTION(BlueprintCallable, Category = "DialogueSystem|DialogueContext")
		void PauseDialog();
	UPROPERTY(BlueprintAssignable, Category = "DialogueSystem|DialogueContext|EventsForBind")
		FOnDialogPaused OnDialogPausedBind;

	UFUNCTION(BlueprintCallable, Category = "DialogueSystem|DialogueContext")
		void ContinueDialog();
	UPROPERTY(BlueprintAssignable, Category = "DialogueSystem|DialogueContext|EventsForBind")
		FOnDialogContinued OnDialogContinuedBind;

	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "DialogueSystem|DialogueContext")
		bool CanStartDialog() const;
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "DialogueSystem|DialogueContext")
		EDialogState GetDialogueState() const;
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "DialogueSystem|DialogueContext")
		bool GetIsDialogStarted() const;





	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "DialogueSystem|DialogueContext")
		bool CanAddParticipant(AActor* ParticipantActor) const;
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "DialogueSystem|DialogueContext")
		bool CanAddParticipantToRole(AActor* ParticipantActor, const FName& RoleName) const;

	UFUNCTION(BlueprintCallable, Category = "DialogueSystem|DialogueContext")
		bool TryAddParticipant(AActor* ParticipantActor);
	UFUNCTION(BlueprintCallable, Category = "DialogueSystem|DialogueContext")
		bool TryAddParticipantToRole(AActor* ParticipantActor, const FName& RoleName);

	UPROPERTY(BlueprintAssignable, Category = "DialogueSystem|DialogueContext|EventsForBind")
		FOnParticipantEnter OnParticipantEnterBind;


	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "DialogueSystem|DialogueContext")
		bool CanRemoveParticipant(AActor* ParticipantActor) const;
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "DialogueSystem|DialogueContext")
		bool CanRemoveParticipantFromRole(const FName& RoleName) const;

	UFUNCTION(BlueprintCallable, Category = "DialogueSystem|DialogueContext")
		bool TryRemoveParticipant(AActor* ParticipantActor);
	UFUNCTION(BlueprintCallable, Category = "DialogueSystem|DialogueContext")
		bool TryRemoveParticipantFromRole(const FName& RoleName);
	UFUNCTION(BlueprintCallable, Category = "DialogueSystem|DialogueContext")
		void CleanParticipants();

	UPROPERTY(BlueprintAssignable, Category = "DialogueSystem|DialogueContext|EventsForBind")
		FOnParticipantLeave OnParticipantLeaveBind;





	UFUNCTION(BlueprintCallable, Category = "DialogueSystem|DialogueContext")
		void SkipCurrentPhrases();
	UPROPERTY(BlueprintAssignable, Category = "DialogueSystem|DialogueContext|EventsForBind")
		FOnSkippedCurrentPhrases OnSkippedCurrentPhrasesBind;

	UFUNCTION(BlueprintCallable, Category = "DialogueSystem|DialogueContext")
		void RequestParticipantChoseNextActions(const FName& ParticipantRoleName, const TArray<UPZ_DialogueRBaseNode*>& PotentialActions, TArray<UPZ_DialogueRBaseNode*>& ChosenNodes);
	
	UFUNCTION(BlueprintCallable, Category = "DialogueSystem|DialogueContext")
		void ExecuteEvents(const TArray<FEventInfo>& Events);

	UFUNCTION(BlueprintCallable, Category = "DialogueSystem|DialogueContext")
		void RunServices(const TArray<UPZ_DialogueService*>& Services);

	UFUNCTION(BlueprintCallable, Category = "DialogueSystem|DialogueContext")
		void StopService(UPZ_DialogueService* Service);
	UFUNCTION(BlueprintCallable, Category = "DialogueSystem|DialogueContext")
		void StopServices(const TArray<UPZ_DialogueService*>& Services);
	UFUNCTION(BlueprintCallable, Category = "DialogueSystem|DialogueContext")
		void StopAllServices();

	UFUNCTION(BlueprintCallable, Category = "DialogueSystem|DialogueContext")
		bool ParticipantStartSaying(const FName& ParticipantRoleName);
	UFUNCTION(BlueprintCallable, Category = "DialogueSystem|DialogueContext")
		void ParticipantFinishSaying(const FName& ParticipantRoleName);


	


	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "DialogueSystem|DialogueContext")
		void GetParticipantsActors(TArray<AActor*>& Participants) const;
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "DialogueSystem|DialogueContext")
		void GetActiveRoles(TArray<FName>& ActiveRoles) const;
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "DialogueSystem|DialogueContext")
		void GetUnusedRoles(TArray<FName>& UnusedRoles) const;
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "DialogueSystem|DialogueContext")
		void GetAllRoles(TArray<FName>& AllRoles) const;
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "DialogueSystem|DialogueContext")
		FName GetActorRole(AActor* Actor) const;
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "DialogueSystem|DialogueContext")
		bool GetRoleInfo(const FName& Role, FParticipantInstanceInfo& RoleInfo) const;


	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "DialogueSystem|DialogueContext")
		bool CheckParticipantAtRoleValid(const FName& RoleName) const;
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "DialogueSystem|DialogueContext")
		bool IsAnybodySaying() const;
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "DialogueSystem|DialogueContext")
		bool IsParticipantSayingNow(const FName& ParticipantRoleName) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "DialogueSystem|DialogueContext")
		void GetAllParticipantsWhoIsSaying(TArray<AActor*>& SayingParticipants) const;
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "DialogueSystem|DialogueContext")
		void GetAllDialogueComponentsWhoIsSaying(TArray<UPZ_DialogueSystemComponent*>& SayingComponents) const;
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "DialogueSystem|DialogueContext")
		TArray<FName> GetAllParticipantsRolesWhoIsSaying() const;



	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "DialogueSystem|DialogueContext")
		UPZ_DialogueEditor* GetImplementedDialog() const;

};

	