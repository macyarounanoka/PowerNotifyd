PowerNotifyd
============

OS X Power awake sleep detect daemon

OS X(Yosemite)のスリープ、目覚めを検出し、シェルスクリプトを実行します。
Yosemite でテストしています。

## Description

ログインしたままのスリープでUSB HDDを抜くと「ディスクの不正な取り出し」警告が発生してしまいます。

スリープを検出し、スリープ用のシェルスクリプトでUSB HDDをアンマウント、

目覚めを検出し、目覚め用のシェルスクリプトでUSB HDDをマウントするために作りました。

## Usage
PowerNotifyd is automatically run the installation.
PowerNotifyd call the shell script when it detects a sleep or awake.

Relationship of shell and events that call:
CanSystemSleep Shell Script
 Location:~/PowerNotifyd/scripts/CanSystemSleep
 Occurrence factor:
 Idle sleep is about to kick in. This message will not be sent for forced sleep.
 Applications have a chance to prevent sleep by calling IOCancelPowerChange.
 Most applications should not prevent idle sleep.

SystemWillSleep Shell Script
 Location:~/PowerNotifyd/scripts/SystemWillSleep
 Occurrence factor:
 The system WILL go to sleep.

SystemWillPowerOn Shell Script 
 Location:~/PowerNotifyd/scripts/SystemWillPowerOn
 Occurrence factor:
 System has started the wake up process.

SystemHasPoweredOn Shell Script
 Location:~/PowerNotifyd/scripts/SystemHasPoweredOn
 Occurrence factor:
 System has finished waking up.
 
## Install
 sh install.sh
 This command I will register to compile and services(LaunchAgents)

## Uninstall
 sh uninstall.sh 
 This command will cancel the registration of LaunchAgents
