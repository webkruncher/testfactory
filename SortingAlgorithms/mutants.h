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


#ifndef MUTANTS_H
#define MUTANTS_H


template <typename B,typename V,typename K,typename Curious>
	struct Mutants : V
{
	Mutants(Curious& _curious) : curious(_curious) {}
	Mutants(const Mutants& a,Curious& _curious) : V(a),curious(_curious) {}
	operator V& (){return *this;}
	void operator-=(K n) { typename V::iterator it(find(this->begin(),this->end(),n)); if (it!=this->end()) erase(it); }
	void operator+=(K n) { this->push_back(n); }
	virtual void operator()(Mutants& b,B& tbd)
	{
		Mutants& a(*this);
		if (a.empty()) curious.Run(b,tbd);
		for (typename V::iterator it=a.begin();it!=a.end();it++)
		{
			K n(*it);
			Mutants suba(a); Mutants subb(b);
			suba-=n; subb+=n;
			suba(subb,tbd);
		}
	}
	private:
	Curious& curious;
};

#endif // MUTANTS_H

