#include <iostream>
#include <string>
#include "regexp.h"

using namespace std;

int main()
{
	string str;

	//while(1){
	//	cin >> str;
	bool ret;	
		 ret = reg_exp_match( "aaabbctd","(a|b)*abb" );

		if( ret )
			cout << "matched" << endl;
		else
			cout << "unmatched" << endl;

		ret = reg_exp_match( "aaabbctd","a*c" );

		if( ret )
			cout << "matched" << endl;
		else
			cout << "unmatched" << endl;
	//}

	//system("pause");
	return 0;
}
