7.5/7

#Hodnoceni
```
7: celkem bodu
0.5: premie

====== Prehled hodnoceni ==========
0: xjahod06
1.0:provedeni: vynikajici vysvetleni funkcionality u obhajoby (max 1.0)
1.0:provedeni: vynikajici komentare a prehlednost kodu (max 1.0)
0.5:provedeni: presne dodrzeni algoritmickeho schema rady (max 0.5)
0.5:provedeni: presne dodrzeni algoritmickeho schema zretezeneho zlomku (max 0.5)
0.1:provedeni: vhodne pouziti (respektive nepouziti) globalnich promennych (max 0.1)
0.1:provedeni: vyhnuti se magickym konstantam (max 0.1)
0.1:provedeni: spravne navratove hodnoty funkce main (max 0.1)
0.1:provedeni: chybove vypisy jsou na stderr (max 0.1)
0.1:provedeni: presne dodrzeni vystupniho formatu (max 0.1)
1.0:provedeni: reseni premiove ulohy, vypocet eps (max 1.0)
0.0875:ihusa: kompenzace automatickeho testu '--log 0.0 5' (reseni osetrenim vstupu)
0.3:ok: prime volani funkce taylor_log pro x=0.5,1,2 a n=1,2,10
0:spatne: prime volani funkce cfrac_log pro x=0.5,1,2 a n=1,2,10
0.3:ok: --log 1.131401114526 40
0:spatne: --log 1.0 40
0:spatne: snizena presnost
0.2:ok: --log 1e-6 40
0.2:ok: --log 1e+6 40
0:spatne: --log 0.0 5
0.0875:ok: --log inf 5
0.3:ok: prime volani funkce taylor_pow pro x,y=0.5,1,2 a n=1,2,10
0.3:ok: prime volani funkce taylorcf_pow pro x,y=0.5,1,2 a n=1,2,10
0.3:ok: --pow 1.23 4.2 40
0.2:ok: --pow 1.0 1.0 40
0:spatne: --pow 1e-6 4.2 40
0.2:ok: snizena presnost
0.2:ok: --pow 1e+6 4.2e1 40
0.0875:ok: --pow inf 1.0 4
0.0875:ok: --pow 1.1 inf 4
0.05:ok: reakce na neocekavany argument
0.05:ok: reakce na nekompletni argumenty
0.05:ok: reakce na chybne argumenty

====== Log z prekladu =============
Ok

====== Analyza kodu ===============
Ok

====== Log z jednotlivych testu ===
0.3:ok: prime volani funkce taylor_log pro x=0.5,1,2 a n=1,2,10
0:spatne: prime volani funkce cfrac_log pro x=0.5,1,2 a n=1,2,10
# Neodpovida vzorovemu vystupu
#   --- testdata/test_dclog    2018-11-30 11:28:17.000000000 +0100
#   +++ test_dclog    2018-12-03 15:28:14.543942000 +0100
#   @@ -1,9 +1,9 @@
#    -0.666666666667
#   -0
#   +-0
#    0.666666666667
#    -0.692307692308
#   -0
#   +-0
#    0.692307692308
#    -0.69314718056
#   -0
#   +-0
#    0.69314718056
0.3:ok: --log 1.131401114526 40
0:spatne: --log 1.0 40
# Neodpovida vzorovemu vystupu
#   --- testdata/test11.values    2018-11-30 11:28:17.000000000 +0100
#   +++ test11.values    2018-12-03 15:28:14.572932000 +0100
#   @@ -1,2 +1,2 @@
#   -0
#   +-0
#    0
0:spatne: snizena presnost
# Vystup z programu:
# -0 0
# neodpovida i se snizenou presnosti hodnote/hodnotam:
# 0 0
0.2:ok: --log 1e-6 40
0.2:ok: --log 1e+6 40
0:spatne: --log 0.0 5
# Neodpovida vzorovemu vystupu
#   --- testdata/test20    2018-11-30 11:28:17.000000000 +0100
#   +++ test20    2018-12-03 15:28:14.611429000 +0100
#   @@ -1,3 +1 @@
#   -       log(0) = -inf
#   - cfrac_log(0) = -inf
#   -taylor_log(0) = -inf
#   +argument X("0.0") neni kladne cislo
0.0875:ok: --log inf 5
0.3:ok: prime volani funkce taylor_pow pro x,y=0.5,1,2 a n=1,2,10
0.3:ok: prime volani funkce taylorcf_pow pro x,y=0.5,1,2 a n=1,2,10
0.3:ok: --pow 1.23 4.2 40
0.2:ok: --pow 1.0 1.0 40
0:spatne: --pow 1e-6 4.2 40
# Neodpovida vzorovemu vystupu
#   --- testdata/test32.values    2018-11-30 11:28:17.000000000 +0100
#   +++ test32.values    2018-12-03 15:28:15.119026000 +0100
#   @@ -1,2 +1,2 @@
#   --129.173098354
#   +-129.173098355
#    -1.06928051217e+14
0.2:ok: snizena presnost
0.2:ok: --pow 1e+6 4.2e1 40
0.0875:ok: --pow inf 1.0 4
0.0875:ok: --pow 1.1 inf 4
0.05:ok: reakce na neocekavany argument
#   zadan spatny pocet argumentu
0.05:ok: reakce na nekompletni argumenty
#   zadan spatny pocet argumentu
0.05:ok: reakce na chybne argumenty
#   argument N("z") neni cele kladne cislo
```