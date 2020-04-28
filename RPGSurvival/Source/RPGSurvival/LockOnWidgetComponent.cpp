// Fill out your copyright notice in the Description page of Project Settings.


#include "LockOnWidgetComponent.h"

void ULockOnWidgetComponent::ShowInformation(bool Show)
{
	// Widget is already off/on. Do nothing.
	if (Show == IsVisible())
	{
		return;
	}

	SetVisibility(Show);
}