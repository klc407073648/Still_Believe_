#include "MySQL.h"
#include "Config.h"
#include "CMyRedis.h"
#include "LoginCgi.h"

#include "fcgi_config.h"
#include "fcgi_stdio.h"

#ifdef _WIN32
#include <process.h>
#else
extern char **environ;
#endif

using namespace std;

int main(int argc, char** argv) 
{
	char **initialEnv = environ;
    int count = 0;
	
	//Config mConfig;
	LoginCgi* mLoginCgi = new LoginCgi();
    mLoginCgi->loginExec();

	return 0;
}
