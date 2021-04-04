<?php
	
	//接收表单传递的用户名和密码
	$name=$_POST['fname'];
	$age=$_POST['age'];
	echo "名字";
	echo $_POST["fname"];
	echo "<br>";
	echo "年龄";
    echo $_POST["age"]; 
	echo "<br>";
	echo "可以登录http://127.0.0.1/phpMyAdmin 查看html数据库下面的user表";
	//通过php连接到mysql数据库
	$conn=mysql_connect("localhost","root","klczxas789");	
			//选择数据库
			mysql_select_db("html");
 
			//设置客户端和连接字符集
			mysql_query("set names utf8");
 
			//通过php进行insert操作
			$sqlinsert="insert into user(fname,age) values('{$name}','{$age}')";
			mysql_query($sqlinsert);
			/*
			//通过php进行select操作
			$sqlselect="select * from t1 order by id";
 
			//添加用户信息到数据库
			mysql_query($sqlinsert);
			
			//返回用户信息字符集
			$result=mysql_query($sqlselect);
			
			echo "<h1>USER INFORMATION</h1>";
			echo "<hr>";
			echo "<table width='700px' border='1px'>";
			//从结果中拿出一行
			echo "<tr>";
			echo "<th>ID</th><th>USERNAME</th><th>PASSWORD</th>";
			echo "</tr>";
			while($row=mysql_fetch_assoc($result)){
				echo "<tr>";
				//打印出$row这一行
 
				echo "<td>{$row['id']}</td><td>{$row['username']}</td><td>{$row['password']}</td>";
				
				echo "</tr>";
			}
			echo "</table>";
 */
			//释放连接资源
		mysql_close($conn);					
?>
 

