
#ifndef CUSTOM_KEY_H
#define CUSTOM_KEY_H


namespace CustomKey
{
	struct Stuff
	{
		Stuff() {}
		Stuff(int j) {Value=j;}
		Stuff(const Stuff& a) : Value(a.Value) {}
		Stuff& operator=(const Stuff& a) {Value=a.Value;}
		Stuff& operator=(char c) {Value=c;}
		Stuff& operator+=(char c) {Value+=c;}
		bool operator==(const Stuff& a) const { return Value==a.Value; }
		bool operator<(const Stuff& a) const { return (Value<a.Value); }
		bool operator>(const Stuff& a) { return (Value>a.Value); }
		bool operator>=(const Stuff& a) { return (Value>=a.Value); }
		private:
		char Value;
		friend ostream& operator<<(ostream&,const Stuff&);
		ostream& operator<<(ostream& o) const { o<<Value<<" "; return o; }
	};
	inline ostream& operator<<(ostream& o,const Stuff& s){return s.operator<<(o);}
} // CustomKey

#endif //__CUSTOM_KEY_H

