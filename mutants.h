
#ifndef MUTANTS_H
#define MUTANTS_H


template <typename B,typename V,typename K,typename Curious>
	struct Mutants : V
{
	Mutants(Curious& _curious) : curious(_curious) {}
	Mutants(const Mutants& a,Curious& _curious) : V(a),curious(_curious) {}
	operator V& (){return *this;}
	void operator-=(K n) { typename V::iterator it(find(this->begin(),this->end(),n)); if (it!=this->end()) erase(it); }
	void operator+=(K n) { this->push_back(n); }
	virtual void operator()(Mutants& b,B& tbd)
	{
		Mutants& a(*this);
		if (a.empty()) curious.Run(b,tbd);
		for (typename V::iterator it=a.begin();it!=a.end();it++)
		{
			K n(*it);
			Mutants suba(a); Mutants subb(b);
			suba-=n; subb+=n;
			suba(subb,tbd);
		}
	}
	private:
	Curious& curious;
};

#endif // MUTANTS_H

