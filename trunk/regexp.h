#ifndef _REGEXP_H
#define _REGEXP_H

#include "nfa.h"
#include "dfa.h"

//���ַ���text�ƥ��������ʽregexp
//����TRUE,��ʾƥ��ɹ�������ʧ�ܡ�
bool reg_exp_match(char *text,const char *regexp);

//��NFAת��ΪDFA
DFA *nfa_to_dfa(NFA *);

//�󼯺ϵıհ�
list<NFA *> set_e_cloure(list<NFA *>);

//�����ַ������Ƿ�����DFAƥ����ַ���
bool match(char *,DFA *);

//������ʽ��BNF���������BNF����NFA
// r -> r '|' s | r
// s -> s t | s 
// t -> a '*' | a
// a -> token | '(' r ')' | ��

//��������ʽת��ΪNFA 
NFA *regexp_to_nfa(const char *&regexp);

//r -> r '|' s | r
NFA *regexp_to_nfa_or(const char *&regexp);

// s -> s t | s 
NFA *regexp_to_nfa_and(const char * &regexp);

// t -> a '*' | a
NFA *regexp_to_nfa_star(const char * &regexp);

// a -> token | '(' r ')' | ��
NFA *regexp_to_nfa_atom(const char * &regexp);

//������ǲ��ù��ɵ�˼�빹��NFA��������Բο���Compilers Principles,Techniques & Tools��3.7

//basic(����ŵ�NFA)
NFA *construct_nfa_e();

//basic(���쵥���ַ���NFA)
NFA *construct_nfa_from_token(char token);

//case r = s | t
NFA *construct_nfa_or(NFA *s,NFA *t);

//case r = s t
NFA *construct_nfa_and(NFA *s,NFA *t);

//case r = s*
NFA *construct_nfa_star(NFA *s);

//case r = (s)
NFA *construct_nfa_parenthesis(NFA *s);

bool is_token(char);

//�ͷ����е�NFA���
int free_all_nfa(NFA *head);

//�ͷ����е�NFA���
int free_all_dfa(DFA *head);

#endif
