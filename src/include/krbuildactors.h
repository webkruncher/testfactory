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

#ifndef KRBUILDER_ACTORS_H
#define KRBUILDER_ACTORS_H
//namespace krbuilder {
	using namespace KrDirectories;
	struct BuildActorNode : BuilderNode
	{
		BuildActorNode(XmlFamily::Xml& _doc,const XmlNodeBase* _parent,stringtype _name, InfoKruncher::ServiceList& _servicelist, const string _optionnode, const string _filter ) 
			: BuilderNode(_doc,_parent,_name,_servicelist,_optionnode,_filter )  
		{}
		virtual XmlFamily::XmlNodeBase* NewNode(XmlFamily::Xml& _doc,XmlFamily::XmlNodeBase* parent,stringtype name ) const;
		operator bool () { return BuilderNode::operator bool(); }
		private:
		virtual void Creating( const ftime& what ) 	{ throw name + string( " Unimplemented BuildActorNode - Creating" ) 	+  what ; }
		virtual void Retreiving( const ftime& what ) 	{ throw name + string( " Unimplemented BuildActorNode - Retreiving" ) 	+  what ; }
		virtual void Updating( const ftime& what ) 	{ throw name + string( " Unimplemented BuildActorNode - Updating" ) 	+  what ; }
		virtual void Deleting( const ftime& what ) 	{ throw name + string( " Unimplemented BuildActorNode - Deleting" ) 	+  what ; }
		virtual void operator()( const KrDirectories::ftimevector& ftimes ); 
	};



	struct BuildMakeNode : BuildActorNode
	{
		BuildMakeNode(XmlFamily::Xml& _doc,const XmlNodeBase* _parent,stringtype _name, InfoKruncher::ServiceList& _servicelist, const string _optionnode, const string _filter ) 
			: BuildActorNode(_doc,_parent,_name,_servicelist,_optionnode,_filter )  
		{}
		virtual XmlFamily::XmlNodeBase* NewNode(XmlFamily::Xml& _doc,XmlFamily::XmlNodeBase* parent,stringtype name ) const
			{ return new BuildMakeNode	( _doc, parent, name, servicelist, optionnode, filter); }
		operator bool () { return BuildActorNode::operator bool(); }
		virtual void Creating( const ftime& );
		virtual void Retreiving( const ftime& );
		virtual void Updating( const ftime& );
		virtual void Deleting( const ftime& );
	};

	struct BuildSourceNode : BuildActorNode
	{
		BuildSourceNode(XmlFamily::Xml& _doc,const XmlNodeBase* _parent,stringtype _name, InfoKruncher::ServiceList& _servicelist, const string _optionnode, const string _filter ) 
			: BuildActorNode(_doc,_parent,_name,_servicelist,_optionnode,_filter )  
		{}
		virtual XmlFamily::XmlNodeBase* NewNode(XmlFamily::Xml& _doc,XmlFamily::XmlNodeBase* parent,stringtype name ) const
			{ return new BuildSourceNode	( _doc, parent, name, servicelist, optionnode, filter); }
		operator bool () { return BuildActorNode::operator bool(); }
		virtual void Creating( const ftime& );
		virtual void Retreiving( const ftime& );
		virtual void Updating( const ftime& );
		virtual void Deleting( const ftime& );
	};

	struct BuildLibraryNode : BuildActorNode
	{
		BuildLibraryNode(XmlFamily::Xml& _doc,const XmlNodeBase* _parent,stringtype _name, InfoKruncher::ServiceList& _servicelist, const string _optionnode, const string _filter ) 
			: BuildActorNode(_doc,_parent,_name,_servicelist,_optionnode,_filter )  
		{}
		virtual XmlFamily::XmlNodeBase* NewNode(XmlFamily::Xml& _doc,XmlFamily::XmlNodeBase* parent,stringtype name ) const
			{ return new BuildLibraryNode	( _doc, parent, name, servicelist, optionnode, filter); }
		operator bool () { return BuildActorNode::operator bool(); }
		virtual void Creating( const ftime& what );
		virtual void Retreiving( const ftime& what );
		virtual void Updating( const ftime& what );
		virtual void Deleting( const ftime& what );
	};

	struct BuildHeaderNode : BuildActorNode
	{
		BuildHeaderNode(XmlFamily::Xml& _doc,const XmlNodeBase* _parent,stringtype _name, InfoKruncher::ServiceList& _servicelist, const string _optionnode, const string _filter ) 
			: BuildActorNode(_doc,_parent,_name,_servicelist,_optionnode,_filter )  
		{}
		virtual XmlFamily::XmlNodeBase* NewNode(XmlFamily::Xml& _doc,XmlFamily::XmlNodeBase* parent,stringtype name ) const
			{ return new BuildHeaderNode	( _doc, parent, name, servicelist, optionnode, filter); }
		operator bool () { return BuildActorNode::operator bool(); }
		virtual void Creating( const ftime& );
		virtual void Retreiving( const ftime& );
		virtual void Updating( const ftime& );
		virtual void Deleting( const ftime& );
	};


//} //krbuilder
#endif // KRBUILDER_ACTORS_H

