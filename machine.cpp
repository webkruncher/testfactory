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
	CustomFactory(Machine::MainBase& _main) : Factory(_main) { }
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

class Main : Machine::MainBase
{
	friend int main(int,char**);
	Main(int _argc,char** _argv) : MainBase(_argc,_argv) {}
	virtual operator const bool ()
	{
		Tbd& tbd(*this);
		if (!Tbd::operator const bool ()) return false;
		check=tbd;
		sort(check.begin(),check.end());
		return true;
	}
	Tbd check;
	public: 
	virtual const bool operator()(Tbd& tested,const bool expectation) 
	{ 
		Tbd& me(*this);
		cout<<setw(10)<<"Un-sorted:"<<me<<endl;
		cout<<setw(10)<<"Sorted:"<<check<<endl;
		cout<<setw(10)<<"Test:"<<tested<<endl;
		const bool result(tested==check); 
		const bool pass(result==expectation);
		cout<<"Expected:"<<boolalpha<<expectation<<", result:"<<boolalpha<<result<<", pass:"<<boolalpha<<pass<<endl<<endl;
		return pass;
	}
};

int main(int argc,char** argv)
{
	stringstream except;
	try
	{
		bool Pass(true);
		{
			cout<<endl<<"System Tests"<<endl;
			Main main(argc,argv);
			if (!main) throw "cannot load main";
			CustomFactory factory(main);
			factory.generate<Tests::Positive>();
			factory.generate<Tests::Negative>();
			const bool results(factory);
			cout<<"Success:"<<boolalpha<<results<<endl<<endl;
			if (!results) Pass=false;
		}
		{
			cout<<endl<<"Sort Tests"<<endl;
			Main main(argc,argv);
			if (!main) throw "cannot load main";
			CustomFactory factory(main);
			factory.generate<Tests::Bubble>();
			factory.generate<Tests::Insertion>();
			factory.generate<Tests::Selection>();
			const bool results(factory);
			cout<<"Success:"<<boolalpha<<results<<endl<<endl;
			if (!results) Pass=false;
		}
		cout<<"Overall results:"<<boolalpha<<Pass<<endl;
	}
	catch(char* who) {except<<"Exception: "<<who;}
	catch(string& who) {except<<"Exception: "<<who;}
	catch(exception& e) {except<<"Exception: "<<e.what();}
	catch(...) {except<<"Unknown Exception";}
	if (!except.str().empty()) cerr<<except.str()<<endl;
	return 0;
}

