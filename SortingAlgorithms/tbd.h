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

