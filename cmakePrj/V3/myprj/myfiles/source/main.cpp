#include "MySQL.h"
#include "Config.h"
#include "CMyRedis.h"
#include "MyFilesCgi.h"

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
	
	MyFilesCgi* mMyFilesCgi = new MyFilesCgi();
    mMyFilesCgi->myfilesExec();

	return 0;
}
