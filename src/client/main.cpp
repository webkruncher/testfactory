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
#include <exexml.h>
#include <infofigur.h>
#include <restful.h>
#include <RequestDirectory.h>

namespace InfoKruncher
{

	template<> void InfoKruncher::Consumer< RestfulClient::Restful >::ForkAndRequest( const SocketProcessOptions& svcoptions )
	{
		RunClients( svcoptions );
	}
 
	template<> void InfoKruncher::Consumer< RestfulClient::Restful >::GetSiteMetaData( const SocketProcessOptions& svcoptions )
	{
		mode=RestfulClient::Cookie;
		streamingsocket sock( svcoptions.host.c_str(), svcoptions.port, KruncherTools::GetUuid() );
		sock.blocking( true );
		if ( sock.open() && sock.connect() )
		{
			if ( svcoptions.scheme == InfoKruncher::http ) 
			{
				InfoKruncher::Requests< PlainInformation::Socket > client;
				client.plain( sock.GetSock(), *this, svcoptions );
			}
			if ( svcoptions.scheme == InfoKruncher::https ) 
			{
				InfoKruncher::Requests< SecureInformation::Socket > client;
				client.secure( sock.GetSock(), *this, svcoptions );
			}
		}
		mode=RestfulClient::None;
	
		RequestDirectory::Dir directory;
		const string& path( svcoptions.path );	
		regex_t rx;
		//const string exp( "^.*\\.js$|^.*\\.xml$|^.*\\.png$|^.*\\.jpg$" );
		const string exp( "^.*\\.xml$" );
		if ( regcomp( &rx, exp.c_str(), REG_EXTENDED ) ) throw exp;
		directory( path, true, rx );
		if ( ! directory ) throw path;
		stringstream sss; sss << directory;
		stringvector files;
		files.split( sss.str(), "\n" );
		for ( stringvector::const_iterator it=files.begin();it!=files.end();it++)
		{
			const string fname( *it );
			Files.push_back( fname.substr( path.size(), fname.size()-path.size() ) ); 
		}
		fit=Files.begin();
	}
	template<> void InfoKruncher::Consumer< RestfulClient::Restful >::Terminate() { subprocesses.Terminate(); }
} // InfoKruncher


#if 0
#include <unistd.h>
#include <sys/wait.h>
#include <iostream>
#include <sys/types.h>
using namespace std;
int forkmain(){
 char str[1024], *cp;
 int pipefd[2];
 pid_t pid;
 int status, died;

  pipe (pipefd);
  switch(pid=fork()){
   case -1: cout << "can't fork\n";
            exit(-1);
   
   case 0 : // this is the code the child runs 
            close(1);      // close stdout
            dup (pipefd[1]); // points pipefd at file descriptor
            close (pipefd[0]);
   default: // this is the code the parent runs 

            close(0); // close stdin
            // Set file descriptor 0 (stdin) to read from the pipe
            dup (pipefd[0]);
            // the parent isn't going to write to the pipe
            close (pipefd[1]);
            // Now read from the pipe
            cin.getline(str, 1023);
            cout << "The date is " << str << endl;
            died= wait(&status);
   }
}

#endif

int main( int argc, char** argv )
{
	stringstream ssexcept;
	try
	{
		srand( time(0) );
		//VERBOSITY=VERB_SIGNALS|VERB_ASOCKETS;
		cerr << green << "Restful is starting up" << normal << endl;
		InfoKruncher::Options< InfoKruncher::ServiceList > options( argc, argv );
		if ( ! options ) throw string( "Invalid options" );
		KruncherTools::Daemonizer daemon( options.daemonize, "RestfulClient" );

		if ( options.find( "-d" ) == options.end() ) Initialize();
		const InfoKruncher::ServiceList& clientlist( options.workerlist );
		const size_t nClients( options.workerlist.size() );
		InfoKruncher::Consumer< RestfulClient::Restful > clients[ nClients ];
		
		for ( size_t c=0; c < nClients; c++ )
		{
			InfoKruncher::Consumer<RestfulClient::Restful>& client( clients[ c ] );
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


