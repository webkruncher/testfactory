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
using namespace InfoBuilderService;
#include "directories.h"
#include "krbuildactors.h"


KrBuildDefinitions::operator bool( )
{
	stringmap& me( *this );
	KruncherTools::CharVector parameters{ (char*) "BuildTools", (char*) "-GetBuildDefines", (char*) builddefines.c_str(), nullptr };
	stringstream ss;
	KruncherTools::forkpipe( buildtools, parameters, "", ss );
	stringvector lines; lines.split( ss.str(), "\n" );
	for ( stringvector::const_iterator lit=lines.begin();lit!=lines.end();lit++)
	{
		const string line( *lit );
		stringvector parts; parts.split( line, "|" );
		if ( parts.size() < 3 ) continue;
		const string name( parts[ 1 ] );
		const string value( parts[ 2 ] );
		me[ name ] = value;
	}
	return true;
}

void ScanForMakefiles( const string buildtools, const BuilderProcessOptions& options, stringstream& ss )
{
	KruncherTools::CharVector parameters{ (char*) "BuildTools", (char*) "-GetCmakeLists", nullptr };
	KruncherTools::forkpipe( options.buildtools, parameters, "", ss );
}

const string SliceProjectName( const KrBuildDefinitions& defines,  const string& pathname )
{
	const string LibPath( defines[ "LIBPATH" ] );
	if ( pathname.find( LibPath ) != 0 ) throw pathname;
	const string pathlessname( pathname.substr( LibPath.size(), pathname.size()-LibPath.size() ) );
	const size_t ls( pathlessname.find_last_of( "/" ) );
	if ( ls == string::npos ) throw pathlessname;
	const string projectname( pathlessname.substr( 1, ls-1 ) );
	return projectname;
}


void ScanCmake( const KrBuildDefinitions& defines, const string& buildtools, const string cmake, KrBuilder& builder, const string how )
{
	const size_t ls( cmake.find_last_of( '/' ) );
	if ( ls == string::npos ) return;
	const string pathname( cmake.substr( 0, ls ) );
	KrProjects empty;
	builder.emplace( pathname, empty );

	const string ProjectName( SliceProjectName( defines, pathname ) );


	KrProjects& projects( builder[ ProjectName ] );
	KruncherTools::CharVector parameters{ (char*) "BuildTools", (char*) how.c_str(), (char*) pathname.c_str(), nullptr };
	stringstream ss;
	KruncherTools::forkpipe( buildtools, parameters, "", ss );
	stringvector lines; lines.split( ss.str(), "\n" );
	for ( stringvector::const_iterator lit=lines.begin(); lit!=lines.end(); lit++ )
	{
		const string line( *lit );
		stringvector items;
		items.split( line, " " );
		if ( items.empty() ) continue;
		const string target( items[ 0 ] );

		for ( size_t t=1; t<items.size();t++)
		{
			const crudstring lib( items[ t ] );
			if ( lib.empty() ) continue;
			projects[ target ]( lib );
		}
	}
}

void UpdateBuildSpecs( const KrBuildSpecs& krbuilder, const string where )
{
	const KrBuilder& buildprinter( krbuilder );	
	stringstream sspost;
	sspost << krbuilder;

	if ( true )
	{
		cout << "Post:" << endl << sspost.str();
		cout << setw( 128 ) << setfill( '-' ) << "-" << endl;
	}
}


static int times( 0 );



InfoKruncher::SocketProcessOptions* BuilderServiceList::NewOptions( XmlFamily::XmlNode& node ) 
{ 
	XmlFamily::XmlAttributes& attrs( node.Attributes() );
	XmlFamily::XmlAttributes::iterator a( attrs.find( "purpose" ) );
	
	for ( XmlFamily::XmlAttributes::iterator ait=attrs.begin();ait!=attrs.end();ait++)
	{
		const string& name( ait->first );
		const string& value( ait->second );
		if ( name == "purpose" )
			if ( value == "scanner" )
				return new BuildScanner;
	}
	return new BuilderProcessOptions ; 
}

