
#ifndef __CLASS_FACTORY_H__
#define __CLASS_FACTORY_H__


namespace ClassFactory
{
	class Factory : protected map<string,Products::Product*>
	{
		protected:
		Factory(Machine::MainBase& _main) : main(_main) {}
		virtual ~Factory() { for (iterator it=begin();it!=end();it++) delete it->second; }
		Machine::MainBase& main;
		public:
		template <typename T>
			void generate(string name)
		{
			Factory& me(*this);
			if (find(name)!=end()) throw string("Attempting to add ")+name+string(" more than one time");
			me[name]=T::create(main);
		}
		Products::Product& operator()(string); 
		virtual operator const bool ()  = 0;
	};



} // ClassFactory

#endif //__CLASS_FACTORY_H__

