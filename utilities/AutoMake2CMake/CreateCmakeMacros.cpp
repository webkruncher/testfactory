

#include <iostream>
#include <memory>
#include <vector>
using namespace std;




struct Lines : vector< string >
{
	virtual ~Lines() {}

	virtual operator bool () = 0;
	private:
	friend ostream& operator<<(ostream&,const Lines&);
	virtual ostream& operator<<(ostream& o) const = 0;
}; 

inline ostream& operator<<(ostream& o,const Lines& m) { return m.operator<<(o); }



struct Macro
{
};

struct Macros : vector< Macro >
{
};

struct CompileLines : Lines
{
	virtual operator bool ()
	{
		for (const_iterator it=begin();it!=end();it++) 
		{
		}
		
		return true;
	} 
	virtual ostream& operator<<(ostream& o) const
	{
		for (const_iterator it=begin();it!=end();it++) o << "COMPILE:" << it->c_str() << endl;
		return o;
	}
	private:
	Macros macros;
};

struct LinkLines : Lines
{
	virtual operator bool ()
	{
		return false;
	} 
	virtual ostream& operator<<(ostream& o) const
	{
		for (const_iterator it=begin();it!=end();it++) o << "LINK:" << it->c_str() << endl;
		return o;
	}
};

int main( int argc, char** argv)
{
	if ( argc < 3 ) { cerr << "Usage: CreateCmakeMacros maindirectory [ -compile | -link ]" << endl; return -1; }

	const string where( argv[ 1 ] );
	const string how( argv[ 2 ] );
	cerr << where << " / " << how << endl;


	unique_ptr< Lines > sublines;
	
	if ( how == "-compile" )  sublines.reset( new CompileLines() );
	if ( how == "-link" )  sublines.reset( new LinkLines() );

	Lines& lines( *sublines.get() );

	while ( ! cin.eof() )
	{
		string line;
		getline( cin, line );
		lines.push_back( line );
	}

	if ( ! lines )
	{
		cerr << "Cannot generate macros for " << how << endl;
		return 0;
	}
	cout << lines << endl;
	return 0;
}

