
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
				cout<<"Testing "<<Name<<endl;
				sort(me.begin(),me.end());
				const bool success(main(*this));
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
				cout<<"Testing "<<Name<<endl;
				for (typename Tbd<TT>::iterator it=me.begin();it!=me.end();it++) (*it)+=(rand()%5);
				const bool success(main(*this,false));  // expected to fail
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
				cout<<"Testing "<<Name<<endl;
				int times(me.size()-1);
				while (times)
				{
					for (int j=0;j<(times);j++)
						if (me[j]>me[j+1]) 
							ty.swap<KK>(j,j+1);
					times--;
				}
				const bool success(main(*this));
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
				cout<<"Testing "<<Name<<endl;
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
				cout<<"Testing "<<Name<<endl;

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
				cout<<"Testing "<<Name<<endl;

				Heapify(me.size());

				int finish(me.size()-1);
				while (finish)
				{
					ty.swap<K>(finish,0);
					--finish;
					SiftDown(0,finish);
				}
				const bool success(main(*this));
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

