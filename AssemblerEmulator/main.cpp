#include <stdio.h>
#include <tuple>
#include <string>
#include <fstream>
#include <math.h>
#include "Machine.h"
#include "Converters.h"

using namespace std;

static void init(machine * machin, memori_part mnemo[])
{
	machin->running = false;
	for (int i = 0; i < 65536; i++)
	{
		//Alapból mindent nullára állítunk
		machin->mem[i].setHex("");
		machin->mem[i].setMnemonik(NULLA);
		machin->mem[i].setParamnum(0);
	}

	//Beolvasás
	ifstream be;
	be.open("D:\\Data\\input_assembleremulator.txt");
	if(be.fail())
	{
		cerr << "Hibas fajlnyitasi muvelet!" << endl;
		cin.get();
		exit(-1); // megszakítás
	}
	int i = 0;
	while(!be.eof())
	{
		//8bitenként
		char sor[3];
		be >> sor;
		//cout << sor << endl;

		//Input adatok berakása a memóriába
		machin->mem[i].setHex(sor);
		machin->mem[i].setMnemonik(NULLA);
		machin->mem[i].setParamnum(0);
		i++;
	}

	//Mnemonikok beazonosítása
	for (int i = 3; i < 65536; i++) //PC - Stack - Flag
	{
		for (int j = 0; j < 25; j++) //Mnemonik lista hosszáig
		{
			if(machin->mem[i].getHex().compare(mnemo[j].getHex()) == 0) //ha megegyeznek
			{
				machin->mem[i].setMnemonik(mnemo[j].getMnemonik());
				machin->mem[i].setParamnum(mnemo[j].getParamnum()); //!! lehet h nem fog kelleni
				//az adott utasítás paramétereit nem vizsgáljuk!!
				i += mnemo[j].getParamnum();
				break;
			}
		}
		//ha az utasítássorozat végére érünk nem vizsgájluk tovább a memóriát
		if(machin->mem[i].getMnemonik() == END) break;
	}
	//cin.get();
}

