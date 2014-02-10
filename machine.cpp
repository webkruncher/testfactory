
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
	virtual operator ToBeDone::TbdBase& () {return unsorted;}
	ToBeDone::Tbd<T> unsorted,check;
	public: 
	virtual const bool operator()(ToBeDone::TbdBase& tested,const bool expectation) 
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


template <typename KeyType, typename ContainerType>
	const bool Test(int argc,char** argv)
	{
		bool Pass(true);
		{
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
		return Pass;
	}

int main(int argc,char** argv)
{
	stringstream except;
	bool Pass(true);
	try
	{
		if (!Test<int,vector<int> >(argc,argv)) Pass=false;
		if (!Test<float,vector<float> >(argc,argv)) Pass=false;
		if (!Test<CustomKey::Stuff,vector<CustomKey::Stuff> >(argc,argv)) Pass=false;
		cout<<"Overall results:"<<boolalpha<<Pass<<endl;
	}
	catch(char* who) {except<<"Exception: "<<who;}
	catch(string& who) {except<<"Exception: "<<who;}
	catch(exception& e) {except<<"Exception: "<<e.what();}
	catch(...) {except<<"Unknown Exception";}
	if (!except.str().empty()) cerr<<except.str()<<endl;
	return !Pass;
}


