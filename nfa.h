#ifndef _NFA_H
#define _NFA_H

#include <map>
#include <list>

#define NFA_COUNTER int 
#define NFA_STATUS NFA_COUNTER 

using namespace std;

class NFA{
	NFA_STATUS  state;      //NFA结点的序号值
	multimap< char,NFA * >trans; // 指示该NFA遇到某个字符后该转移到哪个NFA结点
	NFA *head,*tail;       //每个NFA有且只有一个头结点和一个尾结点

public:
	static NFA_COUNTER counter;  // 上面state的全局记数器
	const static NFA_STATUS ZERO = 0;  //代表ε（即空转移）

	int set_state(NFA_STATUS);  //设置state的值
	NFA_STATUS get_state();     //返回state的值
	int set_head(NFA *);        //设置头结点head
	int set_tail(NFA *);		//设置尾结点tail
	NFA *get_head();            //返回头结点head
	NFA *get_tail();            //返回尾结点tail 
	int set_trans(char,NFA *);  //添加一个转移表项
	multimap< char,NFA *> get_trans(); //返回转移表trans
	int cat(NFA *);       //将对应NFA（即传入的参数）的转移表(trans) 添加到本NFA的trans中
	list<NFA *> e_closure();  //求出该NFA的ε闭包
	int draw_dot(const char *filename);
	void print();  //打印该NFA
	
	~NFA(){};
};

#endif
