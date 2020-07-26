#include <iostream>
#include "MySQL.h"
#include <map>
#include <string.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include "Request.h"
#include "make_log.h"
#define HTML_PATH "/usr/local/nginx/test/"


#include <string.h>
#include <stdlib.h>
#include <cstdio>
#include <mysql/mysql.h>
#include "MutexLock.h"
#include "Util.h"
#include "EventSet.h"

#ifndef lint
static const char rcsid[] = "$Id: echo.c,v 1.5 1999/07/28 00:29:37 roberts Exp $";
#endif /* not lint */

#include "fcgi_config.h"
#include "fcgi_stdio.h"


#ifdef _WIN32
#include <process.h>
#else
extern char **environ;
#endif


#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>