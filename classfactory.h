
#ifndef __CLASS_FACTORY_H__
#define __CLASS_FACTORY_H__


namespace ClassFactory
{
	class Factory : protected vector<Products::Product*>
	{
		protected:
		Factory(Machine::MainBase& _main) : main(_main) {}
		virtual ~Factory() { for (iterator it=begin();it!=end();it++) delete (*it);}
		Machine::MainBase& main;
		public:
		template <typename T> void generate() { push_back(T::create(main)); }
		Products::Product& operator()(string); 
		virtual operator const bool ()  = 0;
	};



} // ClassFactory

#endif //__CLASS_FACTORY_H__

