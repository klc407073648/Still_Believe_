设计思路：
利用windows机器搭建nginx反向代理服务器，为创建的两个虚机的nginx的web服务器提供代理服务。

1.远程登录192.168.2.230和192.168.2.231虚机，启动nginx服务
[root@192 ~]# systemctl stop httpd
[root@192 ~]# systemctl stop firewalld.service
[root@192 ~]# /usr/local/nginx/sbin/nginx
[root@192 ~]#

2.校验是否能在window机器上访问对应IP地址：192.168.2.230 和 192.168.2.231

3.在window上启动nginx反向代理服务器，设置文件nginx.conf
格式例如：
 	#设置反向代理
	upstream myprj.test{
	   server 192.168.2.230:80;  
	}
	
	
	#要配置代理的服务器信息
	# myprj.com - 192.168.2.230 
    server {
        listen       80;
        server_name  myprj.com;

        location / {
            #设置代理
            proxy_pass http://myprj.test;
        }
	}
	
此外还需要手动修改windows机器上的C:\Windows\System32\drivers\etc 下的hosts文件，使得IP和主机名成映射关系。
192.168.2.230  myprj.com
192.168.2.231  myweb.com

4.使用对应域名访问nginx服务器，如果能够访问成功，表明反向代理服务器搭建完成。

5.进一步实现负载均衡：
格式例如：
    #设置反向代理
	upstream linux.test{
	   server 192.168.2.230:80; 
	   server 192.168.2.231:80;  	   
	}
	
	#设置权值
	#upstream linux.test{
	#   server 192.168.2.230:80 weight=5; 
	#   server 192.168.2.231:80 weight=1;  	   
	#}
	
 
	#要配置代理的服务器信息
    server {
        listen       80;
        server_name  localhost;
 
        location / {
            #设置代理
            proxy_pass http://linux.test;
        }
	}

6.可以直接通过http://localhost/  网址 可以根据权值概率  访问192.168.2.230:80 和192.168.2.231:80 的nginx服务器。