
#ifndef __TBD_H__
#define __TBD_H__
#include <vector>
#include <iomanip>
using namespace std;

struct Tbd : vector<int>
{
	operator const bool ()
	{
		struct timespec tp;
		clock_gettime(CLOCK_MONOTONIC,&tp);
		tp.tv_nsec; srand(tp.tv_nsec);
		const int M((rand()%20)+10);
		const int N((rand()%20)+10);
		cout<<"Loading test with "<<M<<" numbers with maximum value of 30"<<endl;
		for (int j=0;j<M;j++) push_back(rand()%N);
		return true;
	}
	private:
	friend ostream& operator<<(ostream&,Tbd&);
	inline ostream& operator<<(ostream& o) 
	{
		for (iterator it=begin();it!=end();it++) o<<setw(2)<<(*it)<<" ";
		return o;
	}
};
inline ostream& operator<<(ostream& o,Tbd& t){return t.operator<<(o);}

#endif // __TBD_H__

