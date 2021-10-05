/**
 * Kostra programu pro 3. projekt IZP 2018/19
 *
 * Jednoducha shlukova analyza: 2D nejblizsi soused.
 * Single linkage
 */
/********************************************************************/
/*						Treti projekt z IZP							*/
/*						  Vojtech Jahoda							*/
/*																	*/
/*						  Shlukova analyza							*/
/*			Jednoducha shlukova analyza: 2D nejblizsi soused		*/
/*							29.11.2018								*/	
/********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h> // sqrtf
#include <limits.h> // INT_MAX

/*****************************************************************
 * Ladici makra. Vypnout jejich efekt lze definici makra
 * NDEBUG, napr.:
 *   a) pri prekladu argumentem prekladaci -DNDEBUG
 *   b) v souboru (na radek pred #include <assert.h>
 *      #define NDEBUG
 */
#ifdef NDEBUG
#define debug(s)
#define dfmt(s, ...)
#define dint(i)
#define dfloat(f)
#else

// vypise ladici retezec
#define debug(s) printf("- %s\n", s)

// vypise formatovany ladici vystup - pouziti podobne jako printf
#define dfmt(s, ...) printf(" - "__FILE__":%u: "s"\n",__LINE__,__VA_ARGS__)

// vypise ladici informaci o promenne - pouziti dint(identifikator_promenne)
#define dint(i) printf(" - " __FILE__ ":%u: " #i " = %d\n", __LINE__, i)

// vypise ladici informaci o promenne typu float - pouziti
// dfloat(identifikator_promenne)
#define dfloat(f) printf(" - " __FILE__ ":%u: " #f " = %g\n", __LINE__, f)

#endif

#define ROZMER_OSY_X 1000 //urci rozmer pole pro body z ose x
#define ROZMER_OSY_Y 1000 //urci rozmer pole pro body z ose y
#define UHLOPRICKA sqrt(pow(ROZMER_OSY_X,2)+pow(ROZMER_OSY_Y,2)) //urci uhlopricku daneho pole pro urceni maximalni vzdalenosti

/// Chunk of cluster objects. Value recommended for reallocation.
const int CLUSTER_CHUNK = 10;

//definice navratovych hodnot
enum vycet{OK, ERR = -1}; 

/*****************************************************************
 * Deklarace potrebnych datovych typu:
 *
 * TYTO DEKLARACE NEMENTE
 *
 *   struct obj_t - struktura objektu: identifikator a souradnice
 *   struct cluster_t - shluk objektu:
 *      pocet objektu ve shluku,
 *      kapacita shluku (pocet objektu, pro ktere je rezervovano
 *          misto v poli),
 *      ukazatel na pole shluku.
 */

struct obj_t {
    int id;
    float x;
    float y;
};

struct cluster_t {
    int size;
    int capacity;
    struct obj_t *obj;
};

/*
 Inicializace shluku 'c'. Alokuje pamet pro cap objektu (kapacitu).
 Ukazatel NULL u pole objektu znamena kapacitu 0.
*/
//alokuje pamet pro cluster 
void init_cluster(struct cluster_t *c, int cap)
{
	assert(c != NULL);
	assert(cap >= 0);
	if (c == NULL || cap <= 0) //osetreni moznych chyb na vstupu
	{
		fprintf(stderr, "neco se pokazilo, musime to opravit\n");
		return;
	}
	//alokace clusteru
	c->size = 0;
	c->capacity = cap;
	if((c->obj = malloc(sizeof(struct obj_t) * cap)) == NULL) //alokuje pamet pro objekty clusteru
	    c->capacity = 0; //kdyz se alokace nepovede tak nastavi kapacitu clusteru na 0
	return;
}

/*
 Odstraneni vsech objektu shluku a inicializace na prazdny shluk.
 */
