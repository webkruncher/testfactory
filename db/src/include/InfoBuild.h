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



#include <iomanip>
#include <infotools.h>

#ifndef INFOKRUNCHER_INFOBUILD_LIBTIMES_H
#define INFOKRUNCHER_INFOBUILD_LIBTIMES_H

namespace InfoBuilder
{
	struct LibTimes;
	typedef DbRecords::RecordSet<LibTimes> LibraryDb;
	extern unique_ptr< LibraryDb > LibraryData;


	struct LibraryBase : DataFig
	{
		struct LibraryBaseRec
		{
			time_t last_updated;
		};
	public:
		std::string TableName() const { return "builder/libtimes"; }

		typedef LibraryBaseRec		ValueType ;
		typedef std::string 		KeyType ;
		friend std::ostream& operator<<(std::ostream& o,const LibraryBase::LibraryBaseRec& m); 
	};
	std::ostream& operator<<(std::ostream& o,const LibraryBase::LibraryBaseRec& m) ;

	struct LibTimes : LibraryBase
	{
		LibTimes(){ reset(); }
		void reset(){ memset( &record, 0, sizeof( record ) ); }
		static DbRecords::RecordSet<LibTimes>& Get( const string datapath );
		static void Release();

		static DbRecords::DbThang< LibTimes >& Thang() ;

		static bool Integrity( const ValueType& a, const ValueType& b, ostream& ss );
		void operator=( const stringvector& sv );
		const KeyType& Key() const { return library; }
		ValueType& Value() { return record; }
	private:
		friend std::ostream& operator<<( std::ostream& o, const LibTimes::LibraryBase& library );
	protected:
		ValueType		record;
		const KeyType library;
		void SetLastUpdatedUTC( const std::string value );
		void SetLastUpdatedUTC( const time_t when );
	};

//	void Trigger( const krbuilder::BuilderNode& node, const krbuilder::KrBuildSpecs& specs ) { }



} // InfoBuilder

#endif // INFOKRUNCHER_INFOBUILD_LIBTIMES_H



