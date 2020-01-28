---
title: 斐讯K2折腾之——安装OpenWrt/LEDE
createdDate: 2017-05-13 22:58:07
layout: post
comment: true
zone: Asia/Shanghai
tags:
- 斐讯k2
- LEDE
- OpenWrt
- 路由器教程
categories:
- 教程
- 路由器
---
![LEDE (OpenWrt)](images/lede.jpg)

<!--more-->

<!--aplayer
{
    "name": "Higher",
    "artist": "Tobu",
    "theme": "#0080ff",
    "url": "https://music.starry-s.xyz/music/510e_050c_025a_fd784ea9642fd342cc0850037428d687.m4a",
    "cover": "https://music.starry-s.xyz/music/cover/109951164239508401.jpg"
}
-->

[OpenWrt/LEDE](https://openwrt.org)是适合于嵌入式设备的一个Linux发行版。

> LEDE与OpenWrt官方宣布合并，`lede-project.org`域名将重定向到相应的`openwrt.org`域名上。

几个月前入的斐讯K2，解决了家里Wifi信号覆盖范围小以及莫名卡顿等问题。入手当天就拆开看了一眼然后就直接刷固件了。前一阵子突然搜到了LEDE，于是又走上了折腾的道路，顺便赶在这几天~~漆钟烤柿过后~~把博客码出来。

<div class="alert-green">最后更新：2018年8月22日。</div>

# 刷机前的准备

首先网上找几个比较靠谱的刷机教程照着做就可以了，我是按照[这个教程](https://blog.phpgao.com/phicomm_k2.html)刷写固件的。需要注意的是我的路由器硬件版本为A2，现在已买不到了。其他版本的刷机方法有所不同，需要注意以免变砖。

总体上大多数的路由器刷机都可以总结为以下几个步骤：

1. 先安装Bootloader
> [Bootloader 的原帖](http://www.right.com.cn/forum/thread-161906-1-1.html)
> [斐讯K2 V22.X.X.X 新版固件 刷机教程 (开telnet,安装SSH,adbyby,刷breed,华硕Padavan）](http://www.right.com.cn/forum/thread-191833-1-1.html)

刷好Bootloader后就可以放心的去刷别的系统喽~

2. 刷入系统。

# 安装LEDE系统。

[OpenWrt/LEDE官网](https://openwrt.org)上有详细的介绍，Document页面很多的内容已翻译为中文，英文的部分可以拿着[谷歌翻译](https://translate.google.cn)慢慢读（谷歌翻译坠吼用了）。

* [Openwrt Documentation](https://openwrt.org/zh/docs/start)

## 下载固件：

Openwrt官网上面有介绍最新的稳定版下载链接，各版本的[下载地址在此](https://downloads.openwrt.org/releases/)，选择 **你想下载的版本 -> ramips -> mt7620** 找到psg1218开头的固件就是斐讯k2所需要的固件啦。

注：大陆网络环境较为特殊下载固件出奇的慢，可以使用国内的一些镜像站下载固件，速度很友好。[清华大学开源软件镜像站](https://mirrors.tuna.tsinghua.edu.cn/lede/)是个不错的选择。

## 在Breed中刷机（推荐）

路由器拔掉电源，长按reset复位键接通电源10秒后松开复位键，用网线将路由器与电脑连接，打开浏览器网址输入`192.168.1.1`就可以进入Breed后台刷机了。

> 进不去的话检查一下电脑的网络设置，通常使用dhcp就能分配得到ip地址，或者手动设置ipv4，将ip改为`192.168.1.2`，网关为`192.168.1.1`,子网掩码`255.255.255.0`，清除一下浏览器历史记录缓存什么的，再重新链接试试。
> 再或者就是你有必要检查一下你的路由器是否真的进入breed后台了没。

## 在Luci里升级

其实有很多种方法来刷LEDE，如果你的路由器已经刷了PandoraBox这类的基于Openwrt的系统的话，可以直接通过Luci界面里的 **@系统 > 备份/升级 刷写新的固件**
选择你下载好的`.bin`固件直接更新系统（记得取消勾选`保留配置`选项）。

## SSH到路由器中刷机

除了Luci界面里刷机外可以直接将文件通过SCP上传到路由器里然后SSH到你的路由器就可以刷些系统了。Linux用户直接终端输入：
```
# scp /你下载的刷机包的目录/XXX.bin root@192.168.1.1:/tmp/
```
接下来SSH到路由器：
```
# jffs2reset -y
# sysupgrade -n /tmp/psg1218-*.bin
```
当然了你除了用scp之外还可以直接用wget下载刷机包免去scp传输这一步，总之怎么做随你。
警告：刷机过程中不要断电！

# 配置路由器

LEDE和OpenWrt一样自带Luci界面，打开浏览器输入`192.168.1.1`不用输入密码选择登录就可以直接登入后台了。路由器语言默认是英文的。

首先务必更改一下root密码后再操作。

如果想安装软件的话建议把软件源改为国内，清华源是一个不错的选择，在**Luci @ System > Software > Configuration**中找到**Distribution feeds**这一栏，将其中的`http://downloads.lede-project.org`全部替换为`https://mirrors.tuna.tsinghua.edu.cn/lede`，点击右下角的Submit即可将软件源改为国内。

如果想将语言改为简体中文就需要安装`luci-i18n-base-zh-cn`，装软件就需要联网，因此需要你凭借强大的猜的能力（毕竟是英语渣一枚，学霸可以无视我）摸索着找到网络连接进行一番设置后再ssh到路由器上装软件。

通常是在 **Luci @ Network > Interfaces > WAN** 中选择 **Edit** 配置你的网络，默认为`DHCP client`。

```
$ ssh root@192.168.1.1

// 更新一下资源列表
# opkg update

// 安装软件（简体中文为zh-cn，繁体中文为zh-tw）
# opkg install luci-i18n-base-zh-cn
# opkg install luci-i18n-base-zh-tw
```
opkg是一个轻便的包管理器，`opkg install`相当于Linux里常用的`apt-get install`,`pacman -S`,`emerge`这些指令。

安装好中文软件包后去 **Luci @ System > System > Language and style** 中更改语言就可以更改语言了。

接下来在 **系统 > 系统 > 基本设置** 中改一下时区:`Asha/Hong kong`，同步一下浏览器时间。
之后再按照你个人喜好配置你的路由器啦。

# Lede的其他配置

如果你需要用到端口转发和UPNP，例如和小伙伴[联机我的世界](http://blog.starry-s.xyz/2016/11/25/raspbettypi-mc-server/)或玩其他联机游戏什么的，需要用到`miniupnpd`,`luci-app-upnp`这两个软件。

使用动态DNS需要用到`ddns-scripts`,`luci-app-ddns`这两个软件。

以上这几个软件可以完美的和小伙伴联机我的世界了～

以及大陆用户可能会用到的[openwrt-dist](http://openwrt-dist.sourceforge.net/)。

目前Openwrt dist源的访问已变得不是很友好。

不建议直接使用`opkg install luci-app-shadowsocks-libev`和`opkg install shadowsocks-libev`来获取更多功能，因为软件源里安装的软件版本较低，可以看一下[Openwrt dist](http://openwrt-dist.sourceforge.net/)，或者考虑自己编译软件。

Openwrt Wiki正在完善，很多配置介绍都在wiki里，可以多看看。

# 推荐一些资料

* [小米路由器MINI折腾篇](http://blog.starry-s.xyz/2016/09/28/xiaomi-mini-fan-1/)
* [老高的技术博客](https://blog.phpgao.com/xiaomi_router.html)
* [LEDE_Documents](https://lede-project.org/zh/docs/start)
