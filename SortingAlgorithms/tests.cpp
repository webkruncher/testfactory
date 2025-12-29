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

namespace ToBeDone
{

	// Integer specializations
	template <> ostream& Tbd<vector<int> >::operator<<(ostream& o) 
	{
		for (tt::const_iterator it= begin();it!=end();it++) { o<<setw(2)<<(*it)<<" "; }
		return o;
	}


	template <> const bool Tbd<vector<int> >::operator()(const int M,const int N)
	{
		cout<<"Loading int test with "<<M<<" numbers with maximum value of "<<N<<endl; 
		for (int j=0;j<M;j++) push_back(rand()%N);
		return true;
	}

	// Float specializations
	template <> ostream& Tbd<vector<float> >::operator<<(ostream& o) 
	{
		for (tt::const_iterator it= begin();it!=end();it++) { o<<setw(4)<<fixed<<setprecision(1)<<(*it)<<" "; }
		return o;
	}

	template <> const bool Tbd<vector<float> >::operator()(const int M,const int N)
	{
		cout<<"Loading float test with "<<M<<" numbers with maximum value of "<<(N*7)/5<<endl; 
		for (int j=0;j<M;j++) 
		{
			double j(rand()%(N*7));
			if (j) j/=5;
			tt::push_back(j);
		}
		return true;
	}

	// Double specializations
	template <> ostream& Tbd<vector<double> >::operator<<(ostream& o) 
	{
		for (tt::const_iterator it= begin();it!=end();it++) { o<<setw(4)<<fixed<<setprecision(1)<<(*it)<<" "; }
		return o;
	}

	template <> const bool Tbd<vector<double> >::operator()(const int M,const int N)
	{
		cout<<"Loading double test with "<<M<<" numbers with maximum value of "<<(N*7)/5<<endl; 
		for (int j=0;j<M;j++) 
		{
			double j(rand()%(N*7));
			if (j) j/=5;
			tt::push_back(j);
		}
		return true;
	}

	// String specializations
	template <> ostream& Tbd<vector<string> >::operator<<(ostream& o) 
	{
		for (tt::const_iterator it= begin();it!=end();it++) { o<<(*it)<<" "; }
		return o;
	}

	template <> const bool Tbd<vector<string> >::operator()(const int M,const int N)
	{
		cout<<"Loading string test with "<<M<<" strings, prefixed with 't', followed by a number with maximum value of "<<(N*7)/5<<endl; 
		for (int j=0;j<M;j++) 
		{
			int j(rand()%(N*7));
			if (j) j/=5;
			stringstream ss; ss<<"t"<<setfill('0')<<setw(2)<<j;
			tt::push_back(ss.str());
		}
		return true;
	}

} // ToBeDone

