
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
using namespace std;
#include "tbd.h"
#include "machine.h"
#include "products.h"
#include "classfactory.h"
namespace MainProgram {class CustomFactory;}
#include "tests.h"
#include <set>
#include <iomanip>

struct CustomFactory : ClassFactory::Factory
{
	virtual operator const bool ()  
	{
		bool success(true);
		for (iterator it=begin();it!=end();it++)
		{
			Products::Product& product(**it);
			if (!product) success=false;
		}
		return success;
	}
};

template <typename T>
	struct Main : public Machine::MainBase
{
	Main(int _argc,char** _argv) : MainBase(_argc,_argv) {}
	virtual operator const bool ()
	{
		if (!unsorted) return false;
		check=unsorted;
		sort(check.begin(),check.end());
		return true;
	}
	virtual operator TbdBase& () {return unsorted;}
	Tbd<T> unsorted,check;
	public: 
	virtual const bool operator()(TbdBase& tested,const bool expectation) 
	{ 
		cerr<<setw(10)<<"Un-sorted:"<<unsorted<<endl;
		cerr<<setw(10)<<"Sorted:"<<check<<endl;
		cerr<<setw(10)<<"Test:"<<tested<<endl;
		const bool result(tested==check); 
		const bool pass(result==expectation);
		cerr<<"Expected:"<<boolalpha<<expectation<<", result:"<<boolalpha<<result<<", pass:"<<boolalpha<<pass<<endl<<endl;
		return pass;
	}
};

int main(int argc,char** argv)
{
	stringstream except;
	bool Pass(true);
	try
	{
		{
			typedef int KeyType ;
			typedef vector<KeyType> ContainerType;
			cerr<<endl<<"System Tests"<<endl;
			Main<ContainerType> main(argc,argv);
			if (!main) throw "cannot load main";
			CustomFactory factory;
			factory.generate<Tests::Positive<ContainerType>,Main<ContainerType> >(main);
			factory.generate<Tests::Negative<ContainerType>,Main<ContainerType> >(main);
			const bool results(factory);
			cerr<<"Success:"<<boolalpha<<results<<endl<<endl;
			if (!results) Pass=false;
		}
		{
			cerr<<endl<<"Sort Tests"<<endl;
			typedef int KeyType ;
			typedef vector<KeyType> ContainerType;
			cerr<<endl<<"System Tests"<<endl;
			Main<ContainerType> main(argc,argv);
			if (!main) throw "cannot load main";

			CustomFactory factory;
			factory.generate<Tests::Bubble<ContainerType>,Main<ContainerType> >(main);
			factory.generate<Tests::Insertion<ContainerType>,Main<ContainerType> >(main);
			factory.generate<Tests::Selection<ContainerType>,Main<ContainerType> >(main);
			const bool results(factory);
			cerr<<"Success:"<<boolalpha<<results<<endl<<endl;
			if (!results) Pass=false;
		}
		{
			typedef float KeyType ;
			typedef vector<KeyType> ContainerType;
			cerr<<endl<<"System Tests"<<endl;
			Main<ContainerType> main(argc,argv);
			if (!main) throw "cannot load main";
			CustomFactory factory;
			factory.generate<Tests::Positive<ContainerType>,Main<ContainerType> >(main);
			factory.generate<Tests::Negative<ContainerType>,Main<ContainerType> >(main);
			const bool results(factory);
			cerr<<"Success:"<<boolalpha<<results<<endl<<endl;
			if (!results) Pass=false;
		}
		{
			cerr<<endl<<"Sort Tests"<<endl;
			typedef float KeyType ;
			typedef vector<KeyType> ContainerType;
			cerr<<endl<<"System Tests"<<endl;
			Main<ContainerType> main(argc,argv);
			if (!main) throw "cannot load main";

			CustomFactory factory;
			factory.generate<Tests::Bubble<ContainerType>,Main<ContainerType> >(main);
			factory.generate<Tests::Insertion<ContainerType>,Main<ContainerType> >(main);
			factory.generate<Tests::Selection<ContainerType>,Main<ContainerType> >(main);
			const bool results(factory);
			cerr<<"Success:"<<boolalpha<<results<<endl<<endl;
			if (!results) Pass=false;
		}
		cout<<"Overall results:"<<boolalpha<<Pass<<endl;
	}
	catch(char* who) {except<<"Exception: "<<who;}
	catch(string& who) {except<<"Exception: "<<who;}
	catch(exception& e) {except<<"Exception: "<<e.what();}
	catch(...) {except<<"Unknown Exception";}
	if (!except.str().empty()) cerr<<except.str()<<endl;
	return !Pass;
}

#if 1
template <> inline	ostream& Tbd<vector<int> >::operator<<(ostream& o) 
{
	for (tt::const_iterator it= begin();it!=end();it++) { o<<setw(2)<<(*it)<<" "; }
	return o;
}

template <> inline TbdBase& Tbd<vector<int> >::operator=(TbdBase& b) 
{
	Tbd& you(static_cast<Tbd&>(b));
	tt& me(*this);
	me=you;
	return *this;
}

template <> inline Tbd<vector<int> >::operator const bool()
{
	struct timespec tp;
	clock_gettime(CLOCK_MONOTONIC,&tp);
	tp.tv_nsec; srand(tp.tv_nsec);
	const int M((rand()%10)+10);
	const int N((rand()%20)+10);
	cout<<"Loading test with "<<M<<" numbers with maximum value of 30"<<endl;
	for (int j=0;j<M;j++) push_back(rand()%N);
	return true;
}

template <> inline	ostream& Tbd<vector<float> >::operator<<(ostream& o) 
{
	for (tt::const_iterator it= begin();it!=end();it++) { o<<setw(4)<<fixed<<setprecision(1)<<(*it)<<" "; }
	return o;
}

template <> inline TbdBase& Tbd<vector<float> >::operator=(TbdBase& b) 
{
	Tbd& you(static_cast<Tbd&>(b));
	tt& me(*this);
	me=you;
	return *this;
}


template <> inline Tbd<vector<float> >::operator const bool()
{
	struct timespec tp;
	clock_gettime(CLOCK_MONOTONIC,&tp);
	tp.tv_nsec; srand(tp.tv_nsec);
	const int M((rand()%10)+10);
	const int N((rand()%20)+10);
	cout<<"Loading test with "<<M<<" numbers with maximum value of 30"<<endl;
	for (int j=0;j<M;j++) 
	{
		double j(rand()%(N*7));
		if (j) j/=5;
		tt::push_back(j);
	}
	return true;
}
#endif
