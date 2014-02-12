
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
#include <set>
#include <iomanip>
#include "tests.h"
#include "customkey.h"
#include "mutants.h"


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


template <typename T,typename K>
	struct Mash : Mutants<ToBeDone::TbdBase,T,K,Machine::MainBase>
{
	Mash(Machine::MainBase& _machine) : Mutants<ToBeDone::TbdBase,T,K,Machine::MainBase>(_machine) {}
	Mash(Machine::MainBase& _machine, const Mash& a) : Mutants<ToBeDone::TbdBase,T,K,Machine::MainBase>(a,_machine) {}
	virtual void operator()(Mash& b,ToBeDone::TbdBase& tbd)
	{ 
		Mutants<ToBeDone::TbdBase,T,K,Machine::MainBase>::operator()(b,tbd); 
	}
	virtual void operator()() 
	{
		T& o(*this);
		cout<<">:"<<o<<endl;
	}
};


template <typename T,typename K>
	struct Main : public Machine::MainBase
{
	Main(int _argc,char** _argv) : MainBase(_argc,_argv),unsorted(*this) {}
	virtual operator const bool ()
	{
		randomlimits.clear();
		ToBeDone::reseed();	
		//randomlimits["M"]=((rand()%4)+3);
		randomlimits["M"]=3;
		randomlimits["N"]=((rand()%20)+10);
		ToBeDone::Tbd<T>& loader(unsorted);
		if (!loader(randomlimits["M"],randomlimits["N"])) return false;
		check=unsorted;
		sort(check.begin(),check.end());
		return true;
	}
	virtual operator ToBeDone::TbdBase& () {return unsorted;}
	ToBeDone::Tbd<T> check;
	Mash<ToBeDone::Tbd<T>,K> unsorted;
	public: 
	virtual bool Traverse(ToBeDone::TbdBase& tbd)
	{
		Mash<ToBeDone::Tbd<T>,K > full(*this,unsorted),empty(*this);
		cout<<"Traverse:"<<tbd<<endl;
		full(empty,tbd);
		cout<<"Done Traverse:"<<tbd<<endl;
		return true;
	} 
	virtual void Run(ToBeDone::TbdBase& mutant,ToBeDone::TbdBase& tbd)
	{
		tbd=mutant;
		cout<<"Mashing:"<<tbd<<endl;
		tbd.Run();
	}
	virtual const bool operator()(ToBeDone::TbdBase& tested,const bool expectation) 
	{ 
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
			Main<ContainerType,KeyType> main(argc,argv);
			if (!main) throw "cannot load main";
			CustomFactory factory;
			factory.generate<Tests::Positive<ContainerType>,Main<ContainerType,KeyType> >(main);
			factory.generate<Tests::Negative<ContainerType,KeyType>,Main<ContainerType,KeyType> >(main);
			const bool results(factory);
			cerr<<"Success:"<<boolalpha<<results<<endl<<endl;
			if (!results) Pass=false;
		}
		if (true)
		{
			cerr<<endl<<"Sort Tests"<<endl;
			Main<ContainerType,KeyType> main(argc,argv);
			if (!main) throw "cannot load main";

			CustomFactory factory;
			factory.generate<Tests::Bubble<ContainerType,KeyType>,Main<ContainerType,KeyType> >(main);
			factory.generate<Tests::Insertion<ContainerType,KeyType>,Main<ContainerType,KeyType> >(main);
			factory.generate<Tests::Selection<ContainerType,KeyType>,Main<ContainerType,KeyType> >(main);
			factory.generate<Tests::HeapSort<ContainerType,KeyType>,Main<ContainerType,KeyType> >(main);
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
		if (!Test<double,vector<double> >(argc,argv)) Pass=false;
		if (!Test<string,vector<string> >(argc,argv)) Pass=false;
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