void BuilderNode::Scanner( const InfoBuilderService::BuilderProcessOptions& options)
{
	using namespace KrDirectories;

	KrBuildDefinitions defines( options.builddefines, options.buildtools );
	if ( ! defines ) throw string("Cannot load build definitions");
	const string LibPath( defines[ string( "LIBPATH" ) ] );
	
	regex_t rxupdates;
	const string expfiles( "^.*\\.cpp$|^.*\\.h$|^CMakeLists.txt$|^.*\\.a$" );
	if ( regcomp( &rxupdates, expfiles.c_str(), REG_EXTENDED ) ) throw expfiles;
	FileTimeTracker tracker;
	bool first( true );
	while ( ! TERMINATE )
	{
		FileTimes fileupdates( LibPath, true, rxupdates, tracker );
		if ( ! fileupdates ) return;

		ftimevector cpp,h,make,lib;

		struct P : pair< string, ftimevector* >
			{ P( const string what, ftimevector* how ) : pair< string, ftimevector* >( what, how ) {} };
		struct KrCollections : map< string, ftimevector* > {};

		KrCollections collection;

		P Cpp( ".cpp", &cpp );
		P H( ".h", &h );
		P Make( ".txt", &make );
		P Lib( ".a", &lib );
		
		collection.insert( Cpp );
		collection.insert( H );
		collection.insert( Make );
		collection.insert( Lib );

		tracker >> collection;

		auto Run = []( XmlFamily::NodeIndex& index, ftimevector& ftimes, const string& nodename )
		{
			XmlNodeBase* p( index[ nodename ] );
			if ( !p ) 
			{
				Log( VERB_ALWAYS, "NodeBuilder - No support for ", nodename );
			}
			BuilderNode& b( static_cast< BuilderNode& >( *p ) );
			if ( ! ftimes.empty() ) b( ftimes );
		};

		if ( ! first )
		{
			Run( index, make, "Makefiles" );
			Run( index, cpp, "Sources" );
			Run( index, h, "Headers" );
			Run( index, lib, "Libraries" );
		}

		
		if ( ! tracker ) throw string("File time tracker error");
		first=false;
	} 

return;	

#if 0
	//cerr << defines << endl;

	KrBuildSpecs libraries, includes;	

	while ( ! TERMINATE )
	{
		stringstream ssCMakefiles;
		ScanForMakefiles( options.buildtools, ssCMakefiles );
		stringvector sv; sv.split( ssCMakefiles.str(), "\n" );
	
		for ( stringvector::const_iterator sit=sv.begin();sit!=sv.end();sit++)
		{
			const string projectpath( *sit );
			if ( projectpath.empty() ) continue;
			if ( projectpath[ 0 ] != '/' ) continue;
			ScanCmake( defines, options.buildtools, projectpath, libraries, "-GetCmakeLinkage" );
			ScanCmake( defines, options.buildtools, projectpath, includes, "-GetCmakeIncludes" );
		}

		UpdateBuildSpecs( libraries, "libraries" );	
		UpdateBuildSpecs( includes, "includes" );	

		sleep( 1 );
	}
#endif
}

	XmlFamily::XmlNodeBase* BuilderNode::NewNode(XmlFamily::Xml& _doc,XmlFamily::XmlNodeBase* parent,stringtype name ) const
	{ 
		XmlFamily::XmlNodeBase* ret(NULL);
		if ( name == "builder" ) 
			ret=new KrBuildActors::BuildActorNode( _doc, parent, name, servicelist, optionnode, filter); 
		if ( ! ret ) ret=new BuilderNode( _doc, parent, name, servicelist, optionnode, filter); 
		return ret;
	}

	BuilderNode::operator bool ()
	{
		for (XmlFamily::XmlNodeSet::iterator it=children.begin();it!=children.end();it++) 
		{
			XmlNode& n=static_cast<XmlNode&>(*(*it));
			BuilderNode& b( static_cast<BuilderNode&>( n ) );
			//cerr << name << "->" << b.name << endl;
			index( b.name, &b );
		}
		return ServiceXml::Item::operator bool ();
	}


