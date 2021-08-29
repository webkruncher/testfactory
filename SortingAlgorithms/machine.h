/*
* Copyright (c) Jack M. Thompson WebKruncher.com, exexml.com
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * Neither the name of the WebKruncher nor the
*       names of its contributors may be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY Jack M. Thompson ``AS IS'' AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL Jack M. Thompson BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


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
		virtual void Run(TbdBase&,TbdBase&) = 0;
		virtual bool Traverse(TbdBase&) = 0;
	};
} // Machine

#endif // MACHINE_H
