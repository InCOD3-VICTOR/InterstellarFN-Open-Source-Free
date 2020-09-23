#pragma once
#include <windows.h>
#include "structs.h"
#include "_spoofer_stub.h"
#include "object.h"
#include "Offsets_Signatures.h"
#include "imports.h"
#include "Settings.h"

PVOID(*UEEvent)(PVOID, PVOID, PVOID, PVOID) = nullptr;
FVector AimTo;
PVOID Enemy;


PVOID UEEventHook(UObject* object, UObject* func, PVOID params, PVOID result) 
{
	auto objectname = gobj::getobjectname(object);
	auto funcName = gobj::getobjectname(func);

	if (objectname == "" || funcName == "")
		return spoof_call(jmp, UEEvent, (PVOID)object, (PVOID)func, params, result);

	if(Enemy && settings::sniperbullettp)                      
	if (strstr(objectname.c_str(), "B_Prj_Bullet_Sniper") && funcName == "OnRep_FireStart") //check if function is called with bullet
	{
		// x = 0, y = 0, z = 0
		FVector nullvector = { 0 };

		//rootcomponent
		PVOID rootcomp = read(object, offsets::RootComponent);

		//changing firelocation to players head
		*reinterpret_cast<FVector*>((PBYTE)object + offsets::P_FireStartLocation) = AimTo;

		//changing bullet location to players head
		*reinterpret_cast<FVector*>((PBYTE)rootcomp + offsets::RelativeLocation) = AimTo;

		//setting velocity of bullet to 0
		*reinterpret_cast<FVector*>((PBYTE)rootcomp + offsets::ComponentVelocity) = nullvector;
	}

	return spoof_call(jmp, UEEvent, (PVOID)object, (PVOID)func, params, result);
}
