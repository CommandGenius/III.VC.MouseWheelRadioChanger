#define GINPUT_COMPILE_CROSSCOMPATIBLE_VERSION
#include "plugin.h"
#include "stdafx.h"
#include "GInputAPI.h"
#include "extensions/ScriptCommands.h"

using namespace plugin;
using namespace std;

#define gGameVersion (*(unsigned int *)0x601048)
#define GTA_3_1_0     0x3A83126F
#define GTA_3_1_1     0x3F8CCCCD
#define GTA_3_STEAM     0x47BDA5
#define GTA_VC_1_0     0x53FF1B8B
#define GTA_VC_STEAM     0xF04F883
#define GTA_VC_1_1     0x783EE8
#define GTA_SA_1_0     0x0
#define GTA_SA_1_1     0x0
#define GTA_SA_steam     0x0

char* gMouseWheelBinding;
char* gMouseWheelDownState;
char* gMouseWheelUpState;

HANDLE HndThread;
IGInputPad* myInterfacePtr;

DWORD WINAPI Thread(LPVOID param)
{
	if (GInput_Load(&myInterfacePtr))
	{

		switch (gGameVersion)
		{
		case GTA_3_1_0:
			gMouseWheelBinding = (char*)0x8F54BC;  //wheel up = 04, down = 05, next radio station in settings
			gMouseWheelDownState = (char*)0x6F1E64;
			gMouseWheelUpState = (char*)0x6F1E63;
			break;

		case GTA_3_1_1:
			gMouseWheelBinding = (char*)0x8F5570;  //wheel up = 04, down = 05, next radio station in settings
			gMouseWheelDownState = (char*)0x6F1E64;
			gMouseWheelUpState = (char*)0x6F1E63;
			break;

		case GTA_3_STEAM:
			gMouseWheelBinding = (char*)0x9056B0;  //wheel up = 04, down = 05, next radio station in settings
			gMouseWheelDownState = (char*)0x701FA4;
			gMouseWheelUpState = (char*)0x701FA3;
			break;

		case GTA_VC_1_0:
			gMouseWheelBinding = (char*)0x9B5B64;  //wheel up = 04, down = 05, next radio station in settings
			gMouseWheelDownState = (char*)0x7DD864;
			gMouseWheelUpState = (char*)0x7DD863;
			break;

		case GTA_VC_1_1:
			gMouseWheelBinding = (char*)0x9B5B6C;  //wheel up = 04, down = 05, next radio station in settings
			gMouseWheelDownState = (char*)0x7DD86C;
			gMouseWheelUpState = (char*)0x7DD86B;
			break;

		case GTA_VC_STEAM:
			gMouseWheelBinding = (char*)0x9B4B6C;  //wheel up = 04, down = 05, next radio station in settings
			gMouseWheelDownState = (char*)0x7DC86C;
			gMouseWheelUpState = (char*)0x7DC86B;
			break;
		}
	}
	return 0;
}




class Radiofix {
public:
	Radiofix() {
		HndThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)&Thread, NULL, 0, NULL);
		Events::processScriptsEvent += []
		{
			bool lockMovement = false;
			CPad* pad = CPad::GetPad(0);
			if (pad->DisablePlayerControls)
				lockMovement = true;
			if (!lockMovement && Command<Commands::IS_PLAYER_SITTING_IN_ANY_CAR>(0))
			{
				if (*gMouseWheelBinding == 4)
				{
					if (*gMouseWheelDownState)
					{
						myInterfacePtr->SendEvent(GINPUT_EVENT_CHANGE_RADIO_STATION, 1);
					}
				}
				if (*gMouseWheelBinding == 5)
				{
					if (*gMouseWheelUpState)
					{
						myInterfacePtr->SendEvent(GINPUT_EVENT_CHANGE_RADIO_STATION, 1);
					}
				}
			}
		};
	}
	~Radiofix() {
		GInput_Release();
	}
} radiofix;