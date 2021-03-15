
#  ConfigMap学习

### 学习内容

* [Kubernetes之使用ConfigMap配置Pod](https://www.cnblogs.com/minseo/p/12512203.html)
* [Kubernetes官网例子](https://kubernetes.io/examples/pods/pod-single-configmap-env-variable.yaml)

## ConfigMap 创建

## 使用目录创建

```
[root@localhost k8s]# kubectl create configmap game-config --from-file=/home/k8s/dir
configmap "game-config" created

[root@localhost k8s]# kubectl get cm -o yaml
apiVersion: v1
items:
- apiVersion: v1
  data:
    game.properties: |-
      enemies=aliens
      lives=3
      enemies.cheat=true
      enemies.cheat.level=noGoodRotten
      secret.code.passphrase=UUDDLRLRBABAS
      secret.code.allowed=true
      secret.code.lives=30
    ui.properties: |
      color.good=purple
      color.bad=yellow
      allow.textmode=true
      how.nice.to.look=fairlyNice
  kind: ConfigMap
  metadata:
    creationTimestamp: 2021-02-01T23:24:01Z
    name: game-config
    namespace: default
    resourceVersion: "137679"
    selfLink: /api/v1/namespaces/default/configmaps/game-config
    uid: 914d9e0f-64e4-11eb-b628-000c2925d2e7
kind: List
metadata: {}
resourceVersion: ""
selfLink: ""

[root@localhost k8s]# kubectl describe cm game-config
Name:           game-config
Namespace:      default
Labels:         <none>
Annotations:    <none>

Data
====
game.properties:        157 bytes
ui.properties:          83 bytes
```

## 使用文件创建
```
[root@localhost k8s]# kubectl create configmap game-config-2 --from-file=/home/k8s/dir/game.properties
configmap "game-config-2" created

[root@localhost k8s]# kubectl get cm game-config-2 -o yaml
apiVersion: v1
data:
  game.properties: |-
    enemies=aliens
    lives=3
    enemies.cheat=true
    enemies.cheat.level=noGoodRotten
    secret.code.passphrase=UUDDLRLRBABAS
    secret.code.allowed=true
    secret.code.lives=30
kind: ConfigMap
metadata:
  creationTimestamp: 2021-02-01T23:27:47Z
  name: game-config-2
  namespace: default
  resourceVersion: "137928"
  selfLink: /api/v1/namespaces/default/configmaps/game-config-2
  uid: 17ef9599-64e5-11eb-b628-000c2925d2e7

```

## 使用字面值创建
```
[root@localhost k8s]# kubectl create configmap special-config --from-literal=special.how=very --from-literal=special.type=charm
configmap "special-config" created

[root@localhost k8s]# kubectl get cm special-config -o yaml                                        apiVersion: v1
data:
  special.how: very
  special.type: charm
kind: ConfigMap
metadata:
  creationTimestamp: 2021-02-01T23:30:44Z
  name: special-config
  namespace: default
  resourceVersion: "138123"
  selfLink: /api/v1/namespaces/default/configmaps/special-config
  uid: 816d8bdd-64e5-11eb-b628-000c2925d2e7
```

## Pod中使用ConfigMap

```yaml
apiVersion: v1
kind: ConfigMap
metadata:
  name: env-config
  namespace: default
data:
  log_level: DEBUG
```

```
[root@localhost k8s]# kubectl apply -f env-config.yaml
configmap "env-config" created

[root@localhost k8s]# kubectl get cm env-config -o yaml
apiVersion: v1
data:
  log_level: DEBUG
kind: ConfigMap
metadata:
  annotations:
    kubectl.kubernetes.io/last-applied-configuration: '{"kind":"ConfigMap","apiVersion":"v1","metadata":{"name":"env-config","namespace":"default","creationTimestamp":null},"data":{"log_level":"DEBUG"}}'
  creationTimestamp: 2021-02-01T23:37:15Z
  name: env-config
  namespace: default
  resourceVersion: "138553"
  selfLink: /api/v1/namespaces/default/configmaps/env-config
  uid: 6a68d23d-64e6-11eb-b628-000c2925d2e7
```
 
## 使用ConfigMap 来替代环境变量

```
[root@localhost k8s]# kubectl apply -f pod-single-configmap-env-variable.yaml
pod "dapi-test-pod" created

[root@localhost k8s]# kubectl logs dapi-test-pod |grep SPECIAL
SPECIAL_TYPE_KEY=charm
SPECIAL_LEVEL_KEY=very
```

## 通过数据卷插件使用ConfigMap
```
[root@localhost k8s]# kubectl apply -f volume.yaml
pod "dapi-test-pod" created

[root@localhost k8s]# kubectl logs dapi-test-pod
special.how
special.type

```