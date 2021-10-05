/**
 * @mainpage analyza shluku
 * @author Vojtech Jahoda <xjahod06@stud.fit.vutbr.cz>
 */

/**
 * Objekt
 * obsahuje:
 *	- 'id' indetifikacni cislo
 *	- souradnice 'x' a 'y'
 */
struct obj_t {
    int id;
    float x;
    float y;
};

/**
 * Shluk
 * obsahuje: 
 *	- 'size' momentalni pocet objektu ve shluku
 *	- 'capacity' maximalni pocet objektu ve shluku
 *	- 'obj' samotne objekty ve shluku
 */
struct cluster_t {
    int size;
    int capacity;
    struct obj_t *obj;
};

/**
 * @defgroup priprava_clusteru priprava clusteru
 * @brief pripravuje clustery pro budouci praci
 * @{
 */

/** @ingroup priprava_clusteru
 * @{
 * @brief Inicializace shluku 'c'.
 * @details Alokuje pamet pro cap objektu (kapacitu). Ukazatel NULL u pole
 * objektu znamena kapacitu 0.
 *
 * @pre cluster nesmi byt NULL
 * @pre kapacita nesmi byt mensi nez 0
 *
 * @param c vstupni cluster pro alokaci pameti
 * @param cap urcuje pocatecni kapacitu clusteru
 *
 * @post cluster je uspesne alokovany
 */
void init_cluster(struct cluster_t *c, int cap);

/**
 * @brief zjsti stejne 'id'
 * @details zjisti jestli se drive v nejakem objektu nezadalo stejne 'id'
 * a pokud ano tak vrati ukazatek na nej
 *
 * @pre cluster nesmi byt NULL
 * @pre radek nesmi byt mensi nebo roven 0
 *
 * @param id identifikacni cislo pro porovnani
 * @param cluster pole clusteru ze ktereho se kontroluji 'id'
 * @param radek index aktualniho radku v souboru
 * @param id_tmp ukazatel pro vraceni stejneho 'id'
 *
 * @post pokud se vyskytlo stejne 'id' tak vrati ukazatel na objekt se stejnym 'id'
 *
 * @return vrati hodnoty uspechu nebo neuspechu (OK,ERR)
 */
int test_id_unique(int id, struct cluster_t *cluster, int radek,int *id_tmp);

/**
 * @brief nacte objekty ze souboru a alokuje misto pro clustery
 * @details Ze souboru 'filename' nacte objekty. Pro kazdy objekt vytvori shluk
 * a ulozi jej do pole shluku. Alokuje prostor pro pole vsech shluku 
 * a ukazatel na prvni polozku pole (ukalazatel na prvni shluk v alokovanem poli)
 * ulozi do pameti, kam se odkazuje parametr 'arr'.
 * V pripade nejake chyby uklada do pameti, kam se odkazuje 'arr', hodnotu NULL.
 *
 * @pre pole clusteru nesmi byt NULL
 *
 * @param filename nazev souboru ze ktereho se ctou objekty
 * @param arr pole clusteru do ktereho se ukladaji soubory
 *
 * @post uspesne nacteni a alokovani clusteru 
 *
 * @return vrati pocet nactenych clusteru v poli
 */
int load_clusters(char *filename, struct cluster_t **arr);

/**
 * @brief prida do clusteru objekt
 * @details Prida objekt 'obj' na konec shluku 'c'. Rozsiri shluk,
 * pokud se do nej objekt nevejde.
 *
 * @pre cluster nesmi byt NULL
 * @pre obj nesmi byt NULL
 *
 * @param c vstupni cluster pro vlozeni objektu
 * @param obj objekt vkladany do clusteru
 *
 * @post uspesne pridan objekt do clusteru
 */
void append_cluster(struct cluster_t *c, struct obj_t obj);

/**
 * @}
 * @}
 * @defgroup Odstraneni_clusteru Odstraneni clusteru
 * @brief funkce pro odsraneni a dealokaci
 * @{
 */

/** @ingroup Odstraneni_clusteru
 * @{
 * @brief Dealokuje pamet clusteru
 * @details Odstraneni vsech objektu shluku a inicializace na prazdny shluk.
 *
 * @pre cluster nesmi byt NULL
 *
 * @param c vstupni cluster pro dealokaci
 *
 * @post cluster je uspesne dealokovan
 */
void clear_cluster(struct cluster_t *c);

/**
 * @brief odstani cluster
 * @details Odstrani shluk z pole shluku 'carr'. Pole shluku obsahuje 'narr'
 * polozek (shluku). Shluk pro odstraneni se nachazi na indexu 'idx'.
 *
 * @pre index musi byt vetsi nez pocet prvku v poli shluku
 * @pre pocet prvku v shluku nesmi byt mensi nez 0
 *
 * @param carr pole clusteru pro vstup
 * @param narr pocet clusteru v poli 'carr'
 * @param idx poradi clusteru co se ma odstanit v poli
 *
 * @post uspesne odstaneni clusteru a posunuti clusteru v poli
 *
 * @return vrati aktualni pocet clusteru v poli 'carr'
 */
int remove_cluster(struct cluster_t *carr, int narr, int idx);

