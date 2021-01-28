#!/bin/bash
ser=("etcd" "docker" "kube-apiserver" "kube-controller-manager" "kube-scheduler" "kubelet" "kube-proxy")
for i in ${ser[@]};do
  systemctl restart $i
  systemctl enable $i
  systemctl status $i

done
