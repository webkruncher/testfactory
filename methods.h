
namespace Tests
{
	using namespace ToBeDone;

	template <> Negative<ContainerType>::operator const bool () 
	{
		cout<<"Testing "<<name<<endl;
		TbdBase& tbd(main); TbdBase& me(*this);
		me=tbd;
		// Just make sure the test result is not equal to the sorted results
		for (iterator it=begin();it!=end();it++) (*it)+=(rand()%5);
		const bool success(main(*this,false));  // expected to fail
		return success;
	}




	template <> Selection<ContainerType>::operator const bool () 
	{
		cout<<"Testing "<<name<<endl;
		TbdBase& _tbd(main); 
		TbdBase& _me(*this);
		_me=_tbd;
		Tbd<TT>& tbd(static_cast<Tbd<TT>&>(_tbd));
		Tbd<TT>& me(static_cast<Tbd<TT>&>(_me));

		int split(size());
		while (split>0)
		{
			pair<KeyType,int> selection(me[0],0);
			for (int j=0;j<split;j++)
				if (me[j]>=selection.first) {selection.first=me[j]; selection.second=j;}
			swap<KeyType>(selection.second,split-1);
			--split;
		}
		const bool success(main(*this));
		return success;
	}


	template <> void HeapSort<ContainerType>::SiftDown(int start,int finish) 
	{
		TbdBase& _me(*this);
		Tbd<TT>& me(static_cast<Tbd<TT>&>(_me));
		int root(start);
		while (((root * 2) + 1) <= finish)
		{
			int child((root * 2) + 1);
			int swp(root);
			if (me[swp] < me[child]) swp=child;
			if ( ( (child+1) <= finish) and (me[swp] < me[child+1]) )
			swp=(child + 1);
			if (swp!=root)
			{
				swap<KeyType>(root,swp);
				root=swp;
			} else return;
		}
	}

	template <> void HeapSort<ContainerType>::Heapify(int finish) 
	{
		int start((finish-2)/2);
		while (start >= 0) 
		{
			SiftDown(start, finish-1);
			--start; 
		}
	}

	template <> HeapSort<ContainerType>::operator const bool () 
	{
		cout<<"Testing "<<name<<endl;
		TbdBase& _tbd(main); 
		TbdBase& _me(*this);
		_me=_tbd;
		Tbd<TT>& tbd(static_cast<Tbd<TT>&>(_tbd));
		Tbd<TT>& me(static_cast<Tbd<TT>&>(_me));

		Heapify(size());

		int finish(size()-1);
		while (finish)
		{
			swap<KeyType>(finish,0);
			--finish;
			SiftDown(0,finish);
		}

		const bool success(main(*this));
		return success;
	}

} // Tests

namespace ToBeDone
{
	template <> TbdBase& Tbd<ContainerType>::operator=(TbdBase& b) 
	{
		Tbd& you(static_cast<Tbd&>(b));
		tt& me(*this);
		me=you;
		return *this;
	}
} // namespace


