# k8s集群安装

## 参考资料

* [搭建 Harbor 私有镜像仓库](https://blog.csdn.net/zhichunqi/article/details/104477215/)
* [部署Harbor私有镜像仓库](https://blog.csdn.net/weixin_41465338/article/details/80146218)

## Harbor安装

Harbor简介

```
Harbor 是一个 CNCF 基金会托管的开源的可信的云原生 docker registry 项目，可以用于存储、签名、扫描镜像内容，
   Harbor 通过添加一些常用的功能如安全性、身份权限管理等来扩展 docker registry 项目，此外还支持在 registry 之间复制镜像，
   还提供更加高级的安全功能，如用户管理、访问控制和活动审计等，在新版本中还添加了 Helm 仓库托管的支持。
      Harbor 最核心的功能就是给 docker registry 添加上一层权限保护的功能，要实现这个功能，就需要我们在使用 docker 
   login、pull、push 等命令的时候进行拦截，先进行一些权限相关的校验，再进行操作，其实这一系列的操作 docker registry v2 就
   已经为我们提供了支持，v2 集成了一个安全认证的功能，将安全认证暴露给外部服务，让外部服务去实现。
```

主要执行命令如下：

```bash
cd /home/klc/software/
mkdir harbor
cd ./harbor/
wget https://github.com/goharbor/harbor/releases/download/v1.10.1/harbor-offline-installer-v1.10.1.tgz
tar xf harbor-offline-installer-v1.10.1.tgz

cd harbor/
vim harbor.yml(修改hostname)

yum install -y docker-ce-18.09.9 docker-ce-cli-18.09.9 containerd.io
 
curl -L "https://github.com/docker/compose/releases/download/1.23.2/docker-compose-$(uname -s)-$(uname -m)" -o /usr/local/bin/docker-compose
chmod +x /usr/local/bin/docker-compose
docker-compose --version
  
vim harbor.yml (注释https)

./install.sh
```

```
--ignore-preflight-errors=NumCPU

Centos7部署k8s集群 v1.18.2:
https://blog.csdn.net/weixin_42181917/article/details/106264731

安装cfssl证书工具:
https://www.icode9.com/content-4-806115.html

kubeadm config print init-defaults > kubeadm-config.yaml
```

问题记录：

```
安装Docker harbor报错：Docker harborERROR:root:Error: The protocol is https but attribute ssl_cert is not set

解决方法：
注释下面这些配置。https的注释
```
