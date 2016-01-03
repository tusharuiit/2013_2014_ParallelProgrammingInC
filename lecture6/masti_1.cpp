
#include <iostream>
using namespace std ;
char *colored_digit[] =
{
	"\e[1;30;40m0",
	"\e[1;31;40m1",
	"\e[1;32;40m2",
	"\e[1;33;40m3",
	"\e[1;34;40m4",
	"\e[1;35;40m5",
	"\e[1;36;40m6",
	"\e[1;37;40m7"
};

int main(int argc, char** argv)
{
	char *colored_digit[] =
{
	"\e[1;30;40m0",
	"\e[1;31;40m1",
	"\e[1;32;40m2",
	"\e[1;33;40m3",
	"\e[1;34;40m4",
	"\e[1;35;40m5",
	"\e[1;36;40m6",
	"\e[1;37;40m7"
};
	cout << colored_digit[2] << endl ; 
	return 0;
}
