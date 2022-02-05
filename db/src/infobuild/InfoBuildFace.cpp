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

#include <datarest.h>
#include <recordset.h>
#include <RestInterface.h>


namespace InfoBuildFace
{
	using namespace RestData;
	using namespace DbRecords;
	using namespace InfoBuilder;
	void LibTimesMethods( Bindings& b, const InfoKruncher::SocketProcessOptions& options )  
	{
		typedef LibTimes library ;
		b[ "POST|/libs" ] = 			new Binding< RecordUpdateCreator< library > >	( options );
		b[ "POST|/libs/*" ] = 			new Binding< RecordUpdateCreator< library > >	( options );
		b[ "POST|/libs/integrity" ] = 		new Binding< RecordIntegrity< library > >	( options );
		b[ "GET|/libs/*/list" ] = 		new Binding< KeyLister< library > >		( options );
		b[ "GET|/libs/list" ] = 		new Binding< KeyLister< library > >		( options );
		b[ "GET|/libs" ] = 			new Binding< RecordPrinter< library > >		( options );
	}


	bool ThreadLocal::operator()( const InfoKruncher::SocketProcessOptions& options ) 
	{
		//Log( VERB_ALWAYS, "TraderFace", "Loading" );
		LibTimesMethods( binders, options );
		return true;
	}


	RestData::BindingBase* ThreadLocal::operator()( const InfoKruncher::Responder& r ) const
	{
		if ( r.resource.empty() ) return nullptr;
		if ( r.resource[ 0 ] != '/' ) return nullptr;
		const size_t questionmark( r.resource.find( "?", 1 ) );
		const size_t eores( ( questionmark == string::npos ) ? r.resource.size() : questionmark );
		const string wild( KruncherTools::DeBrace( r.resource.substr( 0, eores )  ) );
		const string uri( r.method + string( "|" ) + wild );

		if ( ( VERBOSITY & VERB_REST_1 ) == VERB_REST_1 )
			{ stringstream ss; ss<< "[LOOKUP]->" << r.method << fence << r.resource ; Log( VERB_ALWAYS, "markets/RestInterface", ss.str() ); }
		
		Bindings::const_iterator it( binders.find( uri ) );
		if ( it == binders.end() ) 
		{
			if ( ( VERBOSITY & VERB_REST_1 ) == VERB_REST_1 )
				{ stringstream ss; ss<< "[NOTFOUND]->" << r.method << fence << r.resource ; Log( VERB_ALWAYS, "markets/RestInterface", ss.str() ); }
			if ( ( VERBOSITY & VERB_REST_2 ) == VERB_REST_2 )
				Log( VERB_ALWAYS, "markets/RestInterface", uri );
			if ( ( VERBOSITY & VERB_REST_3 ) == VERB_REST_3 )
				for ( Bindings::const_iterator mit=binders.begin(); mit!=binders.end(); mit++ )
					Log( VERB_ALWAYS, uri, mit->first );
			return nullptr;
		}

		times++;
		if ( times &&  ( ! ( times % 10 ) ) ) reporttrigger=true;
		return it->second;
	}

} // InfoBuildFace


