#pragma once
#include <stdlib.h>
#include <stdio.h>
#include "Define.h"
#include "Structures.h"
#include <string.h>
#include <time.h>
int compterligne(const char * fichierlu);
float pourcentagecommandefait(Commande *demande, ArticleStocks *stockentrepot, int indiceentrepot, int (*quantitémanquante)[6], const char *fichier, const char *comandealias);
int fonction(const char *fichier, const char *comandealias, const char *fichierenvoitalias);