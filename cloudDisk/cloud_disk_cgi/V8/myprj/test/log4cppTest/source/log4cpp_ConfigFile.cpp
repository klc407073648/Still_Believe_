#include <log4cpp/PropertyConfigurator.hh>
#include <log4cpp/Category.hh>

#include"LoggerManager.h"
#include"sort.h"
#include"randomArray.h"
using namespace std; 
using namespace minilog;
using namespace algorithm::sort;
using namespace algorithm::klc;


int main() {
    Sort* mySort = new Sort();
	randomArray* myRandomArray = new randomArray();
    delete mySort;
    delete myRandomArray;
    return 0;
}
