
#ifndef TBD_H
#define TBD_H
#include <iomanip>
using namespace std;

namespace ToBeDone
{
	inline void reseed()	
	{
		struct timespec tp;
		clock_gettime(CLOCK_MONOTONIC,&tp);
		tp.tv_nsec; srand(tp.tv_nsec);
	}

	struct TbdBase 
	{
		virtual TbdBase& operator=(TbdBase& b) {return b;}
		virtual bool operator==(TbdBase& b) = 0;
		virtual const bool Run()
		{
			throw string("Tbd::Run must be specialized");
			return true;
		}
		private:
		friend ostream& operator<<(ostream&,TbdBase&);
		virtual ostream& operator<<(ostream&) = 0;
	};
	inline ostream& operator<<(ostream& o,TbdBase& t){return t.operator<<(o);}

	template <typename T>
		struct Tbd : TbdBase, T
	{

		typedef T tt;
		virtual TbdBase& operator=(TbdBase& b)
		{
			Tbd& you(static_cast<Tbd&>(b));
			tt& me(*this);
			me=you;
			return *this;
		}
		virtual bool operator==(TbdBase& b) 
		{
			Tbd& you(static_cast<Tbd&>(b));
			tt& me(*this);
			return (me==you);
		}
		virtual const bool operator()(const int,const int)
		{
			throw string("Tbd::bool operator()(int,int) must be specialized");
			return true;
		}
		virtual const bool Run()
		{
			throw string("Tbd::Run must be specialized");
			return true;
		}
		private:
		virtual ostream& operator<<(ostream& o) ;
	};
}

#endif // TBD_H

