#ifndef _RANDOMARRAR_IMPL_H_
#define _RANDOMARRAR_IMPL_H_

#include <stdio.h>
#include <stdlib.h>

namespace algorithm
{
	namespace  random
	{
        template<class T>
		randomArray<T>::randomArray(bool isTypeInt)
		{
			m_size=10;
			m_minValue =0 ;
			m_maxValue =40 ;
			m_isTypeInt=isTypeInt;
			generateRandomArray();
		}

		template<class T>
		randomArray<T>::randomArray(int size,T minValue,T maxValue,bool isTypeInt)
		{
			m_size=size;
			m_minValue =minValue ;
			m_maxValue =maxValue ;
			m_isTypeInt=isTypeInt;
			generateRandomArray();
		}

		template<class T>
		randomArray<T>::~randomArray()
		{

		}

        template<class T>
		void randomArray<T>::generateRandomArray()
		{
			generateResult.clear();
			//srand((unsigned)time(int(rand())));
			time_t t=(time_t)rand();
			srand((unsigned) time(&t)); 
			//m_maxValue 
			//int length = (int)((size)*((rand() % 100)*0.01));
			//m_size = (int)((m_size)*((rand() % 100)*0.01))+1;
			if (m_isTypeInt==false)
			{
				for (int i = 0; i < m_size; i++)
				{
					//T random[i] = rand() % (N + 1) / (float)(N + 1)
					//b.push_back((int)(rand() % value)- (int)(rand() % value));
					generateResult.push_back(((T)((rand()%10000))/10000)*(m_maxValue-m_minValue)+m_minValue);
				}
			}
			else
			{
				for (int i = 0; i < m_size; i++)
				{
					//T random[i] = rand() % (N + 1) / (float)(N + 1)
					//b.push_back((int)(rand() % value)- (int)(rand() % value));
					generateResult.push_back((rand()%(int)(m_maxValue-m_minValue))+m_minValue);
				}
			}
			
		}
	    
		template<class T>
		const vector<T>&  randomArray<T>::getResultArray()
		{
			return generateResult;
		}
	}
}

#endif //_RANDOMARRAR_IMPL_H_
