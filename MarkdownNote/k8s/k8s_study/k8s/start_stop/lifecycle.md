
#  pod的生命周期管理

## kubernetes--pod的生命周期管理（PostStart，PreStop）

* lifecycle
    * 创建资源对象时，可以使用lifecycle来管理容器在运行前和关闭前的一些动作。

* lifecycle有两种回调函数:
    * PostStart：容器创建成功后，运行前的任务，用于资源部署、环境准备等。
    * PreStop：在容器被终止前的任务，用于优雅关闭应用程序、通知其他系统等等。

具体实例如下:

* 在容器创建成功后, 将 “test postStart" 打印消息，写入/tmp/lifecycle

* 在容器终止前, 将 “test preStop" 打印消息，写入/tmp/lifecycle

```
apiVersion: v1
kind: Pod
metadata:
  name: lifecycle-demo
  namespace: default
spec:
  containers:
  - name: lifecycle-demo-container
    image: my_hello_docker
    imagePullPolicy: IfNotPresent
    lifecycle:
      postStart:
        exec:
          command: ["/bin/sh", "-c", "echo test postStart > /tmp/lifecycle"]
      preStop:
        exec:
          command: ["/bin/sh", "-c", "echo test preStop > /tmp/lifecycle"]
```