//uvolni pamet clusteru
void clear_cluster(struct cluster_t *c)
{
	assert(c);
	if (c == NULL) //osetreni moznych chyb na vstupu
	{
		fprintf(stderr, "neco se pokazilo, musime to opravit\n");
		return;
	}
    c->size = 0;
    c->capacity = 0;
    free(c->obj); //dealokuje pamet pro objecty v clusteru (jedina polozka clusteru kde je pouzit malloc)
    return;
}

/*
 Zmena kapacity shluku 'c' na kapacitu 'new_cap'.
 */
//funkce dana zadanim
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap)
{
	// TUTO FUNKCI NEMENTE
	assert(c);
	assert(c->capacity >= 0);
	assert(new_cap >= 0);
	if (c->capacity >= new_cap)
	    return c;

	size_t size = sizeof(struct obj_t) * new_cap;

	void *arr = realloc(c->obj, size);
	if (arr == NULL)
	    return NULL;

	c->obj = (struct obj_t*)arr;
	c->capacity = new_cap;
	return c;
}
/*
 Prida objekt 'obj' na konec shluku 'c'. Rozsiri shluk, pokud se do nej objekt
 nevejde.
 */
//prida do clusteru object
void append_cluster(struct cluster_t *c, struct obj_t obj)
{
	assert(c != NULL);
	assert(obj != NULL);
	if (c == NULL) //osetreni moznych chyb na vstupu
	{
		fprintf(stderr, "neco se pokazilo, musime to opravit\n");
		return;
	}
    if (c->size >= c->capacity) //pokud se velikost clusteru rovna kapacite tak je nutno cluster zvetsit
        if (resize_cluster(c,c->capacity + CLUSTER_CHUNK) == NULL)
        	return;
    c->obj[c->size]=obj; //prida do pole objektu na index 'size' novy objekt
    c->size++; //zvetsi size o 1
    return;
}
/*
 Seradi objekty ve shluku 'c' vzestupne podle jejich identifikacniho cisla.
 */
void sort_cluster(struct cluster_t *c);

/*
 Do shluku 'c1' prida objekty 'c2'. Shluk 'c1' bude v pripade nutnosti rozsiren.
 Objekty ve shluku 'c1' budou serazeny vzestupne podle identifikacniho cisla.
 Shluk 'c2' bude nezmenen.
 */
//slouci 2 clustery do jednoho
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2)
{
	assert(c1 != NULL);
	assert(c2 != NULL);
	if (c1 == NULL || c2 == NULL) //osetreni moznych chyb na vstupu
	{
		fprintf(stderr, "neco se pokazilo, musime to opravit\n");
		return;
	}
	if ( (c1->capacity - c1->size) < c2->size ) //podminka pro resize clusteru, pokud ve v c1 není místo (capacita - velikost) pro velikost c2 tak se udela resize
		c1=resize_cluster(c1,c1->capacity+c2->size); //udela resize clusteru jako kapacitu urci: kapacita c1 + velikost c2
	for (int i = 0; i < c2->size; i++)
		append_cluster(c1,c2->obj[i]); //prida jeden objekt co druheho
	sort_cluster(c1);
	return;
}
/**********************************************************************/
/* Prace s polem shluku */

/*
 Odstrani shluk z pole shluku 'carr'. Pole shluku obsahuje 'narr' polozek
 (shluku). Shluk pro odstraneni se nachazi na indexu 'idx'. Funkce vraci novy
 pocet shluku v poli.
*/
//odstani a dealokuje cluster z pole clusteru a posune zbyle na puvodni pozici
int remove_cluster(struct cluster_t *carr, int narr, int idx)
{
	assert(idx < narr);
	assert(narr > 0);
	if (idx > narr || narr <= 0) //osetreni moznych chyb na vstupu
	{
		fprintf(stderr, "neco se pokazilo, musime to opravit\n");
		return ERR;
	}
	clear_cluster(&carr[idx]); //dealokuje pamet clusteru
	for (int i = idx; i < narr-1; i++) //posune prvky ind - narr na mista o jedno mensi
		carr[i]=carr[i+1];
	return narr-1;
}

