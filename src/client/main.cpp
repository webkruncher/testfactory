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


#include <clientkruncher.h>
#include <krbuilder.h>


namespace InfoKruncher
{

	template<> void InfoKruncher::Consumer< Ingest::Ingester >::ForkAndRequest( const SocketProcessOptions& svcoptions )
		{ RunClients( svcoptions ); }
 
	template<> void InfoKruncher::Consumer< Ingest::Ingester >::GetSiteMetaData( int _argc, char** _argv, const SocketProcessOptions& svcoptions )
		{ argc=_argc; argv=_argv; }
	template<> void InfoKruncher::Consumer< Ingest::Ingester >::Terminate() { subprocesses.Terminate(); }


	template<> void InfoKruncher::Consumer< Ingest::Activator >::ForkAndRequest( const SocketProcessOptions& svcoptions )
		{ RunClients( svcoptions ); }
 
	template<> void InfoKruncher::Consumer< Ingest::Activator >::GetSiteMetaData( int _argc, char** _argv, const SocketProcessOptions& svcoptions ) 
		{ argc=_argc; argv=_argv; }

	template<> void InfoKruncher::Consumer< Ingest::Activator >::Terminate() { subprocesses.Terminate(); }

} // InfoKruncher

	struct Ingestive : ClientKruncher::Objective 
	{ 
		virtual int operator()( int argc, char** argv )
		{
			return ClientKruncher::Main< Ingest::Ingester >( argc, argv );
		} 
	};

	struct Informative : ClientKruncher::Objective 
	{
		virtual int operator()( int argc, char** argv ) 
		{
			return ClientKruncher::Query( argc, argv );
		} 
	};


	struct Objects : ClientKruncher::Objects
	{
		operator bool ()
		{
			insert( pair< string, Ingestive* > ( "ingest", new Ingestive ) );
			insert( pair< string, Informative* > ( "inform", new Informative ) );
			return true;
		}

		int operator()( int argc, char** argv )
		{
			bool Dashed=false;
			for ( int j=0; j < argc; j++ )
			{
				const string dash( argv[ j ] );
				if ( dash == "-" ) { Dashed=true; break; }
				if ( j == ( argc - 1 ) ) break;
				const string opt( argv[ j+1 ] );
				if ( dash == "-o" )
				{
					//cerr << teal << dash << " " << opt << normal << endl;
					const_iterator tit( find( opt ) );
					if ( tit == end() ) throw string( "Unknown objective" );
					ClientKruncher::Objective& objective( *tit->second );
					return objective( argc, argv );
				}
			}
			const string objname(  ( Dashed ) ? "ingest" : "inform" );
			const_iterator tit( find( objname ) );
			if ( tit == end() ) throw string( "Unknown objective" );
			ClientKruncher::Objective& objective( *tit->second );
			return objective( argc, argv );
		}
	};




int main( int argc, char** argv )
{
	#if 0

		{

			KruncherTools::CharVector parameters{ (char*) "find", (char*) "..", nullptr };
			KruncherTools::forkpipe( "/usr/bin/find", parameters, "", cout );
		}



		{

			KruncherTools::CharVector parameters{ (char*) "tr", (char*) "C", (char*) "X", nullptr };
			KruncherTools::forkpipe( "/usr/bin/tr", parameters, "ABCD", cout );
		}


		return 0;
	#endif

	//cerr << krbuildertest() << endl;


	stringstream ssexcept;
	try
	{
		KruncherTools::CharVector defaults=
		{ 
			(char*) "--xml", (char*) "/home/jmt/Info/testfactory/src/text/builder.xml",
			(char*) "-P", (char*) "83"
		};
		KruncherTools::CmdArguments args( KruncherTools::Defaults( argc, argv, defaults ) );
		
		const bool HasObjective( args.exists( "-o" ) );
		if ( ! HasObjective ) 
		{
			if ( ! args.exists( "-" ) )
			{
				args( "-o", "inform" ); 
			} else {
				args( "-o", "ingest" ); 
			}
		}

		Objects objects;
		if ( !objects ) throw string("Cannot load main objectives" );
		return objects( args.size(), &args[0] );
	}
	catch( const exception& e ) { ssexcept<<e.what(); }
	catch( const string& s ) { ssexcept<<s;}
	catch( const char* s ) { ssexcept<<s;}
	catch( ... ) { ssexcept<<"unknown";}
	if (!ssexcept.str().empty())
	{
		stringstream ssout; 
		ssout << fence << "[EXCEPT]" << fence << ssexcept.str(); Log(VERB_ALWAYS, "restfulclientmain", ssout.str());
		cerr << red << ssout.str() << normal << endl;
	}
	return 0;
}
