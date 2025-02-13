# TvTestRPC

[![GitHub Workflow Status](https://img.shields.io/github/actions/workflow/status/SlashNephy/TvTestRPC/latest.yml?style=flat-square)](https://github.com/SlashNephy/TvTestRPC/actions/workflows/latest.yml)
[![GitHub release (latest by date)](https://img.shields.io/github/v/release/SlashNephy/TvTestRPC?style=flat-square)](https://github.com/SlashNephy/TvTestRPC/releases)

TVTest で Discord Rich Presence を使えるようにするプラグインです。(TVTest ver.0.6.0 or later)

![image](https://user-images.githubusercontent.com/7302150/175781038-7905b66d-4067-49f6-ad96-b4bccdad94ea.png)

## Get Started

ビルド済み成果物は以下からダウンロードできます。

- ワークフロー [![GitHub Workflow Status](https://img.shields.io/github/actions/workflow/status/SlashNephy/TvTestRPC/latest.yml?style=flat-square)](https://github.com/SlashNephy/TvTestRPC/actions/workflows/latest.yml) のビルド履歴 -> Artifacts
- [![GitHub release (latest by date)](https://img.shields.io/github/v/release/SlashNephy/TvTestRPC?style=flat-square)](https://github.com/SlashNephy/TvTestRPC/releases)

## Features

[本家さんバージョン](https://github.com/noriokun4649/TvTestRPC) と同等ですが, 以下の相違点があります。

- TvtPlay プラグインと連携し, ファイル再生時にも経過時間を表示
  - シーク位置を加味して経過時間を計算
- 東京近辺の地上波だけでなく, NHK や BS, 東経110度 CS, 様々な地域の地上波のロゴ表示にも対応
- 視聴中の番組が終了したときに Presence が更新されないバグを修正
- 全角文字を半角に変換するオプションを追加
- サブチャンネル (TOKYO MX2 等) でも Presence を表示

## Station Logos

現在, 次の放送局ロゴに対応しています。ロゴは Wikipedia 上で公開されているもので Public Domain となっています。

- 地上波
  - 全国  
    NHK 総合, NHK Eテレ
  - 関東広域  
    日テレ, TBS, テレビ朝日, テレビ東京, フジテレビ  
  - 近畿広域  
    MBS毎日放送, ABCテレビ, 関西テレビ, 読売テレビ  
  - 北海道域  
    HBC 北海道放送, STV 札幌テレビ, HTB 北海道テレビ, UHB 北海道文化放送, TVh テレビ北海道

  - 宮城  
    ミヤギテレビ, KHB, TBC, 仙台放送
  - 東京  
    TOKYO MX
  - 神奈川  
    tvk  
    イッツコム (CATV)
  - 千葉  
    チバテレビ
  - 埼玉  
    テレ玉
  - 新潟  
    BSN 新潟放送, NST 新潟総合テレビ, TeNY テレビ新潟, UX 新潟テレビ21
  - 山梨  
    YBS 山梨放送, UTY テレビ山梨
  - 石川  
    HAB
  - 富山  
    KNB, チューリップテレビ, 富山テレビ放送  
    コミュチャン (CATV)
  - 大阪  
    テレビ大阪
  - 京都  
    KBS 京都放送
  - 兵庫  
    サンテレビ
  - 広島  
    テレビ新広島 (by [@yude](https://github.com/yude))

- BS  
  2022/03/11 現在の全局に対応しています。
- 東経110度 CS  
  2021/06/03 現在の全局に対応しています。

ロゴ追加のリクエストは [Issue](https://github.com/SlashNephy/TvTestRPC/issues/new/choose) からお願いします。

## Configuration

`TvTestRPC.ini` を編集してください。

```ini
[Settings]
; 番組の終了時刻を Discord に通知します。
; カウントダウンされるようになります。
ShowEndTime=1

; チャンネルロゴを表示します。
ShowChannelLogo=1

; 番組名やチャンネル名の全角文字を半角に変換します。
ConvertToHalfWidth=1

; チューニング空間を無視してチャンネルロゴを適用します。
IgnoreTuningSpace=0

[Logos]
; 特定のサービス ID のロゴを任意に書き換えられます。
; <サービス ID>=<ロゴの Image Key> のペアを記述します。
; 例えば以下の場合, BS11 (サービス ID = 211) のロゴを NHK 総合に変更します。
; 利用可能なロゴの Image Key は "Logo.h" を参照してください。
; 211=gr_nhkg
```

## Build

依存関係は [vcpkg](https://github.com/microsoft/vcpkg) で管理されています。

```bat
vcpkg integrate install

msbuild TvTestRPC.sln -property:Configuration="Release" -property:Platform="x64" -m
```