// pomocna funkce pro razeni shluku
static int obj_sort_compar(const void *a, const void *b)
{
    // TUTO FUNKCI NEMENTE
    const struct obj_t *o1 = (const struct obj_t *)a;
    const struct obj_t *o2 = (const struct obj_t *)b;
    if (o1->id < o2->id) return -1;
    if (o1->id > o2->id) return 1;
    return 0;
}

/*
 Razeni objektu ve shluku vzestupne podle jejich identifikatoru.
*/
void sort_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    qsort(c->obj, c->size, sizeof(struct obj_t), &obj_sort_compar);
}

/*
 Pocita Euklidovskou vzdalenost mezi dvema objekty.
 */
//pomoci pythagorovi vety zjisti vzdalenost 2 bodu
float obj_distance(struct obj_t *o1, struct obj_t *o2)
{
    assert(o1 != NULL);
    assert(o2 != NULL);
    if (o1 == NULL || o2 == NULL) //osetreni moznych chyb na vstupu
    {
    	fprintf(stderr, "neco se pokazilo, musime to opravit\n");
		return ERR;
    }
    float vzdalenost;
    vzdalenost = sqrt(pow(o1->x-o2->x,2) + pow(o1->y-o2->y,2)); //vypocita vzorec [(x^2)+(y^2)]^1/2
    return vzdalenost;
}

/*
 Pocita vzdalenost dvou shluku.
*/
//urci vzdalenost 2 nejblizsich objektu z clusteru
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2)
{
	assert(c1 != NULL);
	assert(c1->size > 0);
	assert(c2 != NULL);
	assert(c2->size > 0);
	if (c1 == NULL || c2 == NULL || c1->size <= 0 || c2->size <= 0) //osetreni moznych chyb na vstupu
	{
		fprintf(stderr, "neco se pokazilo, musime to opravit\n");
		return ERR;
	}
	float vzdalenost;
	float min_vzdalenost = UHLOPRICKA; //nastavi jako min vzdalenost nejvetsi moznou vzdalenost v poli tj. uhlopricka 
	for (int i = 0; i < c1->size; i++)
	{
		for (int j = 0; j < c2->size; j++)
		{
			vzdalenost = obj_distance(&c1->obj[i],&c2->obj[j]); //urci vzdalenost i-teho objektu z clusteru od j-teho a ulozi ji
			if (vzdalenost == ERR) //kontrola vystupu z funkce obj_distance
				return ERR;
			if (vzdalenost < min_vzdalenost)
				min_vzdalenost = vzdalenost;
		}
	}
	return min_vzdalenost;
}

/*
 Funkce najde dva nejblizsi shluky. V poli shluku 'carr' o velikosti 'narr'
 hleda dva nejblizsi shluky. Nalezene shluky identifikuje jejich indexy v poli
 'carr'. Funkce nalezene shluky (indexy do pole 'carr') uklada do pameti na
 adresu 'c1' resp. 'c2'.
*/
//najde 2 nejblizsi clustery z poli
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2)
{
	assert(narr > 0);
	assert(carr);
	if (narr <= 0 || carr == NULL) //osetreni moznych chyb na vstupu
	{
		fprintf(stderr, "neco se pokazilo, musime to opravit\n");
		*c1 = ERR;
		return;
	}
	float vzdalenost;
	float min_vzdalenost = UHLOPRICKA; //nastavi jako min vzdalenost nejvetsi moznou vzdalenost v poli tj. uhlopricka
	for (int i = 0; i < narr-1; i++)
	{
		for (int j = i+1; j < narr; j++)
		{
			vzdalenost = cluster_distance(&carr[i],&carr[j]); //urci vzdalenost i-teho clusteru od j-teho a ulozi ji
			if (vzdalenost == ERR) //konrola jestli nastala chyba ve fci cluster_distance
			{
				*c1 = ERR;
				return;
			}
			if (vzdalenost < min_vzdalenost) //urci jestli neni vypocitana vzdalenost mensi nez zatim minimalni
			{
				//pokud je vzdalenost mensi nez minimalni tak ji prepise a prepise i indexi vstupnich clusteru
				min_vzdalenost = vzdalenost;
				*c1= i;
				*c2= j;
			}
		}
	}
	return;
}

