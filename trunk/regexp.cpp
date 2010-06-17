#include <iostream>
#include <ctype.h>
#include <map>
#include <algorithm>

#include "regexp.h"
#include "nfa.h"
#include "dfa.h"

using namespace std;

static char gv_count = '1';  //生成的gv_count有限.

//在字符串text里，匹配正则表达式regexp
//返回TRUE,表示匹配成功，否则失败。
bool reg_exp_match(char *text,const char *regexp){
	const char *_regexp = regexp;
	NFA *temp_nfa = regexp_to_nfa( _regexp );
	string nfa_filename( &gv_count );
	nfa_filename.append("_nfa.gv");
	temp_nfa->print();
	temp_nfa->draw_dot(nfa_filename.c_str());

	DFA *temp_dfa = nfa_to_dfa( temp_nfa );
	string dfa_filename( &gv_count );
	dfa_filename.append("_dfa.gv");
	temp_dfa->print();
	temp_dfa->draw_dot(dfa_filename.c_str());

	bool ret = match( text, temp_dfa );

	free_all_nfa( temp_nfa );
	free_all_dfa( temp_dfa );

	gv_count ++;
	if( gv_count > '9')
		gv_count = '1';
	DFA::counter = 1;
	NFA::counter = 1;

	return ret; 
}

DFA *nfa_to_dfa(NFA *nfa){
	list<NFA *> init_closure = nfa->get_head()->e_closure();
	DFA *dfa_start = new DFA();
	list<DFA *>dfa_list;
	map<DFA *,bool>dfa_mark;
	map<list<NFA *>,DFA *> nfa_list_to_dfa;

	dfa_start->set_state( DFA::counter ++,init_closure );
	dfa_start->set_head( dfa_start );
	dfa_start->set_tail_state( nfa->get_tail()->get_state() );

	dfa_list.push_back( dfa_start );

	while( !dfa_list.empty() ){
		DFA *dfa = dfa_list.front();
		dfa_list.pop_front();
		dfa_mark[dfa] = true;
		
		map<char,bool> token = dfa->get_token();
		for(map<char,bool>::iterator it = token.begin(); it != token.end() ; it ++){
			list<NFA *> nfa_list = set_e_cloure( dfa->move(it->first) );
			if( nfa_list_to_dfa.count( nfa_list ) ){
				dfa->set_dfa_trans(it->first,nfa_list_to_dfa[nfa_list]);
			}else{
				DFA *s = new DFA();
				s->set_state( DFA::counter ++,nfa_list );
				dfa->set_dfa_trans( it->first,s );
				dfa_list.push_back( s );
				nfa_list_to_dfa[nfa_list] = s;
			}
		}
	}
	
	return dfa_start;
}

list<NFA *> set_e_cloure(list<NFA *> s){
		list<NFA *> t,stack;
		t.assign(s.begin(),s.end());
		stack.assign(s.begin(),s.end());
		
		while( !stack.empty() ){
			NFA *temp_nfa = stack.front();
			stack.pop_front();
			list<NFA *> nfa_list = temp_nfa->e_closure();
			for(list<NFA *>::iterator it = nfa_list.begin(); it != nfa_list.end() ; it ++){
				if( find(t.begin(),t.end(),*it ) == t.end() ){
					t.push_back( *it );
					stack.push_back( *it );
				}
			}
		}

		return t;
}

bool match(char *text,DFA *dfa){
	DFA *next = dfa->get_head();
	char *s = text;

	while( *s ){
		next = next->transfer( *s );
		if( next ){
			s ++;
			if( next->is_tail() )
				return true;
		}
		else {
			next = dfa->get_head();
			s = ++text;
		}
	}

	return false;	
}

// r -> r '|' s | r
// s -> s t | s 
// t -> a '*' | a
// a -> token | '(' r ')' | ε
NFA *regexp_to_nfa(const char *&regexp){
	return regexp_to_nfa_or( regexp );
}

//r -> r '|' s | r
NFA *regexp_to_nfa_or(const char *&regexp){
	NFA *r;  
	
	NFA *s;
       
	r = regexp_to_nfa_and( regexp );
	
	while( *regexp == '|' ){
		regexp ++;
		s = regexp_to_nfa_and( regexp );
		r = construct_nfa_or( r,s );
	}

	return r;
}

// s -> s t | s 
NFA *regexp_to_nfa_and(const char * &regexp){
	NFA *s,*t;

	s = regexp_to_nfa_star( regexp );

	while( (t = regexp_to_nfa_star(regexp)) ){
		s = construct_nfa_and( s,t );
	}

	return s;
}

// t -> a '*' | a
NFA *regexp_to_nfa_star(const char * &regexp){
	NFA *t,*a;

	a = regexp_to_nfa_atom(regexp);
	t = a;
	
	if( *regexp == '*' ){
		regexp ++;
		t = construct_nfa_star( a );
	}

	return t;
}

