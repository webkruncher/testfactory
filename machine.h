
#ifndef __MACHINE_H__
#define __MACHINE_H__

namespace Machine
{
	struct ProductBase 
	{
		virtual operator const string& () = 0;
	};
	class MainBase 
	{
		protected:
		MainBase(int _argc,char** _argv) : argc(_argc), argv(_argv) {}
		int argc; char** argv;
		public: 
		virtual const bool operator()(TbdBase&,const bool expectation=true) = 0;
		virtual operator TbdBase& () = 0;
	};
} // Machine

#endif // __MACHINE_H__
