
#ifndef PRODUCTS_H
#define PRODUCTS_H

namespace ClassFactory { class Factory; }
namespace Products
{
	class Product : protected Machine::ProductBase
	{
		friend class ClassFactory::Factory;
		protected:
		Product(Machine::MainBase& _main,const string _name) : main(_main),name(_name) {}
		virtual ~Product(){cerr<<"Deleting a "<<name<<" object"<<endl;}
		Machine::MainBase& main;
		const string name;
		public: 
		virtual operator const bool () = 0;
		virtual operator const string& (){return name;}
	};
} // Products

#endif //__PRODUCTS_H

