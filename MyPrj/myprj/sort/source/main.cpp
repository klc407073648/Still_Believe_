#include"sort.h"
#include"randomArray.h"
#include"LoggerManager.h"
#include <log4cpp/PropertyConfigurator.hh>
#include <log4cpp/Category.hh>

#define log4cpp_path "../conf/log4cpp.conf"

using namespace algorithm::sort;
using namespace algorithm::klc;
using namespace std;
using namespace minilog;
using namespace std;
//CREATE_LOG(MAIN);
int main()
{
	randomArray m_randomArray;
	//m_randomArray.myTest();
	
	Sort mysort;
	//CREATE_LOG(MAIN);
	
	
	try
    {
        //log4cpp::PropertyConfigurator::configure("/home/111/log4cpp_Prj_MutiExe111/myprj/sort/conf/log4cpp.conf");
		log4cpp::PropertyConfigurator::configure(log4cpp_path);
    }
    catch (log4cpp::ConfigureFailure& f)
    {
        std::cout << "Configure Problem: " << f.what() << std::endl;
        return -1;
    }
	//log4cpp::Category& root = log4cpp::Category::getRoot();
	log4cpp::Category* root = &log4cpp::Category::getRoot();
    log4cpp::Category& sample = log4cpp::Category::getInstance(std::string("sample"));
    
    root->fatal("root's log");
    sample.info("sub1's log");
	
	//LoggerManager::getInstance("MAIN11");
	//map<std::string,log4cpp::Category &>::iterator iter;

	
	
	
	/* for(iter = LoggerManager::getLogger().begin();iter!= LoggerManager::getLogger().end();iter++)
	{
		cout<<"size:"<<LoggerManager::getLogger().size()<<endl;//还待解决  不加这一�?后续logError不输�?
		//cout<<"________________"<<endl;
		if( iter->first == "MAIN11")
		{
			logError(iter->second,"main Func Test");
			break;
		}
	} */
	//logError(LoggerManager::getLogger("MAIN11"),"main Func Test");
	//LoggerManager* my_LoggerManager=new LoggerManager();
	return 0;
}