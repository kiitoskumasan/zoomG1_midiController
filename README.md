# zoomG1_midiController
M5Stack Basicで動作する、zoom G1X Four用 Midiフットコントローラです。

以下のハードウエアが必要です。

・M5Stack Basic

・M5Stack用 USBモジュール (G1X Four接続用)

・M5Stack用 プロトタイプモジュール PROTO-13.2 (外部フットコントローラジャック設置用)

ビルドには以下のライブラリが必要です。

・M5Stack Library　https://github.com/m5stack/M5Stack

・button2 from Lennart Hennigs https://github.com/LennartHennigs/Button2

・USB Host Library Rev. 2.0 https://github.com/felis/USB_Host_Shield_2.0


動画(YouTube)
https://www.youtube.com/watch?v=dlGKATR8eMw


フットスイッチは以下のGPIOに割り当てています。

M5Stackのピンからスイッチに配線する場合、以下のGPIOから配線して下さい。

・パッチカウントアップ(右ペダル)       GPIO 22

・パッチカウントダウン(左ペダル)       GPIO 26

・バンクアップ                         GPIO 16

・バンクダウン                         GPIO 21
