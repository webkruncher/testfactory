
#include <iostream>
using namespace std;
#include "tbd.h"

#include "machine.h"
#include "products.h"
#include "tests.h"
#include <set>
#include <vector>
#include <sstream>
using namespace std;
#include "customkey.h"

namespace ToBeDone
{
	// CustomKey::Stuff specializations
	template <> ostream& Tbd<vector<CustomKey::Stuff> >::operator<<(ostream& o) 
	{
		for (tt::const_iterator it= begin();it!=end();it++) { o<<setw(2)<<(*it)<<" "; }
		return o;
	}

	template <> const bool Tbd<vector<CustomKey::Stuff> >::operator()(const int M,const int N)
	{
		for (int j=0;j<M;j++) 
		{ 
			while (true)
			{
				char c(('g'+(rand()%N))); 
				if (!isalpha(c)) continue;	
				push_back(c);
				break;
			}
		}
		return true;
	}


} // ToBeDone

