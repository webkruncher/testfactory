
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

	class Positive : public Test
	{
			friend class ClassFactory::Factory;
			Positive(Machine::MainBase& _main) : Test(_main,"Positive Test") {}
			virtual operator const bool () ;
			public: 
			static Positive* create(Machine::MainBase& _main){return new Positive(_main);}
	};

	class Negative : public Test
	{
			Negative(Machine::MainBase& _main) : Test(_main,"Negative Test") {}
			virtual operator const bool () ;
			public: 
			static Negative* create(Machine::MainBase& _main){return new Negative(_main);}
	};

	class Bubble : public Test
	{
			Bubble(Machine::MainBase& _main) : Test(_main,"Bubble Sort") {}
			virtual operator const bool () ;
			public: 
			static Bubble* create(Machine::MainBase& _main){return new Bubble(_main);}
	};

	class Insertion : public Test
	{
			Insertion(Machine::MainBase& _main) : Test(_main,"Insertion Sort") {}
			virtual operator const bool () ;
			public: 
			static Insertion* create(Machine::MainBase& _main){return new Insertion(_main);}
	};
} // Tests

#endif // __MISC_H__

