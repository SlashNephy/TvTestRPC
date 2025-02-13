#pragma once

#include "stdafx.h"

#include "Utils.h"

constexpr auto LOGO_GR_NHKG = "gr_nhkg";
constexpr auto LOGO_GR_NHKE = "gr_nhke";
constexpr auto LOGO_DEFAULT = "logo";
constexpr auto SUB_SERVICE_ID_ALLOWANCE = 3;

/*
 * 対象のサービスが NHK総合 であるかどうか判定する
 */
inline bool IsNHKGService(const WORD serviceId, const wchar_t* serviceName)
{
    switch (serviceId)
    {
    case 1024:  // 関東広域: NHK総合・東京

    case 10240: // 北海道(札幌): NHK総合・札幌
    case 11264: // 北海道(函館): NHK総合・函館
    case 12288: // 北海道(旭川): NHK総合・旭川
    case 13312: // 北海道(帯広): NHK総合・帯広
    case 14336: // 北海道(釧路): NHK総合・釧路
    case 15360: // 北海道(北見): NHK総合・北見
    case 16384: // 北海道(室蘭): NHK総合・室蘭
    case 17408: // 宮城: NHK総合・仙台
    case 18432: // 秋田: NHK総合・秋田
    case 19456: // 山形: NHK総合・山形
    case 20480: // 岩手: NHK総合・盛岡
    case 21504: // 福島: NHK総合・福島
    case 22528: // 青森: NHK総合・青森
    case 25600: // 群馬: NHK総合・前橋
    case 26624: // 茨城: NHK総合・水戸
    case 28672: // 栃木: NHK総合・宇都宮
    case 30720: // 長野: NHK総合・長野
    case 31744: // 新潟: NHK総合・新潟
    case 32768: // 山梨: NHK総合・甲府
    case 33792: // 愛知: NHK総合・名古屋
    case 34816: // 石川: NHK総合・金沢
    case 35840: // 静岡: NHK総合・静岡
    case 36864: // 福井: NHK総合・福井
    case 37888: // 富山: NHK総合・富山
    case 38912: // 三重: NHK総合・津
    case 39936: // 岐阜: NHK総合・岐阜
    case 40960: // 大阪: NHK総合・大阪
    case 41984: // 京都: NHK総合・京都
    case 43008: // 兵庫: NHK総合・神戸
    case 44032: // 和歌山: NHK総合・和歌山
    case 45056: // 奈良: NHK総合・奈良
    case 46080: // 滋賀: NHK総合・大津
    case 47104: // 広島: NHK総合・広島
    case 48128: // 岡山: NHK総合・岡山
    case 49152: // 島根: NHK総合・松江
    case 50176: // 鳥取: NHK総合・鳥取
    case 51200: // 山口: NHK総合・山口
    case 52224: // 愛媛: NHK総合・松山
    case 53248: // 香川: NHK総合・高松
    case 54272: // 徳島: NHK総合・徳島
    case 55296: // 高知: NHK総合・高知
    case 56320: // 福岡: NHK総合・福岡
    case 56832: // 福岡: NHK総合・北九州
    case 57344: // 熊本: NHK総合・熊本
    case 58368: // 長崎: NHK総合・長崎
    case 59392: // 鹿児島: NHK総合・鹿児島
    case 60416: // 宮崎: NHK総合・宮崎
    case 61440: // 大分: NHK総合・大分
    case 62464: // 佐賀: NHK総合・佐賀
    case 63488: // 沖縄: NHK総合・沖縄
        return true;
    default:
        return StartsWith(serviceName, L"ＮＨＫ総合");
    }
}

/*
 * 対象のサービスが NHKEテレ であるかどうか判定する
 */
