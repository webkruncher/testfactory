
#ifndef __MISC_H__
#define __MISC_H__
namespace Tests
{
	using namespace ToBeDone;
	template <typename T>
		struct Test : public Products::Product, Tbd<T>
	{
			Test(Machine::MainBase& _main,const string _name) : Product(_main,_name) {}
			protected:
			template <typename S>
				void swap(S a,S b)
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
			virtual operator const bool () ;
			static Positive* create(Machine::MainBase& _main){return new Positive(_main);}
	};

	template <typename T>
		class Negative : Test<T>
	{
			typedef T TT;
			friend class ClassFactory::Factory;
			Negative(Machine::MainBase& _main) : Test<T>(_main,"Negative Test") {}
			virtual operator const bool () ;
			static Negative* create(Machine::MainBase& _main){return new Negative(_main);}
	};

	template <typename T>
		class Bubble : Test<T>
	{
			typedef T TT;
			friend class ClassFactory::Factory;
			Bubble(Machine::MainBase& _main) : Test<T>(_main,"Bubble Sort") {}
			virtual operator const bool () ;
			static Bubble* create(Machine::MainBase& _main){return new Bubble(_main);}
	};

	template <typename T>
		class Insertion : Test<T>
	{
			typedef T TT;
			friend class ClassFactory::Factory;
			Insertion(Machine::MainBase& _main) : Test<T>(_main,"Insertion Sort") {}
			virtual operator const bool () ;
			static Insertion* create(Machine::MainBase& _main){return new Insertion(_main);}
	};

	template <typename T>
		class Selection : Test<T>
	{
			typedef T TT;
			friend class ClassFactory::Factory;
			Selection(Machine::MainBase& _main) : Test<T>(_main,"Selection Sort") {}
			virtual operator const bool () ;
			static Selection* create(Machine::MainBase& _main){return new Selection(_main);}
	};

} // Tests

#endif // __MISC_H__

