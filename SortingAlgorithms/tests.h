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


#ifndef TESTS_H
#define TESTS_H

namespace Tests
{
	using namespace ToBeDone;
	template <typename T>
		struct Test : public Products::Product, Tbd<T>
	{
			Test(Machine::MainBase& _main,const string _name) : Product(_main,_name) {}
			virtual operator const bool () 
			{
				Machine::MainBase& _main(static_cast<Machine::MainBase&>(this->main)); 
				return _main.Traverse(*this);
			}
			protected:
			template <typename S>
				void swap(int a,int b)
			{
				Tbd<T>& me(*this);
				S B(me[b]);
				me[b]=me[a];
				me[a]=B;
			}
	};

	template <typename T>
		class Positive : Test<T>
	{
			typedef T TT;
			friend class ClassFactory::Factory;
			Positive(Machine::MainBase& _main) : Test<T>(_main,"Positive Test") {}

			virtual const bool Run()
			{
				TbdBase& _me(*this);
				Tbd<TT>& me(static_cast<Tbd<TT>&>(_me));
				const string& Name(*this);
				sort(me.begin(),me.end());
				const bool success(main(*this));
				cout<<Name<<"->"<<boolalpha<<success<<endl;
				return success;
			}
			static Positive* create(Machine::MainBase& _main){return new Positive(_main);}
	};

	template <typename T,typename K>
		class Negative : Test<T>
	{
			typedef T TT;
			typedef K KK;
			typedef Test<T> TY;
			friend class ClassFactory::Factory;
			Negative(Machine::MainBase& _main) : Test<T>(_main,"Negative Test") {}
			virtual const bool Run()
			{
				TbdBase& _me(*this);
				Tbd<TT>& me(static_cast<Tbd<TT>&>(_me));
				TY& ty(*this);
				const string& Name(*this);
				for (typename Tbd<TT>::iterator it=me.begin();it!=me.end();it++) (*it)+=(rand()%5);
				const bool success(main(*this,false));  // expected to fail
				cout<<Name<<"->"<<boolalpha<<success<<endl;
				return success;
			}
			static Negative* create(Machine::MainBase& _main){return new Negative(_main);}
	};

	template <typename T,typename K>
		class Bubble : Test<T>
	{
			typedef T TT;
			typedef K KK;
			typedef Test<T> TY;
			friend class ClassFactory::Factory;
			Bubble(Machine::MainBase& _main) : Test<T>(_main,"Bubble Sort") {}
			virtual const bool Run()
			{
				TbdBase& _me(*this);
				Tbd<TT>& me(static_cast<Tbd<TT>&>(_me));
				TY& ty(*this);
				const string& Name(*this);
				int times(me.size()-1);
				while (times)
				{
					for (int j=0;j<(times);j++)
						if (me[j]>me[j+1]) 
							ty.swap<KK>(j,j+1);
					times--;
				}
				const bool success(main(*this));
				cout<<Name<<"->"<<boolalpha<<success<<endl;
				return success;
			}
			static Bubble* create(Machine::MainBase& _main){return new Bubble(_main);}
	};

	template <typename T,typename K>
		class Insertion : Test<T>
	{
			typedef T TT;
			typedef K KK;
			typedef Test<T> TY;
			friend class ClassFactory::Factory;
			Insertion(Machine::MainBase& _main) : Test<T>(_main,"Insertion Sort") {}
			virtual const bool Run()
			{
				TbdBase& _me(*this);
				Tbd<TT>& me(static_cast<Tbd<TT>&>(_me));
				TY& ty(*this);
				const string& Name(*this);
				for (int i=1; i<me.size(); i++)
				{
					int j=i;
					while ((j>0) and (me[j-1] > me[j]) )
					{
						ty.swap<K>(j,j-1);
						--j;
					}
				}
				const bool success(main(*this,true));
				cout<<Name<<"->"<<boolalpha<<success<<endl;
				return success;
			}
			static Insertion* create(Machine::MainBase& _main){return new Insertion(_main);}
	};

	template <typename T,typename K>
		class Selection : Test<T>
	{
			typedef T TT;
			typedef K KK;
			typedef Test<T> TY;
			friend class ClassFactory::Factory;
			Selection(Machine::MainBase& _main) : Test<T>(_main,"Selection Sort") {}
			virtual const bool Run()
			{
				TbdBase& _me(*this);
				Tbd<TT>& me(static_cast<Tbd<TT>&>(_me));
				TY& ty(*this);
				const string& Name(*this);

				int split(me.size());
				while (split>0)
				{
					pair<K,int> selection(me[0],0);
					for (int j=0;j<split;j++)
						if (me[j]>=selection.first) {selection.first=me[j]; selection.second=j;}
					ty.swap<K>(selection.second,split-1);
					--split;
				}
				const bool success(main(*this));
				cout<<Name<<"->"<<boolalpha<<success<<endl;
				return success;
			}
			static Selection* create(Machine::MainBase& _main){return new Selection(_main);}
	};

	template <typename T,typename K>
		class HeapSort : Test<T>
	{
			typedef T TT;
			typedef K KK;
			typedef Test<T> TY;
			friend class ClassFactory::Factory;
			HeapSort(Machine::MainBase& _main) : Test<T>(_main,"Heap Sort") {}
			virtual const bool Run()
			{
				TbdBase& _me(*this);
				Tbd<TT>& me(static_cast<Tbd<TT>&>(_me));
				TY& ty(*this);
				const string& Name(*this);

				Heapify(me.size());

				int finish(me.size()-1);
				while (finish)
				{
					ty.swap<K>(finish,0);
					--finish;
					SiftDown(0,finish);
				}
				const bool success(main(*this));
				cout<<Name<<"->"<<boolalpha<<success<<endl;
				return success;
			}
			static HeapSort* create(Machine::MainBase& _main){return new HeapSort(_main);}
			void Heapify(int finish)
			{
				int start((finish-2)/2);
				while (start >= 0) 
				{
					SiftDown(start, finish-1);
					--start; 
				}
			}
			void SiftDown(int start,int finish)
			{
				Machine::MainBase& _main(static_cast<Machine::MainBase&>(this->main)); 
				TbdBase& _tbd(_main);
				TbdBase& _me(*this);
				TY& ty(*this);
				Tbd<TT>& tbd(static_cast<Tbd<TT>&>(_tbd));
				Tbd<TT>& me(static_cast<Tbd<TT>&>(_me));
				int root(start);
				while (((root * 2) + 1) <= finish)
				{
					int child((root * 2) + 1);
					int swp(root);
					if (me[swp] < me[child]) swp=child;
					if ( ( (child+1) <= finish) and (me[swp] < me[child+1]) ) swp=(child + 1);
					if (swp!=root)
					{
						ty.swap<K>(root,swp);
						root=swp;
					} else return;
				}
			}
	};

} // Tests

#endif // TESTS_H