//dealokuje pole clusteru a zavre program 
void free_clusters(FILE* soubor,struct cluster_t **arr,int count)
{
	fclose(soubor);
	for (int i = 0; i < count; i++)
		clear_cluster(&(*arr)[i]); //dealokuje postupne jednotlive polozky pole
	free(*arr); //dealokuje pole samotne
	return;
}

//zjisti jestli je cislo id jedinecne v celem poli clusteru
int test_id_unique(int id, struct cluster_t *cluster, int radek,int *id_tmp)
{
	if (cluster == NULL || radek < 0) //osetreni moznych chyb na vstupu
		return ERR;
	for (int i = 0; i < radek; i++)
		if (cluster[i].obj->id == id) //kdyz se id objektu v momentalnim clusteru rovna tak ulozi index radku
		{	
			*id_tmp = i;
			return ERR;
		}
	return OK;
}
/*
 Ze souboru 'filename' nacte objekty. Pro kazdy objekt vytvori shluk a ulozi
 jej do pole shluku. Alokuje prostor pro pole vsech shluku a ukazatel na prvni
 polozku pole (ukalazatel na prvni shluk v alokovanem poli) ulozi do pameti,
 kam se odkazuje parametr 'arr'. Funkce vraci pocet nactenych objektu (shluku).
 V pripade nejake chyby uklada do pameti, kam se odkazuje 'arr', hodnotu NULL.
*/
//nacte objekty ze souboru a alokuje misto pro clustery
int load_clusters(char *filename, struct cluster_t **arr)
{
    assert(arr != NULL);
    if (arr == NULL) //osetreni moznych chyb na vstupu
    {
    	fprintf(stderr, "neco se pokazilo, musime to opravit\n");
    	return ERR;
    }
    FILE * soubor = fopen(filename,"r");
    if (soubor == NULL) //podminka jestli se spravne nacetl soubor
    {
        fprintf(stderr, "nepodarilo se nacist soubor \"%s\"\n",filename);
        fprintf(stderr, "pro vypsani napovedy staci spustit jako \"./proj3 --help\"\n");
        return ERR;
    }

    int max_delka_radku = 20; //max delka radku urcena jako pocet cislic v INT_MAX(10) + pocet znaku pro souradnice x a y v rozmezi 0-1000 + rezerva
    int ind_radek = 1; //promenna urcujici index momentalniho radku
    char radek[max_delka_radku];
    int count;
    char pojistka; //promenna pro ukladani nadbytecnych cisel v souboru (urcuje se pres ni chyba)

    //nacteni prvniho radku souboru a urceni cisla count
    if (fgets(radek,max_delka_radku,soubor) != NULL) //testuje jestli radek existuje
    {
        if (sscanf(radek, "count=%d %c", &count,&pojistka) != 1) // funkce sscanf vrati pocet uspesne nactenych promenych tj. pokud bude zadano vice nez jedno cislo tak se nravti 1 ale 2
        {
            fprintf(stderr, "radek %d v souboru %s byl chybne zadan\n",ind_radek , filename);
            fprintf(stderr, "pro vypsani napovedy staci spustit jako \"./proj3 --help\"\n");
            fclose(soubor);
            return ERR;
        }
        if (count <= 0) //urci jestli je cislo zaporne nebo kladne
        {
            fprintf(stderr, "count v souboru %s musi byt kladne cele cislo\n",filename);
            fprintf(stderr, "pro vypsani napovedy staci spustit jako \"./proj3 --help\"\n");
            fclose(soubor);
            return ERR;
        }
    }
    else
    {
    	fprintf(stderr, "chybi radek count=\n");
    	fprintf(stderr, "pro vypsani napovedy staci spustit jako \"./proj3 --help\"\n");
    	fclose(soubor);
    	return ERR;
    }
    
    //alokace mista pro pole structur cluster
    *arr = malloc(sizeof(struct cluster_t) * count); //alokuje samotne pole 
    if (arr == NULL)
    {
    	fprintf(stderr, "alokace se nezdarila\n");
    	fclose(soubor);
    	return ERR;
    }
    for (int i = 0; i < count; i++) //alokuje jednotlive polozky
    	init_cluster(&(*arr)[i], 1);

    //nacitani objektu a pridani do clusteru
	struct obj_t obj;
    for (int i = 0; i < count; i++)
    {
        ind_radek++;
        if (fgets(radek,max_delka_radku,soubor) != NULL) //kontroluje jestli existuje dalsi radek
        {
        	//kontroluje jestli na radku jsou pouze 3 cisla a souradnice x a y jsou v rozsahu 1-1000
            if (sscanf(radek,"%d %g %g %c",&obj.id ,&obj.x ,&obj.y, &pojistka) != 3 || obj.x > ROZMER_OSY_X || obj.x < 0 || obj.y > ROZMER_OSY_Y || obj.y < 0) 
            {
                fprintf(stderr, "radek %d v souboru %s byl chybne zadan\n",ind_radek , filename);
                fprintf(stderr, "pro vypsani napovedy staci spustit jako \"./proj3 --help\"\n");
                free_clusters(soubor,arr,count); //funkce pro dealokaci pole
                return ERR;
            }
            //kontroluje jestli je id cisla jedinecne v celem souboru
            int id_tmp;
            int id_test = test_id_unique(obj.id,*arr,ind_radek-2,&id_tmp);
            if ( id_test == 1 )
            {
            	fprintf(stderr, "Objekt na radku %d v souboru %s ma duplicitni id s objektem na radku %d\n",ind_radek,filename,id_tmp+2);
            	fprintf(stderr, "pro vypsani napovedy staci spustit jako \"./proj3 --help\"\n");
            	free_clusters(soubor,arr,count); //funkce pro dealokaci pole
                return ERR;
            }
            else if ( id_test == ERR )
            {
            	fprintf(stderr, "neco se pokazilo, musime to opravit\n");
            	free_clusters(soubor,arr,count); //funkce pro dealokaci pole
            	return ERR;
            }
            append_cluster(&(*arr)[i],obj);
        }
        else
        {
            fprintf(stderr, "radek %d v souboru %s neexistuje\n",ind_radek , filename);
            fprintf(stderr, "pro vypsani napovedy staci spustit jako \"./proj3 --help\"\n");
            free_clusters(soubor,arr,count); //funkce pro dealokaci pole
            return ERR;
        }
    }
    //zavre program (uz z nej je vse potrebe precteno) a vrati pocet clusteru
    fclose(soubor);
    return count;
}

