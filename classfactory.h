

#ifndef __CLASS_FACTORY_H__
#define __CLASS_FACTORY_H__


namespace ClassFactory
{
	class Factory : protected vector<Products::Product*>
	{
		protected:
		virtual ~Factory() { for (iterator it=begin();it!=end();it++) delete (*it);}
		public:
		template <typename T,typename M> void generate(M& _main) { push_back(T::create(_main)); }
		Products::Product& operator()(string); 
		virtual operator const bool ()  = 0;
	};



} // ClassFactory

#endif //__CLASS_FACTORY_H__

