#ifndef _REGEXP_H
#define _REGEXP_H

#include "nfa.h"
#include "dfa.h"

//在字符串text里，匹配正则表达式regexp
//返回TRUE,表示匹配成功，否则失败。
bool reg_exp_match(char *text,const char *regexp);

//将NFA转化为DFA
DFA *nfa_to_dfa(NFA *);

//求集合的闭包
list<NFA *> set_e_cloure(list<NFA *>);

//查找字符串中是否有与DFA匹配的字符串
bool match(char *,DFA *);

//正则表达式的BNF，按照这个BNF构造NFA
// r -> r '|' s | r
// s -> s t | s 
// t -> a '*' | a
// a -> token | '(' r ')' | ε

//将正则表达式转换为NFA 
NFA *regexp_to_nfa(const char *&regexp);

//r -> r '|' s | r
NFA *regexp_to_nfa_or(const char *&regexp);

// s -> s t | s 
NFA *regexp_to_nfa_and(const char * &regexp);

// t -> a '*' | a
NFA *regexp_to_nfa_star(const char * &regexp);

// a -> token | '(' r ')' | ε
NFA *regexp_to_nfa_atom(const char * &regexp);

//下面的是采用归纳的思想构造NFA，具体可以参考《Compilers Principles,Techniques & Tools》3.7

//basic(构造ε的NFA)
NFA *construct_nfa_e();

//basic(构造单个字符的NFA)
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

//释放所有的NFA结点
int free_all_nfa(NFA *head);

//释放所有的NFA结点
int free_all_dfa(DFA *head);

#endif
