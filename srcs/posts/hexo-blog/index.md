---
title: Hexo 你的博客
date: 2016-09-15 14:21:00
layout: post
tags:
- HEXO
- 静态博客
- Github_Pages
categories:
- 教程
- Hexo
---
# 前言：
在我建好我的网站后，就一直想把我搭建博客的过程记录下来。搭建博客的过程蛮有趣的,不同的平台搭建方法还有所不同。

这期间对新手来说会面对各种各样的问题。本篇文章记录了我建造Hexo博客的过程，很多细节方面的东西都能在Hexo官网找到。

![Hexo](https://blog.starry-s.xyz/images/making-blog/hexo1.jpg "Hexo")
<!--more-->

> 最后内容更新时间：2018年7月29日。

# 安装Hexo

你需要一台你经常使用的电脑来写博客，Hexo可以在Windows，Linux还有MacOS作业系统上部署。所以可以说它基本上没有系统方面的限制,所以在你没有电脑的时候还可以直接把Hexo部署到树莓派上面（当然对树莓派来说部署时候会慢一些），租一个便宜点的外国的linux的vps也可以。本章中我使用我的linux电脑来部署hexo博客。

> 如果你不会使用linux，不妨借此机会学习一下linux吧，你会发现很多有趣的事情的。

 **[HEXO官网：hexo.io](https://hexo.io/)**
## 首先，我们需要在电脑上安装Git和Node.js
### 安装Git
```
## Ubuntu,Debian
$ sudo apt-get install git-core
## CentOS
sudo yum install git-core
## Arch linux
sudo pacman -S git-core
```

### 安装Node.js

请参考[Hexo官网的教程](https://hexo.io/zh-cn/docs/#%E5%AE%89%E8%A3%85-Node-js)。

## 安装Hexo

重启你的终端后

```
$ npm install hexo-cli -g
```

安装的时常与网络情况有关。耐心等待即可。

安装Hexo后就可以使用Hexo生成一个你自己的博客了！

```
$ mkdir blog
$ hexo init blog
$ cd blog
$ npm install
```

其中`blog`就是你的博客文件的文件夹名字，执行以上命令部署所需文件。

接下来更改一些你喜欢的配置使其成为你的独一无二的博客。

# DIY你的博客

博客根目录为：
```
.
├── _config.yml
├── package.json
├── scaffolds
├── source
|   ├── _drafts
|   └── _posts
└── themes
```

这时候你可以在终端打开你的Hexo文件夹输入`hexo s`后打开浏览器访问`http://localhost:4000/`来通过内网访问你的博客。不过这只是你的博客的雏形，你需要向里面加入许多你自己的元素进去。

## \_config.yml

网站的配置信息，许多参数都保存在这个文件里面。Hexo官网有详细的[配置教程](https://hexo.io/zh-cn/docs/configuration.html),请参考Hexo官网一步一步的按照你自己的口味配置。

## themes

`themes`文件夹存放主题文件，Hexo根据主题文件来生成静态文件。

[Hexo官网](https://hexo.io/themes/)上有一些很不错的主题，选一个你喜欢的用`git clone`下来！

## 写文章

配置好主题后就可以水一篇文章来告诉世界你来啦！！！


`hexo new [layout] <tytle>`来生成一篇新文章，文章默认保存在`source/_posts`文件夹内。

hexo支持Markdown语法。所以你根本不需要写复杂的html代码，但是这并不代表Hexo不支持html。

接下来就需要一个域名来让更多的人通过这个域名访问你的网站了。

## 购买域名

没错！域名是需要来买(租用)的。这里我推荐在[godaddy](https://sg.godaddy.com/)<s>狗爹</s>上面买域名。个人博客的话不必买太贵的域名，顺便说一句，godaddy是可以用优惠码的。优惠码网上就能找到，然后就能享受几折的优惠啦。如果你在国内购买了域名的话你就得按照国家的法律规定进行备案操作，新手来讲建议在国外买域名。

## 使用github

> 使用pages服务的话就需要在你的博客根目录下的`source`文件夹内新建一个纯文本文件：`CNAME`,把你的博客的网址输入进去。

github有pages服务，很多人都拿它做网站，又不收费而且很稳定。

 **接下来请参考[github pages](https://pages.github.com/)官网食用以下教程**

> 如果你不熟悉git，请先按照官网教程熟悉一下他的工作原理再进行操作。

### 首先绑定sshkey到你的github和coding上

使用ssh上传文件就不需要每次部署博客时都输入一遍用户名和密码了。

> 首先使用一个邮箱注册github，以方便绑定sshkey

``` bash
$ ssh-keygen -t rsa -C"你申请Github和coding账号的邮箱"
```
系统会要求你输入密码，这个密码在每次提交项目到github时会用到。

* 复制生成的sshkey

  生成的sshkey保存在`~/.ssh/id_rsa.pub`内。使用`pbcopy < ~/.ssh/id_rsa.pub`复制到剪切版。

* 添加sshkey到github上

  在Github登陆按钮处找到Settings,点击Add SSH key按钮把复制下来的密钥粘贴到key栏。

* 测试sshkey能否链接成功

``` bash
$ ssh -T git@github.com
```
如有问题，就再试着重新添加一遍。

只有出现了Hi！等字样，才能证明链接成功！

## 部署pages
### 部署github pages
打开github个人主页，选择`Repositories`,点击New。新建一个仓库。名字自定（不要输入中文）。

![github new repositories](https://blog.starry-s.xyz/images/making-blog/github1.jpg)

![github set repositories](https://blog.starry-s.xyz/images/making-blog/github1.jpg)

### 新建好仓库后，编辑`_config.yml`将文件上传到github和coding pages上。

设置好_config.yml的deploy就能在每次执行`hexo d`时把文件上传到giuthub和coding上面了。

> 首先需要输入`npm install hexo-deployer-git --save`安装一下hexo-deployer-git

```
deploy:
- type: git
  repo: 'git@github.com:your_username/your_repositories'
  branch: master
```

接下来执行生成静态文件并上传

``` bash
$ hexo g
$ hexo d
```
之后每次上传生成的文件都是通过`hexo d`这个指令来完成的。

### 开启Pages服务

>在部署好的github仓库里面打开settings,在GitHub Pages里开启Pages服务。

![GitHub Pages](https://blog.starry-s.xyz/images/making-blog/github3.jpg)

![Github Pages](https://blog.starry-s.xyz/images/making-blog/github4.jpg)

**
注：务必选择正确的分支！本教程设置的github的分支为master
**

## DNS分流

> 使用CloudXNS解析域名。

1.在CloudXNS中添加域名。

2.回到godaddy的我的域名，选择：管理DNS，更改域名服务器如图：

![Godaddy DNS](https://blog.starry-s.xyz/images/making-blog/godaddy1.jpg)

3.等几分钟后，回到CloudXNS，添加记录如图：

![CloudXNS](https://blog.starry-s.xyz/images/making-blog/dns1.jpg)


> **更改记录值时把指向国外的Github的记录值改为`"你的用户名".github.io'**

全部部署完后，博客就彻底建完了。

如果刚建好的博客还不能完美访问，可能是因为全球DNS解析时间较长，需要等几个小时后重试。

# CDN
这步可选，如果你想更快的访问你的博客的话弄个CDN是个很好的选择，国内的CDN加速服务都需要ICP备案，没有备案的话一些国外的免费CDN也很好用，但是速度可能没有提升反而变慢，可以自行选择。


<br/>
<br/>

# Hexo 你的博客 &hearts;
