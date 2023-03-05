#include "stdafx.h"
#include "DirectInput8W.h"

STDMETHODIMP DirectInput8W::QueryInterface(REFIID riid, LPVOID* ppvObj) {
	return m_real->QueryInterface(riid, ppvObj);
}

STDMETHODIMP_(ULONG) DirectInput8W::AddRef(void) {
	InterlockedIncrement(&ref_c);
	return m_real->AddRef();
}

STDMETHODIMP_(ULONG) DirectInput8W::Release(void) {
	auto new_ref_c = InterlockedDecrement(&ref_c);
	if (!new_ref_c)
		delete this;
	return m_real->Release();
}

STDMETHODIMP DirectInput8W::CreateDevice(REFGUID did, LPDIRECTINPUTDEVICE8W* desc, LPUNKNOWN intf) {
	return m_real->CreateDevice(did, desc, intf);
}

STDMETHODIMP DirectInput8W::EnumDevices(DWORD p1, LPDIENUMDEVICESCALLBACKW p2, LPVOID p3, DWORD p4) {
	auto k = std::make_tuple(p1, p4);
	m_devs[k].Set(p1, p4, m_real);
	return m_devs[k].EnumDevices(p1, p2, p3, p4);
}

STDMETHODIMP DirectInput8W::GetDeviceStatus(REFGUID did) {
	return m_real->GetDeviceStatus(did);
}

STDMETHODIMP DirectInput8W::RunControlPanel(HWND p1, DWORD p2) {
	return m_real->RunControlPanel(p1, p2);
}

STDMETHODIMP DirectInput8W::Initialize(HINSTANCE p1, DWORD p2) {
	return m_real->Initialize(p1, p2);
}

STDMETHODIMP DirectInput8W::FindDevice(REFGUID p1, LPCWSTR p2, LPGUID p3) {
	return m_real->FindDevice(p1, p2, p3);
}

STDMETHODIMP DirectInput8W::EnumDevicesBySemantics(LPCWSTR p1, LPDIACTIONFORMATW p2, LPDIENUMDEVICESBYSEMANTICSCBW p3, LPVOID p4, DWORD p5) {
	return m_real->EnumDevicesBySemantics(p1, p2, p3, p4, p5);
}

STDMETHODIMP DirectInput8W::ConfigureDevices(LPDICONFIGUREDEVICESCALLBACK p1, LPDICONFIGUREDEVICESPARAMSW p2, DWORD p3, LPVOID p4) {
	return m_real->ConfigureDevices(p1, p2, p3, p4);
}

DirectInput8W::DirectInput8W(IDirectInput8W* real_intf) : m_real(real_intf)
{
}

BOOL My_DIEnumDevicesCallback(
	LPCDIDEVICEINSTANCEW lpddi,
	LPVOID pvRef) {
	Devs_CacheW* obj = (Devs_CacheW*)pvRef;
	obj->devs.push_back(*lpddi);
	return DIENUM_CONTINUE;
}

STDMETHODIMP_(HRESULT __stdcall) Devs_CacheW::EnumDevices(DWORD p1, LPDIENUMDEVICESCALLBACKW p2, LPVOID p3, DWORD p4)
{
	DWORD new_idx = GetTickCount() / 30000;
	if (new_idx != time_idx) {
		devs.clear();
		m_real->EnumDevices(p1, My_DIEnumDevicesCallback, this, p4);
		time_idx = new_idx;
	}
	for (const DIDEVICEINSTANCEW& item : devs)
		if (p2(&item, p3) != DIENUM_CONTINUE) break;
	return S_OK;
}

void Devs_CacheW::Set(DWORD type, DWORD flags, IDirectInput8W* intf)
{
	dwDevType = type;
	dwFlags = flags;
	m_real = intf;
}