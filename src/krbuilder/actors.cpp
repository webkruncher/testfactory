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

namespace KrBuildActors
{
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
			Log( VERB_ALWAYS, name, sso.str() );
		}
	}

	void BuildMakeNode::Creating( const ftime& what ) 	{ cerr << "MakeNode:" << what << endl; }
	void BuildMakeNode::Retreiving( const ftime& what ) 	{ cerr << "MakeNode:" << what << endl; }
	void BuildMakeNode::Updating( const ftime& what ) 	{ cerr << "MakeNode:" << what << endl; }
	void BuildMakeNode::Deleting( const ftime& what ) 	{ cerr << "MakeNode:" << what << endl; }


	void BuildSourceNode::Creating( const ftime& what ) 	{ cerr << "SourceNode:" << what << endl; }
	void BuildSourceNode::Retreiving( const ftime& what ) 	{ cerr << "SourceNode:" << what << endl; }
	void BuildSourceNode::Updating( const ftime& what ) 	{ cerr << "SourceNode:" << what << endl; }
	void BuildSourceNode::Deleting( const ftime& what )
	{
		cerr << "Deleted " << what << endl << (*this) << endl;
	}


	void BuildLibraryNode::Creating( const ftime& what ) 	{ cerr << "LibraryNode:" << what << endl; }
	void BuildLibraryNode::Retreiving( const ftime& what ) 	{ cerr << "LibraryNode:" << what << endl; }
	void BuildLibraryNode::Updating( const ftime& what ) 	{ cerr << "LibraryNode:" << what << endl; }
	void BuildLibraryNode::Deleting( const ftime& what ) 	{ cerr << "LibraryNode:" << what << endl; }


	void BuildHeaderNode::Creating( const ftime& what ) 	{ cerr << "HeaderNode:" << what << endl; }
	void BuildHeaderNode::Retreiving( const ftime& what ) 	{ cerr << "HeaderNode:" << what << endl; }
	void BuildHeaderNode::Updating( const ftime& what ) 	{ cerr << "HeaderNode:" << what << endl; }
	void BuildHeaderNode::Deleting( const ftime& what ) 	{ cerr << "HeaderNode:" << what << endl; }

} //KrBuildActors






