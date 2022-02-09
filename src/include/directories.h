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

#ifndef KRBUILDER_DIRECTORIES_H
#define KRBUILDER_DIRECTORIES_H
#include <infobuilder.h>
#include <directory.h>
#include <regex.h>

namespace KrDirectories
{
	struct CmakeDirectory : KruncherDirectory::Directory
	{
		CmakeDirectory( const string& _where, const bool _recurse, const regex_t _ex ) 
			: Directory( _where, _recurse ), ex( _ex )
		{}
		const CmakeDirectory& operator = (const CmakeDirectory& that )
		{
			if ( this == &that ) return *this;
			where=that.where;
			recurse=that.recurse;
			ex=that.ex;
			return *this;
		}
		Directory& NewSub( const string _where, const bool _recurse );
		private:
		mutable regex_t ex;
		bool Filter( const dirent& ent ) const 
		{
			if ( ent.d_type == DT_DIR ) return false;
			if ( ent.d_type != DT_REG ) return true;
			return !!regexec( &ex, ent.d_name, 0, 0, 0 );
		}
		vector< CmakeDirectory > subs;
		friend ostream& operator<<(ostream&,const CmakeDirectory&);
		ostream& operator<<(ostream& o) const
		{
			for ( const_iterator it=begin();it!=end();it++ )
				o << where << separator << *it << endl;
			for ( vector< CmakeDirectory >::const_iterator sit=subs.begin();sit!=subs.end(); sit++ )
			{
				const CmakeDirectory& sub( *sit );
				o << sub;
			}
			return o;
		}
	}; 

	inline ostream& operator<<(ostream& o,const CmakeDirectory& m) { return m.operator<<(o); }

	inline KruncherDirectory::Directory& CmakeDirectory::NewSub( const string _where, const bool _recurse )
	{
		CmakeDirectory tmp( _where, recurse, ex );
		subs.push_back( tmp );
		return subs.back();
	}


	struct ftime : string
	{
		ftime() {}
		ftime( const string& n ) : string( n ), crud( Create ), kil( false ) {}
		mutable Crud crud;
		mutable bool kil;
		mutable time_t mtime;
	};

	struct FileTimeTracker : set< ftime >
	{
		friend ostream& operator<<(ostream&,const FileTimeTracker&);
		ostream& operator<<(ostream& o) const
		{
			for ( const_iterator sit=begin();sit!=end(); sit++ )
			{
				const ftime& n( *sit );
				switch ( n.crud )
				{
					case Update: o << green << n << normal << endl; break;
					case Delete: 
						o << red << n << normal << endl; 
						n.kil=true;
					break;
					case Create: o << yellow << n << normal << endl; break;
					case Retreive: break;
				}
			}
			return o;
		}
		operator bool () 
		{
			vector< ftime > kil;
			for ( const_iterator sit=begin();sit!=end(); sit++ )
			{
				const ftime& n( *sit );
				if ( n.kil ) kil.push_back( n );
				n.crud=Delete;
			}
			for ( vector< ftime >::const_iterator kit=kil.begin();kit!=kil.end();kit++)
			{
				const ftime& k( *kit );
				erase( k );
			}
			return true;
		}

		void operator()( const string name, const time_t mtime )
		{
			const_iterator found( find( name ) );
			if ( found == end() ) insert( name );
			else 
			{
				if ( found->mtime != mtime )
				{
					found->mtime=mtime;
					found->crud=Update;
				} else 
					found->crud=Retreive;
			}
		}
	}; 

	inline ostream& operator<<(ostream& o,const FileTimeTracker& m) { return m.operator<<(o); }


	struct FileTimes : KruncherDirectory::Directory
	{
		FileTimes( const string& _where, const bool _recurse, const regex_t _ex, FileTimeTracker& _tracker ) 
			: Directory( _where, _recurse ), ex( _ex ), tracker( _tracker )
		{}
		const FileTimes& operator = (const FileTimes& that )
		{
			if ( this == &that ) return *this;
			where=that.where;
			recurse=that.recurse;
			ex=that.ex;
			return *this;
		}
		Directory& NewSub( const string _where, const bool _recurse );
		private:
		mutable regex_t ex;
		FileTimeTracker& tracker;
		bool Filter( const dirent& ent ) const 
		{
			const ftime n( ent.d_name );
			if ( n == "src.build" ) return true;
			if ( n == ".git" ) return true;
			if ( n == "CMakeFiles" ) return true;
			if ( ent.d_type == DT_DIR ) return false;
			if ( ent.d_type != DT_REG ) return true;
			const bool r( !!regexec( &ex, ent.d_name, 0, 0, 0 ) );
			if ( ! r )
			{
				ftime N( where + string("/") + n );	
				struct stat sb;
				if ( stat( N.c_str(), &sb ) ) throw N;
				const time_t mtime( sb.st_mtim.tv_sec );
				tracker( N, mtime );
			}
			return r;
		}
		vector< FileTimes > subs;
		friend ostream& operator<<(ostream&,const FileTimes&);
		ostream& operator<<(ostream& o) const
		{
			for ( const_iterator it=begin();it!=end();it++ )
				o << where << separator << *it << endl;
			for ( vector< FileTimes >::const_iterator sit=subs.begin();sit!=subs.end(); sit++ )
			{
				const FileTimes& sub( *sit );
				o << sub;
			}
			return o;
		}
	}; 

	inline ostream& operator<<(ostream& o,const FileTimes& m) { return m.operator<<(o); }

	inline KruncherDirectory::Directory& FileTimes::NewSub( const string _where, const bool _recurse )
	{
		FileTimes tmp( _where, recurse, ex, tracker );
		subs.push_back( tmp );
		return subs.back();
	}






} // KrDirectories

#endif //KRBUILDER_DIRECTORIES_H

