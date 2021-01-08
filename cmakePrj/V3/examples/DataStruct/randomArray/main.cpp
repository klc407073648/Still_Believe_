#include "randomArray.h"

using namespace algorithm::random;

int main()
{

	randomArray<int> *m_randomArray1 = new randomArray<int>(true);
	randomArray<double> *m_randomArray2 = new randomArray<double>(20, 50, 100);

	vector<int> m1 = m_randomArray1->getResultArray();
	vector<double> m2 = m_randomArray2->getResultArray();

	cout << "m1 begin" << endl;
	for (int i = 0; i < m1.size(); i++)
	{
		cout << m1[i] << " ";
	}
	cout << endl;
	cout << "m1 end" << endl;

	cout << "m2 begin" << endl;
	for (int i = 0; i < m2.size(); i++)
	{
		cout << m2[i] << " ";
	}
	cout << endl;
	cout << "m2 end" << endl;

	return 0;
}
