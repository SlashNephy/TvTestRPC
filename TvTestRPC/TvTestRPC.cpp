// TvTestRPC.cpp : DLL アプリケーション用にエクスポートされる関数を定義します。
//

#include "stdafx.h"
#include <time.h>
#include <shlwapi.h>
#include "resource.h"
#include <vector>
#include "discord.h"
#pragma comment(lib, "shlwapi.lib")

class CMyPlugin : public TVTest::CTVTestPlugin
{
	discord::Core* core{};
	
	static LRESULT CALLBACK EventCallback(UINT Event, LPARAM lParam1, LPARAM lParam2, void* pClientData);
	static INT_PTR CALLBACK SettingsDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam, void* pClientData);
	bool ShowDialog(HWND hwndOwner);
	TCHAR m_szIniFileName[MAX_PATH];
	bool conf_ChannelMode = false;
	bool conf_TimeMode = false;
	bool conf_LogoMode = false;
	bool conf_isFinalized = false;
	bool InitSettings();
	bool pluginState = false;
	void SaveConf();
	void UpdateState();
	const std::vector<WORD> knownIds = { 32736, 32737, 32738, 32739, 32740, 32741, 3274, 32742, 32327, 32375, 32391};

public:
	time_t SystemTime2Timet(const SYSTEMTIME&);

	DWORD GetVersion() override
	{
		return TVTEST_PLUGIN_VERSION_(0, 0, 1);
	}

	bool GetPluginInfo(TVTest::PluginInfo* pInfo) override
	{
		pInfo->Type = TVTest::PLUGIN_TYPE_NORMAL;
		pInfo->Flags = TVTest::PLUGIN_FLAG_HASSETTINGS;
		pInfo->pszPluginName = L"TvTest RPC";
		pInfo->pszCopyright = L"(c) 2019-2020 noriokun4649";
		pInfo->pszDescription = L"DiscordRPCをTvTestで実現します。Discordで視聴中のチャンネルなどの情報が通知されます。";
		
		return true;
	}

	bool Initialize() override
	{
		discord::Core::Create(577065126084214816, DiscordCreateFlags_Default, &core);
	    if (!core) {
	        return false;
	    }

		core->SetLogHook(
			discord::LogLevel::Info, [this](discord::LogLevel level, const char* message) {
				TCHAR buf[256];
				MultiByteToWideChar(CP_OEMCP, MB_PRECOMPOSED, message, strlen(message), buf, sizeof buf / 2);
				m_pApp->AddLog(buf);
	    });
	
		m_pApp->SetEventCallback(EventCallback, this);
		return true;
	}
	
	bool Finalize() override
	{
		SaveConf();
		return true;
	}
};

bool CMyPlugin::InitSettings() {
	::GetModuleFileName(g_hinstDLL, m_szIniFileName, MAX_PATH);
	::PathRenameExtension(m_szIniFileName, TEXT(".ini"));
	conf_ChannelMode = ::GetPrivateProfileInt(TEXT("Settings"), TEXT("Mode"), conf_ChannelMode, m_szIniFileName) != 0;
	conf_TimeMode = ::GetPrivateProfileInt(TEXT("Settings"), TEXT("TimeMode"), conf_ChannelMode, m_szIniFileName) != 0;
	conf_LogoMode = ::GetPrivateProfileInt(TEXT("Settings"), TEXT("LogoMode"), conf_ChannelMode, m_szIniFileName) != 0;
	m_pApp->AddLog(m_szIniFileName);
	conf_isFinalized = true;
	return true;
}

void CMyPlugin::UpdateState()
{
	discord::Activity activity{};
    
	TVTest::ProgramInfo Info;
	TVTest::ServiceInfo Service;
	TVTest::ChannelInfo ChannelInfo;
	Info.Size = sizeof(Info);
	Service.Size = sizeof(Service);
	ChannelInfo.Size = sizeof(ChannelInfo);
	wchar_t eventName[128];
	wchar_t eventText[128];
	wchar_t eventExtText[128];
	Info.pszEventName = eventName;
	Info.MaxEventName = sizeof(eventName) / sizeof(eventName[0]);
	Info.pszEventText = eventText;
	Info.MaxEventText = sizeof(eventText) / sizeof(eventText[0]);
	Info.pszEventExtText = eventExtText;
	Info.MaxEventExtText = sizeof(eventExtText) / sizeof(eventExtText[0]);
	std::string channelName;
	std::string eventNamed;

	activity.SetDetails(channelName.c_str());
	activity.SetState(eventNamed.c_str());
	activity.GetAssets().SetLargeText(channelName.c_str());
	
	if (m_pApp->GetCurrentProgramInfo(&Info)) {
		eventNamed = wide_to_utf8(Info.pszEventName);
		
		auto start = SystemTime2Timet(Info.StartTime);
		activity.GetTimestamps().SetStart(start);
		
		if (!conf_TimeMode) {
			auto end = SystemTime2Timet(Info.StartTime) + Info.Duration;
			activity.GetTimestamps().SetEnd(end);
		}
	}

	if (m_pApp->GetServiceInfo(0, &Service) && m_pApp->GetCurrentChannelInfo(&ChannelInfo)) {
		if (!conf_ChannelMode) {
			channelName = wide_to_utf8(Service.szServiceName);
		}
		else {
			channelName = wide_to_utf8(ChannelInfo.szChannelName);
		}

		auto id = ChannelInfo.NetworkID;
		auto isId = find(knownIds.begin(), knownIds.end(), id) != knownIds.end();

		if (isId && conf_LogoMode) {
			auto netId = std::to_string(id);
			activity.GetAssets().SetLargeImage(netId.c_str());
			activity.GetAssets().SetSmallImage("tvtest");
			activity.GetAssets().SetSmallText("TVTest");
		}
		else {
			activity.GetAssets().SetLargeImage("tvtest");
		}
	}

	core->ActivityManager().UpdateActivity(activity, [](discord::Result) {});
	core->RunCallbacks();
}

