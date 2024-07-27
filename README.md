# edge-server-control

![20240727_115723000_iOS](https://github.com/user-attachments/assets/fd70001b-2540-47e5-9311-8a5759f26d00)
![20240727_115751000_iOS](https://github.com/user-attachments/assets/e80502a8-fdc9-4e85-a47c-1d5bcd669a4c)


ESP32で実行可能なWebサーバー

利用可能なマイコンは以下です．
- ESP32

## Build Environment
- PlatformIO

## Installation

先にPlatformIO Coreをインストールしてください．
インストール方法は[こちら](https://docs.platformio.org/en/latest/core/installation/index.html)

```
cd <workspace>
git clone git@github.com:sgrsn/edge-server-control.git
cd edge-server-control
pio run -t uploadfs
pio run -t upload
```
