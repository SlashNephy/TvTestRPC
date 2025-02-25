#include "stdafx.h"

#include "Config.h"
#include "Activity.h"

constexpr auto TvTestRPCWindowClass = L"TvTestRPC Window";
constexpr auto UpdateActivityTimerId = WM_APP + 1;
constexpr auto UpdateActivityTimerIntervalMs = 3000;
constexpr auto RunCallbacksTimerId = WM_APP + 2;
constexpr auto RunCallbacksTimerIntervalMs = 100;

class CTvTestRPCPlugin final : public TVTest::CTVTestPlugin
{
    IDiscordCore* m_discord{};
    IDiscordActivityManager* m_activities{};
    DiscordActivity m_lastActivity{};

    wchar_t m_iniFileName[MAX_PATH]{};
    HWND m_hwnd{};
    std::mutex m_mutex;
    bool m_isReady = false;

    Config m_config{};

    void LoadConfig();

    void InitializeDiscord();
    void UpdateActivity();

    static LRESULT CALLBACK EventCallback(UINT Event, LPARAM lParam1, LPARAM lParam2, void* pClientData);
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
    /*
     * プラグインの API バージョンを返す
     */
    DWORD GetVersion() override
    {
        // TVTest ver.0.6.0 or later
        return TVTEST_PLUGIN_VERSION_(0, 0, 8);
    }

    /*
     * プラグインの情報を返す
     */
    bool GetPluginInfo(TVTest::PluginInfo* pInfo) override
    {
        pInfo->Type = TVTest::PLUGIN_TYPE_NORMAL;
        pInfo->Flags = 0;
        pInfo->pszPluginName = L"Discord Rich Presence";
        pInfo->pszCopyright = L"© 2025 SlashNephy, 2019-2020 noriokun4649";
        pInfo->pszDescription = L"Discord Rich Presence を表示します。";

        return true;
    }

    /*
     * プラグインの初期化を行う
     */
    bool Initialize() override
    {
        // TVTest イベントコールバックの設定
        m_pApp->SetEventCallback(EventCallback, this);

        // ウィンドウクラスの登録
        WNDCLASS wc;
        wc.style = 0;
        wc.lpfnWndProc = WndProc;
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = g_hinstDLL;
        wc.hIcon = nullptr;
        wc.hCursor = nullptr;
        wc.hbrBackground = nullptr;
        wc.lpszMenuName = nullptr;
        wc.lpszClassName = TvTestRPCWindowClass;
        if (::RegisterClass(&wc) == 0)
        {
            return false;
        }

        // ウィンドウの作成
        m_hwnd = ::CreateWindowEx(
            0, TvTestRPCWindowClass, nullptr, WS_POPUP,
            0, 0, 0, 0, HWND_MESSAGE, nullptr, g_hinstDLL, this
        );
        if (m_hwnd == nullptr)
        {
            return false;
        }

        return true;
    }

    /*
     * プラグインの終了処理を行う
     */
    bool Finalize() override
    {
        // Discord クライアントの破棄
        if (m_isReady)
        {
            m_discord->destroy(m_discord);
        }

        // タイマー・ウィンドウの破棄
        ::KillTimer(m_hwnd, UpdateActivityTimerId);
        ::DestroyWindow(m_hwnd);

        return true;
    }
};

/*
 * プラグインのインスタンスを作成する
 */
TVTest::CTVTestPlugin* CreatePluginClass()
{
    return new CTvTestRPCPlugin;
}

/*
 * 設定を読み込む
 */
void CTvTestRPCPlugin::LoadConfig()
{
    ::GetModuleFileName(g_hinstDLL, m_iniFileName, MAX_PATH);
    ::PathRenameExtension(m_iniFileName, L".ini");

    const auto settings = GetPrivateProfileSectionBuffer(L"Settings", m_iniFileName);
    m_config.ShowEndTime = GetBufferedProfileInt(settings.data(), L"ShowEndTime", m_config.ShowEndTime) > 0;
    m_config.ShowChannelLogo = GetBufferedProfileInt(settings.data(), L"ShowChannelLogo", m_config.ShowChannelLogo) > 0;
    m_config.ConvertToHalfWidth = GetBufferedProfileInt(settings.data(), L"ConvertToHalfWidth", m_config.ConvertToHalfWidth) > 0;
    m_config.IgnoreTuningSpace = GetBufferedProfileInt(settings.data(), L"IgnoreTuningSpace", m_config.IgnoreTuningSpace) > 0;

    const auto logos = GetPrivateProfileSectionBuffer(L"Logos", m_iniFileName);
    for (auto p = logos.data(); *p; p += wcslen(p) + 1) {
        WORD serviceId;

        if (char logoKey[MaxImageKeyLength]; swscanf_s(p, L"%hd=%S", &serviceId, logoKey, MaxImageKeyLength) == 2)
		{
            m_config.Logos[serviceId] = logoKey;
		}
	}
}

/*
 * Discord クライアントを初期化する
 */
void CTvTestRPCPlugin::InitializeDiscord()
{
    if (m_isReady)
    {
        m_discord->destroy(m_discord);
    }

    DiscordCreateParams params{};
    DiscordCreateParamsSetDefault(&params);
    params.client_id = 844540020286685184;
    params.flags = DiscordCreateFlags_NoRequireDiscord;

    if (DiscordCreate(DISCORD_VERSION, &params, &m_discord) == DiscordResult_Ok)
    {
        m_activities = m_discord->get_activity_manager(m_discord);
        m_isReady = true;

        m_pApp->AddLog(L"Discord との接続に成功しました。");
    }
    else
    {
        m_isReady = false;

        m_pApp->AddLog(L"Discord との接続に失敗しました。Discord が起動していない可能性があります。");
    }
}