void CMyPlugin::SaveConf() {
	if (conf_isFinalized) {
		struct IntString {
			IntString(int Value) { ::wsprintf(m_szBuffer, TEXT("%d"), Value); }
			operator LPCTSTR() const { return m_szBuffer; }
			TCHAR m_szBuffer[16];
		};

		::WritePrivateProfileString(TEXT("Settings"), TEXT("Mode"), IntString(conf_ChannelMode), m_szIniFileName);
		::WritePrivateProfileString(TEXT("Settings"), TEXT("TimeMode"), IntString(conf_TimeMode), m_szIniFileName);
		::WritePrivateProfileString(TEXT("Settings"), TEXT("LogoMode"), IntString(conf_LogoMode), m_szIniFileName);
	}
}

time_t CMyPlugin::SystemTime2Timet(const SYSTEMTIME& st)
{
	struct tm gm = { st.wSecond, st.wMinute, st.wHour, st.wDay, st.wMonth - 1, st.wYear - 1900, st.wDayOfWeek, 0, 0 };
	return mktime(&gm);
}

TVTest::CTVTestPlugin* CreatePluginClass()
{
	return new CMyPlugin;
}

bool CMyPlugin::ShowDialog(HWND hwndOwner) {

	TVTest::ShowDialogInfo Info;
	Info.Flags = 0;
	Info.hinst = g_hinstDLL;
	Info.pszTemplate = MAKEINTRESOURCE(IDD_DIALOG1);
	Info.pMessageFunc = SettingsDlgProc;
	Info.pClientData = this;
	Info.hwndOwner = hwndOwner;

	return m_pApp->ShowDialog(&Info) == IDOK;
}

INT_PTR CALLBACK CMyPlugin::SettingsDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam, void* pClientData) {
	CMyPlugin* pThis = static_cast<CMyPlugin*>(pClientData);
	switch (uMsg)
	{
	case WM_INITDIALOG:
		::CheckDlgButton(hDlg, IDC_CHECK1, pThis->conf_ChannelMode ? BST_CHECKED : BST_UNCHECKED);
		::CheckDlgButton(hDlg, IDC_CHECK2, pThis->conf_TimeMode ? BST_CHECKED : BST_UNCHECKED);
		::CheckDlgButton(hDlg, IDC_CHECK3, pThis->conf_LogoMode ? BST_CHECKED : BST_UNCHECKED);
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
			pThis->conf_ChannelMode = ::IsDlgButtonChecked(hDlg, IDC_CHECK1) == BST_CHECKED;
			pThis->conf_TimeMode = ::IsDlgButtonChecked(hDlg, IDC_CHECK2) == BST_CHECKED;
			pThis->conf_LogoMode = ::IsDlgButtonChecked(hDlg, IDC_CHECK3) == BST_CHECKED;
			pThis->UpdateState();
		case IDCANCEL:
			::EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
	default:
		break;
		return FALSE;
	}
	return FALSE;
}

LRESULT CALLBACK CMyPlugin::EventCallback(UINT Event, LPARAM lParam1, LPARAM lParam2, void* pClientData)
{
	CMyPlugin* pThis = static_cast<CMyPlugin*>(pClientData);
	bool fEnable = lParam1 != 0;
	switch (Event)
	{
	case TVTest::EVENT_PLUGINENABLE:
		if (fEnable) {
			pThis->UpdateState();
			pThis->InitSettings();
		}
		else
		{
			pThis->pluginState = false;
			
			pThis->core->ActivityManager().ClearActivity([](discord::Result) {});
		}
		return TRUE;
	case TVTest::EVENT_SERVICECHANGE:
	case TVTest::EVENT_CHANNELCHANGE:
	case TVTest::EVENT_SERVICEUPDATE:
		pThis->UpdateState();
		return TRUE;
	case TVTest::EVENT_PLUGINSETTINGS:
		return pThis->ShowDialog(reinterpret_cast<HWND>(lParam1));
	}
	return 0;
}