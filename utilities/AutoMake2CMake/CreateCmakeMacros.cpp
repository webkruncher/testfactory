

#include <iostream>
#include <memory>
#include <vector>
using namespace std;




struct Lines : vector< string >
{
	private:
	friend ostream& operator<<(ostream&,const Lines&);
	virtual ostream& operator<<(ostream& o) const
	{
		for (const_iterator it=begin();it!=end();it++) o << it->c_str() << endl;
		return o;
	}
}; 

inline ostream& operator<<(ostream& o,const Lines& m) { return m.operator<<(o); }

struct CompileLines : Lines
{
};

struct LinkLines : Lines
{
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

	cout << lines << endl;
	return 0;
}

