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
				//fence << m.name << 
				fence << KruncherTools::TimeFormat(localtime( &m.last_updated)) 
				;
			return o;
		}

		o << 
			//fence << "name:" <<  m.name << 
			fence << "last_updated:" << KruncherTools::TimeFormat(localtime( &m.last_updated)) ;
		;
		return o;
	}



#if 0
	void LibTimes::SetName( const std::string value )
	{
Log( VERB_ALWAYS, "SetName", value );
		#define target record.name
		const size_t valuesize( value.size()+1 );
		if ( valuesize >= sizeof( target ) ) { Log(VERB_ALWAYS, "Name too long, truncating", value ); }
		const size_t len( min( sizeof( target ), valuesize ) );
		snprintf( target, len, "%s", value.c_str() );
		#undef target
	}

#endif

	void LibTimes::SetLastUpdatedUTC( const std::string value )
	{
Log( VERB_ALWAYS, "SetLastUpdatedUTC", value );
		struct tm tm;
		if (strptime(value.c_str(), "%Y-%m-%dT%H:%M:%S%z", &tm) == NULL)
		{ Log(VERB_ALWAYS, "Cannot convert time", value ); return; }
		record.last_updated=mktime(&tm);
	}

	void LibTimes::SetLastUpdatedUTC( const time_t when )
	{
Log( VERB_ALWAYS, "SetLastUpdatedUTC", "(when)" );
return;
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

		//Test( ok, "name", o, a.name, b.name );
		return ok;
	}
	void LibTimes::operator=( const stringvector& sv )
	{
		reset();
		int J( 2 );
		//SetName( sv[ J++ ] );
		SetLastUpdatedUTC( sv[ J++ ] );
	}



} // InfoBuilder


