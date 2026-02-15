// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#define FLOG(Format, ...) \
UE_LOG(LogTemp, Log, TEXT("[%f] %s"), GetWorld() ? GetWorld()->GetTimeSeconds() : -1.0f, *FString::Printf(Format, ##__VA_ARGS__))