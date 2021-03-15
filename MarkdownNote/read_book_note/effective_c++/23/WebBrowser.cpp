#include"WebBrowser.h"

namespace  WebBrowserStuff{

WebBrowser::WebBrowser()
{
	cout<<"WebBrowser()"<<endl;
}

WebBrowser::~WebBrowser()
{
	cout<<"~WebBrowser()"<<endl;		
}

void WebBrowser::clearCache()
{
	cout<<"clearCache()"<<endl;
}

void WebBrowser::clearHistory()
{
	cout<<"clearHistory()"<<endl;
}

void WebBrowser::removeCookies()
{
	cout<<"removeCookies()"<<endl;
}


}


