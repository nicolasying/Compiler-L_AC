# Concept de VM

La VM est composée d'un memoire et un processeur.

Le memoire est divisé en plusieur partie:

1. RESERVE, indice 0 - 19, pour les informations important
    
    INDICERETURN 1, INDICEDATA 2, INDICETYPE 3,

    INDICEDYNA 4,
    INDSOURCE 0, INDDES 5,
2. DYNAMIQUE, indice 20 - 69, pour les usages libres
3. DATASTACK, indice 70 - 79
4. TYPESTACK, indice 80 - 89
5. RETURNSTACK, indice 90 - 99
6. TABLESYMBOLS, indice 100 - 199
7. TABLESVM, indice 200 - 255

Le processeur implémente les fonctions suivantes:

1. MOD, qui modifie le block de memoire 0 comme [para]
2. ADD, qui fait addition de [0] et [para]
3. UPL, qui copie le memoire de block 0 vers [para]
4. INV, qui inverse le nombre stocké en block [INDSOURCE]
5. DIS, qui affiche le nombre stocké en INDSOURCE
6. DWL, qui copie le memoire de block [para] vers 0

Les fonctions de base seront implemente comme

Addition:
MOD 2 // set to prepare to retrieve [2]
DWL 2 // retrieve [2]
DWL 0 // retrieve [INDICEDATA]
UPL 4 // copy to cache pos 1

- first
- second
- third

1. -
