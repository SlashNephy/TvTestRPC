#pragma once

#include "stdafx.h"

#include "Logo.h"
#include "TvtPlay.h"
#include "Utils.h"
#include "Version.h"

constexpr auto MaxDetailsLength = 128;
constexpr auto MaxStateLength = 128;
constexpr auto MaxImageKeyLength = 128;
constexpr auto MaxImageTextLength = 128;

constexpr auto ServiceNameLength = 32;
constexpr auto ChannelNameLength = 64;

static DiscordActivity CreatePresence(
    const std::optional<TuningSpace> TuningSpace,
    const std::optional<TVTest::ServiceInfo> Service,
    const std::optional<TVTest::ChannelInfo> Channel,
    const std::optional<TVTest::ProgramInfo> Program,
    const std::optional<TVTest::HostInfo> Host,
    Config& Config
)
{
    // ロケールの設定
    setlocale(LC_ALL, ".utf8");

    DiscordActivity Activity{};
    memset(&Activity, 0, sizeof Activity);

    Activity.type = DiscordActivityType_Watching;

    // 番組データがあるなら時間情報を付与する
    if (Program.has_value())
    {
        const auto duration = static_cast<time_t>(Program.value().Duration);

        // TvtPlay が有効なら再生位置を加味する
        if (const auto tvtPlayHwnd = FindTvtPlayFrame(); tvtPlayHwnd)
        {
            const auto now = time(nullptr);
            const auto pos = GetTvtPlayPositionSec(tvtPlayHwnd);

            Activity.timestamps.start = now + pos;
            if (Config.ShowEndTime && duration > 0)
            {
                Activity.timestamps.end = now + (duration - pos);
            }
        }
        else
        {
            const auto rawStartTime = Program.value().StartTime;

            Activity.timestamps.start = SystemTime2Timet(rawStartTime);
            if (Config.ShowEndTime && duration > 0)
            {
                Activity.timestamps.end = Activity.timestamps.start + duration;
            }
        }
    }

    // サービス名を assets.large_text に設定
    if (Config.ShowChannelLogo) {
        if (Service.has_value())
        {
            // const wchar_t* → wchar_t*
            // const auto rawServiceName = const_cast<LPWSTR>(Service.value().szServiceName);
            wchar_t serviceName[ServiceNameLength] = {};

            if (const auto rawServiceName = Service.value().szServiceName; !IsBlank(rawServiceName, ServiceNameLength))
            {
                wcsncpy_s(serviceName, rawServiceName, ServiceNameLength);

                // 半角変換
                if (Config.ConvertToHalfWidth)
                {
                    Full2Half(serviceName);
                }

                wcstombs_s(nullptr, Activity.assets.large_text, serviceName, ServiceNameLength - 1);
            }
        }

        if (Channel.has_value() && strlen(Activity.assets.large_text) == 0)
        {
            wchar_t channelName[ChannelNameLength] = {};
            const auto rawChannelName = Channel.value().szChannelName;
            wcsncpy_s(channelName, !IsBlank(rawChannelName, ChannelNameLength) ? rawChannelName : L"取得中…", ChannelNameLength);

            // 半角変換
            if (Config.ConvertToHalfWidth)
            {
                Full2Half(channelName);
            }

            wcstombs_s(nullptr, Activity.assets.large_text, channelName, ChannelNameLength - 1);
        }
    }

    // 番組名を details に設定
    if (Program.has_value())
    {
        if (const auto rawEventName = Program.value().pszEventName; !IsBlank(rawEventName, MaxStateLength))
        {
            // 半角変換
            if (Config.ConvertToHalfWidth)
            {
                Full2Half(rawEventName);
            }

            wcstombs_s(nullptr, Activity.details, rawEventName, MaxStateLength - 1);
        }
    }

    // 番組説明を state に設定
    if (Program.has_value())
    {
        if (const auto rawEventText = Program.value().pszEventText; rawEventText != nullptr && !IsBlank(rawEventText, MaxImageTextLength))
        {
            // 半角変換
            if (Config.ConvertToHalfWidth)
            {
                Full2Half(rawEventText);
            }

            wcstombs_s(nullptr, Activity.state, rawEventText, MaxImageTextLength - 1);
        }
        else if (const auto rawEventExtText = Program.value().pszEventExtText; rawEventExtText != nullptr && !IsBlank(rawEventExtText, MaxImageTextLength))
        {
            // 半角変換
            if (Config.ConvertToHalfWidth)
            {
                Full2Half(rawEventExtText);
            }

            wcstombs_s(nullptr, Activity.state, rawEventExtText, MaxImageTextLength - 1);
        }
    }

    // チャンネルロゴ or TVTest ロゴを付与する
    if (Config.ShowChannelLogo)
    {
        WORD serviceId = 0;
        wchar_t serviceName[ServiceNameLength]{};
        if (Service.has_value())
        {
            serviceId = Service.value().ServiceID;
            wcscpy_s(serviceName, Service.value().szServiceName);
        }
        else if (Channel.has_value())
        {
            wcscpy_s(serviceName, Channel.value().szChannelName);
        }

        const auto logoKey = Config.Logos.count(serviceId) > 0 ? Config.Logos[serviceId].c_str() : GetServiceLogoKey(TuningSpace, serviceId, serviceName);
        strcpy_s(Activity.assets.large_image, logoKey);
    }

    // バージョン情報を付与する
    {
        strcpy_s(Activity.assets.small_image, LOGO_DEFAULT);

        if (Host.has_value()) {
            sprintf_s(Activity.assets.small_text, "%ws v%ws / TvTestRPC v%s", Host.value().pszAppName, Host.value().pszVersionText, TvTestRPCVersion);
        }
    }
    
    return Activity;
}
