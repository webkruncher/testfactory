/*
 * Copyright (c) Jack M. Thompson WebKruncher.com, exexml.com
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the WebKruncher nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY Jack M. Thompson ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL Jack M. Thompson BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <time.h>
#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
using namespace std;

#include <recordset.h>
#include <InfoBuild.h>

namespace InfoBuilder
{
	ostream& operator<<(ostream& o,const LibTimes::LibraryBaseRec& m) 
	{
		if ( true )
		{
			o << 
				fence << m.name << 
				fence << KruncherTools::TimeFormat(localtime( &m.last_updated)) 
				;
			return o;
		}

		o << 
			fence << "name:" <<  m.name << 
			fence << "last_updated:" << KruncherTools::TimeFormat(localtime( &m.last_updated)) ;
		;
		return o;
	}




	void LibTimes::SetName( const std::string value )
	{
		#define target record.name
		const size_t valuesize( value.size()+1 );
		if ( valuesize >= sizeof( target ) ) { Log(VERB_ALWAYS, "Name too long, truncating", value ); }
		const size_t len( min( sizeof( target ), valuesize ) );
		snprintf( target, len, "%s", value.c_str() );
		#undef target
	}


	void LibTimes::SetLastUpdatedUTC( const std::string value )
	{
		struct tm tm;
		if (strptime(value.c_str(), "%Y-%m-%dT%H:%M:%S%z", &tm) == NULL)
		{ Log(VERB_ALWAYS, "Cannot convert time", value ); return; }
		record.last_updated=mktime(&tm);
	}

	void LibTimes::SetLastUpdatedUTC( const time_t when )
	{
		record.last_updated=when;
	}


	DbRecords::RecordSet<LibTimes>& LibTimes::Get( const string datapath )
	{
		DbRecords::RecordSet<LibTimes>* d( LibraryData.get() );
		if ( d ) return *d;
		LibraryData.reset( new DbRecords::RecordSet<LibTimes>( datapath ) ); 
		d=LibraryData.get();
		if ( ! d ) throw string( datapath );
		d->OpenThang();
		return *d;
	}

	void LibTimes::Release()
	{
		LibraryData.reset( nullptr );
	}

	DbRecords::DbThang< LibTimes >& LibTimes::Thang() 
	{ 
		DbRecords::RecordSet<LibTimes>* d( LibraryData.get() );
		if ( ! d ) throw string( "Thang is not allocated" );
		return *d;
	}

	bool LibTimes::Integrity( const ValueType& a, const ValueType& b, ostream& o )
	{
		bool ok( true );

		auto Test = []( bool& ok, const string& name, ostream& o, const char* a, const char* b )
		{
			if ( strcmp( a, b ) )
			{
				ok=false;
				o << name << fence;
			}
			return ok;
		};

		Test( ok, "name", o, a.name, b.name );
		return ok;
	}
	void LibTimes::operator=( const stringvector& sv )
	{
		reset();
		int J( 2 );
		SetName( sv[ J++ ] );
		SetLastUpdatedUTC( sv[ J++ ] );
	}

	bool isEmpty(const LibraryKey &s) 
	{
		return s.empty(); 
	}

	string StartKeyOf(const LibraryKey &s) { return s; }
	string EndKeyOf(const LibraryKey &s) { return s.EndKey; }
	size_t StartKeySize(const LibraryKey &s) 
	{ 
		return s.size(); 
	}

	void* StartKeyDataPtr(const LibraryKey &s) 
	{ 
		return (void*) s.c_str(); 
	}


	size_t EndKeySize(const LibraryKey &s) 
	{ 
		return s.EndKey.size();
	} 

	void* EndKeyStr(const LibraryKey &s) 
	{
		return (void*) s.EndKey.c_str(); 
	}

	string StringOf(const LibraryKey &s)
	{
		return s;
	}

	string StringOfPtr(const void* start, const size_t sz) 
	{
		string ret;
		if ( ( start ) && ( sz ) ) ret.assign( (char*) start, sz );
		return ret;
	}

	int KeyLimiter(const LibraryKey &s) { return s.Limitter; }

	int KeySkipper(const LibraryKey &s) { return s.Skip; }


	const char opposite( const char what )
	{
		switch ( what )
		{
			case '(': return ')';
			case '{': return '}';
			case '[': return ']';
			default: return what;
		}
	}


	LibraryKey::LibraryKey() : Limitter( 0 ), Skip( 0 ) {}
	LibraryKey::LibraryKey( const string& that ) : std::string( that ) ,
	Limitter( 0 ), Skip( 0 )
	{
		assgn( that );
	}

	void LibraryKey::assgn( const string& a, const string b ) 
	{
		assign( a );
		if ( ! b.empty() ) { EndKey.assign( b ); return; }
		if ( size() < 2 ) return;
		if ( ! ( find_first_of( "[{(" )  == string::npos ) )
		{ 
			string wip( *this );
			const char how( opposite( string::operator[]( 0 ) ) );

			const size_t e( find_last_of( how ) );
			if ( e == string::npos ) return;
			const string first( substr( 1, e-1 ) );

			stringvector sv; sv.split( first, "," );
			if ( sv.size() == 1 ) return;
			clear();
			assign( sv[ 0 ] );

			if ( sv.size() >= 2 )
			{ 
				EndKey=( sv[ 1 ] );
			}
			if ( sv.size() >= 3 )
			{ 
				Limitter=( atoi( sv[ 2 ].c_str() ) );
			}
			if ( sv.size() >= 4 )
			{ 
				Skip=( atoi( sv[ 3 ].c_str() ) );
			}
		}
	}

	LibraryKey::LibraryKey( const char* that) : std::string( that ) ,
	Limitter( 0 ), Skip( 0 )
	{
	}

	LibraryKey::LibraryKey( const char* thatptr, size_t thatsz) : std::string( thatptr, thatsz ),
	Limitter( 0 ), Skip( 0 )

	{
	}

	LibraryKey::LibraryKey( const LibraryKey& that ) : 
		std::string( that ), EndKey( that.EndKey ), Limitter( that.Limitter ) , Skip( that.Skip )
	{
	}

	LibraryKey::operator const char* () 
	{
		return c_str(); 
	}

	LibraryKey& LibraryKey::operator=( const LibraryKey& that ) 
	{
		EndKey=that.EndKey;
		Limitter=that.Limitter;
		Skip=that.Skip;
		return *this; 
	}


	LibraryKey& LibraryKey::operator=( const string& that ) 
	{
		assgn( that, EndKey );
		return *this; 
	}



} // InfoBuilder


