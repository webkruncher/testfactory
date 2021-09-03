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
		if ( true )
		{
			InfoKruncher::SocketProcessOptions o;
			o.port=80;
			o.protocol=InfoKruncher::SocketProcessOptions::Protocol::http;
			o.path="/home/jmt/websites/text/webkruncher/";
			o.host="webkruncher.com";
			push_back( o );
		}

		KruncherTools::Args::const_iterator usehttps( options.find( "--https" ) );
		if ( usehttps != options.end() )
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

			const string certs( "/home/jmt/websites/certs/webkruncher/" );
			o.cadir=certs;
			o.certfile=certs+string("WEBKRUNCHER.COM.crt");
			o.cafile=certs+string("dv_chain.txt");
			o.keyfile=certs+string("server.key");
			o.host="webkruncher.com";
			push_back( o );
		}
		return true;
	}

	void WebKruncher::LoadRequest( Requester& r  )
	{
		r.ss << "GET /ajax/WinMover.js HTTP/1.1" << endl;
		r.ss << "Host: WebKruncher.com" << endl;
		r.ss << "Accept: text/html" << endl;
		r.ss << endl;
	}

	void WebKruncher::HandlePayload( const unsigned char* payload, const Hyper::MimeHeaders& headers, const InfoKruncher::SocketProcessOptions& options ) throw()
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
		if ( ! ssexcept.str().empty() ) ExceptionLog( "main", ssexcept.str() );
		
	} 

	void WebKruncher::Throttle( const InfoKruncher::SocketProcessOptions& svcoptions )
	{
		usleep( (rand()%1000)+2000 );
		//sleep( 1 );
	}


	void WebKruncher::HandleText( const string& text, const Hyper::MimeHeaders& headers, const InfoKruncher::SocketProcessOptions& )
	{
		stringstream ss;
		ss << blue << headers << normal << endl;
		cout << ss.str() ;
	}
