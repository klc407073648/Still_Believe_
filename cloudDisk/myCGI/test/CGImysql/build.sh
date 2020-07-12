g++ -o ./register ./register.cpp  -I /usr/include/mysql -L /usr/lib64/mysql -lmysqlclient -lpthread -lfcgi -std=c++11
g++ -o ./login ./login.cpp  -I /usr/include/mysql -L /usr/lib64/mysql -lmysqlclient -lpthread -lfcgi -std=c++11
g++ upload.cpp -o upload -lfcgi -std=c++11