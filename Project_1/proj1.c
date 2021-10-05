/****************************************************************/
/*						Prvni projekt z IZP						*/
/*						  Vojtech Jahoda						*/
/*																*/
/*						  Prace s textem						*/
/*					 proudovy textovy editor					*/
/*							30.10.2018							*/
/****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

enum vycet{ALL_OK, ERR_IN, ERR_OUT,NEXT,
	MAX_DELKA_RADKU=1500, //urci max velikost pole "radek"
	MAX_DELKA_CONTENTU=250, //urci max velikost pole "content"
	MAX_POCET_PRIKAZU=100, //urci max pocet prikazu v souboru s prikazy
	MAX_DELKA_RADKU_PRO_NACTENI=1000 //urci max pocet znaku nactenych pro jeden radek ze vstupu
}; 

char content[MAX_DELKA_CONTENTU]; //globalni promena promena pro ukladani contentu za prikazem
char radek[MAX_DELKA_RADKU]; //globalni promena pro ukladani nacteneho radku

int nacti_radek(FILE* prikazy); //funkce na nacteni radku do pole "radek"
int nacti_content(FILE* prikazy); //fuknce na nacteni contentu do promene "content"
int prikaz_r(); //funkce pro vymazani konce radku
int prikaz_n(FILE* prikazy); //funkce "NEXT" na vypis nacteneho radku
int prikaz_d(FILE* prikazy); //funkce "DELETE" pro preskoceni radku
int prikaz_g(FILE* prikazy,int * index,char prikaz); //funkce "GOTO" pro skok v programu
int prikaz_s(int znak,FILE* prikazy); //funkce "substitute" pro vymenu slov v radku
int prikaz_s_1(int znak,int s,FILE* prikazy); //funkce samotne substituce pro vymenu radku
int prikaz_f(FILE* prikazy); //fuknce "find" pro skok na radek obsahujici zvolene slovo
int prikaz_c(); //funkce pro skok v podminkou v podobe pole "pattern"
void kontrola_radku(int vstup_pro_kontrolu,FILE* prikazy); //funkce pro zkontrolovani delky radku
int volba(int prikaz,FILE* prikazy,int * index); //funkce na vyber prikazu

int main(int argc, char* argv[])
{
	if (argc != 2) //zkontroluje, pocet atributu spousteneho souboru
	{
		if (argc == 1)
			fprintf(stderr, "nezadan soubor s prikazy\n");
		else
			fprintf(stderr, "chybny pocet atributu\n");
		return ERR_IN;
	}
	FILE* prikazy=fopen(argv[1], "r");
	if (prikazy == NULL) //zkontroluje, jestli se podarilo soubor otevrit, pokud ne, ukonci program
	{
		fprintf(stderr, "nepodarilo se nacist soubor s prikazy s nazvem %s\n", argv[1]);
		return ERR_IN;
	}
	int index = 0; //promenna pro pocitani prikazu
	int* pi = &index;
	while(nacti_radek(prikazy) != ERR_IN) //nacte radek a pokud funkce nevrati ERR_IN (dosly radky), pokracuje na zpracovani prikazu
	{
		int znak = 0;
		while(znak != 'n') //dokud nenastane prikaz 'n', tak pokracuje s nacitanim prikazu, pokud ano, tak vyskoci do nadrazeneho cyklu, ktery nacte dalsi radek (pokud existuje)
		{
			if ((znak = fgetc(prikazy)) != EOF) //nacte prvni znak radku ze souboru "prikazy" a kontroluje, zda nenastal EOF
			{
				*content = 0;
				nacti_content(prikazy); //nacita dalsi znaky na radku vstupniho souboru a uklada je do pole "content"
				if(volba(znak,prikazy,pi) == ERR_IN) //kontroluje, jestli funkce "volba" nevraci ERR_IN, pokud ano, tak ukonci program
				{
					printf("%i. radku v souboru %s\n", index+1, argv[1]);
					return ERR_IN;
				}
				if (index == MAX_POCET_PRIKAZU) //kontroluje, jestli neni zadano vice nez 100 prikazu, pokud ano, ukonci program
				{
					fprintf(stderr, "prilis mnoho prikazu (vice nez %i)\n",MAX_POCET_PRIKAZU);
					return ERR_IN;
				}
				index++;
			}
			else
			{
				printf("%s", radek); //pokud se narazi na konec souboru s prikazy, tak vypise zbyle radky ze vstupu
				break;
			}
		}
	}
	fclose(prikazy);
	return ALL_OK;
}
int nacti_radek(FILE* prikazy) //funkce na nacteni radku do pole "radek"
{
	if ( fgets (radek , MAX_DELKA_RADKU_PRO_NACTENI+5 , stdin) != NULL ) //kontroluje, jestli nenastal konec souboru, pokud ano, tak vrati hodnotu ERR_IN
	{
		if (strlen(radek)> MAX_DELKA_RADKU_PRO_NACTENI+2) //kontroluje, jestli velikost radku neni vetsi nez 1000 znaku (+2 protoze funkce Fgets prida dalsi 2 znaky)
		{
			fclose(prikazy);
			fprintf(stderr, "radek je prilis dlouhy (vice nez %i znaku)\n",MAX_DELKA_RADKU_PRO_NACTENI);
			exit(ERR_IN);
		}
		return ALL_OK;
	}
	return ERR_IN;
}
int nacti_content(FILE* prikazy) //fuknce na nacteni contentu do promene "content"
{
	if ( fgets (content , MAX_DELKA_CONTENTU+5 , prikazy) != NULL ) //kontroluje, jestli nenastal konec souboru, pokud ano, tak vrati hodnotu ERR_OUT
	{
		int i = 0;
		int znak;
		if(strlen(content) > MAX_DELKA_CONTENTU+2) //kontrola delky contentu
		{
			fclose(prikazy);
			fprintf(stderr, "content je prilis dlouhy (vice nez %i znaku)\n",MAX_DELKA_CONTENTU);
			exit(ERR_IN);
		}
		do
		{
			znak = content[i];
			i++;
			if(i == MAX_DELKA_CONTENTU+2) //ochrana proti zacykleni, pokud neni vlozen na konec souboru znak konce radku
				return ALL_OK;
		}while(znak != '\n'); 
		content[i-2] = 0; //odstrani znak konce radku (i-2 protoze na konci cyklu je zvetseno o 1 a za to se jeste vlozi '\0')
		return ALL_OK;
	}
	return ERR_OUT;
}
int prikaz_r() //funkce pro vymazani konce radku
{
	int znak;
	int i = 0;
	int d_radku = strlen(radek);
	do
	{
		znak = radek[i];
		i++;
		if (i>d_radku) //kontroluje, jestli se nepresahla velikost radku, pro pripadne zacykleni v pripade ze mezera neexistuje
			return ALL_OK;
	}while(znak != '\n');
	radek[i-2] = 0; //odstrani znak konce radku (i-2 protoze na konci cyklu je zvetseno o 1 a za to se jeste vlozi '\0')
	return ALL_OK;
}
int prikaz_n(FILE* prikazy) //funkce "NEXT" na vypis nacteneho radku
{	
	if (content[0] != 0) //kontroluje, jestli je za prikazem 'n' uveden pocet n
	{
		int n = 0;
		n = strtol(content, NULL, 10); //prevede "content" za prikazem na cislo
		if (n <= 0) //kontroluje, jestli toto cislo je vetsi nez 0
		{
			fprintf(stderr, "neplatny argument prikazu 'n' na ");
			return ERR_IN;
		}
		for (int i = 0; i < n; ++i)
		{
			printf("%s", radek);
			if (i<n-1)
				if(nacti_radek(prikazy) == ERR_IN) //pokud prikaz 'n' mel nacist vice radku nez bylo k dispozici, tak ukonci program
				{
					fprintf(stderr, "uz nejsou zadne dalsi radky na vypis\n");
					fclose(prikazy);
					exit(ERR_IN);
				}
		}
	}
	else 
	{
		printf("%s", radek);
	}
	return ALL_OK;
}
int prikaz_d(FILE* prikazy) //funkce "DELETE" pro preskoceni radku
{	
	if (content[0] != 0) //kontroluje, jestli je za prikazem 'd' uveden pocet n
	{
		int n = 0;
		n = strtol(content, NULL, 10); //prevede "content" za prikazem na cislo
		if (n <= 0) //kontroluje, jestli toto cislo je vetsi nez 0
		{
			fprintf(stderr, "neplatny argumentu prikazu 'd' na ");
			return ERR_IN;
		}
		for (int i = 0; i < n; ++i)
		{
			if(nacti_radek(prikazy) == ERR_IN) //zkontroluje, jestli nacteny radek, ktery se nevypise, existuje
			{
				fprintf(stderr, "prilis velke n u prikazu 'd' na ");
				return ERR_IN;
			}
		}
	}
	else
	{
		if(nacti_radek(prikazy) == ERR_IN)//nacte radek, ale nevypise ho, takze se tento radek pri dalsim nacteni smaze
		{
			fclose(prikazy);
			exit(ALL_OK);
		}
	}	
	return ALL_OK;
}
int prikaz_g(FILE* prikazy,int * index,char prikaz) //funkce "GOTO" pro skok v programu
{
	if (content[0] != 0) //kontroluje, jestli je za prikazem 'g' uveden pocet n
	{
		int n = strtol(content, NULL, 10); //prevede "content" za prikazem na cislo
		if (n <= 0) //kontroluje, jestli toto cislo je vetsi nez 0
		{
			fprintf(stderr, "cislo skoku je mensi nebo rovno 0, v prikazu '%c' na ", prikaz);
			return ERR_IN;
		}
		if(n == *index)
		{
			fprintf(stderr, "cislo skoku je stejne jako cislo radku prikazu '%c' na ",prikaz);
			return ERR_IN;
		}
		rewind(prikazy); //obnovi soubor tak, aby se mohlo cist z radku 1
		for (int i = 0; i < n-1; ++i)
		{
			if(fgets(content,MAX_DELKA_CONTENTU+2,prikazy) == NULL) //nacte a pote zahodi radek s prikazy a takto preskoci n-1 radku a zkontroluje, jestli tento radek existuje
			{
				fprintf(stderr, "cislo skoku je vetsi nez pocet prikazu, v prikazu '%c' na ", prikaz);
					return ERR_IN;
			}
			*content = 0; //vymate obsah pole "content"
		}
		*index = n-1; //nastavi pocitadlo prikazu na radek, na ktery je nastaven skok
		return ALL_OK;
	}
	fprintf(stderr, "chybi cislo skoku u prikazu '%c' na ", prikaz);
	return ERR_IN; //pokud za prikazem 'g' nebylo zadano zadne cislo, tak vrati ERR_IN
}
int prikaz_s(int znak,FILE* prikazy) //funkce "substitute" pro vymenu slov v radku
{
	if(content[1] == 0) //zkontroluje, jestli existuje pole "content"
	{
		fprintf(stderr, "spatne zadany prikaz '%c' na ",znak);
		return ERR_IN;
	}
	else
	{
		if(prikaz_s_1(content[0],znak,prikazy)==ERR_IN) //zkontroluje, jestli se nanasla chyba ve funkci, popripade ukonci program
		{
			fprintf(stderr, "chybi replacement v prikazu '%c' na ",znak);
			return ERR_IN;
		}
		return ALL_OK;
	}
}
int prikaz_s_1(int znak,int s,FILE* prikazy) //funkce samotne substituce pro vymenu radku
{
	int i  = 0;
	int j = 0;
	char * pr;
	char pattern[MAX_DELKA_CONTENTU];
	char replacement[MAX_DELKA_CONTENTU];
	int d_pointeru,d_radku,orig_pokracovani,d_tisku,rozdil,d_pattern,d_replacement = 0;
	while (content[i+1] != znak) //nacita "pattern", dokud nenajde znak oddeleni
		{
			pattern[i] = content[i+1];
			if (pattern[i] == '\n') //kontroluje, jestli je zadany "replacement"
				return ERR_IN;
			i++;
		}
	d_pattern = i;
	while (content[i+2] != '\n') //nacita "replacement", dokud nenajde znak konce radku (i+2 aby nepocital oddelovaci znak, ale pokracoval na "replacement")
		{
			replacement[j] = content[i+2];
			i++;
			j++;
		}
	d_replacement = j;
	if(strcmp(pattern,replacement) == 0) //kontroluje, jestli se pattern a replacement rovnaji
		return ALL_OK;
	pr = strstr(radek, pattern); //ulozi do pointeru pozici zacatku pole "pattern" v poli "radek"
	if (pr == NULL) 
		return ALL_OK;
	while (pr != NULL) //kontroluje, jestli se dany "pattern" vyskytuje v poli "radek" znovu nebo ne
	{	
		char tisk[MAX_DELKA_RADKU] = {0};
		d_radku = strlen(radek);
		d_pointeru = strlen(pr); //urci delku pointeru, tj. kolik znaku zbyva od prvniho vyskytu "pattern" v poli "radek" do konce radku
		for (int i = 0; i < d_radku-d_pointeru; ++i) //nacte radek do pozice prvniho vyskytu "pattern" v poli "radek" a ulozi jej do pole "tisk"
			tisk[i] = radek[i];
		orig_pokracovani = strlen(tisk)+d_pattern; //urci puvodni pozici v radku (kde konci "pattern" v poli "radek")
		strcat(tisk, replacement); //prida k poli "tisk" pole "replacement"
		d_tisku = strlen(tisk);
		rozdil = d_pattern - d_replacement; //urci rozdil, o ktery se ma posunout nove pole oproti puvodnimu
		kontrola_radku(rozdil,prikazy); //zkontroluje, jestli je v poli "radek" misto na zvetseni pole oproti puvodni velikosti
		for (int i = d_tisku; i < d_radku - rozdil; ++i) //nacte do pole "tisk" znaky z radku od konce vyskytu "pattern"
		{
			tisk[i]=radek[orig_pokracovani];
			orig_pokracovani++;
		}
		strcpy(radek, tisk); //zkopiruje pole "tisk" do pole "radek"
		pr = strstr(radek, pattern); //ulozi do pointeru pozici zacatku pole "pattern" v poli "radek"
		if (s == 's') //urci jetli bylo zadano male s nebo velke s a podle toho pokracuje nebo konci v cyklu
			break;
	}
	return ALL_OK;
}
int prikaz_f(FILE* prikazy)//fuknce "find" pro skok na radek obsahujici zvolene slovo
{
	if(content[1] == 0) //zkontroluje, jestli existuje pole "content"
	{
		fprintf(stderr, "chybi pattern u prikazu 'f' na ");
		return ERR_IN;
	}
	else
	{
		while(strstr(radek,content) == NULL) //dokud se vyskyt patternu na radku rovna NULL, tak prohledava vstup
				if(nacti_radek(prikazy) == ERR_IN) //pokud dojdou radky, tak ukonci program
					{
						fprintf(stderr, "pattern \"%s\" se nevyskytuje na vstupu\n", content);
						fclose(prikazy);
						exit(ERR_IN);
					}
		return ALL_OK;
	}
}
int prikaz_c() //funkce pro skok v podminkou v podobe pole "pattern"
{
	if(content[1] == 0) //zkontroluje, jestli existuje pole "content"
	{
		fprintf(stderr, "chybi parametry prikazu 'c' na ");
		return ERR_IN;
	}
	else
	{
		char char_x[3];
		int i = 0;
		int j = 0;
		char pattern[MAX_DELKA_CONTENTU];
		char * pcx = char_x;
		while (content[i] != ' ') //najde X v prikazu, ale ma omezeni pouze 3 cifer protoze je podporovano maximalne 100 prikazu
		{	
			if (i == 3) //kontroluje, jestli nema X vice nez 3 cifry (maximalne 100 radku prikazu)
			{
				fprintf(stderr, "prilis velke X v prikazu 'c' na ");
				return ERR_IN;
			}
			char_x[i]=content[i];
			i++;
		}
		while(content[i+1] != '\n') //najde pattern prikazu
		{
			pattern[j]=content[i+1];
			i++;
			j++;
		}
		if (strstr(radek,pattern) != NULL) //pokud se pattern vystkytne na radku tak zmeni content na X
		{
			*content = *pcx;
			return NEXT; // vrati nestandartni hodnotu, takze switch ve funkci "volba" bude pokracovat na dalsi prikaz, coz je 'g' ktery bude mit nastaveny content X
		}
		return ALL_OK;
	}
}
void kontrola_radku(int vstup_pro_kontrolu,FILE* prikazy) //funkce pro zkontrolovani delky radku
{
	int d_contentu;
	if(vstup_pro_kontrolu < 0) //pokud je vstup mensi nez 0 udela z nej absolutni hodnotu
		vstup_pro_kontrolu *= -1;
	if(vstup_pro_kontrolu == 'a' || vstup_pro_kontrolu == 'b') //pokud se kontrola vola z prikazu 'a' nebo 'b' tak nacte cele pole "content" pro kontrolu
		d_contentu = strlen(content);
	else //pokud se funkce vola z jineho prikazu nez 'a' nebo 'b' tak porovna vstupni hodnotu
		d_contentu = vstup_pro_kontrolu;
	if(strlen(radek)+ d_contentu > sizeof(radek))//zkontroluje, jestli pocet znaku v radku + pocet znaku co chceme pridat, nepresahl celkovou velikost pole "radek"
	{
		fprintf(stderr, "prilis mnoho znaku na radku\n");
		fclose(prikazy);
		exit(ERR_IN);
	}
}
int volba(int prikaz, FILE* prikazy,int * index) //funkce na vyber prikazu
{
	int c;
	switch (prikaz)
	{
		case 'i':
			printf("%s\n", content); 
			break;
		case 'b':
			kontrola_radku(prikaz,prikazy); //zkonroluje, jestli je v poli "radek" dostatek mista pro prikaz 'b'
			strcat(content, radek); //prida obsah pole "radek" za pole "content"
			strcpy(radek, content); //zkopiruje obsah pole "content" do pole "radek"
			break;
		case 'a':
			prikaz_r(); //oddela mezeru pro pridani contentu
			kontrola_radku(prikaz,prikazy); //zkonroluje, jestli je v poli "radek" dostatek mista pro prikaz 'a'
			strcat(radek, content); //prida za pole "radek" pole "content"
			strcat(radek, "\n"); //prida mezeru, ktera byla oddelana
			break;
		case 'n':
			if(prikaz_n(prikazy) == ERR_IN)
				return ERR_IN;
			break;
		case 'd':
			if(prikaz_d(prikazy) == ERR_IN)
				return ERR_IN;
			break;
		case 'r':
			prikaz_r(); //odstrani konec radku pro snadnejsi manipulaci s polem
			break;
		case 'q': //zavre soubor s prikazy a ukonci program s hodnotou ALL_OK
			fclose(prikazy);
			exit(ALL_OK);
		case 'e':
			strcat(radek, "\n"); // prida za pole "radek" znak konce radku
			break;
		case 's':
		case 'S':
			if(prikaz_s(prikaz,prikazy) == ERR_IN) 
				return ERR_IN;
			break;
		case 'f':
			if(prikaz_f(prikazy) == ERR_IN) 
				return ERR_IN;
			break;
		case 'c':
			c = prikaz_c(prikazy);
			if(c == ERR_IN) 
				return ERR_IN;
			else if (c == NEXT) //pokud se navratova hodnota rovna NEXT tak provede prikaz 'g'
				if(prikaz_g(prikazy,index,prikaz) == ERR_IN) 
					return ERR_IN;
			break;
		case 'g':
			if(prikaz_g(prikazy,index,prikaz) == ERR_IN) 
				return ERR_IN;
			break;	
		default: //pokud byl zadan neznamy prikaz, tak napise na jakem radku, zavre soubor a ukonci program
			fprintf(stderr, "neplatny prikaz '%c' na %ls. radku\n",prikaz,index+1);
			fclose(prikazy);
			exit(ERR_IN);
	}
	return ALL_OK;
}