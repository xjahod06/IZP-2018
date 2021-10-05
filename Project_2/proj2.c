
/*__/\\\\\\\\\\\\\_____________________________________________/\\\\\\\\\_____*/       
/* _\/\\\/////////\\\___________________________________/\\\__/\\\///////\\\___*/       
/*  _\/\\\_______\/\\\__________________________________\///__\///______\//\\\__*/      
/*   _\/\\\\\\\\\\\\\/___/\\/\\\\\\\______/\\\\\__________/\\\___________/\\\/___*/     
/*    _\/\\\/////////____\/\\\/////\\\___/\\\///\\\_______\/\\\________/\\\//_____*/    
/*     _\/\\\_____________\/\\\___\///___/\\\__\//\\\______\/\\\_____/\\\//________*/   
/*      _\/\\\_____________\/\\\_________\//\\\__/\\\___/\\_\/\\\___/\\\/___________*/  
/*       _\/\\\_____________\/\\\__________\///\\\\\/___\//\\\\\\___/\\\\\\\\\\\\\\\_*/ 
/*        _\///______________\///_____________\/////______\//////___\///////////////__*/
/********************************************************************/
/*						Druhy projekt z IZP							*/
/*						  Vojtech Jahoda							*/
/*																	*/
/*						  iteracni pocty							*/
/*				vypocet prirozeneho logaritmu a mocniny				*/
/*							15.11.2018								*/	
/********************************************************************/
/*pro vypocet ln(x) na n iteraci zadejte ve tvaru: ./proj2 --log X N*/
/*pro vypocet x^y na n iteraci zadejre ve tvaru: ./proj2 --pow X Y N*/
/*pro vypocet ln(x) za pomoci bonusove funkce mylog(x) zajdte ve tvaru: ./proj2 --log X N bonus  */
/*pro vypocet x^y za pomoci bonusove funkce mypow(x,y) zajdte ve tvaru: ./proj2 --pow X Y N bonus*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>

#define PRESNOST_BONUS 0.00000001 //urci presnost bonusoveho prikladu

enum vycet {ALL_OK,ERR}; //definuje navratove hodnoty

double kontrola_x (char* arg_x, bool bonus, double * x); //funkce pro kontrolu vstupniho agumentu x
double kontrola_y (char* arg_y, double * y); //funkce pro kontrolu vstupniho argumentu y
unsigned int kontrola_n (char* arg_n, unsigned int * n); //funkce pro kontrolu argumentu n
bool kontrola_bonus (char* arg_bonus); //funkce pro kontrolu zadaneho argumentu pro spusteni bonus funkce
double taylor_log (double x, unsigned int n); //funkce pro vypocet prirozeneho loaritmu pomoci taylorova polynomu na 'n' iteraci	
double cfrac_log (double x, unsigned int n); //funkce pro vypocet prirozeneho loaritmu pomoci zretezeneho zlomku na 'n' iteraci
double taylor_pow (double x, double y, unsigned int n); //funkce pro vypocet mocniny pomoci prirozeneho loaritmu pomoci taylorova polynomu na 'n' iteraci
double taylorcf_pow (double x, double y, unsigned int n); //funkce pro vypocet mocniny pomoci prirozeneho loaritmu pomoci zretezeneho zlomku na 'n' iteraci
double vypocet_pow (double y,double n, double a); //funkce pro samotny vypocet mocniny
double mylog (double x); //fuknce pro vypocet prirozeneho logaritmu podle zadane presnosti
double mypow (double x, double y); //fuknce pro vypocet mocniny cisla podle zadane presnosti
double kontrola_mypow (double b, double x, int p); //funkce pro kontrolu vystupu z funkce mypow

int main (int argc, char* argv[])
{
	double x; // promena pro zaklad mocniny nebo logaritmu
	unsigned int n; // promena pro pocet iteraci
	bool bonus; // promenna pro urceni zapnuti nebo vypnuti bonusu
	if (strcmp(argv[1],"--log") == 0 && (argc == 4 || (bonus = kontrola_bonus(argv[4])) == true)) // overi jestli se druhy argument programu rovna "--log" a pote overi pocet dalsich argumentu, nebo jestli byl zadan argument "bonus"
	{
		if ( kontrola_n(argv[3],&n) == ERR || kontrola_x(argv[2],bonus,&x) == ERR ) // overi jestli nedoslo k chybe pri kontrole vstupnich argumentu, pripadne ukonci program
			return ERR;
		if (bonus == true) //podminky pro urceni volani bonus prikladu nebo obyc
		{
			printf("  log(%g) = %.7e\n", x , log(x) );
			printf("mylog(%g) = %.7e\n", x , mylog(x) );
		}
		else
		{
			printf("       log(%g) = %.12g\n", x , log(x) );
			printf(" cfrac_log(%g) = %.12g\n", x , cfrac_log(x,n) );
			printf("taylor_log(%g) = %.12g\n", x , taylor_log(x,n) );
		}		
	}
	else if (strcmp(argv[1],"--pow") == 0 && (argc == 5 || (bonus = kontrola_bonus(argv[5])) == true)) // overi jestli se druhy argument programu rovna "--pow" a pote overi pocet dalsich argumentu, nebo jestli byl zadan argument "bonus"
	{
		double y; // promena pro ulozeni mocniny
		if ( kontrola_n(argv[4],&n) == ERR || kontrola_y(argv[3],&y) == ERR || kontrola_x(argv[2],bonus,&x) == ERR ) //overi jestli nedoslo k chybe pri kontrole vstupnich argumentu, pripadne ukonci program
			return ERR;
		if (bonus == true) //podminky pro urceni volani bonus prikladu nebo obyc
		{
			printf("  pow(%g,%g) = %.7e\n", x , y , pow(x,y) );
			printf("mypow(%g,%g) = %.7e\n", x , y , mypow(x,y) );
		}
		else
		{
			printf("         pow(%g,%g) = %.12g\n", x , y , pow(x,y) );
			printf("  taylor_pow(%g,%g) = %.12g\n", x , y , taylor_pow(x,y,n) );
			printf("taylorcf_pow(%g,%g) = %.12g\n", x , y , taylorcf_pow(x,y,n) );
		}
	}
	else //pokud byly splneny vyse uvedene podminky tak ukonci program
	{
		fprintf(stderr, "zadan spatny pocet argumentu\n");
		return ERR;
	}
	return ALL_OK;
}

double kontrola_x (char * arg_x, bool bonus, double * x) //funkce pro kontrolu vstupniho agumentu x
{
	char * c_x;
	* x = strtod(arg_x,&c_x); // prevede zadany argument na cislo typu double, pokud je to cislo tak do ukazatele('c_x') na znak ulozi 0 a do promenne x ulozi prevedene cislo, pokud znak tak do ukazele ulozi cislo znaku 
	if (*c_x != '\0' || (*x <= 0 && bonus == false)) //zkontroluje jestli je ukazatel nulovy(v argumentu byl ulozen znak misto cisla) nebo jestli je x kladne a nebyl zadan argument bonus 
	{
		fprintf(stderr, "argument X(\"%s\") neni kladne cislo\n", arg_x);
		return ERR;			
	}
	return ALL_OK;
}

double kontrola_y (char * arg_y,double * y) //funkce pro kontrolu vstupniho argumentu y
{
	char * c_y;
	* y = strtod(arg_y,&c_y); // prevede zadany argument na cislo typu double, pokud je to cislo tak do ukazatele('c_y') na znak ulozi 0 a do promenne y ulozi prevedene cislo, pokud znak tak do ukazele ulozi cislo znaku 
	if (*c_y != '\0' ) //zkontroluje jestli je ukazatel nulovy(v argumentu byl ulozen znak misto cisla)
	{
		fprintf(stderr, "argument Y(\"%s\") neni cislo\n", arg_y);
		return ERR;			
	}
	return ALL_OK;
}

unsigned int kontrola_n (char * arg_n,unsigned int * n) //funkce pro kontrolu argumentu n
{
	char * c_n;
	* n = strtoul(arg_n,&c_n,10); // prevede zadany argument na cislo typu double, pokud je to cislo tak do ukazatele('c_n') na znak ulozi 0 a do promenne n ulozi prevedene cislo, pokud znak tak do ukazele ulozi cislo znaku
	if (*c_n != '\0' || arg_n[0] == '-' ) //zkontroluje jestli je ukazatel nulovy(v argumentu byl ulozen znak misto cisla) nebo jestli prvni znak neni '-' co znamena ze je zadano zaporne cislo (v unsigned int se zaporne cislo ulozi jako kladne)
	{
		fprintf(stderr, "argument N(\"%s\") neni cele kladne cislo\n", arg_n);
		return ERR;			
	}
	return ALL_OK;
}

bool kontrola_bonus (char* arg_bonus) //funkce pro kontrolu zadaneho argumentu pro spusteni bonus funkce
{
	if (arg_bonus == NULL) //pokud argument nebyl zadal vrati funkce false
		return false;
	if (strcmp(arg_bonus,"bonus") == 0) //pokud argument byl zadan a rovna se "bonus" tak vrati true
		return true;
	return false; //pokud argument byl zadan ale nerovna se "bonus" tak vrati false
}

double taylor_log (double x, unsigned int n) //funkce pro vypocet prirozeneho loaritmu pomoci taylorova polynomu na 'n' iteraci
{
	double s;
	if (x == INFINITY) //dle vzorove funkce log z math.h se po zadani nekonecna vraci nekonecno
		return INFINITY;
	if (isnan(x) == 1) //pokud je zaklad nan vrati hodnotu nan
		return NAN;
	else if (x < 1) //pokud je x mensi nez 1, tak pouziva 1. algorytmus ze zadani [log(1-y) = -y - (y^2)/2 - (y^3)/3 - ...]
	{
		double y = 1-x; //ze vztahu 1-y = x si vyjadri y
		double prvek = -y; //urci prvni hodnotu jako -y
		s = prvek; //ulozi prvni hodnotu z vypoctu do souctu vsech
		for (double i = 1.0; i < n; i++)
		{
			prvek = prvek * (y/((i+1)/i)); //z vyse uvedeneho vztahu je vyjadren vztah mezi prvnimi 2ma cleny [(y^n)/n * y/((n+1)/n) = (y^(n+1))/(n+1)]
			s += prvek; //pricte do souctu vsech prvku dalsi hodnotu
		}
	}
	else if (x > 1)//pokud je x vetsi nez 1, tak pouziva 2. algorytmus ze zadani [log(x)=(((x-1)/x)^n)/n]
	{
		double y = ((x-1.0)/x); //vyjadri si y jako prvni prvek rady
		s = y; //priradi prvni prvek rady do souctu
		for (double i = 1.0; i < n; ++i)
		{
			y = y * (((x-1.0)/x)/((i+1)/i)); //minuly prvek rady nasobi [(x-1.0)/x/((i+1)/i)] tak aby vznikl nasledujici
			s += y; //pricte do souctu vsech prvku dalsi hodnotu
		}	
	}
	else if (x == 1)// pro x = 1 se vysledek vzdy rovna 0 
		return 0;
	return s;
}

double cfrac_log (double x, unsigned int n) //funkce pro vypocet prirozeneho loaritmu pomoci zretezeneho zlomku na 'n' iteraci
{
	double a = 1.0;
	double b = 1.0;
	double c = 0.0;
	double z = -(1-x)/(1+x); //vyjadri si z ze vzrahu [log(1+z/1-z)=x]
	if (isnan(x) == 1) //pokud je zaklad nan vrati hodnotu nan
		return NAN;
	if (x == INFINITY) //dle vzorove funkce log z math.h se po zadani nekonecna vraci nekonecno
		return INFINITY;
	while ( n > 1) // cislo n postupne zmensuje, aby se mohlo pocitat odzadu (provede vsechny kroky krome prvniho)
	{
		a = 2 * n - 1; //vypocita prvek a z rovnici [b/a-c] v zavislosti na poradi zlomku
		b = ((n-1)*z); //vypocita prvek b z rovnice [b/a-c] v zavislosti na z a na poradi zlomku
		b *= b; //umocni prvek b^2
		c = b / (a - c); //dosadi prvky do vzorce a vypocita prvek c pro budouci vypocet
		n--;
	}
	c = 2.0*z/(1.0-c); //provede prvni krok
	return c;
}

double taylor_pow (double x, double y, unsigned int n) //funkce pro vypocet mocniny pomoci prirozeneho loaritmu pomoci taylorova polynomu na 'n' iteraci
{
	double a = taylor_log(x,n); //urci a ze vztahu [x^y = 1 + (y*ln(x))^1/1! + (y*ln(x))^2/2!+...] tj. a=ln(x) pomoci taylorova polynomu
	return vypocet_pow(y,n,a); // preda data funkci rpo vypocet a vrati vysledek
}

double taylorcf_pow (double x, double y, unsigned int n) //funkce pro vypocet mocniny pomoci prirozeneho loaritmu pomoci zretezeneho zlomku na 'n' iteraci
{
	double a = cfrac_log(x,n); //urci a ze vztahu [x^y = 1 + (y*ln(x))^1/1! + (y*ln(x))^2/2!] tj. a=ln(x) pomoci zretezeneho zlomku
	return vypocet_pow(y,n,a); // preda data funkci pro vypocet a vrati vysledek
}

double vypocet_pow (double y,double n, double a) //funkce pro samotny vypocet mocniny
{
	double c = 1.0;
	double s = 1.0;
	double b = 0.0;
	bool zaporne_y = false;
	if (y == 0) //cokoliv na 0 je 1
		return 1;
	if (y < 0) //pokud je y zaporne tak urci jeho absolutni hodnotu a nastavi promennou 'zaporne_y' na true
	{
		y = fabs(y);
		zaporne_y = true;
	}
	b = y * a; //urci prvek b ze vztahu [x^y = 1 + (y*ln(x))^1/1! + (y*ln(x))^2/2! + ...] tj b = y*a (je konstantní takže může být mimo cyklus)
	for (double i = 0; i < n-1; ++i)
	{
		c = c * (b/(i+1)); //urci nasledujici prvek na zaklade minuleho 
		s += c; //pricte prvek do souctu prvku
	}
	if (zaporne_y == true) //pokud bylo y zaporne tak vrati prevracenou hodnotu vysledku
		return 1/s;
	return s;
}

double mylog (double x) //fuknce pro vypocet prirozeneho logaritmu podle zadane presnosti
{
	double a_t, b_t, a_c, b_c;
	double presnost_t = 1.0, presnost_c = 1.0;
	int i = 1;
	if (x < 0) //pokud je zadane x zaporne vrati hodnotu 'nan' dle vzoru funkce z knihovny math.h log
		return NAN;
	else if (x == 0) //pokud je zadane x nulove vrati hodnotu '-inf' dle vzoru funkce z knihovny math.h log
		return -INFINITY;
	if (x == INFINITY) //dle vzorove funkce log z math.h se po zadani nekonecna vraci nekonecno
		return INFINITY;
	a_t = taylor_log(x,i); //ulozi soucet i prvku ve funkce taylor_log
	a_c = cfrac_log(x,i); //ulozi soucet i prvku z cfrac_log
	while (presnost_t > PRESNOST_BONUS && presnost_c > PRESNOST_BONUS) //pokud rozdil 2 nasledujicich prvku(presnost_t nebo presnost_c) u funkci taylor_log nebo cfrac_log je vetsi nez zadana presnost tak pokracuje dal
	{
		b_t = taylor_log(x,i+1); //ulozi soucet i+1 prvku z taylor log
		presnost_t = fabs(b_t - a_t); //urci velikost i+1 prvku jakozto presnost vypoctu
		a_t = b_t; //ulozi do a_t hodnotu z b_t tj. "predchoziho prvku pro nasledujici krok"
		
		b_c = cfrac_log(x,i+1); //ulozi soucet i+1 prvku z cfrac_log
		presnost_c = fabs(b_c - a_c); //urci velikost i+1 prvku jakozto presnost vypoctu
		a_c = b_c; //ulozi do a_c hodnotu z b_c tj. "predchoziho prvku pro nasledujici krok"
		i++;
	}
	if (presnost_c < presnost_t) // pote co skonci cyklus tak urci, ktera funkce byla presnejsi a tu vraci
		return b_c;
	else
		return b_t;
	return ALL_OK;
}

double mypow (double x, double y) //fuknce pro vypocet mocniny cisla podle zadane presnosti
{
	double a_t, b_t, a_c, b_c;
	double presnost_t = 1.0, presnost_c = 1.0;
	int i = 1;
	int p = y; //do p ulozi hodnotu mocniny y zaokrouhlenou dolu
	if (y == 0 || x == 1 || ( x == -1 && fabs(y) == INFINITY)) //podminky podle manualu vzorove funkce "pow" pro vraceni cisla 1
		return 1;
	if ((x == 0 && y < 0) || (fabs(x) < 1 && y == -INFINITY) || (x == INFINITY && y > 0)) //podminky podle manualu vzorove funkce "pow" pro vraceni IFNINITY
		return INFINITY;
	if (x == 0 || (fabs(x) > 1 && y == -INFINITY) || (fabs(x) < 1 && y == INFINITY) || (x == INFINITY && y < 0)) //podminky podle manualu vzorove funkce "pow" pro vraceni cisla 0
		return 0;
	else if (y-p != 0 && x < 0) //pokud je mocnina desetine cislo a zaklad mocniny je zaporny tak vrati '-nan' dle voru funkce pow z math.h
		return -NAN;
	a_t = taylor_pow(fabs(x),y,i); //ulozi soucet i prvku ve funkce taylor_pow
	a_c = taylorcf_pow(fabs(x),y,i); //ulozi soucet i prvku z taylorcf_pow
	while (presnost_t > PRESNOST_BONUS && presnost_c > PRESNOST_BONUS) //pokud rozdil 2 nasledujicich prvku(presnost_t nebo presnost_c) u funkci taylor_pow nebo taylorcf_pow je vetsi nez zadana presnost tak pokracuje dal
	{	
		b_t = taylor_pow(fabs(x),y,i+1); //ulozi soucet i+1 prvku z taylor_pow
		presnost_t = fabs(b_t - a_t); //urci velikost i+1 prvku jakozto presnost vypoctu
		a_t = b_t; //ulozi do a_t hodnotu z b_t tj. "predchoziho prvku pro nasledujici krok"
		
		b_c = taylorcf_pow(fabs(x),y,i+1); //ulozi soucet i+1 prvku z taylorcf_pow
		presnost_c = fabs(b_c - a_c); //urci velikost i+1 prvku jakozto presnost vypoctu
		a_c = b_c; //ulozi do a_c hodnotu z b_c tj. "predchoziho prvku pro nasledujici krok"
		i++;
	}
	if (presnost_c < presnost_t) //urci ktera z techto funkci vypocitala presnejsi vypocet
		return kontrola_mypow(b_c,x,p); //zkontroluje hodnotu a vrati vysledek
	else
		return kontrola_mypow(b_t,x,p); //zkontroluje hodnotu a vrati vysledek
	return ALL_OK;
}

double kontrola_mypow (double b, double x, int p) //funkce pro kontrolu vystupu z funkce mypow
{
 	if (p%2 == 1 && x < 0) //urci jestli je y desetinne cislo a zaroven x zaporne a rozhodne jestli vracet kladnou nebo zapornou hodnotu
		return -b;
	return b;
}