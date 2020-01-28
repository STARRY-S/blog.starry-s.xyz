---
title: 使用PaperMC搭建Minecraft服务器教程
createdDate: 2019-08-25 22:19:18
layout: post
comment: true
zone: Asia/Shanghai
tags:
- Minecraft
- 教程
categories:
- 教程
- Minecraft
---
很久以前尝试过在树莓派上搭建一个Minecraft服务器，当时刚接触Linux踩了好多坑，前一阵子在腾讯的学生版服务器上搭了一个可以几个人玩的小服务器，在这里把我搭建服务器的大致过程记录下来。

<!--more-->

实际上只要有一台能运行Java的电脑，搭建Minecraft服务器的方式都没有多大的变化，所以无论你是Windows主机还是Linux云服务器，性能够好都可以作为服务器使用的。

本篇文章为Linux上搭建服务器的教程所以需要有一定Linux基础的人才能读懂，如果是一位没用过Linux萌新的话不妨借此机会接触一下Linux呢，说不定会打开什么新世界大门（

顺便声明一下本篇教程所指的「Minecraft」非网易代理版「我的世界」。

<!-- <iframe frameborder="no" border="0" marginwidth="0" marginheight="0" width=100% height=86 src="//music.163.com/outchain/player?type=2&id=4888328&auto=0&height=66"></iframe> -->

<!--aplayer
{
    "name": "炉心融解",
    "artist": "Iroha/kuma/鏡音リン",
    "theme": "#36BDE3",
    "url": "https://music.starry-s.xyz/music/0252_530f_0559_58140863e1de649de504237f43d6124f.m4a",
    "cover": "https://music.starry-s.xyz/music/cover/6036318836780705.jpg"
}
-->

----

# 安装Java

因为大多数插件都对Java8有良好的支持，所以推荐安装Java8。

```
# Debian/Ubuntu
$ sudo apt-get install openjdk-8-jdk

# Archlinux
$ sudo pacman -S jdk8-openjdk
```

可以通过`java -version`查看版本号的指令来确定Java是否安装成功。

除了通过包管理器安装Java外还可以在[Oracle官网](https://www.oracle.com/technetwork/java/javase/downloads/jdk8-downloads-2133151.html)下载安装包解压在常用的目录中使用Java（不推荐）
```
$ cd java
$ tar -zxvf ./jdk-8*.tar.gz
```
# 下载Paper

本篇使用Paper搭建服务器因为Paper对性能优化较好。

在[Paper的官网](https://papermc.io/downloads)上下载最新版本的jar文件。

使用指令`java -jar paper-*.jar`就可以运行Paper。

首次运行Paper会下载很多文件，身处天朝可能需要等上很长时间才能下载完，然后因为是第一次运行服务器没有同意[MoJang的EULA](https://account.mojang.com/documents/minecraft_eula)而报错，使用文本编辑器更改`eula.txt`中的`eula=true`后再运行服务器即可。

如果没有正版帐号的话是无法登录进服务器内的，需要编辑`server.properties`中的`onlinemode=false`关掉正版验证后重启服务器才能登录。

可以添加flags例如`java -Xmx1024M -Xms1024M -jar paper-*.jar`指定消耗内存的大小来运行服务器，更多有关Flags放说明可参考[此处](https://aikar.co/2018/07/02/tuning-the-jvm-g1gc-garbage-collector-flags-for-minecraft/)。

需要注意的是内存的最大值即`-Xmx`不能超过你的服务器主机的剩余内存否则服务器很有可能在人多的时候内存不足而崩掉，可通过`free -m`查看云主机所剩余的内存。

接下来就是参照[Paper的官方文档](https://paper.readthedocs.io/en/stable/server/configuration.html)修改一些配置文件。

# 安装常用插件

我目前只给服务器装了俩插件，主要因为学生云主机的性能并不高没搞太耗费性能的插件。

在网络上下载好插件的jar文件后将其放入服务器的`plugins/`文件夹，重启服务器即可。

可以在网上找一些防止高频红石，防止作弊，用户登录这类的插件，在此不全部列举（因为我没用过）。

## WorldEdit

创世神插件算得上是建筑党必备的插件了，可在[Bukkit](https://dev.bukkit.org/projects/worldedit)上下载。

比较不错且详细的[中文教程在此](http://mineplugin.org/WorldEdit)，服务器上的话只要安装上插件获得op权限的帐号就能用，一把斧子就能指点江山。

## AuthMe Reloaded

因为服务器关掉了正版验证意味着所有人随便起一个用户名就能登录进服务器而且十分不安全，所以装了AuthMe。

AuthMe Reloaded的下载网址：[https://dev.bukkit.org/projects/authme-reloaded](https://dev.bukkit.org/projects/authme-reloaded)

因为AuthMe的配置文件太多不详细列举了，Github和Bukkit上都有文档教程，youtube上还有视频教程，常用的指令就是`/register`、`/login`，可修改一下配置文件改一下语言以及邮箱smtp配置这类的，mysql配置不用更改就能用而且我也不会改，smtp安全起见并不推荐用gmail邮箱，要是拥有域名的话搞个企业邮箱也是个不错的选择。

# 常见问题

1. AuthMe安装后会覆盖掉服务器记录的原有的用户权限，需要重新设置op帐号和gamemode这类权限。
2. 更新服务器版本只需要下载新的Paper的jar文件并覆盖掉老的文件即可，注意当Minecraft的版本涉及更新例如从1.12到1.13这样的更新时务必做好备份工作并注意原有的插件可能在新版本不适用的问题。
3. 在低配置主机上运行服务器时log中容易遇到有关「服务器已经xxx毫秒没有响应」这类的“报错”，这不是一个bug，只是提醒你的服务器有点卡了，实际并无大碍，可以无视。
4. 服务器长时间后台运行可使用GNU Screen程序。
5. 比较简单粗暴的备份服务器的方式就是用`tar`打个压缩包存起来，除此之外还可以装一些自动备份的插件。

# Done

服务器搭建好了之后，不妨康康「[使用HMCL启动Minecraft游戏教程](/posts/hmcl-minecraft/)」这篇文章安装游戏客户端食用。
