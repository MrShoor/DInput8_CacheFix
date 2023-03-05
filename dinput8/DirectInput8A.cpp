#include "stdafx.h"
#include "DirectInput8A.h"
#include <string>

STDMETHODIMP DirectInput8A::QueryInterface(REFIID riid, LPVOID *ppvObj) {
	return m_real->QueryInterface(riid, ppvObj);
}

STDMETHODIMP_(ULONG) DirectInput8A::AddRef(void) {
	InterlockedIncrement(&ref_c);
	return m_real->AddRef();
}

STDMETHODIMP_(ULONG) DirectInput8A::Release(void) {
	auto new_ref_c = InterlockedDecrement(&ref_c);
	if (!new_ref_c)
		delete this;
	return m_real->Release();
}

STDMETHODIMP DirectInput8A::CreateDevice(REFGUID did, LPDIRECTINPUTDEVICE8A * desc, LPUNKNOWN intf) {
	return m_real->CreateDevice(did, desc, intf);
}

STDMETHODIMP DirectInput8A::EnumDevices(DWORD p1, LPDIENUMDEVICESCALLBACKA p2, LPVOID p3, DWORD p4) {
	auto k = std::make_tuple(p1, p4);
	m_devs[k].Set(p1, p4, m_real);
	return m_devs[k].EnumDevices(p1, p2, p3, p4);
}

STDMETHODIMP DirectInput8A::GetDeviceStatus(REFGUID did) {
	return m_real->GetDeviceStatus(did);
}

STDMETHODIMP DirectInput8A::RunControlPanel(HWND p1, DWORD p2) {
	return m_real->RunControlPanel(p1, p2);
}

STDMETHODIMP DirectInput8A::Initialize(HINSTANCE p1, DWORD p2) {
	return m_real->Initialize(p1, p2);
}

STDMETHODIMP DirectInput8A::FindDevice(REFGUID p1, LPCSTR p2, LPGUID p3) {
	return m_real->FindDevice(p1, p2, p3);
}

STDMETHODIMP DirectInput8A::EnumDevicesBySemantics(LPCSTR p1, LPDIACTIONFORMATA p2, LPDIENUMDEVICESBYSEMANTICSCBA p3, LPVOID p4, DWORD p5) {
	return m_real->EnumDevicesBySemantics(p1, p2, p3, p4, p5);
}

STDMETHODIMP DirectInput8A::ConfigureDevices(LPDICONFIGUREDEVICESCALLBACK p1, LPDICONFIGUREDEVICESPARAMSA p2, DWORD p3, LPVOID p4) {
	return m_real->ConfigureDevices(p1, p2, p3, p4);
}

DirectInput8A::DirectInput8A(IDirectInput8A* real_intf) : m_real(real_intf)
{
}

BOOL My_DIEnumDevicesCallback(
	LPCDIDEVICEINSTANCEA lpddi,
	LPVOID pvRef) {
	Devs_CacheA* obj = (Devs_CacheA*)pvRef;
	obj->devs.push_back(*lpddi);
	return DIENUM_CONTINUE;
}

STDMETHODIMP_(HRESULT __stdcall) Devs_CacheA::EnumDevices(DWORD p1, LPDIENUMDEVICESCALLBACKA p2, LPVOID p3, DWORD p4)
{
	DWORD new_idx = GetTickCount() / 30000;
	if (new_idx != time_idx) {
		devs.clear();
		m_real->EnumDevices(p1, My_DIEnumDevicesCallback, this, p4);
		time_idx = new_idx;
	}
	for (const DIDEVICEINSTANCEA& item : devs) {
		if (p2(&item, p3) != DIENUM_CONTINUE) break;
	}
	return S_OK;
}

void Devs_CacheA::Set(DWORD type, DWORD flags, IDirectInput8A* intf)
{
	dwDevType = type;
	dwFlags = flags;
	m_real = intf;
}
