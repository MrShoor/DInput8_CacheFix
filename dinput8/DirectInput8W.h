#pragma once

#include "stdafx.h"
#include <vector>
#include <map>

struct Devs_CacheW {
	DWORD time_idx = 0;
	DWORD dwDevType = 0;
	DWORD dwFlags = 0;
	IDirectInput8W* m_real = nullptr;
	std::vector<DIDEVICEINSTANCEW> devs;

	STDMETHOD(EnumDevices)(DWORD, LPDIENUMDEVICESCALLBACKW, LPVOID, DWORD);

	void Set(DWORD type, DWORD flags, IDirectInput8W* intf);
};

class DirectInput8W : public IDirectInput8W {
private:
	volatile unsigned long long ref_c = 0;
	IDirectInput8W* m_real = nullptr;

	std::map<std::tuple<DWORD, DWORD>, Devs_CacheW> m_devs;
public:
	STDMETHOD(QueryInterface)(REFIID riid, LPVOID* ppvObj);
	STDMETHOD_(ULONG, AddRef)(void);
	STDMETHOD_(ULONG, Release)(void);
	STDMETHOD(CreateDevice)(REFGUID, LPDIRECTINPUTDEVICE8W*, LPUNKNOWN);
	STDMETHOD(EnumDevices)(DWORD, LPDIENUMDEVICESCALLBACKW, LPVOID, DWORD);
	STDMETHOD(GetDeviceStatus)(REFGUID);
	STDMETHOD(RunControlPanel)(HWND, DWORD);
	STDMETHOD(Initialize)(HINSTANCE, DWORD);
	STDMETHOD(FindDevice)(REFGUID, LPCWSTR, LPGUID);
	STDMETHOD(EnumDevicesBySemantics)(LPCWSTR, LPDIACTIONFORMATW, LPDIENUMDEVICESBYSEMANTICSCBW, LPVOID, DWORD);
	STDMETHOD(ConfigureDevices)(LPDICONFIGUREDEVICESCALLBACK, LPDICONFIGUREDEVICESPARAMSW, DWORD, LPVOID);

	DirectInput8W(IDirectInput8W* real_intf);
};
