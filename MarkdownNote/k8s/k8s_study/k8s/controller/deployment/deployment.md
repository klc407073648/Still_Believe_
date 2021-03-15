# Deployment

Deployment 为 Pod 和 ReplicaSet 提供了一个声明式定义（declarative）方法，用来替代以前的 ReplicationController 来方便的管理应用。典型的应用场景包括：

- 定义 Deployment 来创建 Pod 和 ReplicaSet
- 滚动升级和回滚应用
- 扩容和缩容
- 暂停和继续 Deployment

比如一个简单的 nginx 应用可以定义为：

```yaml
apiVersion: extensions/v1beta1
kind: Deployment
metadata:
  name: nginx-deployment
spec:
  replicas: 3
  template:
    metadata:
      labels:
        app: nginx
    spec:
      containers:
      - name: nginx
        image: nginx:1.7.9
        ports:
        - containerPort: 80
```

扩容：

```bash
kubectl scale deployment nginx-deployment --replicas 10
```

如果集群支持 horizontal pod autoscaling 的话，还可以为 Deployment 设置自动扩展：

```bash
kubectl autoscale deployment nginx-deployment --min=10 --max=15 --cpu-percent=80
```

更新镜像也比较简单：

```bash
kubectl set image deployment/nginx-deployment nginx=nginx:1.9.1
```

回滚：

```bash
kubectl rollout undo deployment/nginx-deployment
```

具体流程如下：

```
创建deployment：
[root@192 docker]# kubectl apply -f nginx-deployment.yaml  --record
deployment "nginx-deployment" created

deployment通过rs去管理pod：
[root@192 docker]# kubectl get deployment
NAME               DESIRED   CURRENT   UP-TO-DATE   AVAILABLE   AGE
nginx-deployment   3         3         3            3           35s
[root@192 docker]# kubectl get rs
NAME                          DESIRED   CURRENT   READY     AGE
nginx-deployment-4087004473   3         3         3         44s
[root@192 docker]# kubectl get pod
NAME                                READY     STATUS    RESTARTS   AGE
nginx-deployment-4087004473-kgtw4   1/1       Running   0          57s
nginx-deployment-4087004473-qgmxh   1/1       Running   0          57s
nginx-deployment-4087004473-wmwlp   1/1       Running   0          57s

扩容成5个rs：
[root@192 docker]# kubectl scale deployment nginx-deployment --replicas 5
deployment "nginx-deployment" scaled
[root@192 docker]# kubectl get pod
NAME                                READY     STATUS    RESTARTS   AGE
nginx-deployment-4087004473-kgtw4   1/1       Running   0          1m
nginx-deployment-4087004473-m1ds6   1/1       Running   0          6s
nginx-deployment-4087004473-qgmxh   1/1       Running   0          1m
nginx-deployment-4087004473-vg24x   1/1       Running   0          6s
nginx-deployment-4087004473-wmwlp   1/1       Running   0          1m

查看nginx版本为1.7.9
[root@192 docker]# kubectl exec nginx-deployment-4087004473-kgtw4 -it -- /bin/bash
root@nginx-deployment-4087004473-kgtw4:/# nginx -v
nginx version: nginx/1.7.9

更新镜像为nginx:1.9.1
[root@192 docker]# kubectl set image deployment/nginx-deployment nginx=nginx:1.9.1
deployment "nginx-deployment" image updated
[root@192 docker]# kubectl get pod
NAME                                READY     STATUS        RESTARTS   AGE
nginx-deployment-3599678771-1sxb4   1/1       Running       0          7s
nginx-deployment-3599678771-4zxkn   1/1       Running       0          6s
nginx-deployment-3599678771-nk259   1/1       Running       0          6s
nginx-deployment-3599678771-wqpw3   1/1       Running       0          5s
nginx-deployment-3599678771-zlttc   1/1       Running       0          8s
nginx-deployment-4087004473-kgtw4   1/1       Terminating   0          3m

pod从原先的nginx-deployment-4087004473 rs 转到 nginx-deployment-3599678771
[root@192 docker]# kubectl get rs
NAME                          DESIRED   CURRENT   READY     AGE
nginx-deployment-3599678771   5         5         5         16s
nginx-deployment-4087004473   0         0         0         3m

查看nginx版本为1.9.1，表明镜像更新成功
[root@192 docker]# kubectl exec nginx-deployment-3599678771-1sxb4 -it -- /bin/bash
root@nginx-deployment-3599678771-1sxb4:/# nginx -v
nginx version: nginx/1.9.1

进行回滚操作：
[root@192 docker]# kubectl rollout undo deployment/nginx-deployment
deployment "nginx-deployment" rolled back

pod从 nginx-deployment-3599678771(rs) 转回 老的 nginx-deployment-4087004473(rs)
[root@192 docker]# kubectl get rs
NAME                          DESIRED   CURRENT   READY     AGE
nginx-deployment-3599678771   0         0         0         1m
nginx-deployment-4087004473   5         5         5         4m
[root@192 docker]# kubectl get pod
NAME                                READY     STATUS    RESTARTS   AGE
nginx-deployment-4087004473-1gfnb   1/1       Running   0          11s
nginx-deployment-4087004473-6wtq6   1/1       Running   0          14s
nginx-deployment-4087004473-91s0r   1/1       Running   0          13s
nginx-deployment-4087004473-vj84l   1/1       Running   0          14s
nginx-deployment-4087004473-zw7f7   1/1       Running   0          13s

查看版本回退成功：
[root@192 docker]# kubectl exec nginx-deployment-4087004473-1gfnb -it -- /bin/bash
root@nginx-deployment-4087004473-1gfnb:/# nginx -v
nginx version: nginx/1.7.9


```