/*
 Tisk shluku 'c' na stdout.
*/
//zadana funkce zadanim
void print_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    for (int i = 0; i < c->size; i++)
    {
        if (i) putchar(' ');
        printf("%d[%g,%g]", c->obj[i].id, c->obj[i].x, c->obj[i].y);
    }
    putchar('\n');
    return;
}

/*
 Tisk pole shluku. Parametr 'carr' je ukazatel na prvni polozku (shluk).
 Tiskne se prvnich 'narr' shluku.
*/
//zadana funkce zadanim
void print_clusters(struct cluster_t *carr, int narr)
{
    printf("Clusters:\n");
    for (int i = 0; i < narr; i++)
    {
        printf("cluster %d: ", i);
        print_cluster(&carr[i]);
    }
    return;
}

void print_help() //funkce pro vypasni napovedy
{
	printf("\nasi chyba pri spusteni, ale to nevadi pomoc je uz na ceste\n");
	printf("pro spravne spusteni je potreba zapsat syntax ve tvaru \"./proj3 'nazev souboru' [N]\"\n");
	printf("kde N znaci pocet shluku ktare chceme mit na konci a N musi byt kladne cele cislo\n");
	printf("a soubor musi byt ve tvaru:\n");
	printf("count='pocet zadanych objektu'\n");
	printf("cislo cislo cislo\n");
	printf("  |     |     |\n");
	printf(" id     x     y\n");
	printf("\n");
	printf("cislo 'id' musi byt jedinecne pro cely soubor\n");
	printf("napriklad:\n");
	printf("count=3\n");
	printf("40 520 126\n");
	printf("42 168 545\n");
	printf("45 158 42\n");
	printf("\n");
	printf("Nazev: Shlukova analyza\n");
	printf("Autor: Vojtech Jahoda\n");
	printf("xlogin: xjahod06\n");
	printf("Datum: 29.11.2018\n");
	return;
}

