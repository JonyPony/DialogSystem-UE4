// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueServices/PZ_DialogueCurveService.h"

#include "Curves/CurveFloat.h"




UPZ_DialogueCurveService::UPZ_DialogueCurveService()
{
}






void UPZ_DialogueCurveService::OnServiceBegin_Implementation(UPZ_DialogueContext* DialogueContext)
{
	if( Curve == nullptr )
	{
		StopThisService();
		return;
	}

	CurrentTime = 0.0f;
	OnCurveValueUpdate(Curve->GetFloatValue(CurrentTime));
}

void UPZ_DialogueCurveService::OnServiceUpdate_Implementation(float DeltaTime)
{
	if( Curve == nullptr )
	{
		StopThisService();
		return;
	}


	CurrentTime += DeltaTime * CurveChangeSpeed;

	float CurveMinTime, CurveMaxTime;
	Curve->GetTimeRange(CurveMinTime, CurveMaxTime);
	if( CurrentTime > CurveMaxTime )
	{
		if( LoopCurve )
		{
			CurrentTime = 0.0f;
		}
		else
		{
			StopThisService();
		}
	}


	OnCurveValueUpdate(Curve->GetFloatValue(CurrentTime));
}
