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
#include <restful.h>

namespace RestfulClient
{
	void Restful::Throttle( const InfoKruncher::SocketProcessOptions& svcoptions ) { usleep( (rand()%10000)+1000000 ); }

	void Restful::Consume( KruncherMimes::SocketManager& sock, const InfoKruncher::SocketProcessOptions& options ) throw()
	{
		const string proto( ( options.protocol == InfoKruncher::http ) ? "http" : "https" );	
		if ( ! sock ) return;
		string& headertext( sock.Headers() );
		
		Hyper::MimeHeaders headers( headertext );
		const size_t ContentLength( headers.ContentLength() );
		const binarystring& Payload( sock.Payload( ContentLength ) );
		ProcessPayload( Payload, headers, options);

#if 0
		KruncherTools::stringvector Headers;
		Headers.split( headers, "\r\n" );

		size_t ContentLength( 0 );
		for ( KruncherTools::stringvector::const_iterator hit=Headers.begin();hit!=Headers.end();hit++)
		{
			const string H( *hit );
			if ( H.find("Content-Length:") == 0 )
			{ 
				const size_t coln( H.find( ":" ) );
				if ( coln == string::npos ) throw H;
				const string cls( H.substr( coln+1, H.size()-1 ) );
				char *Ender( NULL );
				ContentLength=strtol( cls.c_str(), &Ender, 10 );
			}
		}
		const binarystring& payload( sock.Payload( ContentLength ) );
		ProcessPayload( payload.data(), Headers, options);
#endif
	}

	void Restful::HandlePayload( const unsigned char* payload, const Hyper::MimeHeaders& headers, const InfoKruncher::SocketProcessOptions& options ) throw()
	{
		stringstream ssexcept;
		try
		{
			if ( ! payload ) return;
			Hyper::MimeHeaders::const_iterator ctypeit( headers.find( "content-type" ) );
			if ( ctypeit == headers.end() )
			{
				Log( "No content type" );
				return;
			}
			const string contenttype( ctypeit->second );
			Log( "Restful::HandlePayload",  contenttype );
			ProcessPayload( payload, headers, options );
		}
		catch( const exception& e ) { ssexcept<<e.what(); }
		catch( const string& s ) { ssexcept<<s;}
		catch( const char* s ) { ssexcept<<s;}
		catch( ... ) { ssexcept<<"unknown";}
		if (!ssexcept.str().empty())
		{
			stringstream ssout; 
			ssout << fence << "[EXCEPT]" << fence << ssexcept.str(); 
			Log(VERB_ALWAYS, "Restful::HandlePayload", ssout.str());
		}
	} 


	void Restful::LoadRequest( Requester& r  )
	{
		const stringmap& metadata( r.options.metadata );

		string uri;
		if ( mode == Cookie ) uri="Home.xml";
		else
		{
			const size_t item( rand() % Files.size() );
			uri=Files[ item ];
		}

		r.ss << "GET /" << uri << " HTTP/1.1" << endl;
		r.ss << "Host: WebKruncher.com" << endl;
		r.ss << "Accept: text/html; image/png;" << endl;
		if ( metadata.find( "cookie" ) != metadata.end() )
			r.ss << "Cookie: " << r.options.metadata[ "cookie" ] << endl;
		r.ss << endl;
	
		const string proto( ( r.options.protocol == InfoKruncher::http ) ? "http" : "https" );	
		//cout << green << proto << "://" << blue << r.options.host << "/" << yellow << uri << normal << endl;
	}

	void Restful::ProcessPayload( const binarystring& payload, const Hyper::MimeHeaders& headers, const InfoKruncher::SocketProcessOptions& options)
	{
		if ( mode == Cookie )
		{ 
			stringmap& metadata( options.metadata );
			Hyper::MimeHeaders::const_iterator cookit( headers.find( "set-cookie" ) ); 
			if ( cookit != headers.end() ) metadata[ "cookie" ] = cookit->second;
		}
		stringmap::const_iterator cit( headers.find("content-length") );
		stringmap::const_iterator rit( headers.find("request") );
		if ( ( rit != headers.end() ) && ( cit != headers.end() ) )
		{
			bool Same( true );	
			char *Ender( NULL );
			const int ContentLength( strtol(cit->second.c_str(), &Ender, 10) );
			const string request( rit->second );
			const string pathname( pathseparators( options.path, request ) );
			if ( FileExists( pathname ) )
			{
				const int fsize( FileSize( pathname ) );
				if ( fsize != ContentLength ) Same=false;
				if ( Same ) 	
					cout << green << request << fence << pathname << normal << endl ;
				else 
					cout << red << request << fence << pathname << fence << fsize << "!=" << ContentLength << normal << endl ;

				if ( Same )
				{
					const size_t fsize( FileSize( pathname ) );
					unsigned char* data( (unsigned char*) malloc( fsize ) );
					if ( ! data ) throw pathname;
					LoadBinaryFile( pathname , data, fsize );
					if ( memcmp( data, payload.data(), fsize ) ) Same=false;
					free( data );
				}

			
				if ( ! Same ) 	
					cout << red << request << fence << pathname << " payloads differ" << endl;
			
			}
		}
	}

} // RestfulClient

