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
	ostream& operator<<(ostream& o,const LibTimes::TickerBaseRec& m) 
	{
		if ( true )
		{
			o << 
				fence << m.name << 
				fence << m.market <<  
				fence << m.locale <<
				fence << m.primary_exchange <<  
				fence << m.type <<  
				fence << boolalpha << m.active <<  
				fence << m.currency_name <<  
				fence << m.cik <<  
				fence << m.composite_figi <<  
				fence << m.share_class_figi <<  
				fence << KruncherTools::TimeFormat(localtime( &m.last_updated)) <<
				fence << KruncherTools::TimeFormat(localtime( &m.locally_updated));
			;
			return o;
		}

		o << 
			fence << "name:" <<  m.name << 
			fence << "market:" << m.market <<  
			fence << "locale:" << m.locale <<
			fence << "primary_exchange:" << m.primary_exchange <<  
			fence << "type:" << m.type <<  
			fence << "active:" << boolalpha << m.active <<  
			fence << "currency_name:" << m.currency_name <<  
			fence << "cik:" << m.cik <<  
			fence << "composite_figi:" << m.composite_figi <<  
			fence << "share_class_figi:" << m.share_class_figi <<  
			fence << "last_updated:" << KruncherTools::TimeFormat(localtime( &m.last_updated)) <<
			fence << "locally_updated:"<< KruncherTools::TimeFormat(localtime( &m.locally_updated));
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

		void LibTimes::SetMarket( const std::string value )
		{
			#define target record.market
			const size_t valuesize( value.size()+1 );
			if ( valuesize >= sizeof( target ) ) { Log(VERB_ALWAYS, "Market too long, truncating", value ); }
			const size_t len( min( sizeof( target ), valuesize ) );
			snprintf( target, len, "%s", value.c_str() );
			#undef target
		}

		void LibTimes::SetLocale( const std::string value )
		{
			#define target record.locale
			const size_t valuesize( value.size()+1 );
			if ( valuesize >= sizeof( target ) ) { Log(VERB_ALWAYS, "Locale too long, truncating", value ); }
			const size_t len( min( sizeof( target ), valuesize ) );
			snprintf( target, len, "%s", value.c_str() );
			#undef target
		}

		void LibTimes::SetPrimaryExchange( const std::string value )
		{
			#define target record.primary_exchange
			const size_t valuesize( value.size()+1 );
			if ( valuesize >= sizeof( target ) ) { Log(VERB_ALWAYS, "Primary Exchange too long, truncating", value ); }
			const size_t len( min( sizeof( target ), valuesize ) );
			snprintf( target, len, "%s", value.c_str() );
			#undef target
		}

		void LibTimes::SetType( const std::string value )
		{
			#define target record.type
			const size_t valuesize( value.size()+1 );
			if ( valuesize >= sizeof( target ) ) { Log(VERB_ALWAYS, "Type too long, truncating", value ); }
			const size_t len( min( sizeof( target ), valuesize ) );
			snprintf( target, len, "%s", value.c_str() );
			#undef target
		}

		void LibTimes::SetActive( const std::string value )
		{
			record.active=( value == "true" ); 
		}

		void LibTimes::SetCurrencyName( const std::string value )
		{
			#define target record.currency_name
			const size_t valuesize( value.size()+1 );
			if ( valuesize >= sizeof( target ) ) { Log(VERB_ALWAYS, "Currency Name too long, truncating", value ); }
			const size_t len( min( sizeof( target ), valuesize ) );
			snprintf( target, len, "%s", value.c_str() );
			#undef target
		}

		void LibTimes::SetPrimaryCIK( const std::string value )
		{
			record.cik=atol( value.c_str() );
		}

		void LibTimes::SetCompositeFigi( const std::string value )
		{
			#define target record.composite_figi
			const size_t valuesize( value.size()+1 );
			if ( valuesize >= sizeof( target ) ) { Log(VERB_ALWAYS, "Composite Figi too long, truncating", value );  }
			const size_t len( min( sizeof( target ), valuesize ) );
			snprintf( target, len, "%s", value.c_str() );
			#undef target
		}

		void LibTimes::SetShareClassFigi( const std::string value )
		{
			#define target record.share_class_figi
			const size_t valuesize( value.size()+1 );
			if ( valuesize >= sizeof( target ) ) { Log(VERB_ALWAYS, "Share Class Figi too long, truncating", value ); }
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
			record.locally_updated=time( 0 );
		}

		void LibTimes::SetLastUpdatedUTC( const time_t when )
		{
			record.locally_updated=when;
		}


		DbRecords::RecordSet<LibTimes>& LibTimes::Get( const string datapath )
		{
			DbRecords::RecordSet<LibTimes>* d( TickerData.get() );
			if ( d ) return *d;
			TickerData.reset( new DbRecords::RecordSet<LibTimes>( datapath ) ); 
			d=TickerData.get();
			if ( ! d ) throw string( datapath );
			d->OpenThang();
			return *d;
		}

		void LibTimes::Release()
		{
			TickerData.reset( nullptr );
		}

		DbRecords::DbThang< LibTimes >& LibTimes::Thang() 
		{ 
			DbRecords::RecordSet<LibTimes>* d( TickerData.get() );
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
			Test( ok, "market", o, a.market, b.market );
			Test( ok, "locale", o, a.locale, b.locale );
			Test( ok, "primary_exchange", o, a.primary_exchange, b.primary_exchange );
			Test( ok, "type", o, a.type, b.type );
			Test( ok, "currency_name", o, a.currency_name, b.currency_name );
			Test( ok, "composite_figi", o, a.composite_figi, b.composite_figi );
			Test( ok, "share_class_figi", o, a.share_class_figi, b.share_class_figi );
			return ok;
		}
		void LibTimes::operator=( const stringvector& sv )
		{
			reset();
			int J( 2 );
			SetName( sv[ J++ ] );
			SetMarket( sv[ J++ ] );
			SetLocale( sv[ J++ ] );
			SetPrimaryExchange( sv[ J++ ] );
			SetType( sv[ J++ ] );
			SetActive( sv[ J++ ] );
			SetCurrencyName( sv[ J++ ] );
			SetPrimaryCIK( sv[ J++ ] );
			SetCompositeFigi( sv[ J++ ] );
			SetShareClassFigi( sv[ J++ ] );
			SetLastUpdatedUTC( sv[ J++ ] );
		}

	bool isEmpty(const TickerKey &s) 
	{
		return s.empty(); 
	}

	string StartKeyOf(const TickerKey &s) { return s; }
	string EndKeyOf(const TickerKey &s) { return s.EndKey; }
	size_t StartKeySize(const TickerKey &s) 
	{ 
		return s.size(); 
	}

	void* StartKeyDataPtr(const TickerKey &s) 
	{ 
		return (void*) s.c_str(); 
	}


	size_t EndKeySize(const TickerKey &s) 
	{ 
		return s.EndKey.size();
	} 

	void* EndKeyStr(const TickerKey &s) 
	{
		return (void*) s.EndKey.c_str(); 
	}

	string StringOf(const TickerKey &s)
	{
		return s;
	}

	string StringOfPtr(const void* start, const size_t sz) 
	{
		string ret;
		if ( ( start ) && ( sz ) ) ret.assign( (char*) start, sz );
		return ret;
	}

	int KeyLimiter(const TickerKey &s) { return s.Limitter; }

	int KeySkipper(const TickerKey &s) { return s.Skip; }


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


	TickerKey::TickerKey() : Limitter( 0 ), Skip( 0 ) {}
	TickerKey::TickerKey( const string& that ) : std::string( that ) ,
		Limitter( 0 ), Skip( 0 )
	{
		assgn( that );
	}

	void TickerKey::assgn( const string& a, const string b ) 
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

	TickerKey::TickerKey( const char* that) : std::string( that ) ,
		Limitter( 0 ), Skip( 0 )
	{
	}

	TickerKey::TickerKey( const char* thatptr, size_t thatsz) : std::string( thatptr, thatsz ),
		Limitter( 0 ), Skip( 0 )
 
	{
	}

	TickerKey::TickerKey( const TickerKey& that ) : 
		std::string( that ), EndKey( that.EndKey ), Limitter( that.Limitter ) , Skip( that.Skip )
	{
	}

	TickerKey::operator const char* () 
	{
		return c_str(); 
	}

	TickerKey& TickerKey::operator=( const TickerKey& that ) 
	{
		EndKey=that.EndKey;
		Limitter=that.Limitter;
		Skip=that.Skip;
		return *this; 
	}


	TickerKey& TickerKey::operator=( const string& that ) 
	{
		assgn( that, EndKey );
		return *this; 
	}



} // InfoBuilder