inline bool isNHKEService(const WORD serviceId, const wchar_t* serviceName)
{
    switch (serviceId)
    {
    case 1032:  // 関東広域: NHKEテレ・東京
    case 2056:  // 近畿広域: NHKEテレ・大阪
    case 3080:  // 中京広域: NHKEテレ・名古屋

    case 10248: // 北海道(札幌): NHKEテレ・札幌
    case 11272: // 北海道(函館): NHKEテレ・函館
    case 12296: // 北海道(旭川): NHKEテレ・旭川
    case 13320: // 北海道(帯広): NHKEテレ・帯広
    case 14344: // 北海道(釧路): NHKEテレ・釧路
    case 15368: // 北海道(北見): NHKEテレ・北見
    case 16392: // 北海道(室蘭): NHKEテレ・室蘭
    case 17416: // 宮城: NHKEテレ・仙台
    case 18440: // 秋田: NHKEテレ・秋田
    case 19464: // 山形: NHKEテレ・山形
    case 20488: // 岩手: NHKEテレ・盛岡
    case 21512: // 福島: NHKEテレ・福島
    case 22536: // 青森: NHKEテレ・青森
    case 30728: // 長野: NHKEテレ・長野
    case 31752: // 新潟: NHKEテレ・新潟
    case 32776: // 山梨: NHKEテレ・甲府
    case 34824: // 石川: NHKEテレ・金沢
    case 35848: // 静岡: NHKEテレ・静岡
    case 36872: // 福井: NHKEテレ・福井
    case 37896: // 富山: NHKEテレ・富山
    case 47112: // 広島: NHKEテレ・広島
    case 48136: // 岡山: NHKEテレ・岡山
    case 49160: // 島根: NHKEテレ・松江
    case 50184: // 鳥取: NHKEテレ・鳥取
    case 51208: // 山口: NHKEテレ・山口
    case 52232: // 愛媛: NHKEテレ・松山
    case 53256: // 香川: NHKEテレ・高松
    case 54280: // 徳島: NHKEテレ・徳島
    case 55304: // 高知: NHKEテレ・高知
    case 56328: // 福岡: NHKEテレ・福岡
    case 56840: // 福岡: NHKEテレ・北九州
    case 57352: // 熊本: NHKEテレ・熊本
    case 58376: // 長崎: NHKEテレ・長崎
    case 59400: // 鹿児島: NHKEテレ・鹿児島
    case 60424: // 宮崎: NHKEテレ・宮崎
    case 61448: // 大分: NHKEテレ・大分
    case 62472: // 佐賀: NHKEテレ・佐賀
    case 63496: // 沖縄: NHKEテレ・沖縄
        return true;
    default:
        return StartsWith(serviceName, L"ＮＨＫＥテレ");
    }
}

inline const char* GetGRServiceLogoKey(const WORD serviceId, const wchar_t* serviceName)
{
    // 全国: NHK総合
    if (IsNHKGService(serviceId, serviceName))
    {
        return LOGO_GR_NHKG;
    }

    // 全国: NHKEテレ
    if (isNHKEService(serviceId, serviceName))
    {
        return LOGO_GR_NHKE;
    }

    // http://soranikakaruhashi.blog.fc2.com/blog-entry-71.html
    switch (serviceId)
    {
    case 1040: // 関東広域: 日テレ
        return "gr_1040";
    case 1048: // 関東広域: TBS
        return "gr_1048";
    case 1056: // 関東広域: フジテレビ
        return "gr_1056";
    case 1064: // 関東広域: テレビ朝日
        return "gr_1064";
    case 1072: // 関東広域: テレビ東京
        return "gr_1072";
    case 1088: // 関東広域: 放送大学
        return "bs_231";

    case 2064: // 近畿広域: MBS 毎日放送
        return "gr_2064";
    case 2072: // 近畿広域: ABCテレビ
        return "gr_2072";
    case 2080: // 近畿広域: 関西テレビ
        return "gr_2080";
    case 2088: // 近畿広域: 読売テレビ
        return "gr_2088";

    case 17440: // 宮城: ミヤギテレビ
        return "gr_17440";
    case 17448: // 宮城: KHB 東日本放送
        return "gr_17448";
    case 17424: // 宮城: TBC 東北放送
        return "gr_17424";
    case 17432: // 宮城: 仙台放送
        return "gr_17432";

    case 23608: // 東京: TOKYO MX
        return "gr_23608";

    case 24632: // 神奈川: tvk
        return "gr_24632";
    case 24680: // 神奈川: イッツコムch10 (CATV)
    case 24696: // 神奈川: イッツコムch11 (CATV)
        return "gr_24680";

    case 34840: // 石川: HAB
        return "gr_34840";

    case 37904: // 富山: KNB
        return "gr_37904";
    case 37920: // 富山: チューリップテレビ
        return "gr_37920";
    case 37912: // 富山: 富山テレビ放送
        return "gr_37912";
    case 38008: // 富山: コミュチャン091 (CATV)
        return "gr_38008";

    case 41008: // 大阪: テレビ大阪
        return "gr_41008";

    case 43056: // 兵庫: サンテレビ
        return "gr_43056";

    case 46128: // 滋賀: BBC びわ湖放送
        return "gr_46128";

    case 47528: // 広島: テレビ新広島
        return "gr_47528";

    default:
        return nullptr;
    }
}

