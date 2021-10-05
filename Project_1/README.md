2.7/5

#Hodnoceni
```
2.7: celkem bodu

====== Prehled hodnoceni ==========
0: xjahod06
1.0:provedeni: vynikajici vysvetleni funkcionality u obhajoby (max 1.0)
0.5:provedeni: vynikajici komentare a prehlednost kodu (max 0.5)
0.5:provedeni: vynikajici pouziti funkci (max 0.5)
0.0:provedeni: nevhodne pouziti globalnich promennych (max 0.1)
0.1:provedeni: vyhnuti se magickym konstantam (max 0.1)
0.1:provedeni: spravne navratove hodnoty funkce main (max 0.1)
0.1:provedeni: chybove vypisy jsou na stderr (max 0.1)
0.1:provedeni: uzavreni souboru pred koncem programu (max 0.1)
0.5:ihusa:kompenzace automaticke penalizace globalnich promennych
-0.5: globalni promenne
0:spatne: prikaz insert1 (i)
0.1:ok: prikaz insert2 (i)
0:spatne: prikaz next1 (n)
0:spatne: prikaz next2 (n)
0:spatne: prikaz quit (q)
0:spatne: prikaz delete1 (d)
0:spatne: prikaz delete2 (d)
0:spatne: prikaz after (a)
0:spatne: prikaz before (b)
0:spatne: prikaz goto (g)
0:spatne: prikaz removeEOL (r)
0:spatne: prikaz substituce1 (s)
0:spatne: prikaz substituce2 (s)
0:spatne: prikaz substituce3 (S)
0.05:ok: reakce na napovedu
0.05:ok: reakce na napovedu
0.1:ok: chybny prikaz v souboru

====== Log z prekladu =============
Ok

====== Analyza kodu ===============
-0.5: globalni promenne
#    proj1.c:22: content
#    proj1.c:23: radek

====== Log z jednotlivych testu ===
0:spatne: prikaz insert1 (i)
# Neodpovida vzorovemu vystupu
#   --- testdata/test_i1    2018-11-10 22:06:31.000000000 +0100
#   +++ test_i1    2018-11-12 09:36:28.661124000 +0100
#   @@ -1,2 +1,2 @@
#    cat vstup | ./proj1 i1.txt  -->
#   -    ---
#   +    --
0.1:ok: prikaz insert2 (i)
0:spatne: prikaz next1 (n)
# Neodpovida vzorovemu vystupu
#   --- testdata/test_n1    2018-11-10 22:06:31.000000000 +0100
#   +++ test_n1    2018-11-12 09:36:28.683001000 +0100
#   @@ -1,3 +1,2 @@
#    cat vstup | ./proj1 n1.txt  -->
#   -    Hello world
#   -    ---
#   +neplatny argument prikazu 'n' na     1. radku v souboru n1.txt
0:spatne: prikaz next2 (n)
# Neodpovida vzorovemu vystupu
#   --- testdata/test_n2    2018-11-10 22:06:31.000000000 +0100
#   +++ test_n2    2018-11-12 09:36:28.694681000 +0100
#   @@ -1,6 +1,6 @@
#    cat vstup | ./proj1 n2.txt  -->
#        Hello world
#   +    --
#        vstupni
#        soubor
#        s nekolika
#   -    ---
0:spatne: prikaz quit (q)
# Neodpovida vzorovemu vystupu
#   --- testdata/test_q    2018-11-10 22:06:31.000000000 +0100
#   +++ test_q    2018-11-12 09:36:28.706976000 +0100
#   @@ -1,2 +1,2 @@
#    cat vstup | ./proj1 q.txt  -->
#   -    Hello world
#   +neplatny argument prikazu 'n' na     1. radku v souboru q.txt
0:spatne: prikaz delete1 (d)
# Neodpovida vzorovemu vystupu
#   --- testdata/test_d1    2018-11-10 22:06:31.000000000 +0100
#   +++ test_d1    2018-11-12 09:36:28.720838000 +0100
#   @@ -1,2 +1,2 @@
#    cat vstup | ./proj1 d1.txt  -->
#   -    vstupni
#   +neplatny argumentu prikazu 'd' na     1. radku v souboru d1.txt
0:spatne: prikaz delete2 (d)
# Neodpovida vzorovemu vystupu
#   --- testdata/test_d2    2018-11-10 22:06:31.000000000 +0100
#   +++ test_d2    2018-11-12 09:36:28.733349000 +0100
#   @@ -1,2 +1,2 @@
#    cat vstup | ./proj1 d2.txt  -->
#   -    soubor
#   +neplatny argument prikazu 'n' na     2. radku v souboru d2.txt
0:spatne: prikaz after (a)
# Neodpovida vzorovemu vystupu
#   --- testdata/test_a    2018-11-10 22:06:31.000000000 +0100
#   +++ test_a    2018-11-12 09:36:28.745823000 +0100
#   @@ -1,2 +1,2 @@
#    cat vstup | ./proj1 a.txt  -->
#   -    Hello world</h1>
#   +    Hello worl</h1
0:spatne: prikaz before (b)
# Neodpovida vzorovemu vystupu
#   --- testdata/test_b    2018-11-10 22:06:31.000000000 +0100
#   +++ test_b    2018-11-12 09:36:28.758634000 +0100
#   @@ -1,2 +1,2 @@
#    cat vstup | ./proj1 b.txt  -->
#   -    <h1>Hello world
#   +    <h1Hello world
0:spatne: prikaz goto (g)
# Neodpovida vzorovemu vystupu
#   --- testdata/test_g    2018-11-10 22:06:31.000000000 +0100
#   +++ test_g    2018-11-12 09:36:28.771604000 +0100
#   @@ -1,11 +1,3 @@
#    cat vstup | ./proj1 g.txt  -->
#   -    ---
#   -    Hello world
#   -    ---
#   -    vstupni
#   -    ---
#   -    soubor
#   -    ---
#   -    s nekolika
#   -    ---
#   -    radky
#   +neplatny argument prikazu 'n' na     --
#   +    2. radku v souboru g.txt
0:spatne: prikaz removeEOL (r)
# Neodpovida vzorovemu vystupu
#   --- testdata/test_r    2018-11-10 22:06:31.000000000 +0100
#   +++ test_r    2018-11-12 09:36:28.783192000 +0100
#   @@ -1,2 +1,2 @@
#    cat vstup | ./proj1 r.txt  -->
#   -    Hello worldvstupni
#   +neplatny argument prikazu 'n' na     2. radku v souboru r.txt
0:spatne: prikaz substituce1 (s)
# Neodpovida vzorovemu vystupu
#   --- testdata/test_s1    2018-11-10 22:06:31.000000000 +0100
#   +++ test_s1    2018-11-12 09:36:28.795510000 +0100
#   @@ -1,2 +1,2 @@
#    cat vstup | ./proj1 s1.txt  -->
#   -    Hello there
#   +    Hello ther
0:spatne: prikaz substituce2 (s)
# Neodpovida vzorovemu vystupu
#   --- testdata/test_s2    2018-11-10 22:06:31.000000000 +0100
#   +++ test_s2    2018-11-12 09:36:28.811183000 +0100
#   @@ -1,2 +1,2 @@
#    cat vstup | ./proj1 s2.txt  -->
#   -    Hello there
#   +    Hello ther
0:spatne: prikaz substituce3 (S)
# Neodpovida vzorovemu vystupu
#   --- testdata/test_s3    2018-11-10 22:06:31.000000000 +0100
#   +++ test_s3    2018-11-12 09:36:28.824545000 +0100
#   @@ -1,2 +1,2 @@
#    cat vstup | ./proj1 s3.txt  -->
#   -    HellO wOrld
#   +    Hell wrld
0.05:ok: reakce na napovedu
#   nezadan soubor s prikazy
0.05:ok: reakce na napovedu
#   nepodarilo se nacist soubor s prikazy s nazvem notexists.txt
0.1:ok: chybny prikaz v souboru
#
```