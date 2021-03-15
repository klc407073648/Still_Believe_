#ifndef _WEBBROWSERCLEAR_H_
#define _WEBBROWSERCLEAR_H_

#include<iostream>

using namespace std;

namespace  WebBrowserStuff{

class WebBrowser;

void clearBrowser(WebBrowser& wb)
{
	wb.clearCache();
	wb.clearHistory();
	wb.removeCookies();
}


} //WebBrowserStuffs


#endif //_WEBBROWSERCLEAR_H_