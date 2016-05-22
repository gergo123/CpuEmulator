#include <tuple>
#include <iostream> //string
#include <stdio.h>

#define Flag 2
#define Stack 1

using namespace std;

//switch - case miatt kell az enum
enum opcodes {
	RST, FLL, REV, CPY, MOV, RTR, RTL, SWP, INC, DEC, ADD, SUB, INV, ABS, NOT, SAN, SOR, _NAN, NOR, XOR, REP, NEX, NOP, CMP, END, NULLA
};

class memori_part
{
private:
	opcodes mnemonik;

	//TODO:stringbõl char * >.>
	string hex;

	//paraméterek száma
	int paramnum; 
public:
	void setMnemonik(opcodes a1)
	{
		mnemonik = a1;
	};
	void setHex(string a1)
	{
		hex = a1;
	};
	void setParamnum(int a1)
	{
		paramnum = a1;
	};
	opcodes getMnemonik()
	{
		return mnemonik;
	};
	string getHex()
	{
		return hex;
	};
	int getParamnum()
	{
		return paramnum;
	}
};

class machine
{
public:
	//0x0000 - PC(IP)
	//0x0001 - Stack
	//0x0002 - OZSXCLMX
	memori_part mem[65536];
	bool running;
	void setPC(string address)
	{
		mem[0].setHex(address);
	};
};