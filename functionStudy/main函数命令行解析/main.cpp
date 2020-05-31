#include <iostream>
#include <unistd.h>
#include <string>

using namespace std;

int main(int argc, char *argv[])
{
	int port = 8080;
	int thread_num = 10;
	std::string myStr = "testStr";
	
	int opt;
    const char *str = "p:t:s:";
    while ((opt = getopt(argc, argv, str)) != -1)
    {
        switch (opt)
        {
        case 'p':
        {
            port = atoi(optarg);
            break;
        }
        case 't':
        {
            thread_num = atoi(optarg);
            break;
        }
        case 's':
        {
            myStr = optarg;
            break;
        }
        default:
            break;
        }
    }
	cout<<"port: "<<port<<endl;
	cout<<"thread_num: "<<thread_num<<endl;
	cout<<"myStr: "<<myStr<<endl;
    return 0;
}

