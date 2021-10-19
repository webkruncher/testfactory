



#include <iostream>
#include <vector>
#include <map>
using namespace std;
#include <graph.h>


int main(int,char*)
{
	EdgeList edges;
	edges.push_back( Edge( 1,2 ) );
	edges.push_back( Edge( 3,2 ) );
	edges.push_back( Edge( 5,8 ) );

	AdjecencyList adj;

	adj<<edges;

	cout << adj;
	return 0;
}

