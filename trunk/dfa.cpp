#include <iostream>
#include <fstream>
#include <list>
#include <map>
#include "dfa.h"

using namespace std;

list<DFA *> DFA::tail_dfa;
DFA_COUNTER DFA::counter = 1;
DFA *DFA::head = NULL;
NFA_STATUS DFA::tail_state = 0;

int DFA::set_state(DFA_STATUS counter,list<NFA *> s){
	state = counter;
	
	for( list<NFA *>::iterator it = s.begin(); it != s.end() ; it ++){
		multimap< char,NFA * > temp_trans = (*it)->get_trans();
		
		if( (*it)->get_state() == tail_state ){
			tail_flag = true;
			tail_dfa.push_back( this );
		}

		for(multimap< char,NFA * >::iterator it2 = temp_trans.begin(); 
			it2 != temp_trans.end() ; it2 ++){
				if( it2->first ){
					nfa_trans.insert( make_pair(it2->first,it2->second) );	
					if( !token.count(it2->first) )
						token[it2->first] = true;
				}
		}
	}

	return 0;
}

DFA_STATUS DFA::get_state(){
	return state;
}

int DFA::set_head(DFA *s){
	head = s;

	return 0;
}

int DFA::set_tail_state(NFA_STATUS tail){
	tail_state = tail;

	return 0;
}

DFA *DFA::get_head(){
	return head;
}

bool DFA::is_tail(){
	return tail_flag;
}

map<char,bool> DFA::get_token(){
	return token;
}

multimap<char,NFA *> DFA::get_nfa_trans(){
	return nfa_trans;
}

map<char,DFA *> DFA::get_dfa_trans(){
	return dfa_trans;
}

int DFA::set_dfa_trans(char c,DFA *s){
	dfa_trans[c] = s;

	return 0;
}

list<NFA *> DFA::move(char c){
	list<NFA *> nfa_list;

	for(multimap<char,NFA *>::iterator it = nfa_trans.lower_bound(c); 
		it != nfa_trans.upper_bound(c) ; it ++){
			nfa_list.push_back( it->second );
	}

	return nfa_list;
}

DFA *DFA::transfer(char c){
	if( dfa_trans.count(c) )
		return dfa_trans[c];
	else
		return NULL;
}

void DFA::print(){
	cout << "======== DFA output =====" << endl;

	list<DFA *>t;
	map<DFA *,bool> dfa_print;
	t.push_back( this );

	while( !t.empty() ){
		DFA *top = t.front();
		t.pop_front();
		dfa_print[top] = true;

		cout << "status : " << top->get_state();
		if( top == head )
			cout << " ( Start state. )" << endl;
		else if( top->is_tail() )
			cout << " ( End state. )" << endl;
		else
			cout << endl;
		map<char,DFA *> trans = top->get_dfa_trans();
		for( map<char,DFA *>::iterator it = trans.begin(); it != trans.end(); it ++){
			cout << it->first << "  ";
			cout << it->second->get_state() << endl;
			if( dfa_print.count(it->second) )
				continue;
			else{
				dfa_print[it->second] = true;
				t.push_back( it->second );
			}
		}
		cout << endl;
	}

	cout << "============ end ========" << endl << endl;
}

int DFA::draw_dot(const char *filename){
	ofstream outfile(filename);

	if( !outfile ){
		cout << "error : unable to open outputfile " << filename << endl;
		return -1;
	}

	outfile << "digraph finite_state_machine {" << endl;
	//outfile << "	charset=latin1;" << endl;
	outfile << "	rankdir=LR;" << endl;
	outfile << "	size=\"8,5\"" << endl;
	outfile << "	node [shape = doublecircle];";
	for( list<DFA *>::iterator it = tail_dfa.begin(); it != tail_dfa.end() ; it ++)
		outfile << (*it)->get_state() << ";";
	outfile << endl;
	outfile << "	node [shape = circle];" << endl;
	
	//结点间的关系
	list<DFA *>t;
	map<DFA *,bool> dfa_draw;
	t.push_back( this );

	while( !t.empty() ){
		DFA *top = t.front();
		t.pop_front();
		dfa_draw[top] = true;

		map<char,DFA *> trans = top->get_dfa_trans();
		for( map<char,DFA *>::iterator it = trans.begin(); it != trans.end(); it ++){
			outfile << "	" << top->get_state() << " -> ";
			outfile << it->second->get_state() << " ";
			outfile << "[ label = ";
			if( it->first )
				outfile << "\"" << it->first << "\" ];" << endl;
			
			if( dfa_draw.count(it->second) )
				continue;
			else{
				dfa_draw[it->second] = true;
				t.push_back( it->second );
			}
		}
	}
	outfile << "}" << endl;

	return 0;
}

DFA::DFA(){
	tail_flag = false;
}
