#include "Structure.h"
#pragma once

int init_BDDclient(ClientFile *Fichierclient);

int lire_champ_suivant(char *ligne, int *idx, char *champ, int taille_champ,char separateur);

int charger(ClientFile * Fichierclient, char nom_fichier[]);

int creation_compte(ClientFile *Fichierclient);

int connexion(ClientFile *FichierClient);