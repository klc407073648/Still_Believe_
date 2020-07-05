1.启动nginx服务器

systemctl stop httpd
systemctl stop firewalld.service

需要修改nginx.conf文件加入
location /echo {
			fastcgi_pass 127.0.0.1:8002;
			include fastcgi.conf;
		}

/usr/local/nginx/sbin/nginx

3.生成echo可执行文件
gcc echo.c -o echo -lfcgi

2.启动fcgi后台进程
spawn-fcgi  -a 127.0.0.1 -p 8001 -f ./echo

3.在window下点击test.html，填写完信息后直接按提交，就会返回处理结果