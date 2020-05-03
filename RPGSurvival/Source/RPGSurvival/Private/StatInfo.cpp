// Fill out your copyright notice in the Description page of Project Settings.


#include "StatInfo.h"

FStatInfo::FStatInfo()
{
	Name = "";
	
	MaxHP = 0;
	CurrentHP = MaxHP;
	HPPercent = 0;

	MaxMP = 0;
	CurrentMP = MaxMP;
	MPPercent = 0;
}

FStatInfo::~FStatInfo()
{
}