int main(int argc, char *argv[])
{
	if ( argc > 1 && argc < 4)
	{
		if (strcmp(argv[1],"--help") == 0) //pokud je prvni argument --help (neuvazuji ze tak nekdo pojmenuje svuj soubor)
			print_help();
		struct cluster_t *clusters; //vytvori pole clusteru do ktereho se bude vse ukladat

		//zktroluje argument n, pokud neni zadan vrati hodnotu 1
		int arg_n;
		if (argc == 3)
		{
			char * c_n;
			arg_n = strtol(argv[2],&c_n,10); //prevede dany argument na cislo a vrati ukazateli hodnotu 0 pokud to cislo je, pokud to cislo neni vrati jeho hodnotu z ASCII tabulky
			if ( *c_n != '\0' || arg_n <= 0)
			{
				fprintf(stderr, "argument N musi byt kladne cele cislo\n");
				fprintf(stderr, "pro vypsani napovedy staci spustit jako \"./proj3 --help\"\n");
				return ERR;
			}
		}
		else //pokud nebylo zadano N tak nastavi jeho hodnotu na 1
			arg_n = 1;

		//nacteni clusteru a vyhrazeni (alokace) pameti pro ne 
		int pocet_clusteru = load_clusters(argv[1],&clusters); //nacte clustery ze souboru
		if (pocet_clusteru == ERR)
			return ERR;

		if (pocet_clusteru >= arg_n) //Kontrola argumentu N
		{
			//rozdeleni clusteru do daneho poctu shluku
			while (pocet_clusteru > arg_n)
			{
				int index_1, index_2;
				find_neighbours(clusters,pocet_clusteru,&index_1,&index_2); //najde 2 nejblizsi a vrati 2 indexi
				if (index_1 == ERR) //pokud se vrati hodnota indexu -1 tak ukonci program
					return ERR;
				merge_clusters(&clusters[index_1],&clusters[index_2]); //spoji 2 clustery na indexem danych predchozi funkci do toho prvniho a seradi shluk
				pocet_clusteru = remove_cluster(clusters,pocet_clusteru,index_2); //odstrani cluster na danem indexu a vrati pocet clusteru v aktualnim poli
				if (pocet_clusteru == ERR) //osetreni navratove hodnoty z funkce remove_cluster
					return ERR;
			}
			print_clusters(clusters,pocet_clusteru);
		}
		else
		{
			fprintf(stderr, "argument N (%d) je vetsi nez pocet objektu v souboru (%d)\n",arg_n,pocet_clusteru);
			fprintf(stderr, "pro vypsani napovedy staci spustit jako \"./proj3 --help\"\n");
		}

		//uvolneni pameti
		for (int i = 0; i < pocet_clusteru; i++)
			clear_cluster(&clusters[i]); //preda funkci adresu clusteru s indexem 'i' ktera uvolni pamet pro jednotlive clustery
		free(clusters); //uvolni pamet pro pole samotne
	}
	else
	{
		fprintf(stderr, "zadan spatny pocet argumentu\n");
		fprintf(stderr, "pro vypsani napovedy staci spustit jako \"./proj3 --help\"\n");
		return ERR;
	}
	return OK;
}