inline const char* GetBSServiceLogoKey(const WORD serviceId)
{
    switch (serviceId)
    {
        // NHK BS1
    case 101:
    case 102:
        return "bs_101";
        // NHK BSプレミアム
    case 103:
    case 104:
        return "bs_103";
        // BS日テレ
    case 141:
    case 142:
    case 143:
        return "bs_141";
        // BS朝日
    case 151:
    case 152:
    case 153:
        return "bs_151";
        // BS-TBS
    case 161:
    case 162:
    case 163:
        return "bs_161";
        // BSテレ東
    case 171:
    case 172:
    case 173:
        return "bs_171";
        // BSフジ
    case 181:
    case 182:
    case 183:
        return "bs_181";
        // WOWOW
    case 191:
    case 192:
    case 193:
        return "bs_191";
        // スターチャンネル
    case 200:
    case 201:
    case 202:
        return "bs_200";
        // BS11イレブン
    case 211:
        return "bs_211";
        // BS12トゥエルビ
    case 222:
        return "bs_222";
        // 放送大学
    case 231:
    case 232:
    case 531:
        return "bs_231";
        // グリーンチャンネル
    case 234:
        return "bs_234";
        // BSアニマックス
    case 236:
        return "bs_236";
        // BSスカパー！
    case 241:
        return "bs_241";
        // J SPORTS
    case 242:
    case 243:
    case 244:
    case 245:
        return "bs_242";
        // BS釣りビジョン
    case 251:
        return "bs_251";
        // WOWOWプラス
    case 252:
        return "bs_252";
        // 日本映画専門ch
    case 255:
        return "bs_255";
        // ディズニーch
    case 256:
        return "bs_256";
        // BS松竹東急
    case 260:
        return "bs_260";
        // BSJapanext
    case 263:
        return "bs_263";
        // BSよしもと
    case 265:
        return "bs_265";
    default:
        return LOGO_DEFAULT;
    }
}

inline const char* GetCSServiceLogoKey(const WORD serviceId)
{
    switch (serviceId)
    {
    case 55:
        return "cs_55";
    case 161:
        return "cs_161";
    case 218:
        return "cs_218";
    case 219:
        return "cs_219";
    case 223:
        return "cs_223";
    case 227:
        return "cs_227";
    case 240:
        return "cs_240";
    case 250:
        return "cs_250";
    case 254:
        return "cs_254";
    case 257:
        return "cs_257";
    case 262:
        return "cs_262";
    case 290:
        return "cs_290";
    case 292:
        return "cs_292";
    case 293:
        return "cs_293";
    case 294:
        return "cs_294";
    case 295:
        return "cs_295";
    case 296:
        return "cs_296";
    case 297:
        return "cs_297";
    case 298:
        return "cs_298";
    case 299:
        return "cs_299";
    case 300:
        return "cs_300";
    case 301:
        return "cs_301";
    case 305:
        return "cs_305";
    case 307:
        return "cs_307";
    case 308:
        return "cs_308";
    case 309:
        return "cs_309";
    case 310:
        return "cs_310";
    case 311:
        return "cs_311";
    case 312:
        return "cs_312";
    case 314:
        return "cs_314";
    case 316:
        return "cs_316";
    case 317:
        return "cs_317";
    case 318:
        return "cs_318";
    case 321:
        return "cs_321";
    case 322:
        return "cs_322";
    case 323:
        return "cs_323";
    case 324:
        return "cs_324";
    case 325:
        return "cs_325";
    case 329:
        return "cs_329";
    case 330:
        return "cs_330";
    case 331:
        return "cs_331";
    case 333:
        return "cs_333";
    case 339:
        return "cs_339";
    case 340:
        return "cs_340";
    case 341:
        return "cs_341";
    case 342:
        return "cs_342";
    case 343:
        return "cs_343";
    case 349:
        return "cs_349";
    case 351:
        return "cs_351";
    case 353:
        return "cs_353";
    case 354:
        return "cs_354";
    case 363:
        return "cs_363";
    case 800:
        return "cs_800";
    case 801:
        return "cs_801";
    default:
        return LOGO_DEFAULT;
    }
}

