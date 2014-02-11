
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

	template <> Tbd<vector<CustomKey::Stuff> >::operator const bool()
	{
		reseed();
		const int M((rand()%10)+10);
		cout<<"Loading CustomKey::Stuff test with "<<M<<" chars "<<endl;
		for (int j=0;j<M;j++) { push_back('g'+(rand()%10)); }
		return true;
	}


} // ToBeDone

