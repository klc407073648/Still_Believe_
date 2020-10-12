#include "SkipList.h"

int main()
{
	Skiplist* m_Skiplist = new Skiplist();
	
	m_Skiplist->insert(7);
    m_Skiplist->insert(14);
    m_Skiplist->insert(21);
    m_Skiplist->insert(32);
    m_Skiplist->insert(37);
    m_Skiplist->insert(71);
    m_Skiplist->insert(85);
	
	m_Skiplist->showSkiplist();

	return 0;
}