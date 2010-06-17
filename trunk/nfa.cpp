#include <iostream>
#include <fstream>
#include <list>
#include "nfa.h"

using namespace std;

NFA_COUNTER NFA::counter = 1;

int NFA::set_state(NFA_STATUS counter){
	state = counter;

	return 0;
}

NFA_STATUS NFA::get_state(){
	return state;
}

int NFA::set_head(NFA *s){
	head = s;

	return 0;
}

int NFA::set_tail(NFA *s){
	tail = s;

	return 0;
}

NFA *NFA::get_head(){
	return head;
}

NFA *NFA::get_tail(){
	return tail;
}

int NFA::set_trans(char c,NFA *s){
	trans.insert( make_pair( c,s ) );

	return 0;
}

multimap<char,NFA *> NFA::get_trans(){
	return trans;
}

int NFA::cat(NFA *s){

	for( multimap<char,NFA *>::iterator it = (s->trans).begin(); it != (s->trans).end(); it ++){
		trans.insert( make_pair( it->first,it->second ) );
	}

	return 0;
}

list<NFA *> NFA::e_closure(){
	list<NFA *> closure,stack;
	map<NFA *,bool>nfa_mark;  //标记对应的NFA是否已求过闭包集,true表示已求过。

	closure.push_back( this );
	nfa_mark[this] = true;

	for( multimap< char,NFA * >::iterator it = trans.lower_bound(NFA::ZERO); 
		it != trans.upper_bound(NFA::ZERO) ; it ++ ){
			closure.push_back( it->second );
	}
	stack.assign( closure.begin(),closure.end() );
	while( !stack.empty() ){
		NFA *temp = stack.front();
		stack.pop_front();
		if( nfa_mark.count(temp) )
			continue;
		else{
			nfa_mark[temp] = true;
			list<NFA *> temp_closure = temp->e_closure();
			closure.insert( closure.end(),temp_closure.begin(),temp_closure.end() );
			stack.insert( stack.end(),temp_closure.begin(),temp_closure.end() );
		}
	}

	//删掉重复的
	closure.sort();
	closure.unique();

	return closure;
}

int NFA::draw_dot(const char *filename){
	ofstream outfile(filename);

	if( !outfile ){
		cout << "error : unable to open outputfile " << filename << endl;
		return -1;
	}

	outfile << "digraph finite_state_machine {" << endl;
	//outfile << "	charset=latin1;" << endl;
	outfile << "	rankdir=LR;" << endl;
	outfile << "	size=\"8,5\"" << endl;
	outfile << "	node [shape = doublecircle];" << tail->get_state() << ";" << endl;
	outfile << "	node [shape = circle];" << endl;
	
	//结点间的关系
	list<NFA *>t;
	map<NFA *,bool> nfa_draw;
	t.push_back( this );

	while( !t.empty() ){
		NFA *top = t.front();
		t.pop_front();
		nfa_draw[top] = true;

		multimap<char,NFA *> trans = top->get_trans();
		for( multimap<char,NFA *>::iterator it = trans.begin(); it != trans.end(); it ++){
			outfile << "	" << top->get_state() << " -> ";
			outfile << it->second->get_state() << " ";
			outfile << "[ label = ";
			if( it->first )
				outfile << "\"" << it->first << "\" ];" << endl;
			else //为空 ε //为了在gv文件中以utf8格式写进ε字符，我把此文件改为了utf8格式,还是比较有效的,
				       //vs中以unicode格式编译可能就是这个原理吧
				outfile << "\"" << "ε" << "\" ];" << endl;
			
			if( nfa_draw.count(it->second) )
				continue;
			else{
				nfa_draw[it->second] = true;
				t.push_back( it->second );
			}
		}
	}
	outfile << "}" << endl;

	return 0;
}

void NFA::print(){
	cout << "======== NFA output =====" << endl;

	list<NFA *>t;
	map<NFA *,bool> nfa_print;
	t.push_back( this );

	while( !t.empty() ){
		NFA *top = t.front();
		t.pop_front();
		nfa_print[top] = true;

		cout << "status : " << top->get_state();
		if( top == head )
			cout << " ( Start state. )" << endl;
		else if( top == tail )
			cout << " ( End state. )" << endl;
		else
			cout << endl;

		multimap<char,NFA *> trans = top->get_trans();
		for( multimap<char,NFA *>::iterator it = trans.begin(); it != trans.end(); it ++){
			if( it->first )
				cout << it->first << "  ";
			else //为空(ε) 
				cout << "ε" << "  ";
			cout << it->second->get_state() << endl;
			if( nfa_print.count(it->second) )
				continue;
			else{
				nfa_print[it->second] = true;
				t.push_back( it->second );
			}
		}
		cout << endl;
	}

	cout << "============ end ========" << endl << endl;
}

