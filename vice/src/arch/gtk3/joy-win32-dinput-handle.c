/** \file   joy-win32-dinput-handle.c
 * \brief   Functionality shared by DirectInput joystick and mouse drivers
 *
 * \author  Fabrizio Gennari <fabrizio.ge@tiscali.it>
 */

/*
 * This file is part of VICE, the Versatile Commodore Emulator.
 * See README for copyright notice.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 *  02111-1307  USA.
 *
 */

#include "vice.h"

#include "debug_gtk3.h"

#ifdef WIN32_COMPILE

#ifndef HAVE_GUIDLIB
/* all GUIDs will be defined in this object file */
#define INITGUID
/* else all GUIDs will be defined in the external library dxguid.lib */
#endif

#include "joy-win32-dinput-handle.h"

#ifdef HAVE_DINPUT

/* #include "winmain.h" */

static LPDIRECTINPUT di = NULL;

LPDIRECTINPUT get_directinput_handle(void)
{
#if 0 /* FIXME */
    NOT_IMPLEMENTED();
#else
#ifndef HAVE_DINPUT_LIB
    HRESULT res;
#endif

    if (di == NULL) {

        HINSTANCE winmain_instance = GetModuleHandle(NULL); /* FIXME */

#ifdef HAVE_DINPUT_LIB
        if (DirectInputCreate(winmain_instance, 0x0500, &di, NULL) != DI_OK) {
            di = NULL;
        }
#else
        res = CoCreateInstance(&CLSID_DirectInput, NULL, CLSCTX_INPROC_SERVER, &IID_IDirectInput2A, (PVOID*)&di);
        if (res != S_OK) {
            return NULL;
        }
        if (IDirectInput_Initialize(di, winmain_instance, 0x0500) != S_OK) {
            IDirectInput_Release(di);
            di = NULL;
        }
#endif
    }
#endif /* FIXME */
    return di;
}
#endif
#endif