/**
 * @brief dealokuje pole clusteru 
 * @details dealokuje kazdou polozku v poli a uzavre soubor
 *
 * @param soubor nazev souboru co chceme uzavrit
 * @param arr pole clusteru urcene pro dealokaci
 * @param count pocet alokovanych clusteru v poli
 *
 * @post uspesna dealokace clusteru z pameti
 */
void free_clusters(FILE* soubor,struct cluster_t **arr,int count);

/**
 * @}
 * @}
 * @defgroup prace_s_clustery Prace s clustery
 * @brief funkce pro praci s clustery
 * @{
 */

/** @ingroup prace_s_clustery
 * @{
 * @brief konstanta pro resize clusteru
 * @details doporucene se nastavuje na 10 
 */
extern const int CLUSTER_CHUNK;

/**
 * @brief Zmena kapacity shluku 'c' na kapacitu 'new_cap'.
 * @details pomoci funkce realloc zmeni velikost vstupniho clusteru
 * na novou kapacitu
 *
 * @pre cluster nesmi byt NULL
 * @pre kapacita clusteru musi byt vetsi nez 0
 * @pre nove zadana kapacita musi byt vetsi nez 0
 *
 * @param c vstupni cluster urceny pro zmenu velikosti
 * @param new_cap kapacita na kterou se ma zmenit kapacita clusteru
 *
 * @post uspesna zmena kapacity clusteru
 *
 * @return vrati cluster s novou velikosti
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap);

/**
 * @brief Razeni objektu ve shluku vzestupne podle jejich identifikatoru.
 *
 * @pre cluster nesmi byt NULL
 *
 * @param c vstupni cluster pro serazeni
 *
 * @post uspesne seradi cluster
 */
void sort_cluster(struct cluster_t *c);

/**
 * @}
 * @}
 * @defgroup spojeni_clusteru spojoveni clusteru
 * @brief funkce pro spojovani clusteru
 * @{
 */

/** @ingroup spojeni_clusteru
 * @{
 * @brief spoji 2 clustery do jednoho 
 * @details Do shluku 'c1' prida objekty 'c2'. Shluk 'c1' bude v pripade
 * nutnosti rozsiren. Objekty ve shluku 'c1' budou serazeny vzestupne podle
 * identifikacniho cisla. Shluk 'c2' bude nezmenen.
 *
 * @pre clustery nesmi byt NULL
 *
 * @param c1 prvni vstupni cluster do ktereho se prida druhy
 * @param c2 druhy stupni cluster ze ktereho se kopiruji objekty do 'c1'
 *
 * @post uspesne spojeni dvou clusteru do 'c1'
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2);

/**
 * @brief Pocita Euklidovskou vzdalenost mezi dvema objekty.
 * @details pomoci pythagorovi vety zjisti vzdalenost 2 objektu
 *
 * @pre objekty nesmi byt NULL
 *
 * @param o1 prvni vstupni objekt
 * @param o2 druhy vstupni objekt
 *
 * @post uspesne vypocitani vzdalenosti 2 objektu
 *
 * @return vrati vzdalenost 2 objektu
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2);

/**
 * @brief Pocita vzdalenost dvou shluku.
 * @details urci vzdalenost nejblizsich objektu z 2 clusteru
 *
 * @pre cluster nesmi byt NULL
 * @pre velikost clusteru nesmi byt 0
 *
 * @param c1 prvni vstupni cluster 
 * @param c2 druhy vstupni cluster
 *
 * @post uspesne urceni vzdalenosti dvou clusteru
 *
 * @return vrati vzdalenost 2 clusteru
 */
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2);

/**
 * @brief najde 2 nejblizsi clustery z poli
 * @details Funkce najde dva nejblizsi shluky. V poli shluku 'carr' o velikosti
 * 'narr' hleda dva nejblizsi shluky. Nalezene shluky identifikuje jejich 
 * indexy v poli 'carr'. Funkce nalezene shluky (indexy do pole 'carr') uklada
 * do pameti na adresu 'c1' resp. 'c2'.
 *
 * @pre pole clusteru nesmi byt NULL
 * @pre pocet clusteru v poli nesmi byt mensi nez 0
 *
 * @param carr vstupni pole clusteru
 * @param narr pocet clusteru v poli
 * @param c1 index prvniho najiteho clusteru
 * @param c2 index druheho najiteho clusteru
 *
 * @post urci indexi nejblizsich clusteru a ulozi je do ukazatelu
 */
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2);

/**
 * @}
 * @}
 * @defgroup tisk tisk
 * @brief funkce pro vypis dat z programu
 * @{
 */

/** @ingroup tisk
 * @brief Tisk shluku 'c' na stdout.
 *
 * @param c vstupni cluster pro tisk
 *
 * @post uspesne vytisknuti clusteru
 */
void print_cluster(struct cluster_t *c);

/**
 * @brief Tisk pole shluku
 * @details Parametr 'carr' je ukazatel na prvni polozku (shluk).
 * Tiskne se prvnich 'narr' shluku.
 *
 * @param carr vstupni pole clusteru pro tisk
 * @param narr pocet clusteru v poli
 *
 * @post uspesne vytisknuti vsech clusteru
 */
void print_clusters(struct cluster_t *carr, int narr);

/**
 * @brief vytiskne napovedu k programu 
 * @details vytiskne napovedu k programu
 */
void print_help();

/**
 * @}
 * @}
 */