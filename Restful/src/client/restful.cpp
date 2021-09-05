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


	bool ClientList::operator()( const KruncherTools::Args& options)
	{
		if ( options.find( "--http" ) != options.end() )
		{
			InfoKruncher::SocketProcessOptions o;
			o.port=80;
			o.protocol=InfoKruncher::SocketProcessOptions::Protocol::http;
			o.path="/home/jmt/websites/text/webkruncher/";
			o.host="webkruncher.com";
			push_back( o );
		}

		if ( options.find( "--https" ) != options.end() )
		{
			SecureInformation::init_openssl();
			InfoKruncher::SocketProcessOptions o;
			o.port=443;
			o.protocol=InfoKruncher::SocketProcessOptions::Protocol::https;
			o.path="/home/jmt/websites/text/webkruncher/";

			const string passwordfile( "/etc/webkruncher.pwd" );
			if ( KruncherTools::FileExists( passwordfile ) )
			{
				o.keypasswd=KruncherTools::LoadFile( passwordfile );
			} else {
				cout << "Ssl Password: ";
				o.keypasswd=KruncherTools::getpass();
			}

			const string certs( "/etc/ssl/" );
			o.cadir=certs;
			o.certfile=certs+string("WEBKRUNCHER.COM.crt");
			o.cafile=certs+string("cert.pem");
			o.host="webkruncher.com";
			push_back( o );
		}
		return true;
	}

	void Restful::Throttle( const InfoKruncher::SocketProcessOptions& svcoptions ) { usleep( (rand()%10000)+1000000 ); }

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
			if ( contenttype.find( "text/" ) != string::npos ) 
			{
				const string text( (char*) payload );
				HandleText( text, headers, options );
			}
		}
		catch( const exception& e ) { ssexcept<<e.what(); }
		catch( const string& s ) { ssexcept<<s;}
		catch( const char* s ) { ssexcept<<s;}
		catch( ... ) { ssexcept<<"unknown";}
		if ( ! ssexcept.str().empty() ) ExceptionLog( "Restful::HandlePayload", ssexcept.str() );
	} 


	void Restful::LoadRequest( Requester& r  )
	{
		const stringmap& metadata( r.options.metadata );

		string uri;
		if ( mode == Mode::Cookie ) uri="Home.xml";

		r.ss << "GET /" << uri << " HTTP/1.1" << endl;
		r.ss << "Host: Restful.com" << endl;
		r.ss << "Accept: text/html" << endl;
		if ( metadata.find( "cookie" ) != metadata.end() )
			r.ss << "Cookie: " << r.options.metadata[ "cookie" ] << endl;
		r.ss << endl;
		cout << green << r.ss.str() << normal << endl;
	}

	void Restful::HandleText( const string& text, const Hyper::MimeHeaders& headers, const InfoKruncher::SocketProcessOptions& options)
	{
		stringstream ss;
		if ( mode == Mode::Cookie )
		{ 
			stringmap& metadata( options.metadata );
			Hyper::MimeHeaders::const_iterator cookit( headers.find( "set-cookie" ) ); 
			if ( cookit != headers.end() ) metadata[ "cookie" ] = cookit->second;
			ss << yellow << headers << normal << endl;
		}
		ss << green << headers << normal << endl;
		cout << ss.str() ;
	}



