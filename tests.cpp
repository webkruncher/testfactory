#include <iostream>
using namespace std;
#include "tbd.h"
#include "machine.h"
#include "products.h"
#include "tests.h"
#include <set>
using namespace std;

namespace Tests
{
		Positive::operator const bool () 
		{
			cout<<"Testing "<<name<<endl;
			Tbd& tbd(main); Tbd& me(*this);
			me=tbd;
			sort(begin(),end());
			const bool success(main(*this));
			return success;
		}

		Negative::operator const bool () 
		{
			cout<<"Testing "<<name<<endl;
			Tbd& tbd(main); Tbd& me(*this);
			me=tbd;
			// Just make sure the test result is not equal to the sorted results
			for (iterator it=begin();it!=end();it++) (*it)+=(rand()%5);
			const bool success(main(*this,false));  // expected to fail
			return success;
		}

		Bubble::operator const bool () 
		{
			cout<<"Testing "<<name<<endl;
			Tbd& tbd(main);
			for (iterator it=tbd.begin();it!=tbd.end();it++) push_back(*it);
			Tbd& me(*this);
			int times(size()-1);
			while (times)
			{
				for (int j=0;j<(times);j++)
					if (me[j]>me[j+1]) swap(j,j+1);
				times--;
			}
			const bool success(main(*this));
			return success;
		}

		Insertion::operator const bool () 
		{
			cout<<"Testing "<<name<<endl;
			Tbd& tbd(main); Tbd& me(*this);
			me=tbd;

			for (int i=1; i<size(); i++)
			{
				int j=i;
				while ((j>0) and (me[j-1] > me[j]) )
				{
					swap(j,j-1);
					--j;
				}
			}

			const bool success(main(*this,true));
			return success;
		}

		Selection::operator const bool () 
		{
			cout<<"Testing "<<name<<endl;
			Tbd& tbd(main); Tbd& me(*this);
			//me=tbd;


			const bool success(main(*this,false));
			return success;
		}
} //namespace Tests

