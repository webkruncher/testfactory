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

//#include <db_cxx.h>
//#include <kruncherbdb.h>
#include <iomanip>
#include <infotools.h>

#ifndef INFOKRUNCHER_INFOBUILD_LIBTIMES_H
#define INFOKRUNCHER_INFOBUILD_LIBTIMES_H

namespace InfoBuilder
{
	struct LibTimes;
	typedef DbRecords::RecordSet<LibTimes> TickerDb;
	extern unique_ptr< TickerDb > TickerData;

	struct TickerKey  : std::string
	{
		TickerKey();
		TickerKey( const string& that ); 
		TickerKey( const char* that) ;
		TickerKey( const char* thatptr, size_t thatsz) ;
		TickerKey( const TickerKey& that ) ;
		operator const char* () ;
		TickerKey& operator=( const string& that ) ;
		TickerKey& operator=( const TickerKey& that ) ;
		private:
		void assgn( const string& k, const string e="" );
		friend size_t EndKeySize(const TickerKey &s) ;
		friend void* EndKeyStr(const TickerKey &s) ;
		friend bool isEmpty(const TickerKey &s) ;
		friend string EndKeyOf(const TickerKey &s) ;
		friend int KeyLimiter(const TickerKey &s);
		friend int KeySkipper(const TickerKey &s) ;
		string EndKey;
		int Limitter;
		int Skip;
	};

	bool isEmpty(const TickerKey &s) ;
	size_t StartKeySize(const TickerKey &s) ;
	void* StartKeyDataPtr(const TickerKey &s) ; 
	size_t EndKeySize(const TickerKey &s) ;
	void* EndKeyStr(const TickerKey &s) ;
	string StringOf(const TickerKey &s) ;
	string StringOfPtr(const void* start, const size_t sz) ;
	int KeyLimiter(const TickerKey &s);
	int KeySkipper(const TickerKey &s);

	struct TickerBase : DataFig
	{
		struct TickerBaseRec
		{
			char name[256];
			char market[8];
			char locale[8];
			char primary_exchange[8];	
			char type[16];
			bool active; 
			char currency_name[48];
			unsigned long cik;
			char composite_figi[16];
			char share_class_figi[16];
			time_t last_updated;
			time_t locally_updated;
		};
	public:
		std::string TableName() const { return "markets/stocks"; }

		typedef TickerBaseRec		ValueType ;
		typedef TickerKey 		KeyType ;
		friend std::ostream& operator<<(std::ostream& o,const TickerBase::TickerBaseRec& m); 
	};
	std::ostream& operator<<(std::ostream& o,const TickerBase::TickerBaseRec& m) ;

	struct LibTimes : TickerBase
	{
		LibTimes(){ reset(); }
		void reset(){ memset( &record, 0, sizeof( record ) ); }
		static DbRecords::RecordSet<LibTimes>& Get( const string datapath );
		static void Release();

		static DbRecords::DbThang< LibTimes >& Thang() ;

		static bool Integrity( const ValueType& a, const ValueType& b, ostream& ss );
		void operator=( const stringvector& sv );
		const KeyType& Key() const { return ticker; }
		ValueType& Value() { return record; }
	private:
		friend std::ostream& operator<<( std::ostream& o, const LibTimes::TickerBase& ticker );
	protected:
		ValueType		record;
		const KeyType ticker;
		void SetName( const std::string value );
		void SetMarket( const std::string value );
		void SetLocale( const std::string value );
		void SetPrimaryExchange( const std::string value );
		void SetType( const std::string value );
		void SetActive( const std::string value );
		void SetCurrencyName( const std::string value );
		void SetPrimaryCIK( const std::string value );
		void SetCompositeFigi( const std::string value );
		void SetShareClassFigi( const std::string value );
		void SetLastUpdatedUTC( const std::string value );
		void SetLastUpdatedUTC( const time_t when );
	};


} // InfoBuilder

#endif // INFOKRUNCHER_INFOBUILD_LIBTIMES_H


