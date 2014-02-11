#include <iostream>
using namespace std;
#include "tbd.h"

#include "machine.h"
#include "products.h"
#include "tests.h"
#include <set>
#include <vector>
using namespace std;

#define KeyType int 
#define ContainerType vector<KeyType> 
#include "methods.h"

#undef KeyType
#undef ContainerType
#define KeyType float 
#define ContainerType vector<KeyType> 
#include "methods.h"

#undef KeyType
#undef ContainerType
#define KeyType CustomKey::Stuff 
#define ContainerType vector<KeyType> 
#include "methods.h"

#undef KeyType
#undef ContainerType


namespace ToBeDone
{
	template <> ostream& Tbd<vector<int> >::operator<<(ostream& o) 
	{
		for (tt::const_iterator it= begin();it!=end();it++) { o<<setw(2)<<(*it)<<" "; }
		return o;
	}


	template <> Tbd<vector<int> >::operator const bool()
	{
		struct timespec tp;
		clock_gettime(CLOCK_MONOTONIC,&tp);
		tp.tv_nsec; srand(tp.tv_nsec);
		const int M((rand()%10)+10);
		const int N((rand()%20)+10);
		cout<<"Loading int test with "<<M<<" numbers with maximum value of 30"<<endl;
		for (int j=0;j<M;j++) push_back(rand()%N);
		return true;
	}

	template <> ostream& Tbd<vector<float> >::operator<<(ostream& o) 
	{
		for (tt::const_iterator it= begin();it!=end();it++) { o<<setw(4)<<fixed<<setprecision(1)<<(*it)<<" "; }
		return o;
	}


	template <> Tbd<vector<float> >::operator const bool()
	{
		struct timespec tp;
		clock_gettime(CLOCK_MONOTONIC,&tp);
		tp.tv_nsec; srand(tp.tv_nsec);
		const int M((rand()%10)+10);
		const int N((rand()%20)+10);
		cout<<"Loading float test with "<<M<<" numbers with maximum value of 30"<<endl;
		for (int j=0;j<M;j++) 
		{
			double j(rand()%(N*7));
			if (j) j/=5;
			tt::push_back(j);
		}
		return true;
	}


	template <> ostream& Tbd<vector<CustomKey::Stuff> >::operator<<(ostream& o) 
	{
		for (tt::const_iterator it= begin();it!=end();it++) { o<<setw(2)<<(*it)<<" "; }
		return o;
	}


	template <> Tbd<vector<CustomKey::Stuff> >::operator const bool()
	{
		struct timespec tp;
		clock_gettime(CLOCK_MONOTONIC,&tp);
		tp.tv_nsec; srand(tp.tv_nsec);
		const int M((rand()%10)+10);
		cout<<"Loading CustomKey::Stuff test with "<<M<<" chars "<<endl;
		for (int j=0;j<M;j++) 
		{
			push_back('g'+(rand()%10));
		}
		return true;
	}


} // ToBeDone

