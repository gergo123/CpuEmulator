//http://stackoverflow.com/a/3382894/2577795
//http://www.cplusplus.com/forum/general/63932/#msg345899 bit mûveletek  http://www.lix.polytechnique.fr/~liberti/public/computing/prog/c/C/CONCEPT/bitwise.html
//http://www.cplusplus.com/reference/cstdlib/strtol/ convert to dec
#include <string>
#include <stdlib.h>

int convertToInt(char * hexnumber, int from)
{
	return strtol(hexnumber, NULL, from);
}

string convertIntToHex(int number)
{
	char* buf = new char();
	sprintf(buf,"%X", number);
	string ret = buf;
	return ret;
}

//http://stackoverflow.com/a/2548374/2577795
string convertDecToBin(int number)
{
    if ( number == 0 ) return "0";
    if ( number == 1 ) return "1";

    if ( number % 2 == 0 )
        return convertDecToBin(number / 2) + "0";
    else
        return convertDecToBin(number / 2) + "1";
}