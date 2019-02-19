---
title: 小米路由器3G折腾之刷OpenWrt/Pandavan记录
date: 2019-02-19 00:49:20
layout: post
comment: true
tags:
- 教程
- 路由器教程
- OpenWrt/LEDE
- OpenWrt
- Pandavan
- PandoraBox
categories:
- 教程
- 路由器
---
生命不息，折腾不止。

家里的路由器又坏了（强行狡辩
<!--more-->

之前有写过斐讯k2和小米路由器mini的教程，但是现在回看之前写完的教程要么通篇没有逻辑要么缺东少西。于是就有了这篇文章。

我尽可能的写的详细一点以便没有经验的小白也能看懂。

<div class="alert-red">温馨提示：刷成砖了本人一概不负任何责任</div>

<iframe frameborder="no" border="0" marginwidth="0" marginheight="0" width=330 height=86 src="//music.163.com/outchain/player?type=2&id=31108216&auto=0&height=66"></iframe>

来一首美妙动人的音乐吧！

# 准备工作

你至少需要以下工具：

* 路由器本体（废话
* 一个已格式化为FAT/FAT32格式的U盘用来刷开发者固件和ssh激活工具
* 一根可以捅‘Reset’钮的针（取卡针或牙签等
* 可以通过ssh连接到路由器的软件（例如：[putty](https://putty.org)）
* [小米路由器客户端](http://www1.miwifi.com/miwifi_download.html)，用来绑定你的小米账号

# 开启路由器SSH权限

1. 在[MiWiFi下载页面](http://www1.miwifi.com/miwifi_download.html)下载所需要的路由器开发者固件,命名为`miwifi.bin`。（ROM -> ROM for R3G开发版）
2. 路由器断电，将下载好的开发者固件放入U盘插入路由器USB接口，捅住reset扭接上电源后待指示灯为黄色闪烁时松开，数分钟后路由器会自动重启，此过程不要乱动路由器。
3. 小米路由器客户端登陆小米账号绑定路由器设备，此过程需要路由器联网。
4. [MiWiFi开放平台](http://www1.miwifi.com/miwifi_open.html)登陆小米账号下载ssh激活工具命名为`miwifi_ssh.bin`并记下root密码。
5. 同第二步骤刷入ssh激活工具。

路由器重启后指示灯会变为蓝色，若变为红色则为刷入失败。

ssh到路由器:
Windows系统用putty，ip为`191.168.31.1`,用户名：`root`，密码为下载激活ssh工具时记下的密码。
Linux系统直接上Terminal:`ssh root@191.168.31.1`

~~ARE YOU OK？ I'm very OK.~~
```
  $$$$$$\  $$$$$$$\  $$$$$$$$\      $$\      $$\        $$$$$$\  $$\   $$\
 $$  __$$\ $$  __$$\ $$  _____|     $$ |     $$ |      $$  __$$\ $$ | $$  |
 $$ /  $$ |$$ |  $$ |$$ |           $$ |     $$ |      $$ /  $$ |$$ |$$  /
 $$$$$$$$ |$$$$$$$  |$$$$$\         $$ |     $$ |      $$ |  $$ |$$$$$  /
 $$  __$$ |$$  __$$< $$  __|        $$ |     $$ |      $$ |  $$ |$$  $$<
 $$ |  $$ |$$ |  $$ |$$ |           $$ |     $$ |      $$ |  $$ |$$ |\$$\
 $$ |  $$ |$$ |  $$ |$$$$$$$$\       $$$$$$$$$  |       $$$$$$  |$$ | \$$\
 \__|  \__|\__|  \__|\________|      \_________/        \______/ \__|  \__|
```

# 刷入Bootloader（推荐/可选）

之所以该步骤可选是因为Breed不支持直接刷入Openwrt固件，具体原因见[本贴](https://www.right.com.cn/forum/forum.php?mod=viewthread&tid=267455), 想刷Openwrt的同学可以直接跳至[4.2 刷入Openwrt](#如果你没有刷Breed:)片段。

但是为了避免路由器刷成砖的情况发生也可以在刷入Breed后再刷OpenWrt，具体选择权归你。

Bootloader原作者为hackpascal，[下载地址](https://breed.hackpascal.net/)
文件名为`breed-mt7621-xiaomi-r3g.bin`


有一种方法是ssh到路由器后通过`wget`下载breed文件并刷入（需要确保路由器联网），下载地址为https所以需要加上`--no-check-certificate`参数
```
cd /tmp
wget --no-check-certificate https://breed.hackpascal.net/breed-mt7621-xiaomi-r3g.bin -O breed.bin
mtd -r write breed.bin Bootloader
```
另一种方法是将breed文件复制到U盘中插上路由器，ssh到路由器将U盘挂载，然后刷入。
```
mkdir /tmp/sdcard
mount /dev/sda1 /tmp/sdcard
mtd -r write /tmp/sdcard/breed.bin Bootloader
```

路由器刷写成功后会自动重启，在刷Breed的过程中不要乱动路由器，坏了就难修了。

# 刷机

下载所需固件。

PandoraBox下载地址：[https://downloads.pangubox.com/pandorabox/19.02/targets/ralink/mt7621/](https://downloads.pangubox.com/pandorabox/19.02/targets/ralink/mt7621/)

Pandavan固件原帖: [https://www.right.com.cn/forum/thread-161324-1-1.html](https://www.right.com.cn/forum/thread-161324-1-1.html)

OpenWrt官方下载地址: [https://downloads.openwrt.org/releases/18.06.2/targets/ramips/mt7621/](https://downloads.openwrt.org/releases/18.06.2/targets/ramips/mt7621/)

## Pandavan/PandoraBox

直接下载固件并在Breed里刷写。

路由器断电，捅住reset按钮后通电，待指示灯变为蓝色闪烁后用网线连接路由器到电脑，浏览器打开网址`192.168.1.1`，进入Breed界面。

在固件更新栏里上传你想要刷的固件即可。

## OpenWrt

```

  _______                     ________        __
 |       |.-----.-----.-----.|  |  |  |.----.|  |_
 |   -   ||  _  |  -__|     ||  |  |  ||   _||   _|
 |_______||   __|_____|__|__||________||__|  |____|
          |__| W I R E L E S S   F R E E D O M
 -----------------------------------------------------
 OpenWrt xx.xx.xx, xx
 -----------------------------------------------------
```

[OpenWrt官网提供的教程](https://openwrt.org/toh/xiaomi/mir3g)是在没有刷入Breed的情况下刷入OpenWrt固件的。

中国大陆用户可以事先看一下[6. 其他问题](#其他问题：)的第5条

### 如果你没有刷Breed:

ssh到路由器,使用`wget`下载固件并刷机。
```
cd /tmp

wget http://downloads.openwrt.org/releases/18.06.2/targets/ramips/mt7621/openwrt-18.06.2-ramips-mt7621-mir3g-squashfs-kernel1.bin
wget http://downloads.openwrt.org/releases/18.06.2/targets/ramips/mt7621/openwrt-18.06.2-ramips-mt7621-mir3g-squashfs-rootfs0.bin

mtd write openwrt-18.06.2-ramips-mt7621-mir3g-squashfs-kernel1.bin kernel1
mtd write openwrt-18.06.2-ramips-mt7621-mir3g-squashfs-rootfs0.bin rootfs0
nvram set flag_try_sys1_failed=1
nvram commit

reboot
```
可能会遇到下载速度巨慢的情况，这时候可使用U盘上传下载好的固件复制到路由器上刷机。
```
mkdir /tmp/sdcard

mount /dev/sda1 /tmp/sdcard

cd /tmp/sdcard

mtd write openwrt-18.06.2-ramips-mt7621-mir3g-squashfs-kernel1.bin kernel1
mtd write openwrt-18.06.2-ramips-mt7621-mir3g-squashfs-rootfs0.bin rootfs0
nvram set flag_try_sys1_failed=1
nvram commit

reboot
```

使用Linux的同学也可以用`scp`上传下载好的固件到路由器上刷机，Windows系统的同学也可以用WinSCP这个软件上传固件，在此不赘述。

至于为啥OpenWrt官方的刷入方式复杂了许多我也说不太清楚，懂的同学可以在下方评论区补充。

### 如果你刷了Breed:
肯定很多人会担心路由器变转想刷Breed的（例如我），因为路由器有两个内核，所以需要在Breed里设置一下让路由器启动kernel1。

按照hackpascal的说法是：

> 如果kernel0存在kernel1不存在，那么启动kernel0
> 如果kernel1存在kernel0不存在，那么启动kernel1
> 如果kernel0和kernel1都存在，那么检查环境变量 xiaomi.r3g.bootfw 的值，如果存在且值为 2，那么启动kernel1，否则启动kernel0

所以同样按照上面的教程把固件上传到路由器里然后刷kernel1然后编辑Breed的参数启动kernel1即可。

```
mtd write openwrt-18.06.2-ramips-mt7621-mir3g-squashfs-kernel1.bin kernel1
mtd write openwrt-18.06.2-ramips-mt7621-mir3g-squashfs-rootfs0.bin rootfs0
```
执行完以上两个命令后重启路由器不会开机的，先别着急。

路由器断电，捅住reset按钮后通电，待指示灯变为蓝色闪烁后用网线连接路由器到电脑，浏览器打开网址`192.168.1.1`，进入Breed界面。

在环境变量编辑里添加`xiaomi.r3g.bootfw`字段，值为`2`,保存后重启即可完美进入Openwrt。

# Done

教程结束。

欢迎在评论区留言。

# 其他问题：

1. 如果刷了Breed + Pandavan/PandoraBox后想换回OpenWrt的话，首先在Breed中刷回小米路由器开发版的官方固件，然后ssh进路由器里参照[4.2.2 如果你刷了Breed](#如果你刷了Breed:)步骤刷机。

2. USB3.0会对路由器的2.4G频段信号造成干扰，不用时可以关掉。

3. OpenWrt默认语言为英文，可安装`luci-i18n-base-zh-cn`,`luci-i18n-base-zh-tw`安装简体/繁体中文。

4. 中国大陆用户有可能会用到[OpenWrt-disk](http://openwrt-dist.sourceforge.net)获取更多功能。

5. 中国大陆用户没科学上网的话会遇到下载OpenWrt固件速度超级慢的情况，可将源更换为[清华大学开源软件镜像站](http://mirrors.tuna.tsinghua.edu.cn/lede/)下载，只需替换教程中的`downloads.openwrt.org`为`mirrors.tuna.tsinghua.edu.cn/lede`。

6. 中国大陆使用OpenWrt可将opkg源改为国内：
在LuCI -> System -> Software -> Configuration中将Distribution feeds里的`http://downloads.openwrt.org`替换为`http://mirrors.tuna.tsinghua.edu.cn/lede`后Submit即可。
