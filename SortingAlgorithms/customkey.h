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


#ifndef CUSTOM_KEY_H
#define CUSTOM_KEY_H


namespace CustomKey
{
	struct Stuff
	{
		Stuff() {}
		Stuff(int j) {Value=j;}
		Stuff(const Stuff& a) : Value(a.Value) {}
		Stuff& operator=(const Stuff& a) {Value=a.Value;}
		Stuff& operator=(char c) {Value=c;}
		Stuff& operator+=(char c) {Value+=c;}
		bool operator==(const Stuff& a) const { return Value==a.Value; }
		bool operator<(const Stuff& a) const { return (Value<a.Value); }
		bool operator>(const Stuff& a) { return (Value>a.Value); }
		bool operator>=(const Stuff& a) { return (Value>=a.Value); }
		private:
		char Value;
		friend ostream& operator<<(ostream&,const Stuff&);
		ostream& operator<<(ostream& o) const { o<<Value<<" "; return o; }
	};
	inline ostream& operator<<(ostream& o,const Stuff& s){return s.operator<<(o);}
} // CustomKey

#endif //__CUSTOM_KEY_H

