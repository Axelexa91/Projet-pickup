#pragma once
#include <stdlib.h>
#include <stdio.h>
#include "Define.h"
#include "Structures.h"
int compterligne(const char * fichierlu);
float pourcentagecommandefait(Commande *demande, ArticleStocks *stockentrepot, int indiceentrepot, int (*quantit�manquante)[6]);