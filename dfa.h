#ifndef _DFA_H
#define _DFA_H

#include <map>
#include <list>
#include "nfa.h"

#define DFA_COUNTER int 
#define DFA_STATUS DFA_COUNTER

class DFA{
	DFA_STATUS  state;           //DFA结点的序号值
	list<NFA_STATUS> nfa_state;  //该DFA对应的NFA（NFA的序号值）
	map<char,DFA *>dfa_trans;    //DFA的转移表
	multimap<char,NFA *>nfa_trans; //NFA的转移表(此表综合了该DFA对应的所有NFA的转移表)
	map<char,bool> token;        //标记nfa_trans表中是否有与某个字符对应的转移项
	bool tail_flag; //一个DFA可能有多个终结点，所以设置一个标志，指示该DFA是否为尾结点。
	static list<DFA *> tail_dfa; //记录所有的DFA终结点
	
public:
	static DFA *head; //一个DFA只可能有一个起始点。
	static NFA_STATUS tail_state; //NFA尾结点的序号值
	static DFA_COUNTER counter; // 上面state的全局记数器

	int set_state(DFA_STATUS,list<NFA *>);  //设置state和nfa_trans的值
	DFA_STATUS get_state();     //返回state的值
	static int set_head(DFA *);  //设置DFA头结点的值
	static int set_tail_state(NFA_STATUS);  //设置tail_state的值
	static DFA *get_head();     //返回DFA头结点的值
	bool is_tail();          //是否为尾结点
	map<char,bool> get_token();  //返回token
	multimap<char,NFA *> get_nfa_trans(); //返回nfa_trans
	map<char,DFA *> get_dfa_trans(); //返回dfa_trans
	int set_dfa_trans(char,DFA *);  //添加一个dfa_trans表项
	list<NFA *> move(char);   //遇到某个字符时会转移到的NFA结合
	DFA *transfer(char);  //遇到某个字符转移到另一个DFA结点，若无转移项则返回空(NULL)
	void print();    //打印该DFA
	int draw_dot(const char *filename); //将该DFA以.gv文件格式存起来
	
	DFA();
	~DFA(){};
};

#endif
