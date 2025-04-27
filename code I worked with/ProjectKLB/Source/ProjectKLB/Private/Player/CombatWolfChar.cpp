// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CombatWolfChar.h"

ACombatWolfChar::ACombatWolfChar()
{
	// 데이터 테이블에서 "Wolf" 행을 읽어오기 위해 StatRowName을 "Wolf"로 설정
	PlayerName = FName("Wolf");
}
