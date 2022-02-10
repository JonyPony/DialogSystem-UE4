// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogueServices/PZ_DialogueService.h"
#include "PZ_DialogueCurveService.generated.h"



class UCurveFloat;



UCLASS(Abstract)
class PZ_DIALOGUESYSTEM_API UPZ_DialogueCurveService : public UPZ_DialogueService
{
	GENERATED_BODY()


//Constructor
public:

	UPZ_DialogueCurveService();




//c++ private values
private:

	float CurrentTime = 0.0f;
	

//>>.................................................................................................................<<//


//Blueprint protected methods
protected:

	virtual void OnServiceBegin_Implementation(UPZ_DialogueContext* DialogueContext) override;
	virtual void OnServiceUpdate_Implementation(float DeltaTime) override;


	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "DialogueSystem|DialogueCurveService")
		void OnCurveValueUpdate(float CurveValue);
		virtual void OnCurveValueUpdate_Implementation(float CurveValue) {}

//Blueprint values
public:

	/*
		Curve to use.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UCurveFloat* Curve = nullptr;

	/*
		When curve ends it will be restarted.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool LoopCurve = false;
	/*
		Multiplyer for curve change step.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float CurveChangeSpeed = 1.0f;

};
