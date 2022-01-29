
#ifndef INFO_BUILD_FACE
#define INFO_BUILD_FACE

#include <InfoBuild.h>

namespace InfoBuildFace
{
	inline void Allocate( const string datapath )
	{
		InfoBuilder::LibTimes::Get( datapath );
	}

	inline void Release()
	{
		InfoBuilder::LibTimes::Release();
	}

	struct Bindings : map< string, RestData::BindingBase* >
	{
		virtual ~Bindings() { for ( iterator it=begin();it!=end();it++) delete it->second; } 
	};

        struct ThreadLocal  : InfoKruncher::ThreadLocalBase
	{
		ThreadLocal() : times( 0 ), reporttrigger( false ) {}
		virtual ~ThreadLocal() {}
		bool operator()( const InfoKruncher::SocketProcessOptions& options ) ;
		RestData::BindingBase* operator()( const InfoKruncher::Responder& r ) const;
		protected:
		virtual ostream& operator<<( ostream& o ) const
		{
			if ( reporttrigger )
				o << fence << getpid() << fence << pthread_self() << fence << times << fence << endl;
			reporttrigger =false;
			return o;
		}
		Bindings binders;
		private:
		mutable long times;
		mutable bool reporttrigger;
	};

	typedef InfoKruncher::ThreadLocalStorage< ThreadLocal > LocalStorage;


	inline void LoadResponse( InfoKruncher::Responder& r, const string& ServiceName, InfoKruncher::RestResponse& response, InfoKruncher::ThreadLocalBase& tl )	
		{ RestData::LoadResponse< LocalStorage >( r, ServiceName, response, tl ); }

        inline void PostProcessing
                ( InfoKruncher::Responder& respond, const string& ServiceName, 
			InfoKruncher::RestResponse& response, const binarystring& PostedContent, InfoKruncher::ThreadLocalBase& tl )
				{ RestData::PostProcessing< LocalStorage >( respond, ServiceName, response, PostedContent, tl ); }
                                                                      
        inline InfoKruncher::ThreadLocalBase* AllocateThreadLocal( const InfoKruncher::SocketProcessOptions& options )
        {                                                             
		LocalStorage* ptls( new LocalStorage );
		LocalStorage& tls( *ptls );
		if ( ! tls( options ) ) throw string( "Cannot initialize ThreadLocalStorage" );
		return ptls;
        }                     

} // InfoBuildFace


#endif // INFO_BUILD_FACE


