// Fill out your copyright notice in the Description page of Project Settings.


#include "NametagWidget.h"

#include "Components/ProgressBar.h"

void UNametagWidget::SetProgressBarPercentage(const float Percentage) const
{
	ProgressBar->SetPercent(Percentage);
}
