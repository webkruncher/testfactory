
#ifndef __TBD_H__
#define __TBD_H__
#include <iomanip>
using namespace std;

namespace ToBeDone
{
	struct TbdBase 
	{
		virtual TbdBase& operator=(TbdBase& b) {return b;}
		virtual bool operator==(TbdBase& b) = 0;
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
		virtual operator const bool ()
		{
			throw string("Tbd::operator bool must be specialized");
			return true;
		}
		private:
		ostream& operator<<(ostream& o) ;
	};
}

#endif // __TBD_H__

