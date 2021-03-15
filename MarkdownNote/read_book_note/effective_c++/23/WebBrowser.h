#ifndef _WEBBROWSER_H_
#define _WEBBROWSER_H_

#include<iostream>

using namespace std;

namespace  WebBrowserStuff{

class WebBrowser
{
    public:
        WebBrowser();
        ~WebBrowser();
		
		void clearCache();
		void clearHistory();
		void removeCookies();
    private:
        
};

} //WebBrowserStuffs


#endif //_WEBBROWSER_H_