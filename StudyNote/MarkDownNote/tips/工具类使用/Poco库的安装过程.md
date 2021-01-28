- 概述

```
基本版本（Basic Edition）不需要外部依赖
完整版本（Complete Edition）：需要外部依赖OpenSSL、MySQL 客户端和ODBC
```
- 精简版
```
cd $cur_path/$poco_path
poco_output="$cur_path/$poco_path/output"
mkdir -p  $poco_output
tar -zxvf poco-1.10.1-all.tar.gz
cd ./poco-1.10.1-all
./configure --config=Linux --no-tests --no-samples --minimal --prefix=$poco_output
make && make install

./configure --no-tests --no-samples --omit=Data/ODBC,Data/SQLite,Data/PostgreSQL,MongoDB,Redis, --prefix=/home/MyPrj_V12/build_comlib/poco/output

选项说明：
–config=Linux：在Linux环境下编译
–minimal：最小功能编译，只构建XML、JSON、工具和网络
–no-tests：不编译测试部分的代码
–no-samples：不编译样例部分的代码
-prefix：指定安装路径
```

- 完整版
```
yum -y install unixODBC
yum -y install unixODBC-devel
yum -y install mysql
yum -y install mysql-devel
yum -y install openssl-devel 
yum -y install postgresql-devel
./configure
make && make install
```

- 密码

```
密码至少包含 数字和英文，长度6-20

String reg = "^(?![0-9]+$)(?![a-zA-Z]+$)[0-9A-Za-z]{6,20}$";
 
密码包含 数字,英文,字符中的两种以上，长度6-20

String reg = "^(?![0-9]+$)(?![a-z]+$)(?![A-Z]+$)(?!([^(0-9a-zA-Z)])+$).{6,20}$";
 
至少包含数字跟字母，可以有字符

String reg = "(?=.*([a-zA-Z].*))(?=.*[0-9].*)[a-zA-Z0-9-*/+.~!@#$%^&*()]{6,20}$";
```


