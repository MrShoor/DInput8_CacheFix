#include "stdafx.h"
#include "DirectInput8A.h"
#include "DirectInput8W.h"

HMODULE real_DInput;

typedef HRESULT (APIENTRY* real_DllCanUnloadNow_F)();
typedef HRESULT (APIENTRY* real_DllGetClassObject_F)(REFCLSID rclsid, REFIID riid, LPVOID* ppv);
typedef HRESULT (APIENTRY* real_DirectInput8Create_F)(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID* ppvOut, LPUNKNOWN punkOuter);
typedef	LPCDIDATAFORMAT(APIENTRY* real_GetdfDIJoystick_F)();

real_DllCanUnloadNow_F real_DllCanUnloadNow;
real_DllGetClassObject_F real_DllGetClassObject;
real_DirectInput8Create_F real_DirectInput8Create;
real_GetdfDIJoystick_F real_GetdfDIJoystick;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        real_DInput = LoadLibrary(L"C:\\Windows\\System32\\_dinput8.dll");
        real_DirectInput8Create = (real_DirectInput8Create_F)GetProcAddress(real_DInput, "DirectInput8Create");
        real_DllCanUnloadNow = (real_DllCanUnloadNow_F)GetProcAddress(real_DInput, "DllCanUnloadNow");
        real_DllGetClassObject = (real_DllGetClassObject_F)GetProcAddress(real_DInput, "DllGetClassObject");
        real_GetdfDIJoystick = (real_GetdfDIJoystick_F)GetProcAddress(real_DInput, "GetdfDIJoystick");
        break;

    case DLL_THREAD_ATTACH:
        break;

    case DLL_THREAD_DETACH:
        break;

    case DLL_PROCESS_DETACH:
        FreeLibrary(real_DInput);
        break;
    }
    return TRUE;
}

static const GUID GUID_IDirectInput8A =
{ 0xBF798030, 0x483A, 0x4DA2, { 0xAA, 0x99, 0x5D, 0x64, 0xED, 0x36, 0x97, 0x00} };
static const GUID GUID_IDirectInput8W =
{ 0xBF798031, 0x483A, 0x4DA2, { 0xAA, 0x99, 0x5D, 0x64, 0xED, 0x36, 0x97, 0x00} };

HRESULT APIENTRY DirectInput8Create(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID *ppvOut, LPUNKNOWN punkOuter) {
    if (GUID_IDirectInput8A == riidltf) {
        LPVOID intf = nullptr;
        HRESULT hr = real_DirectInput8Create(hinst, dwVersion, riidltf, &intf, punkOuter);
        if (FAILED(hr)) return hr;
        *ppvOut = (LPVOID) new DirectInput8A((IDirectInput8A*)intf);
        return S_OK;
    }

    if (GUID_IDirectInput8W == riidltf) {
        LPVOID intf = nullptr;
        intptr_t* p = (intptr_t*)&real_DirectInput8Create;
        HRESULT hr = real_DirectInput8Create(hinst, dwVersion, riidltf, &intf, punkOuter);
        if (FAILED(hr)) return hr;
        *ppvOut = (LPVOID) new DirectInput8W((IDirectInput8W*)intf);
        return S_OK;
    }

    return real_DirectInput8Create(hinst, dwVersion, riidltf, ppvOut, punkOuter);
}

HRESULT DllCanUnloadNow() {
    return real_DllCanUnloadNow();
}

HRESULT DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv) {
    return real_DllGetClassObject(rclsid, riid, ppv);
}

LPCDIDATAFORMAT GetdfDIJoystick() {
    return real_GetdfDIJoystick();
}