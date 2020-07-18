g++ -o ./register ./register.cpp  -I /usr/include/mysql -L /usr/lib64/mysql -lmysqlclient -lpthread -lfcgi -std=c++11
g++ -o ./login ./login.cpp  -I /usr/include/mysql -L /usr/lib64/mysql -lmysqlclient -lpthread -lfcgi -std=c++11
g++ upload.cpp -o upload -lfcgi -std=c++11

spawn-fcgi  -a 127.0.0.1 -p 8050 -f ./register
spawn-fcgi  -a 127.0.0.1 -p 8051 -f ./login
spawn-fcgi  -a 127.0.0.1 -p 8052 -f ./upload