// a -> token | '(' r ')' | NULL
NFA *regexp_to_nfa_atom(const char * &regexp){
	NFA *a = NULL;

	if( is_token( *regexp ) ){
		a = construct_nfa_from_token( *regexp++ );
	}else if( *regexp == '(' ){
		regexp ++;
		a = regexp_to_nfa( regexp );
		if( *regexp++ != ')' ){
			cout << "error in regexp_to_nfa_atom()" << endl;
		}
	}

	return a;
}

//construct the nfa.
NFA *construct_nfa_e(){
	NFA *start = new NFA();
	NFA *end = new NFA();
	
	if( !(start && end)){
		cout << "out of memory" << endl;
	}

	start->set_state( NFA::counter ++ );
	end->set_state( NFA::counter ++ );
	start->set_trans( NFA::ZERO,end );

	start->set_head( start );
	start->set_tail( end );

	return start;	
}

//basic
NFA *construct_nfa_from_token(char token){
	NFA *start = new NFA();
	NFA *end = new NFA();
	
	if( !(start && end)){
		cout << "out of memory" << endl;
	}

	start->set_state( NFA::counter ++ );
	end->set_state( NFA::counter ++ );
	start->set_trans( token,end );

	start->set_head( start );
	start->set_tail( end );

	return start;
}

//case r = s | t
NFA *construct_nfa_or(NFA *s,NFA *t){
	NFA *start = new NFA();
	NFA *end = new NFA();

	if( !(start && end)){
		cout << "out of memory" << endl;
	}

	start->set_state( NFA::counter ++ );
	end->set_state( NFA::counter ++ );

	start->set_trans( NFA::ZERO,s->get_head() );
	start->set_trans( NFA::ZERO,t->get_head() );

	(s->get_tail())->set_trans( NFA::ZERO,end );
	(t->get_tail())->set_trans( NFA::ZERO,end );

	start->set_head( start );
	start->set_tail( end );

	return start;
}

//case r = s t
NFA *construct_nfa_and(NFA *s,NFA *t){
	
	(s->get_tail())->cat( t->get_head() );

	delete( t->get_head() );  //释放t的头结点

	s->set_tail( t->get_tail() );

	return s;
}

//case r = s*
NFA *construct_nfa_star(NFA *s){
	NFA *start = new NFA();
	NFA *end = new NFA();

	if( !(start && end)){
		cout << "out of memory" << endl;
	}

	start->set_state( NFA::counter ++ );
	end->set_state( NFA::counter ++ );

	start->set_trans( NFA::ZERO,s->get_head() );
	start->set_trans( NFA::ZERO,end );

	(s->get_tail())->set_trans( NFA::ZERO,s->get_head() );
	(s->get_tail())->set_trans( NFA::ZERO,end );

	start->set_head( start );
	start->set_tail( end );

	return start;
}

//case r = (s)
NFA *construct_nfa_parenthesis(NFA *s){

	return s;
}

bool is_token(char c){
	if( isalpha(c) )
		return true;
	else 
		return false;
}

int free_all_nfa(NFA *head){
	//释放所有NFA结点
	list<NFA *>t;
	map<NFA_STATUS,bool> nfa_free;
	list<NFA *>nfa_to_be_free;
	t.push_back( head );

	while( !t.empty() ){
		NFA *top = t.front();
		t.pop_front();
		nfa_free[top->get_state()] = true;

		multimap<char,NFA *> trans = top->get_trans();
		for( multimap<char,NFA *>::iterator it = trans.begin(); it != trans.end(); it ++){
			if( nfa_free.count(it->second->get_state()) ){
				continue;
			}
			else{
				nfa_free[it->second->get_state()] = true;
				t.push_back( it->second );
			}
		}
		nfa_to_be_free.push_back( top );
	}
	for( list<NFA *>::iterator it = nfa_to_be_free.begin(); it != nfa_to_be_free.end(); it ++ )
		delete( *it );

	return 0;
}

int free_all_dfa(DFA *head){
	list<DFA *>t;
	map<DFA_STATUS,bool> dfa_free;
	list<DFA *>dfa_to_be_free;
	t.push_back( head );

	while( !t.empty() ){
		DFA *top = t.front();
		t.pop_front();
		dfa_free[top->get_state()] = true;
		
		map<char,DFA *> trans = top->get_dfa_trans();
		for( map<char,DFA *>::iterator it = trans.begin(); it != trans.end(); it ++){
			if( dfa_free.count(it->second->get_state()) )
				continue;
			else{
				dfa_free[it->second->get_state()] = true;
				t.push_back( it->second );
			}
		}
		dfa_to_be_free.push_back( top );
	}

	for( list<DFA *>::iterator it = dfa_to_be_free.begin(); it != dfa_to_be_free.end(); it ++ )
		delete( *it );

	return 0;
}