inline const char* GetSKYServiceLogoKey(const WORD serviceId)
{
    return LOGO_DEFAULT;
}

enum class TuningSpace
{
    GR,
    BS,
    CS,
    SKY
};

inline const char* GetAmbiguousServiceLogoKey(const WORD serviceId, const wchar_t* serviceName)
{
    if (serviceId == 0)
    {
        return LOGO_DEFAULT;
    }

    // BS or CS or SKY
    if (serviceId < 1000)
    {
        auto logoKey = GetBSServiceLogoKey(serviceId);
        if (logoKey != LOGO_DEFAULT)
        {
            return logoKey;
        }

        logoKey = GetCSServiceLogoKey(serviceId);
        if (logoKey != LOGO_DEFAULT)
        {
            return logoKey;
        }

        return GetSKYServiceLogoKey(serviceId);
    }

    // GR
    // サブチャンネルを許容する
    for (WORD i = 0; i < SUB_SERVICE_ID_ALLOWANCE; i++)
    {
        if (const auto logoKey = GetGRServiceLogoKey(serviceId - i, serviceName); logoKey != nullptr)
        {
            return logoKey;
        }
    }

    return LOGO_DEFAULT;
}

inline const char* GetServiceLogoKey(const std::optional<TuningSpace> tuningSpace, const WORD serviceId, const wchar_t* serviceName)
{
    if (!tuningSpace.has_value())
    {
        return GetAmbiguousServiceLogoKey(serviceId, serviceName);
    }

    if (serviceId == 0)
    {
        return LOGO_DEFAULT;
    }

    switch (tuningSpace.value())
    {
        // GR
    case TuningSpace::GR:
        // サブチャンネルを許容する
        for (WORD i = 0; i < SUB_SERVICE_ID_ALLOWANCE; i++)
        {
            if (const auto logoKey = GetGRServiceLogoKey(serviceId - i, serviceName); logoKey != nullptr)
            {
                return logoKey;
            }
        }

        return LOGO_DEFAULT;

        // BS
    case TuningSpace::BS:
        return GetBSServiceLogoKey(serviceId);

        // CS
    case TuningSpace::CS:
        return GetCSServiceLogoKey(serviceId);

        // SKY
    case TuningSpace::SKY:
        return GetSKYServiceLogoKey(serviceId);
    }

    return LOGO_DEFAULT;
}

inline std::string GetTuningSpaceName(const TuningSpace type)
{
    switch (type)
    {
    case TuningSpace::GR:
        return "GR";
    case TuningSpace::BS:
        return "BS";
    case TuningSpace::CS:
        return "CS";
    case TuningSpace::SKY:
        return "SKY";
    }

    throw;
}

inline std::optional<TuningSpace> GetTuningSpaceByName(const std::wstring& name)
{
    if (name == L"GR")
    {
        return std::optional(TuningSpace::GR);
    }
    if (name == L"BS")
    {
        return std::optional(TuningSpace::BS);
    }
    if (name == L"CS")
    {
        return std::optional(TuningSpace::CS);
    }
    if (name == L"SKY")
    {
        return std::optional(TuningSpace::SKY);
    }

    return std::nullopt;
}

inline std::optional<TuningSpace> GetTuningSpaceByIndex(const int index)
{
    switch (index)
    {
    case TVTest::TUNINGSPACE_TERRESTRIAL:
        return std::optional(TuningSpace::GR);
    case TVTest::TUNINGSPACE_BS:
        return std::optional(TuningSpace::BS);
    case TVTest::TUNINGSPACE_110CS:
        return std::optional(TuningSpace::CS);
    default:
        return std::nullopt;
    }
}
