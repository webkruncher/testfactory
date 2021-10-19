#ifndef GRAPH_H
#define GRAPH_H

struct Edge : pair< int, int >
{
	Edge( const int x, const int y ) : pair<int,int>( x,y ) {}
	private:
	friend ostream& operator<<( ostream&, const Edge& );
	ostream& operator<<(ostream& o ) const
	{
		o<< "(" << first << "," << second << ")";
		return o;
	}
};
inline ostream& operator<<( ostream& o, const Edge& a) { return a.operator<<(o);}

struct EdgeList : vector< Edge >
{
	private:
	friend ostream& operator<<( ostream&, const EdgeList& );
	ostream& operator<<(ostream& o ) const
	{
		for ( const_iterator it=begin();it!=end();it++)
			o<<(*it)<<" ";
		return o;
	}
};
inline ostream& operator<<( ostream& o, const EdgeList& a) { return a.operator<<(o);}

struct NodeList : vector<int>
{
	private:
	friend ostream& operator<<( ostream&, const NodeList& );
	ostream& operator<<(ostream& o ) const
	{
		for ( const_iterator it=begin();it!=end();it++)
			o<<(*it)<<" ";
		return o;
	}
};
inline ostream& operator<<( ostream& o, const NodeList& a) { return a.operator<<(o);}

struct AdjecencyList : map<int, NodeList >
{
	void operator<<(const EdgeList& edges)
	{
		for ( EdgeList::const_iterator it=edges.begin();it!=edges.end();it++)
		{
			const Edge& edge( *it );
			const int& x( edge.first );
			const int& y( edge.second );
			map<int,NodeList>& me( *this );
			me[x].push_back(y);
			me[y].push_back(x);
		}
	}
	private:
	friend ostream& operator<<( ostream&, const AdjecencyList& );
	ostream& operator<<(ostream& o ) const
	{
		for ( const_iterator it=begin();it!=end();it++)
		{
			const NodeList& n( it->second );
			o<<it->first<<":"<<n<<endl;
		}
		return o;
	}
};
inline ostream& operator<<( ostream& o, const AdjecencyList& a) { return a.operator<<(o);}


#endif // GRAPH_H

