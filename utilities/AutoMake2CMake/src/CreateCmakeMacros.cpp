

#include <iostream>
#include <memory>
#include <vector>
using namespace std;
#include <infotools.h>
using namespace KruncherTools;



struct Lines : vector< string >
{
	Lines( const string _where ) : where( _where ) {}
	virtual ~Lines() {}

	virtual operator bool () = 0;
	private:
	const string where;
	friend ostream& operator<<(ostream&,const Lines&);
	virtual ostream& operator<<(ostream& o) const = 0;
}; 

inline ostream& operator<<(ostream& o,const Lines& m) { return m.operator<<(o); }



struct Macro
{
	Macro( const string _path, const string _file ) : path( _path ), file( _file ) {}
	const string path;
	const string file;
	private:
	friend ostream& operator<<(ostream&,const Macro&);
	virtual ostream& operator<<(ostream& o) const
	{
		o << path << " -->> " << file << endl;
		return o;
	} 
}; 

inline ostream& operator<<(ostream& o,const Macro& m) { return m.operator<<(o); }


struct Rules : map< string, stringvector > 
{
	Rules( const string _where ) : where( _where ) {}
	operator bool () const
	{
		for (const_iterator it=begin();it!=end();it++) 
		{
			const string macroincludefile( where + string( "/" ) + it->first + string( "/CMake.inc" ) );
			ofstream macrofile( macroincludefile.c_str() );

			const size_t up( std::count(it->first.begin(), it->first.end(), '/') );
			macrofile << "set(INCLUDEPATH " << endl;
			macrofile << tab; for ( int u=0; u<up; u++ ) macrofile << "../"; macrofile << "../build_unix/" << endl;
			macrofile << tab; for ( int u=0; u<up; u++ ) macrofile << "../"; macrofile << "../src/" << endl;
			macrofile << ")" << endl;

			macrofile << "set(SOURCES " << endl;
			for (stringvector::const_iterator sit=it->second.begin();sit!=it->second.end();sit++) 
				macrofile << tab << ( *sit ) << endl;
			macrofile << ")" << endl;
		}
		return true;
	}
	private:
	const string where;
	friend ostream& operator<<(ostream&,const Rules&);
	virtual ostream& operator<<(ostream& o) const
	{
		for (const_iterator it=begin();it!=end();it++) 
			o << it->first << endl ;// << it->second << endl;
		return o;
	} 
}; 

inline ostream& operator<<(ostream& o,const Rules& m) { return m.operator<<(o); }


struct Macros : vector< Macro >
{
	Macros( const string _where ) : rules( _where ) {}
	void operator+=( const string line )
	{
		const string nodots( line.substr( 3, line.size()-3 ) );
		const size_t ls( nodots.find_last_of( "/" ));
		if ( ls == string::npos )
		{
			cerr << redbk << nodots << normal << endl;
		} else {
			const string path( nodots.substr( 0, ls ) );
			const size_t ls1( ls + 1 );
			const string file( nodots.substr( ls1, nodots.size()-ls1 ) );
			Macro macro( path, file );
			push_back( macro );
		}
		
	}
	operator bool()
	{
		for (const_iterator it=begin();it!=end();it++) 
		{
			const Macro& macro( *it );
			rules[ macro.path ].push_back( macro.file );
		}
		return !!rules;
	}
	private:
	Rules rules;
	friend ostream& operator<<(ostream&,const Macros&);
	virtual ostream& operator<<(ostream& o) const
	{
		o << rules;
		return o;
	} 
}; 

inline ostream& operator<<(ostream& o,const Macros& m) { return m.operator<<(o); }

struct CompileLines : Lines
{
	CompileLines( const string _where ) : Lines( _where ), macros( _where ) {}
	virtual operator bool ()
	{
		for (const_iterator it=begin();it!=end();it++) 
		{
			stringvector parts;
			parts.split( *it, " " );
			for ( stringvector::const_iterator vit=parts.begin();vit!=parts.end();vit++)
			{
				const string part( *vit );
				const size_t trigger( part.find("../" ) );
				if ( trigger  == 0 ) macros+=part; 
			}
		}
		
		return !!macros;
	} 
	virtual ostream& operator<<(ostream& o) const
	{
		o << macros;
		return o;
	}
	private:
	Macros macros;
};

struct LinkLines : Lines
{
	LinkLines( const string _where ) : Lines( _where ) {}
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
	//cerr << where << " / " << how << endl;

	unique_ptr< Lines > sublines;
	
	if ( how == "-compile" )  sublines.reset( new CompileLines( where ) );
	if ( how == "-link" )  sublines.reset( new LinkLines( where ) );

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
		return -1;
	}
	cout << lines << endl;
	return 0;
}

