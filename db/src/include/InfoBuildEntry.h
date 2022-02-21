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
#include <infobuilder.h>
#include <krbuilder.h>

namespace InfoBuilderService
{
	XmlFamily::XmlNodeBase* BuilderServiceList::NewNode
	(
		XmlFamily::Xml& _doc,
		XmlFamily::XmlNodeBase* parent,
		stringtype name,
		InfoKruncher::ServiceList& servicelist,
		const string& optionnode,
		const string& filter
	) const
	{ 
		cerr << red << "NewNode:" << name << normal << endl;
		throw "UNDER CONSTRUCTION";
		if ( name == "builder" ) 
			return new krbuilder::BuilderNode(_doc,parent,name,servicelist, optionnode, filter); 

		XmlFamily::XmlNodeBase* ret(NULL);
		ret=new ServiceXml::Item(_doc,parent,name,servicelist, optionnode, filter); 
		ServiceXml::Item& n( static_cast<ServiceXml::Item&>(*(ret)) );
		return ret;
	}


	ServiceXml::Item* BuildInfoConfiguration::FindNode( const string& what, ServiceXml::Item& node )
	{
		if ( node.name == what ) return &node;
		ServiceXml::Item& nodes( static_cast<ServiceXml::Item& >( node ) );
		for (XmlFamily::XmlNodeSet::iterator it=nodes.children.begin();it!=nodes.children.end();it++) 
		{
			ServiceXml::Item& n(static_cast<ServiceXml::Item&>(*(*it)));
			ServiceXml::Item* found( FindNode( what, n ) );
			if ( found ) return found;
		}
		return nullptr;
	}

} // InfoBuilderService


namespace InfoKruncher
{

	struct DbSite : InfoSite
	{
		bool ProcessForm( const string formpath, stringmap& formdata )
		{
			stringstream ssmsg;  ssmsg << "DbSite" << fence << formpath << fence << formdata;
			Log( ssmsg.str() );
			return true;
		}
		void PostProcessing( InfoKruncher::Responder& responder, InfoKruncher::RestResponse& DefaultResponse, const binarystring& PostedContent, InfoKruncher::ThreadLocalBase& threadlocal ) 
		{
			InfoSite::PostProcessing( responder, DefaultResponse, PostedContent, threadlocal );
		}
	};

	template<> 
		void InfoKruncher::Service< InfoBuilderService::InfoSite >::ForkAndServe( PROPERTIES_BASE& node, const SocketProcessOptions& svcoptions )
	{
		const InfoBuilderService::BuilderProcessOptions& builder( static_cast< const InfoBuilderService::BuilderProcessOptions& >( svcoptions ) );
		Log( VERB_ALWAYS, "Krunching" , builder.purpose );
		if ( builder.purpose == "scanner" )
		{
			InfoBuilderService::BuildInfoConfiguration& buildinfo( static_cast< InfoBuilderService::BuildInfoConfiguration& > ( node ) );
			ServiceXml::Item* root( static_cast< ServiceXml::Item* >( buildinfo.Root ) );
			if ( ! root ) throw string("No root node for scanner" );
			ServiceXml::Item* pnode( buildinfo.FindNode( "builder", *root ) );
			if ( ! pnode )  throw string("Cannot find builder node");
			krbuilder::BuilderNode& Builder( static_cast< krbuilder::BuilderNode& >( *pnode ) );
			Builder.Scanner( builder );
			cerr << "Done scanning, exiting" << endl;
			return;
		}
		RunService( node, svcoptions );
	}
	template<> void InfoKruncher::Service< InfoBuilderService::InfoSite >::Terminate() { subprocesses.Terminate(); }
} // InfoKruncher




int main( int argc, char** argv )
{
	//VERBOSITY=VERB_SIGNALS|VERB_SSOCKETS|VERB_PSOCKETS;
	VERBOSITY=VERB_SERVICE;
	stringstream ssexcept;
	try
	{
		InfoBuilderService::BuildInfo options( argc, argv );

		if ( ! options ) throw string( "Invalid options" );
		PROPERTIES_BASE& Cfg( options );
		if ( options.find( "-d" ) == options.end() ) Initialize();

		const InfoKruncher::ServiceList& workerlist( options.workerlist );

		const size_t nSites( options.workerlist.size() );

		if ( options.find( "--check-config" ) != options.end() )
		{
			cerr << "Configuration:" << endl << workerlist << endl;
			return 0;
		}
		
		cerr << yellow << "krbuilder is starting up with " << nSites << " sites " << normal << endl;

		KruncherTools::Daemonizer daemon( options.daemonize, "KrBuilder" );

		InfoKruncher::Service<InfoBuilderService::InfoSite> sites[ nSites ];

		for ( size_t c=0;  c < nSites; c++ )
		{
			InfoKruncher::Service<InfoBuilderService::InfoSite>& site( sites[ c ] );
			const InfoKruncher::SocketProcessOptions& svcoptions( *workerlist[ c ] );
			site.ForkAndServe( Cfg, svcoptions );
		}
		while ( !TERMINATE ) sleep( 1 );
		Log( "krbuilder is exiting" );
		for ( size_t t=0; t < nSites; t++ ) sites[ t ].Terminate();
	}
	catch( const exception& e ) { ssexcept<<e.what(); }
	catch( const string& s ) { ssexcept<<s;}
	catch( const char* s ) { ssexcept<<s;}
	catch( ... ) { ssexcept<<"unknown";}
	if (!ssexcept.str().empty())
	{
		stringstream ssout; ssout << fence << "[EXCEPT]" << fence << ssexcept.str(); Log(VERB_ALWAYS, "webkrunchermain", ssout.str());
	}

	return 0;
}


