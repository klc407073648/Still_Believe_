#include "main.h"
#include "Event.h"

using namespace std;

int main(int argc, char** argv) 
{
	char **initialEnv = environ;
    int count = 0;

    Event mEvent(0);
    //Login mLogin;
    Register mReg;
	Login mLogin;
    Upload mUpload;
    mEvent.addEvent("/register", eventBind(Register::reg, mReg));
	mEvent.addEvent("/login", eventBind(Login::login, mLogin));
    mEvent.addEvent("/logout", eventBind(Login::logout, mLogin));
    mEvent.addEvent("/upload", eventBind(Upload::upload, mUpload));
    mEvent.exec();

	return 0;
}