static void start(machine * machin, memori_part *mnemo)
{	
	//string -> char*
	string tmp = "";
	int tmp0 = 0;
	int tmp1 = 0;
	int tmp2 = 0;
	int tmp3 = 0;
	int tmp4 = 0;
	int tmpe = 0;
	char * chartmp1;
	int tmpindex1 = 0;
	int tmpindex2 = 0;
	machin->running=true;
	while(machin->running == true)
	{
		int PCindex = convertToInt((char*)machin->mem[0].getHex().c_str(), 16);
		int flag = convertToInt((char*)machin->mem[Flag].getHex().c_str(), 16);
		switch (machin->mem[PCindex].getMnemonik())
		{
		case RST:

			machin->setPC(convertIntToHex(PCindex + machin->mem[PCindex].getParamnum()+1));
			break;
		case FLL:

			machin->setPC(convertIntToHex(PCindex + machin->mem[PCindex].getParamnum()+1));
			break;
		case REV:

			machin->setPC(convertIntToHex(PCindex + machin->mem[PCindex].getParamnum()+1));
			break;
		case CPY:

			machin->setPC(convertIntToHex(PCindex + machin->mem[PCindex].getParamnum()+1));
			break;
		case MOV:

			machin->setPC(convertIntToHex(PCindex + machin->mem[PCindex].getParamnum()+1));
			break;
		case RTR:
			tmpindex1 = convertToInt((char*)machin->mem[PCindex+1].getHex().c_str(), 16);
			tmpindex2 = convertToInt((char*)machin->mem[PCindex+2].getHex().c_str(), 16);
			tmp1 = convertToInt((char*)machin->mem[tmpindex1].getHex().c_str(), 16);
			tmp2 = convertToInt((char*)machin->mem[tmpindex2].getHex().c_str(), 16);

			tmp = convertDecToBin(tmp1);
			tmpe = convertToInt((char*)tmp.c_str(), 2);

			for (int i = 0; i < tmp2; i++)
			{
				if(tmpe % 2 != 0) //páros -> 0, ha páratlan akkor 1-re végzõdik
				{
					tmpe >>= 1;
					tmpe += 128;
					machin->mem[Flag].setHex(convertIntToHex(flag |= 0x08)); //utoljára átvitt bit értékére vált a C flag
				} 
				else //a 8. helyen 0 van!
				{
					tmpe >>= 1; 
					machin->mem[Flag].setHex(convertIntToHex(flag &= 0x08));
				}
			}

			machin->setPC(convertIntToHex(PCindex + machin->mem[PCindex].getParamnum()+1));
			break;
		case RTL:
			tmpindex1 = convertToInt((char*)machin->mem[PCindex+1].getHex().c_str(), 16);
			tmpindex2 = convertToInt((char*)machin->mem[PCindex+2].getHex().c_str(), 16);
			tmp1 = convertToInt((char*)machin->mem[tmpindex1].getHex().c_str(), 16);
			tmp2 = convertToInt((char*)machin->mem[tmpindex2].getHex().c_str(), 16); //mennyivel
			
			tmp = convertDecToBin(tmp1);
			tmpe = convertToInt((char*)tmp.c_str(), 2);

			for (int i = 0; i < tmp2; i++)
			{
				if(tmpe >= 128) //akkor 8bites
				{
					tmpe -= 128; //eltûntetem az elsõ egyest
					tmpe <<= 1;
					tmpe++;
					machin->mem[Flag].setHex(convertIntToHex(flag |= 0x08)); //utoljára átvitt bit értékére vált a C flag
				} else //a 8. helyen 0 van!
				{
					tmpe <<= 1; 
					machin->mem[Flag].setHex(convertIntToHex(flag &= 0x08));
				}
			}

			machin->mem[tmpindex1].setHex(convertIntToHex(tmpe)); //visszaírjuk

			machin->setPC(convertIntToHex(PCindex + machin->mem[PCindex].getParamnum()+1));
			break;
		case SWP:

			machin->setPC(convertIntToHex(PCindex + machin->mem[PCindex].getParamnum()+1));
			break;
		case INC:

			machin->setPC(convertIntToHex(PCindex + machin->mem[PCindex].getParamnum()+1));
			break;
		case DEC:
			tmpindex1 = convertToInt((char*)machin->mem[PCindex+1].getHex().c_str(), 16); //1. argumentum hexa címbõl -> dec
			tmp1 = convertToInt((char*)machin->mem[tmpindex1].getHex().c_str(), 16);
			tmp1--;
			if(tmp1 > 255) //....
				machin->mem[Flag].setHex(convertIntToHex(flag |= 0x80)); // O -> 1
			else
				machin->mem[Flag].setHex(convertIntToHex(flag &= 0x80)); // O -> 0
			if(tmp1==0)
				machin->mem[Flag].setHex(convertIntToHex(flag |= 0x40)); // Z -> 1
			else
				machin->mem[Flag].setHex(convertIntToHex(flag &= 0x40)); // Z -> 0
			if(tmp1<0)
				machin->mem[Flag].setHex(convertIntToHex(flag |= 0x20)); // S -> 1
			else
				machin->mem[Flag].setHex(convertIntToHex(flag &= 0x20)); // S -> 0
			machin->mem[tmpindex1].setHex(convertIntToHex(tmp1)); //visszaírjuk
			machin->setPC(convertIntToHex(PCindex + machin->mem[PCindex].getParamnum()+1));
			break;
		case ADD:
			tmpindex1 = convertToInt((char*)machin->mem[PCindex+1].getHex().c_str(), 16);
			tmpindex2 = convertToInt((char*)machin->mem[PCindex+2].getHex().c_str(), 16);
			tmp1 = convertToInt((char*)machin->mem[tmpindex1].getHex().c_str(), 16);
			tmp2 = convertToInt((char*)machin->mem[tmpindex2].getHex().c_str(), 16);
			tmpe = tmp1 + tmp2;

			machin->mem[tmpindex1].setHex(convertIntToHex(tmpe)); //visszaírjuk

			if(tmpe > 255) //....
				machin->mem[Flag].setHex(convertIntToHex(flag |= 0x80)); // O -> 1
			else
				machin->mem[Flag].setHex(convertIntToHex(flag &= 0x80)); // O -> 0
			if(tmpe == 0)
				machin->mem[Flag].setHex(convertIntToHex(flag |= 0x40)); // Z -> 1
			else
				machin->mem[Flag].setHex(convertIntToHex(flag &= 0x40)); // Z -> 0
			if(tmpe<0)
				machin->mem[Flag].setHex(convertIntToHex(flag |= 0x20)); // S -> 1
			else
				machin->mem[Flag].setHex(convertIntToHex(flag &= 0x20)); // S -> 0

			machin->setPC(convertIntToHex(PCindex + machin->mem[PCindex].getParamnum()+1));
			break;
		case SUB:

			machin->setPC(convertIntToHex(PCindex + machin->mem[PCindex].getParamnum()+1));
			break;
		case INV:

			machin->setPC(convertIntToHex(PCindex + machin->mem[PCindex].getParamnum()+1));
			break;
		case ABS:

			machin->setPC(convertIntToHex(PCindex + machin->mem[PCindex].getParamnum()+1));
			break;
		case NOT:
			tmpindex1 = convertToInt((char*)machin->mem[PCindex+1].getHex().c_str(), 16);
			tmp1 = convertToInt((char*)machin->mem[tmpindex1].getHex().c_str(), 16);
			~tmp1;
			if(tmpe == 0)
				machin->mem[Flag].setHex(convertIntToHex(flag |= 0x40)); // Z -> 1
			else
				machin->mem[Flag].setHex(convertIntToHex(flag &= 0x40)); // Z -> 0
			if(tmpe<0)
				machin->mem[Flag].setHex(convertIntToHex(flag |= 0x20)); // S -> 1
			else
				machin->mem[Flag].setHex(convertIntToHex(flag &= 0x20)); // S -> 0

			machin->setPC(convertIntToHex(PCindex + machin->mem[PCindex].getParamnum()+1));
			break;
		case SAN:

			machin->setPC(convertIntToHex(PCindex + machin->mem[PCindex].getParamnum()+1));
			break;
		case SOR:

			machin->setPC(convertIntToHex(PCindex + machin->mem[PCindex].getParamnum()+1));
			break;
		case _NAN:
			tmpindex1 = convertToInt((char*)machin->mem[PCindex+1].getHex().c_str(), 16);
			tmpindex2 = convertToInt((char*)machin->mem[PCindex+2].getHex().c_str(), 16);
			tmp1 = convertToInt((char*)machin->mem[tmpindex1].getHex().c_str(), 16);
			tmp2 = convertToInt((char*)machin->mem[tmpindex2].getHex().c_str(), 16);
			tmpe = ~ (tmp1 & tmp2);
			machin->mem[tmpindex1].setHex(convertIntToHex(tmpe)); //visszaírjuk

			if(tmpe == 0)
				machin->mem[Flag].setHex(convertIntToHex(flag |= 0x40)); // Z -> 1
			else
				machin->mem[Flag].setHex(convertIntToHex(flag &= 0x40)); // Z -> 0
			if(tmpe < 0)
				machin->mem[Flag].setHex(convertIntToHex(flag |= 0x20)); // S -> 1
			else
				machin->mem[Flag].setHex(convertIntToHex(flag &= 0x20)); // S -> 0

			machin->setPC(convertIntToHex(PCindex + machin->mem[PCindex].getParamnum()+1));
			break;
		case NOR:

			machin->setPC(convertIntToHex(PCindex + machin->mem[PCindex].getParamnum()+1));
			break;
		case XOR:

			machin->setPC(convertIntToHex(PCindex + machin->mem[PCindex].getParamnum()+1));
			 break;
		case REP:
			tmpindex1 = convertToInt((char*)machin->mem[PCindex + 1].getHex().c_str(), 16);
			tmp1 = convertToInt((char*)machin->mem[tmpindex1].getHex().c_str(), 16);

			tmp3 = convertToInt((char*)machin->mem[Stack].getHex().c_str(),16);
			//A Rep utasítás argumentumát vizsgáljuk
			tmp = machin->mem[tmp3].getHex();
			tmp2 = convertToInt((char*)tmp.c_str(), 16);
			tmp2++; //paraméterként kapott érték + 1
			machin->mem[tmp1].setHex(convertIntToHex(tmp2));
			machin->mem[Stack].setHex(convertIntToHex(PCindex + 1));

			machin->setPC(convertIntToHex(PCindex + machin->mem[PCindex].getParamnum()+1));
			break;
		case NEX:
			tmp1 = convertToInt((char*)machin->mem[Stack].getHex().c_str(),16);
			//A Rep utasítás argumentumát vizsgáljuk
			tmp = machin->mem[tmp1].getHex();
			tmp2 = convertToInt((char*)tmp.c_str(), 16);
			if(strcmp((char*)tmp.c_str(),"FFFFFFFF") != 0) //0 alá csökken a paraméter értéke
			{
				/*Visszatérünk a rep utasításhoz - a default ág fog egyszer lefutni mivel csak a REP
				utasítás paraméteréhez térünk vissza ami önmagába nem utasítás, csak adat*/
				machin->setPC(machin->mem[Stack].getHex());
				if(tmp2 != 0) //utolsó alkalommal futott le 
					tmp2--;
				else //tovább léptetjük a PC-t
					machin->setPC(convertIntToHex(PCindex + machin->mem[PCindex].getParamnum() + 1));
				machin->mem[tmp1].setHex(convertIntToHex(tmp2));
			} else
			{
				machin->setPC(convertIntToHex(PCindex + machin->mem[PCindex].getParamnum() + 1));
			}
			break;
		case NOP:

			machin->setPC(convertIntToHex(PCindex + machin->mem[PCindex].getParamnum()+1));
			break;
		case CMP: //op1-op2
			tmpindex1 = convertToInt((char*)machin->mem[PCindex+1].getHex().c_str(), 16);
			tmpindex2 = convertToInt((char*)machin->mem[PCindex+2].getHex().c_str(), 16);
			tmp1 = convertToInt((char*)machin->mem[tmpindex1].getHex().c_str(), 16);
			tmp2 = convertToInt((char*)machin->mem[tmpindex2].getHex().c_str(), 16);

			if(tmp1<tmp2) //L = 0 OZSXCLMX
				machin->mem[Flag].setHex(convertIntToHex(flag |= 0x04));
			if(tmp1>tmp2)
				machin->mem[Flag].setHex(convertIntToHex(flag |= 0x02));
			//machin->setPC(convertIntToHex(PCindex + 3)); //utasítás+op1+op2 => PC+3
			machin->setPC(convertIntToHex(PCindex + machin->mem[PCindex].getParamnum() + 1));
			break;
		case END:
			machin->running = false;
			break;
		default:
			cout << "Ismeretlen utasitas!" << endl;
			machin->setPC(convertIntToHex(PCindex + 1));
			break;
		}
		//machin->running=false;
	}
}


