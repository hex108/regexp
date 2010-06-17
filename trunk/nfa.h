#ifndef _NFA_H
#define _NFA_H

#include <map>
#include <list>

#define NFA_COUNTER int 
#define NFA_STATUS NFA_COUNTER 

using namespace std;

class NFA{
	NFA_STATUS  state;      //NFA�������ֵ
	multimap< char,NFA * >trans; // ָʾ��NFA����ĳ���ַ����ת�Ƶ��ĸ�NFA���
	NFA *head,*tail;       //ÿ��NFA����ֻ��һ��ͷ����һ��β���

public:
	static NFA_COUNTER counter;  // ����state��ȫ�ּ�����
	const static NFA_STATUS ZERO = 0;  //����ţ�����ת�ƣ�

	int set_state(NFA_STATUS);  //����state��ֵ
	NFA_STATUS get_state();     //����state��ֵ
	int set_head(NFA *);        //����ͷ���head
	int set_tail(NFA *);		//����β���tail
	NFA *get_head();            //����ͷ���head
	NFA *get_tail();            //����β���tail 
	int set_trans(char,NFA *);  //���һ��ת�Ʊ���
	multimap< char,NFA *> get_trans(); //����ת�Ʊ�trans
	int cat(NFA *);       //����ӦNFA��������Ĳ�������ת�Ʊ�(trans) ��ӵ���NFA��trans��
	list<NFA *> e_closure();  //�����NFA�Ħűհ�
	int draw_dot(const char *filename);
	void print();  //��ӡ��NFA
	
	~NFA(){};
};

#endif
