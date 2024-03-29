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

#include <infokruncher.h>
#include <infosite.h>
#include <infobuilder.h>

//#include <db/auth/infoxmlauth.h>
//#include <visitors/visitor.h>
//#include <db/site/infodataservice.h>
#include <exexml.h>
//#include <db/site/PostProcessor.h>
#include <infofigur.h>
#include <krbuilder.h>

namespace InfoBuilderService
{
	const string ServiceName( "BuildService" );

	void InfoSite::LoadResponse( InfoKruncher::Responder& r, InfoKruncher::RestResponse& Responder, InfoKruncher::ThreadLocalBase& threadlocal )
	{
#if 0
		if ( r.resource.size() > 512 ) return;
		const string ipaddr( dotted( r.ipaddr ) );
		cerr << ipaddr << endl;

		const string schemer( ( r.options.scheme == InfoKruncher::http ) ? "http" : "https" );
		Log( VERB_ALWAYS, "InfoSite::LoadResponse", ipaddr + string( "|" ) + r.method + string( "|" ) + schemer + string("|") + r.resource );
		DbRecords::RecordSet<InfoDataService::Visitor> records( r.options.datapath );
		//records+=r;

		if ( ipaddr != "127.0.0.1" )
		{
			if ( ipaddr == "73.201.184.88" )
			{
				Log( VERB_ALWAYS, "LoadResponse", "Ok for myself" );
				Responder( 200, "text/plain", ServiceName, false, "", "", "Hi me" );
			} else {
				//cerr << red << r.ipaddr << fence << r.method << fence << r.resource << normal << endl;
				const string uauth( "UnAuthorized" );
				Responder( 401, "text/plain", ServiceName, false, "", "", uauth );
			}
			return;
		}



		Responder( 200, "text/plain", ServiceName, false, "", "", "BRIDGE OUT - TBD" );
		return;

		InfoDataService::DataResource Payload( r, records );
		const int payloadstatus( Payload );
		if ( payloadstatus ) 
		{
#if 0
			if ( ipaddr == "73.201.184.88" )
			{
				Log( VERB_ALWAYS, "LoadResponse", "Ok for myself" );
				Responder( 200, Payload.contenttype, ServiceName, false, "", "", Payload.payload.str() );
			} else {
				Responder( payloadstatus, Payload.contenttype, ServiceName, false, "", "", Payload.payload.str() );
			}
#else
				Responder( payloadstatus, Payload.contenttype, ServiceName, false, "", "", Payload.payload.str() );
#endif
			return ;
		}

		if ( Payload.IsBinary() )
		{
			stringstream ss;
			Responder( 200, Payload.contenttype, ServiceName, false, "", "", Payload.Data(), Payload.DataLength() );
			return ;
		}

		if ( ( r.method == "POST" ) || ( r.method == "PUT" ) || ( r.method == "PATCH" ) )
			if ( ( r.ContentLength < 0 ) || ( r.ContentLength > 4096 ) )
			{
				Responder( 414, Payload.contenttype, ServiceName, false, "", "", Payload.payload.str() );
				return ;
			}

		InfoDb::Site::Roles roles( r.options.scheme, Payload.uri, r.headers, r.ipaddr, r.options.text );	
		InfoAuth::Authorization auth( Payload.payload.str(), Payload.contenttype, roles );
		const int AuthorizationStatus( auth );
		const string& txt( auth );
		Responder( AuthorizationStatus, Payload.contenttype, ServiceName, records.IsNewCookie(), records.CookieName(), records.Cookie(), txt );
		//if ( txt.size() > 4096 ) Responder.SetChunked( 4096 );
		return ;
#endif
	}

	bool InfoSite::ProcessForm( const string formpath, stringmap& formdata )
	{
		stringstream ssmsg;  ssmsg << "InfoSite::ProcessForm" << fence << formpath << fence << formdata;
		Log( ssmsg.str() );
		return true;
	}

	void InfoSite::PostProcessing( InfoKruncher::Responder&, InfoKruncher::RestResponse& DefaultResponse, const binarystring& PostedContent, InfoKruncher::ThreadLocalBase& threadlocal ) 
	{
		{ofstream o( "/home/jmt/hists.txt", ios::app ); o << "POSTED:" << endl << (char*) PostedContent.data() << endl; }
		Log( VERB_ALWAYS, "InfoSite::PostProcessing", (char*) PostedContent.data() );
	}

	void InfoSite::Throttle( const InfoKruncher::SocketProcessOptions& svcoptions )
		{ usleep( 100000 ); }

        struct ThreadLocal : InfoKruncher::ThreadLocalBase
	{
		private:
                virtual ostream& operator<<( ostream& o ) const
                {
                        //o << fence << getpid() << fence << pthread_self() << fence << times << fence << endl;
                        return o;
                }

	};


	InfoKruncher::ThreadLocalBase* InfoSite::AllocateThreadLocal( const InfoKruncher::SocketProcessOptions& options )
	{
		//cerr << "T"; cerr.flush();
		return new ThreadLocal();
	}

	XmlFamily::XmlNode* BuildInfoConfiguration::NewNode(XmlFamily::Xml& _doc,stringtype name) const 
	{ 
		return new krbuilder::BuilderNode(_doc,NULL,name, servicelist, optionnode, filter ); 
	} 


} // InfoBuilderService