int main()
{
	//Mnemonik lista
	memori_part *memoris = new memori_part[25];
	memoris[0].setMnemonik(RST);
	memoris[0].setHex("32");
	memoris[0].setParamnum(1);
	
	memoris[1].setMnemonik(FLL);
	memoris[1].setHex("3D");
	memoris[1].setParamnum(1);

	memoris[2].setMnemonik(REV);
	memoris[2].setHex("51");
	memoris[2].setParamnum(1);

	memoris[3].setMnemonik(CPY);
	memoris[3].setHex("CPY");
	memoris[3].setParamnum(2);

	memoris[4].setMnemonik(MOV);
	memoris[4].setHex("63");
	memoris[4].setParamnum(2);

	memoris[5].setMnemonik(RTR);
	memoris[5].setHex("9F");
	memoris[5].setParamnum(2);

	memoris[6].setMnemonik(RTL);
	memoris[6].setHex("F9");
	memoris[6].setParamnum(2);

	memoris[7].setMnemonik(SWP);
	memoris[7].setHex("AA");
	memoris[7].setParamnum(2);

	memoris[8].setMnemonik(INC);
	memoris[8].setHex("C2");
	memoris[8].setParamnum(1);

	memoris[9].setMnemonik(DEC);
	memoris[9].setHex("CD");
	memoris[9].setParamnum(1);

	memoris[10].setMnemonik(ADD);
	memoris[10].setHex("07");
	memoris[10].setParamnum(2);

	memoris[11].setMnemonik(SUB);
	memoris[11].setHex("08");
	memoris[11].setParamnum(2);

	memoris[12].setMnemonik(INV);
	memoris[12].setHex("AF");
	memoris[12].setParamnum(1);

	memoris[13].setMnemonik(ABS);
	memoris[13].setHex("A0");
	memoris[14].setParamnum(1);

	memoris[14].setMnemonik(NOT);
	memoris[14].setHex("E2");
	memoris[14].setParamnum(1);
	
	memoris[15].setMnemonik(SAN);
	memoris[15].setHex("9A");
	memoris[15].setParamnum(2);
	
	memoris[16].setMnemonik(SOR);
	memoris[16].setHex("95");
	memoris[16].setParamnum(2);

	memoris[17].setMnemonik(_NAN);
	memoris[17].setHex("B9");
	memoris[17].setParamnum(2);

	memoris[18].setMnemonik(NOR);
	memoris[18].setHex("B6");
	memoris[18].setParamnum(2);

	memoris[19].setMnemonik(XOR);
	memoris[19].setHex("8C");
	memoris[19].setParamnum(2);

	memoris[20].setMnemonik(REP);
	memoris[20].setHex("F3");
	memoris[20].setParamnum(1);

	memoris[21].setMnemonik(NEX);
	memoris[21].setHex("55");
	memoris[21].setParamnum(0);

	memoris[22].setMnemonik(NOP);
	memoris[22].setHex("11");
	memoris[22].setParamnum(0);

	memoris[23].setMnemonik(CMP);
	memoris[23].setHex("24");
	memoris[23].setParamnum(2);

	memoris[24].setMnemonik(END);
	memoris[24].setHex("00");
	memoris[24].setParamnum(0);

	//Kiíráshoz kell
	char* opcodes_name[] = { "RST", "FLL", "REV", "CPY", "MOV", "RTR", "RTL", "SWP", "INC", "DEC", "ADD", "SUB", "INV", "ABS", "NOT", "SAN", "SOR", "_NAN", "NOR", "XOR", "REP", "NEX", "NOP", "CMP", "END", "NULLA" };
	machine * machin = new machine();
	init(machin, memoris);
	start(machin, memoris);

	/*for (int i = 0; i < 290; i++)
	{
		cout << opcodes_name[machin->mem[i].getMnemonik()] << " - " << machin->mem[i].getHex() << endl;
	}*/

	int ij=0;
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			cout << /*opcodes_name[machin->mem[ij].getMnemonik()] <<*/ " - " << machin->mem[ij].getHex();
			ij++;
		}
		cout << endl;
	}

	cout << "Vege!" <<endl;
	delete machin;
	delete[] memoris;

	cin.get();
	return 0;
}