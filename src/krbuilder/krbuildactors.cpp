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


namespace krbuilder 
{
	void ScanForMakefiles( const string buildtools, stringstream& ss )
	{
		KruncherTools::CharVector parameters{ (char*) "BuildTools", (char*) "-GetCmakeLists", nullptr };
		KruncherTools::forkpipe( buildtools, parameters, "", ss );
	}

	const string SliceProjectName( const string& LibPath,  const string& pathname )
	{
		if ( pathname.find( LibPath ) != 0 ) throw pathname;
		const string pathlessname( pathname.substr( LibPath.size(), pathname.size()-LibPath.size() ) );
		const size_t ls( pathlessname.find_last_of( "/" ) );
		if ( ls == string::npos ) throw pathlessname;
		const string projectname( pathlessname.substr( 1, ls-1 ) );
		return projectname;
	}

	void ScanCmake
	(
		const string& LibPath, 
		const string& buildtools, 
		const string cmake, 
		KrBuilder& builder, 
		const string how
	)
	{
		cerr << green << "scanning " << cmake << normal << endl;
		const size_t ls( cmake.find_last_of( '/' ) );
		if ( ls == string::npos ) return;
		const string pathname( cmake.substr( 0, ls ) );
		KrProjects empty;
		builder.emplace( pathname, empty );

		const string ProjectName( SliceProjectName( LibPath, pathname ) );

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


	XmlFamily::XmlNodeBase* BuildActorNode::NewNode(XmlFamily::Xml& _doc,XmlFamily::XmlNodeBase* parent,stringtype name ) const
	{ 
		XmlFamily::XmlNodeBase* ret(NULL);
		if ( name == "Makefiles" ) 	ret=new BuildMakeNode	( _doc, parent, name, servicelist, optionnode, filter); 
		if ( name == "Sources" ) 	ret=new BuildSourceNode	( _doc, parent, name, servicelist, optionnode, filter); 
		if ( name == "Libraries" ) 	ret=new BuildLibraryNode( _doc, parent, name, servicelist, optionnode, filter); 
		if ( name == "Headers" ) 	ret=new BuildHeaderNode	( _doc, parent, name, servicelist, optionnode, filter); 
		if ( ! ret ) throw name;
		return ret;
	}

	void BuildActorNode::operator()( const KrDirectories::ftimevector& ftimes)
	{
		for ( KrDirectories::ftimevector::const_iterator it=ftimes.begin();it!=ftimes.end();it++)
		{
			const KrDirectories::ftime& what( *it );
			char C( '-' );
			switch ( what.crud )
			{
				case Create: 	Creating( what );   C='C'; break;
				case Retreive: 	Retreiving( what ); C='R'; break;
				case Update: 	Updating( what );   C='U'; break;
				case Delete: 	Deleting( what );   C='D'; break;
			}
			stringstream sso;
			sso << C << fence << what;
			//Log( VERB_ALWAYS, name, sso.str() );
		}
	}

	void BuildMakeNode::Creating( const ftime& what ) 	{ Updating( what ); }
	void BuildMakeNode::Retreiving( const ftime& what ) 	{ throw string( "No retreive method in BuildNode: " ) + what; }


	string ProjectLine( const string& buildtools, const string& fname )
	{
		#if 1
			//cerr << buildtools << " -IsProject:" << fname << endl;
			stringstream ss;
			KruncherTools::CharVector parameters{ (char*) "BuildTools", (char*) "-IsProject", (char*)fname.c_str(), nullptr };
			KruncherTools::forkpipe( buildtools, parameters, "", ss );
			//cerr << blue << buildtools << fence << red << ss.str() << normal << endl;
			return ss.str();
		#else
			ifstream in( fname.c_str() );
			for ( int j=0;j<10;j++ )
			{
				string line;
				getline( in, line );
				if ( line.empty() ) continue;
				if ( line.find( "project") == 0 )
					return line;
			}
			return "";
		#endif
	};


	void BuildMakeNode::Updating( const ftime& _what ) 	
	{ 
		const string& BuildTools( XmlFamilyUtils::AncestorsAttribute( this, "buildtools" ) );
		const string& BuildDefines( XmlFamilyUtils::AncestorsAttribute( this, "builddefines" ) );
		const string& LibPath( Property( "LibPath" ) );


		int depth( 0 );
		XmlFamilyUtils::Depth( this, depth );
		XmlFamily::XmlNodeBase& ScannerNode( Ascend( this, depth-3 ) );
		BuilderNode& scanner( static_cast< BuilderNode& > ( ScannerNode ) );

		auto up = []( string& what, const string& LibPath )
		{
			const size_t ls( what.find_last_of( "/" ) );
			if ( ls == string::npos ) { what=LibPath; return ; }
			if ( ls == 0 ) { what=LibPath; return ; }
			const size_t sls( what.find_last_of( "/", ls-1 ) );
			if ( sls == string::npos ) { what=LibPath; return ; }
			if ( sls == 0 ) { what=LibPath; return ; }
			what.erase( sls, ls-sls );
		};

		string what( _what );
		string projectline;
		while ( projectline.empty() )
		{
			if ( what.empty() ) return;
			if ( what == LibPath ) return;
			projectline=( ProjectLine( BuildTools, what ) );
			if ( ! projectline.empty() ) break;
			up( what, LibPath );
		}

		Log( VERB_ALWAYS, what, projectline );

		KrBuildSpecs libraries, includes;	
		ScanCmake( LibPath, BuildTools, what, libraries, "-GetCmakeLinkage" );
		ScanCmake( LibPath, BuildTools, what, includes, "-GetCmakeIncludes" );

		scanner.UpdateBuildSpecs( libraries, "libraries" );	
		scanner.UpdateBuildSpecs( includes, "includes" );

		InfoBuilderService::BuildInfoConfiguration& Cfg( static_cast< InfoBuilderService::BuildInfoConfiguration& >( GetDoc() ) );
		Cfg( *this, libraries );
	}
	void BuildMakeNode::Deleting( const ftime& what ) 	{}//{ cerr << "MakeNode:" << what << endl; }

	void BuildSourceNode::Creating( const ftime& what ) 	{}//{ cerr << "SourceNode:" << what << endl; }
	void BuildSourceNode::Retreiving( const ftime& what ) 	{}//{ cerr << "SourceNode:" << what << endl; }
	void BuildSourceNode::Updating( const ftime& what ) 	{}//{ cerr << "SourceNode:" << what << endl; }
	void BuildSourceNode::Deleting( const ftime& what ) 	{}//{ cerr << "Deleted " << what << endl << (*this) << endl; }


	void BuildLibraryNode::Creating( const ftime& what ) 	{}//{ cerr << "LibraryNode:" << what << endl; }
	void BuildLibraryNode::Retreiving( const ftime& what ) 	{}//{ cerr << "LibraryNode:" << what << endl; }
	void BuildLibraryNode::Updating( const ftime& what ) 	{}//{ cerr << "LibraryNode:" << what << endl; }
	void BuildLibraryNode::Deleting( const ftime& what ) 	{}//{ cerr << "LibraryNode:" << what << endl; }


	void BuildHeaderNode::Creating( const ftime& what ) 	{}//{ cerr << "HeaderNode:" << what << endl; }
	void BuildHeaderNode::Retreiving( const ftime& what ) 	{}//{ cerr << "HeaderNode:" << what << endl; }
	void BuildHeaderNode::Updating( const ftime& what ) 	{}//{ cerr << "HeaderNode:" << what << endl; }
	void BuildHeaderNode::Deleting( const ftime& what ) 	{}//{ cerr << "HeaderNode:" << what << endl; }

} //krbuilder






