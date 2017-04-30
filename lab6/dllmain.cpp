// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <string>
#include <stdlib.h>

void req1(void) //Changes the message displayed on an illegal move to “Not in this game.” 
{				//strings are stored in a stringtable and are accessed by the loadstring function every time they are needed
	TCHAR sz[] = TEXT("Not in this game"); 
	HANDLE hUpdateRes = BeginUpdateResource((LPCWSTR)"freecell.exe", FALSE);
	UpdateResource(hUpdateRes, RT_STRING, MAKEINTRESOURCE(306), MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT), sz, (lstrlen(sz) + 1));
	EndUpdateResource(hUpdateRes, FALSE);
}

void req2(void) //Sets the total won to 1000 as shown by the Game->Statistics dialog.
{				//values are stored in the registry have to open the registry set values then close the registry
	DWORD number = 0x3E8;//1000
	HKEY key;
	RegOpenKeyEx(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Applets\\FreeCell", 0, KEY_SET_VALUE | KEY_WOW64_64KEY, &key);
	RegSetValueEx(key, L"won", 0, REG_DWORD, (LPBYTE)&number, sizeof(DWORD));
	RegCloseKey(key);
}

void req3(void) //The next valid move wins the game
{									
	*(unsigned int *)0x1007130 = 2; //changed cheat variable to 2 and wins game on next move
}

void req4_5(void) //Change cheat code Ctrl – Shift - F10 to Ctrl - Shift – F6. Creates keybinding Control Shift F2 to win game
{
	LPACCEL myTable = new ACCEL[9];
	myTable[0].cmd = 106;
	myTable[0].key = VK_F1;
	myTable[0].fVirt = FVIRTKEY;
	myTable[1].cmd = 106;
	myTable[1].key = VK_F1;
	myTable[1].fVirt = FVIRTKEY | FSHIFT;
	myTable[2].cmd = 102;
	myTable[2].key = VK_F2;
	myTable[2].fVirt = FVIRTKEY;
	myTable[3].cmd = 103;
	myTable[3].key = VK_F3;
	myTable[3].fVirt = FVIRTKEY;
	myTable[4].cmd = 105;
	myTable[4].key = VK_F4;
	myTable[4].fVirt = FVIRTKEY;
	myTable[5].cmd = 109;
	myTable[5].key = VK_F5;
	myTable[5].fVirt = FVIRTKEY;
	myTable[6].cmd = 115;
	myTable[6].key = VK_F6;
	myTable[6].fVirt = FVIRTKEY;

	myTable[7].cmd = 114;//req 4
	myTable[7].key = VK_F6;//changed control shift f10 to control shift f6
	myTable[7].fVirt = FVIRTKEY | FSHIFT | FCONTROL;

	myTable[8].cmd = 120;//req 5
	myTable[8].key = VK_F2;//added control shift f2
	myTable[8].fVirt = FVIRTKEY | FSHIFT | FCONTROL;

	HACCEL hAccel = CreateAcceleratorTable(myTable, 9);
	HACCEL hAccell = LoadAccelerators(GetModuleHandle(NULL), MAKEINTRESOURCE(hAccel));
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(GetActiveWindow(), hAccell, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		MessageBoxA(0, "Hello World", "Hello Title", 1);
		req1();
		req2();
		req3();
		req4_5();
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

