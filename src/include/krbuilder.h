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

#ifndef KRBUILDER_H
#define KRBUILDER_H
#include <infobuilder.h>
#include <directory.h>
#include <regex.h>



void KrScanner( const InfoBuilderService::BuilderProcessOptions& options);

struct KrBuildDefinitions : stringmap
{
	KrBuildDefinitions( const InfoBuilderService::BuilderProcessOptions& _options) : options( _options ) {}
	operator bool ();
	string operator []( const string& what ) const
	{
		const_iterator it( find( what ) );
		if ( it == end() ) throw what;
		return it->second;
	}
	private:
	const InfoBuilderService::BuilderProcessOptions& options;
};


struct crudstring : string
{
	crudstring() : crud( Create ) {}
	crudstring(const string& s) : string( s ), crud( Create ) {}
	private:
	KruncherTools::Crud crud;
};

struct crudstringset : set< crudstring >
{
	void operator()( const crudstring& s )
	{
		insert( s );
	}
};

struct KrProjects : map< string, crudstringset >
{
	friend ostream& operator<<( ostream&, const KrProjects& );
	ostream& operator<<( ostream& o ) const
	{
		return o;
	}
};
inline ostream& operator<<( ostream& o, const KrProjects& k ) { return k.operator<<( o ); }

struct KrBuilder : map< string, KrProjects >
{
	private:
	friend ostream& operator<<( ostream&, const KrBuilder& );
	ostream& operator<<( ostream& o ) const
	{
		for ( const_iterator it=begin();it!=end();it++ ) 
		{
			o << it->first << endl ;

			const KrProjects& p( it->second );
			for ( KrProjects::const_iterator kit=p.begin();kit!=p.end();kit++ ) 
			{
				const string targetname( kit->first );
				o << tab << targetname << endl;
				const crudstringset& sv( kit->second );
				for ( crudstringset::const_iterator sit=sv.begin();sit!=sv.end();sit++)
				{
					const string& lib( *sit );
					o << tab << tab << lib << endl; 
				}
			}
		}
		return o;
	}
};

inline ostream& operator<<( ostream& o, const KrBuilder& k ) { return k.operator<<( o ); }

struct KrBuildSpecs : KrBuilder 
{
	private:
	friend ostream& operator<<( ostream&, const KrBuildSpecs& );
	ostream& operator<<( ostream& o ) const
	{
		for ( const_iterator it=begin();it!=end();it++ ) 
		{

			const KrProjects& p( it->second );
			for ( KrProjects::const_iterator kit=p.begin();kit!=p.end();kit++ ) 
			{
				const string targetname( kit->first );
				const crudstringset& sv( kit->second );
				for ( crudstringset::const_iterator sit=sv.begin();sit!=sv.end();sit++)
				{
					const string& lib( *sit );
					o << fence << it->first << fence << targetname << fence << lib << fence << endl; 
				}
			}
		}
		return o;
	}
};

inline ostream& operator<<( ostream& o, const KrBuildSpecs& k ) { return k.operator<<( o ); }

struct BuildScanner : InfoBuilderService::BuilderProcessOptions
{
	BuildScanner() {}
};

#endif //KRBUILDER_H

