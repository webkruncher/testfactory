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


namespace InfoKruncher
{

	template<> void InfoKruncher::Consumer< Restful >::ForkAndRequest( const SocketProcessOptions& svcoptions )
	{
		RunClients( svcoptions );
	}
 
	template<> void InfoKruncher::Consumer< Restful >::GetSiteMetaData( const SocketProcessOptions& svcoptions )
	{
		mode=Cookie;
		streamingsocket sock( svcoptions.host.c_str(), svcoptions.port, KruncherTools::GetUuid() );
		sock.blocking( true );
		if ( sock.open() && sock.connect() )
		{
			if ( svcoptions.protocol == InfoKruncher::http ) 
			{
				InfoKruncher::Requests< PlainInformation::Socket > client;
				client.plain( sock.GetSock(), *this, svcoptions );
			}
			if ( svcoptions.protocol == InfoKruncher::https ) 
			{
				InfoKruncher::Requests< SecureInformation::Socket > client;
				client.secure( sock.GetSock(), *this, svcoptions );
			}
		}
		mode=None;
	}
	template<> void InfoKruncher::Consumer< Restful >::Terminate() { subprocesses.Terminate(); }
} // InfoKruncher


int main( int argc, char** argv )
{
	stringstream ssexcept;
	try
	{
		VERBOSITY=VERB_SIGNALS|VERB_ASOCKETS;
		cerr << green << "Restful is starting up" << normal << endl;
		InfoKruncher::Options< ClientList > options( argc, argv );
		if ( ! options ) throw string( "Invalid options" );
		KruncherTools::Daemonizer daemon( options.daemonize, "RestfulClient" );

		Initialize();
		const ClientList& clientlist( options.workerlist );
		const size_t nClients( options.workerlist.size() );
		InfoKruncher::Consumer< Restful > clients[ nClients ];
		
		for ( size_t c=0; c < nClients; c++ )
		{
			InfoKruncher::Consumer<Restful>& client( clients[ c ] );
			const InfoKruncher::SocketProcessOptions& svcoptions( clientlist[ c ] );
			client.GetSiteMetaData( svcoptions ); // pre-load cookies and oauth tokens
			client.ForkAndRequest( svcoptions );
		}
		while ( !TERMINATE ) usleep( (rand()%100000)+100000 );
		for ( size_t t=0; t < nClients; t++ ) clients[ t ].Terminate();
		cerr << green << "Restful is exiting" << normal << endl;
	}
	catch( const exception& e ) { ssexcept<<e.what(); }
	catch( const string& s ) { ssexcept<<s;}
	catch( const char* s ) { ssexcept<<s;}
	catch( ... ) { ssexcept<<"unknown";}
	if (!ssexcept.str().empty())
	{
		stringstream ssout; ssout << fence << "[EXCEPT]" << fence << ssexcept.str(); Log(VERB_ALWAYS, "restfulclientmain", ssout.str());
	}
	return 0;
}


