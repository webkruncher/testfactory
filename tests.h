
#ifndef __MISC_H__
#define __MISC_H__
namespace Tests
{
	struct Test : public Products::Product, Tbd
	{
			Test(Machine::MainBase& _main,const string _name) : Product(_main,_name) {}
			protected:
			void swap(int a,int b)
			{
				Tbd& me(*this);
				int B(me[b]);
				me[b]=me[a];
				me[a]=B;
			}
	};

	class Positive : Test
	{
			friend class ClassFactory::Factory;
			Positive(Machine::MainBase& _main) : Test(_main,"Positive Test") {}
			virtual operator const bool () ;
			static Positive* create(Machine::MainBase& _main){return new Positive(_main);}
	};

	class Negative : Test
	{
			friend class ClassFactory::Factory;
			Negative(Machine::MainBase& _main) : Test(_main,"Negative Test") {}
			virtual operator const bool () ;
			static Negative* create(Machine::MainBase& _main){return new Negative(_main);}
	};

	class Bubble : Test
	{
			friend class ClassFactory::Factory;
			Bubble(Machine::MainBase& _main) : Test(_main,"Bubble Sort") {}
			virtual operator const bool () ;
			static Bubble* create(Machine::MainBase& _main){return new Bubble(_main);}
	};

	class Insertion : Test
	{
			friend class ClassFactory::Factory;
			Insertion(Machine::MainBase& _main) : Test(_main,"Insertion Sort") {}
			virtual operator const bool () ;
			static Insertion* create(Machine::MainBase& _main){return new Insertion(_main);}
	};

	class Selection : Test
	{
			friend class ClassFactory::Factory;
			Selection(Machine::MainBase& _main) : Test(_main,"Selection Sort") {}
			virtual operator const bool () ;
			static Selection* create(Machine::MainBase& _main){return new Selection(_main);}
	};
} // Tests

#endif // __MISC_H__

