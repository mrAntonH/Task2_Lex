#include <iostream>
#include <vector>
#include "string"
#include "fstream"
using namespace std;

struct table {
	char symbol;
	vector<int> z;
};
class Automat {
private:
	string Key;
	int priority;
	string classOf;
	vector<string> InIt; 
	vector<char> one; 
public:
	Automat(int prior, string c, string k) {
		priority=prior;
		classOf = c;
		Key = k;
	}
	void set_InIt(vector<string> p) {
		InIt = p;
	}
	void set_one(vector<char> p) {
		one = p;
	}
	string get_class() {
		return classOf;
	}
	int get_prior() {
		return priority;
	}
	vector<string> get_InIt() {
		return InIt;
	}
	vector<char> get_one() {
		return one;
	}
	string get_key() {
		return Key;
	}
};
pair<bool, int> maxString(Automat* M, string w, int p) {
	bool flag;
	flag = false;
	int r= 0, Imax=-1;
	int max=0;
	if (M->get_InIt().size() > 0) {	
		for (int i = 0; i < M->get_InIt().size(); i++) { 
			for (int j = p, k = 0; j < w.length() && k < M->get_InIt()[i].length(); j++, k++) {
				if (M->get_InIt()[i][k] == w[j] && Imax == k - 1) {
					max = j - p + 1;
					Imax++;
				}
				if (max > r && max== M->get_InIt()[i].length())
					r = max;
			}
		}
			if (r != 0)
				flag = true;
	}
		if (M->get_one().size() > 0) { 
			Imax = p-1;
			for (int j = p; j < w.length(); j++ ) { 
				for (int i = 0; i < M->get_one().size(); i++) {
					if (M->get_one()[i] == w[j] && Imax==j-1) {
						r=j-p+1;
						Imax = j;
					}
				}
			}
			if (r != 0) 
				flag = true;
		}
	pair<bool, int> res = make_pair(flag, r);
	return res;
}
void lex (vector<Automat*> M) { 
	string w;
	ifstream in("input.txt");
	ofstream out("output.txt");
	int kol = 0;
	while (in.peek() != EOF) {
		if (kol > 0) {
			out << "< " << M[0]->get_key() << ", \\n\\r\\t >" << endl;
		}
		getline(in, w);
		int k = 0;
		while (k < w.length()) {
			int r;
			int curLength = 0;
			int curPriority = 0;
			Automat* curAut = NULL;
			for (int i = 0; i < M.size(); i++) {
				pair<bool, int> Resr = maxString(M[i], w, k);
				if (Resr.first) {
					if ((curLength == Resr.second && curPriority < M[i]->get_prior()) || (Resr.second > curLength)) {
						curPriority = M[i]->get_prior();
						curLength = Resr.second;
						curAut = M[i];
					}
				}
			}
			if (curAut == NULL) {
				out<< "<Error " << w[k]<<" >" << endl;
				k++;
			}
			else if (curLength == 0) {
				out <<"< "<< curAut->get_class() <<" >"<< endl;
				out << "<Error " << w[k]<<" >";
				k++;
			}
			else {
				string str;
				str = w.substr(k, curLength);
				out << "< "<< curAut->get_key() << ", " << str<<" >" << endl;
				k += curLength;
			}
		}
		kol++;
		}
	out.close();
	in.close();
}
int main() {
	setlocale(LC_ALL, "Rus");
	string w;
	vector<Automat*> lst;
	vector<char> p;
	// WS
	p.erase(p.begin(), p.end());
	p.push_back(' ');
	Automat* Z = new Automat(5, "бесконечный", "WS");
	Z->set_one(p);
	lst.push_back(Z);
	// KW
	vector<string> prev;
	prev.push_back("begin");
	prev.push_back("end");
	prev.push_back("if");
	prev.push_back("while");
	prev.push_back("else");
	prev.push_back("break");
	Automat* M = new Automat( 10, "конечный", "KW");
	M->set_InIt(prev);
	lst.push_back(M);
	// ID
	p.erase(p.begin(), p.end());
	for (char i = 'A'; i < 'Z'; i++)
		p.push_back(i);
	for (char i = 'a'; i < 'z'; i++)
		p.push_back(i);
	Automat* N = new Automat(  5, "бесконечный", "ID");
	N->set_one(p);
	lst.push_back(N);
	// NUM
	vector<char> it;
	for (char i = '0'; i <= '9'; i++)
		it.push_back(i);
	Automat* A= new Automat(5, "бесконечный", "NUM");
	A->set_one(it);
	lst.push_back(A);
	// COL
	p.erase(p.begin(), p.end());
	p.push_back(';');
	Automat* B = new Automat( 5, "одноэлементный", "COL");
	B->set_one(p);
	lst.push_back(B);
	// LPAR
	p.erase(p.begin(), p.end());
	p.push_back('(');
	Automat* C = new Automat( 5, "одноэлементный", "LPAR");
	C->set_one(p);
	lst.push_back(C);
	// RPAR
	p.erase(p.begin(), p.end());
	p.push_back(')');
	Automat* D = new Automat( 5, "одноэлементный", "RPAR");
	D->set_one(p);
	lst.push_back(D);
	// OP
	p.erase(p.begin(), p.end());
	p.push_back('+');
	p.push_back('-');
	p.push_back('/');
	p.push_back('*');
	p.push_back('^');
	p.push_back('!');
	p.push_back('<');
	p.push_back('>');
	p.push_back('>=');
	p.push_back('<=');
	Automat* E = new Automat( 5, "конечный", "OP");
	E->set_one(p);
	lst.push_back(E);
	// LBR
	p.erase(p.begin(), p.end());
	p.push_back('{');
	Automat* G = new Automat( 5, "одноэлементный", "LBR");
	G->set_one(p);
	lst.push_back(G);
	// RBR
	p.erase(p.begin(), p.end());
	p.push_back('}');
	Automat* H = new Automat( 5, "одноэлементный", "RBR");
	H->set_one(p);
	lst.push_back(H);
	// AS
	p.erase(p.begin(), p.end());
	p.push_back('=');
	Automat* L = new Automat( 5, "одноэлементный", "AS");
	L->set_one(p);
	lst.push_back(L);
	lex(lst);
	system("pause");
	return 0;
}
