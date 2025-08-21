// Fill out your copyright notice in the Description page of Project Settings.


#include "NametagWidget.h"

#include "Components/ProgressBar.h"

void UNametagWidget::SetHealthBarPercent(const float Percent) const
{
	ProgressBar->SetPercent(Percent);
}
