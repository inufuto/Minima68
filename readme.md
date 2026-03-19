# Minima68

 [MachiKania type PU](http://www.ze.em-net.ne.jp/~kenken/machikania/typepu.html)のハードウェアで動作する、架空の8ビットコンピューターのエミュレーターです。

## 仕様

* CPU: MC6800 1MHz相当
* RAM: 52kバイト
* 画面解像度: 128x208ドット (32x26文字)
* 色数: 16色
* スプライト: 8x16ドット/最大32個（水平方向16個）
* サウンド: トーン3チャンネル+ノイズ1チャンネル

技術情報は[technical.md](technical.md)をご覧ください。

## 使い方

### インストール

minima68.uf2をRaspberry Pi Picoに書き込みます。

### 起動

設定ファイル(MINIMA68.INI)とプログラムファイル（拡張子 .BIN）を入れたSDカードを挿して電源を接続します。

画面にプログラムファイル名が表示されるので、矢印キー↑↓で選択しスペースキー(またはZ)を押して実行します。

![](images/launcher.png)

## 設定ファイル

MINIMA68.INIに以下の設定を記述できます。（現時点ではキーボード種別のみ設定可能です）

|項目|内容|
|-|-|
|106KEY|JIS配列キーボードを使用(キーボード設定を省略した場合はこちら)|
|101KEY|US配列キーボードを使用|

## Windows版

Windows版もあります。実行ファイル名はminima68.exeです。