/*
 * Discord Rich Presence を更新する
 */
void CTvTestRPCPlugin::UpdateActivity()
{
    if (!m_isReady)
    {
        return;
    }

    // ロック
    {
        std::lock_guard lock(m_mutex);

        // TuningSpace
        std::optional<TuningSpace> tuningSpace = std::nullopt;
        TVTest::TuningSpaceInfo TuningSpace{};
        if (!m_config.IgnoreTuningSpace && m_pApp->GetTuningSpaceInfo(m_pApp->GetTuningSpace(), &TuningSpace))
        {
            tuningSpace = GetTuningSpaceByName(TuningSpace.szName);
            if (!tuningSpace.has_value())
            {
                tuningSpace = GetTuningSpaceByIndex(TuningSpace.Space);
            }
        }

        // Service: サブチャンネルを許容して取得する
        std::optional<TVTest::ServiceInfo> service = std::nullopt;

        // サブチャンネルを考慮する
        for (auto i = 0; i < SUB_SERVICE_ID_ALLOWANCE; i++)
        {
            TVTest::ServiceInfo Service{};

            if (m_pApp->GetServiceInfo(i, &Service))
            {
                service = std::optional(Service);
                break;
            }
        }

        // ChannelInfo
        TVTest::ChannelInfo Channel{};
        const auto channel = m_pApp->GetCurrentChannelInfo(&Channel) ? std::optional(Channel) : std::nullopt;

        // Program
        TVTest::ProgramInfo Program{};
        wchar_t pszEventName[MaxStateLength];
        Program.pszEventName = pszEventName;
        Program.MaxEventName = _countof(pszEventName);
        wchar_t pszEventText[MaxImageTextLength];
        Program.pszEventText = pszEventText;
        Program.MaxEventText = _countof(pszEventText);
        wchar_t pszEventExtText[MaxImageTextLength];
        Program.pszEventExtText = pszEventExtText;
        Program.MaxEventExtText = _countof(pszEventExtText);
        const auto program = m_pApp->GetCurrentProgramInfo(&Program) ? std::optional(Program) : std::nullopt;

        // Host
        TVTest::HostInfo Host{};
        const auto host = m_pApp->GetHostInfo(&Host) ? std::optional(Host) : std::nullopt;

        auto activity = CreatePresence(tuningSpace, service, channel, program, host, m_config);
        m_activities->update_activity(m_activities, &activity, nullptr, nullptr);
        m_lastActivity = activity;

#ifdef _DEBUG
        const auto serviceName = service.has_value() && !IsBlank(service.value().szServiceName, ServiceNameLength) ? service.value().szServiceName : L"[不明]";
        const auto eventName = program.has_value() && !IsBlank(program.value().pszEventName, MaxStateLength) ? program.value().pszEventName : L"[不明]";
        wchar_t buf[256];
        wsprintf(buf, L"Rich Presence を更新しました。(サービス名: %s, 番組名: %s)", serviceName, eventName);
        OutputDebugString(buf);
#endif
    }
}

/*
 * TVTest のイベントコールバック
 */
LRESULT CALLBACK CTvTestRPCPlugin::EventCallback(const UINT Event, const LPARAM lParam1, LPARAM, void* pClientData)
{
    auto* pThis = static_cast<CTvTestRPCPlugin*>(pClientData);

    switch (Event)
    {
    case TVTest::EVENT_PLUGINENABLE:
        if (lParam1 == 1)
        {
            pThis->LoadConfig();

            pThis->InitializeDiscord();
            pThis->UpdateActivity();
        }
        else if (pThis->m_isReady)
        {
            pThis->m_lastActivity = {};
            pThis->m_activities->clear_activity(pThis->m_activities, nullptr, nullptr);

            pThis->m_discord->destroy(pThis->m_discord);

            pThis->m_isReady = false;
        }

        return true;

    case TVTest::EVENT_CHANNELCHANGE:
    case TVTest::EVENT_SERVICECHANGE:
    case TVTest::EVENT_SERVICEUPDATE:
        pThis->UpdateActivity();

        return true;

    default:
        return false;
    }
}

/*
 * ウィンドウプロシージャ
 * タイマー処理を行う
 */
LRESULT CALLBACK CTvTestRPCPlugin::WndProc(const HWND hWnd, const UINT uMsg, const WPARAM wParam, const LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
        {
            auto* const pcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
            auto* pThis = static_cast<CTvTestRPCPlugin*>(pcs->lpCreateParams);

            ::SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
            SetTimer(hWnd, UpdateActivityTimerId, UpdateActivityTimerIntervalMs, nullptr);
            SetTimer(hWnd, RunCallbacksTimerId, RunCallbacksTimerIntervalMs, nullptr);
        }

        return true;

    case WM_TIMER:
        if (wParam == UpdateActivityTimerId)
        {
            auto* pThis = reinterpret_cast<CTvTestRPCPlugin*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));

            pThis->UpdateActivity();
        }
        else if (wParam == RunCallbacksTimerId)
        {
            if (auto * pThis = reinterpret_cast<CTvTestRPCPlugin*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA)); pThis->m_isReady)
            {
                pThis->m_discord->run_callbacks(pThis->m_discord);
            }
        }

        return false;

    default:
        return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
}
