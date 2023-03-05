#pragma once

#include "stdafx.h"
#include <vector>
#include <map>

struct Devs_CacheA {
	DWORD time_idx = 0;
	DWORD dwDevType = 0;
	DWORD dwFlags = 0;
	IDirectInput8A* m_real = nullptr;
	std::vector<DIDEVICEINSTANCEA> devs;

	STDMETHOD(EnumDevices)(DWORD, LPDIENUMDEVICESCALLBACKA, LPVOID, DWORD);

	void Set(DWORD type, DWORD flags, IDirectInput8A* intf);
};

class DirectInput8A : public IDirectInput8A {
private:
	volatile unsigned long long ref_c = 0;
	IDirectInput8A* m_real = nullptr;

	std::map<std::tuple<DWORD, DWORD>, Devs_CacheA> m_devs;
public:
	STDMETHOD(QueryInterface)(REFIID riid, LPVOID *ppvObj);
	STDMETHOD_(ULONG, AddRef)(void);
	STDMETHOD_(ULONG, Release)(void);
	STDMETHOD(CreateDevice)(REFGUID, LPDIRECTINPUTDEVICE8A *, LPUNKNOWN);
	STDMETHOD(EnumDevices)(DWORD, LPDIENUMDEVICESCALLBACKA, LPVOID, DWORD);
	STDMETHOD(GetDeviceStatus)(REFGUID);
	STDMETHOD(RunControlPanel)(HWND, DWORD);
	STDMETHOD(Initialize)(HINSTANCE, DWORD);
	STDMETHOD(FindDevice)(REFGUID, LPCSTR, LPGUID);
	STDMETHOD(EnumDevicesBySemantics)(LPCSTR, LPDIACTIONFORMATA, LPDIENUMDEVICESBYSEMANTICSCBA, LPVOID, DWORD);
	STDMETHOD(ConfigureDevices)(LPDICONFIGUREDEVICESCALLBACK, LPDICONFIGUREDEVICESPARAMSA, DWORD, LPVOID);

	DirectInput8A(IDirectInput8A* real_intf);
};
