
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


