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


#ifndef WEBKRUNCHER_WEBSITE_H
#define WEBKRUNCHER_WEBSITE_H
#include <infotools.h>
#include <infofigur.h>
#include <infosite.h>
#include <infokruncher.h>
using namespace KruncherTools;
namespace WebKruncherService
{
	struct InfoSite : InfoKruncher::Site
	{
		virtual void LoadResponse( InfoKruncher::Responder& r, InfoKruncher::RestResponse& Responder, InfoKruncher::ThreadLocalBase&  );
		virtual void Throttle( const InfoKruncher::SocketProcessOptions& );
		virtual void PostProcessing( InfoKruncher::Responder&, InfoKruncher::RestResponse& DefaultResponse, const binarystring& PostedContent, InfoKruncher::ThreadLocalBase&  );
		virtual bool ProcessForm( const string, stringmap& );
		private:
		virtual InfoKruncher::ThreadLocalBase* AllocateThreadLocal( const InfoKruncher::SocketProcessOptions& options );
	};

	struct BuilderProcessOptions : InfoKruncher::SocketProcessOptions
	{
		BuilderProcessOptions() : SocketProcessOptions(), purpose( "worker" ) {}
		virtual void operator()( const string name, const string  value )
		{
			SocketProcessOptions::operator()( name, value );
			if ( name == "purpose" ) purpose=value;
			if ( name == "buildtools" ) buildtools=value;
			if ( name == "builddefines" ) builddefines=value;
		}
		string purpose, buildtools, builddefines;
		private:
		virtual ostream& operator<<(ostream& o) const
		{
			SocketProcessOptions::operator<<( o );
			if ( ! purpose.empty() ) o << "purpose:" << purpose << endl;
			return o;
		}
	};

	struct BuilderServiceList : InfoKruncher::ServiceList
	{
		virtual InfoKruncher::SocketProcessOptions* NewOptions( XmlFamily::XmlNode& node ) 
		{ 
			XmlFamily::XmlAttributes& attrs( node.Attributes() );
			XmlFamily::XmlAttributes::iterator a( attrs.find( "purpose" ) );
			for ( XmlFamily::XmlAttributes::iterator ait=attrs.begin();ait!=attrs.end();ait++)
				cerr << ait->first << ":" << ait->second << endl;
			return new BuilderProcessOptions ; 
		}
	};
} // WebKruncherService
#endif //WEBKRUNCHER_WEBSITE_H



