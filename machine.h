
#ifndef MACHINE_H
#define MACHINE_H
#include <map>

namespace Machine
{
	using namespace ToBeDone;
	using ToBeDone::TbdBase;
	using namespace std;
	struct ProductBase 
	{
		virtual operator const string& () = 0;
	};

	struct RandomLimits : map<string,int>
	{
		private:
		friend ostream& operator<<(ostream&,const RandomLimits&);
		virtual ostream& operator<<(ostream& o) const
			{ for (const_iterator it=begin();it!=end();it++) o<<it->first<<":"<<it->second<<endl; return o;}
	};
	inline ostream& operator<<(ostream& o,const RandomLimits& r){return r.operator<<(o);}

	class MainBase 
	{
		protected:
		RandomLimits randomlimits; 
		MainBase(int _argc,char** _argv) : argc(_argc), argv(_argv) {}
		int argc; char** argv;
		public: 
		virtual const bool operator()(TbdBase&,const bool expectation=true) = 0;
		virtual operator TbdBase& () = 0;
		virtual void Run(TbdBase&) = 0;
	};
} // Machine

#endif // MACHINE_H
