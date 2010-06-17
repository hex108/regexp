#ifndef _DFA_H
#define _DFA_H

#include <map>
#include <list>
#include "nfa.h"

#define DFA_COUNTER int 
#define DFA_STATUS DFA_COUNTER

class DFA{
	DFA_STATUS  state;           //DFA�������ֵ
	list<NFA_STATUS> nfa_state;  //��DFA��Ӧ��NFA��NFA�����ֵ��
	map<char,DFA *>dfa_trans;    //DFA��ת�Ʊ�
	multimap<char,NFA *>nfa_trans; //NFA��ת�Ʊ�(�˱��ۺ��˸�DFA��Ӧ������NFA��ת�Ʊ�)
	map<char,bool> token;        //���nfa_trans�����Ƿ�����ĳ���ַ���Ӧ��ת����
	bool tail_flag; //һ��DFA�����ж���ս�㣬��������һ����־��ָʾ��DFA�Ƿ�Ϊβ��㡣
	static list<DFA *> tail_dfa; //��¼���е�DFA�ս��
	
public:
	static DFA *head; //һ��DFAֻ������һ����ʼ�㡣
	static NFA_STATUS tail_state; //NFAβ�������ֵ
	static DFA_COUNTER counter; // ����state��ȫ�ּ�����

	int set_state(DFA_STATUS,list<NFA *>);  //����state��nfa_trans��ֵ
	DFA_STATUS get_state();     //����state��ֵ
	static int set_head(DFA *);  //����DFAͷ����ֵ
	static int set_tail_state(NFA_STATUS);  //����tail_state��ֵ
	static DFA *get_head();     //����DFAͷ����ֵ
	bool is_tail();          //�Ƿ�Ϊβ���
	map<char,bool> get_token();  //����token
	multimap<char,NFA *> get_nfa_trans(); //����nfa_trans
	map<char,DFA *> get_dfa_trans(); //����dfa_trans
	int set_dfa_trans(char,DFA *);  //���һ��dfa_trans����
	list<NFA *> move(char);   //����ĳ���ַ�ʱ��ת�Ƶ���NFA���
	DFA *transfer(char);  //����ĳ���ַ�ת�Ƶ���һ��DFA��㣬����ת�����򷵻ؿ�(NULL)
	void print();    //��ӡ��DFA
	int draw_dot(const char *filename); //����DFA��.gv�ļ���ʽ������
	
	DFA();
	~DFA(){};
};

#endif
