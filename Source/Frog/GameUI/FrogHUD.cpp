// Fill out your copyright notice in the Description page of Project Settings.


#include "FrogHUD.h"

void UFrogHUD::UpdateScoreText(int NewScore) const
{
	ScoreText->SetText(FText::AsNumber(NewScore));
}
