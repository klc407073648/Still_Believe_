#include <Poco/RegularExpression.h>
#include <iostream>

using Poco::RegularExpression;

int main(int argc, char** argv)
{
    RegularExpression p("^[a-zA-Z_]{1}[a-zA-Z\\d_]{5,19}$");
	RegularExpression q("(?=.*([a-zA-Z].*))(?=.*[0-9].*)[a-zA-Z0-9-*/+.~!@#$%^&*()]{8,20}$");

    std::string user1="123klc";
    std::string user2="klc123";
	std::string passwd1="%%123klc@#-";
    std::string passwd2="55456123";
	
	if(p.match(user1))
	{
		std::cout<<"p.match(user1)"<<std::endl;
	}
	
	if(p.match(user2))
	{
		std::cout<<"p.match(user2)"<<std::endl;
	}
	
	if(q.match(passwd1))
	{
		std::cout<<"q.match(passwd1)"<<std::endl;
	}
	
	if(q.match(passwd2))
	{
		std::cout<<"q.match(passwd2)"<<std::endl;
	}
	
    
   return 0;
}