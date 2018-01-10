#include "Structures.h"
#include "Define.h"




int Ecrire(const char NomFichier, ArticleStocksFile *Stock){
	FILE *fichier;
	char Buffer[1028]; //une ligne de 1028 de long maximum
	fopen_s(&fichier, &NomFichier, "w");//ouverture du fichier voulu en mode ecriture
	if (fichier == NULL || Stock->NombreArticle == 0 || Stock->TabArticleStock == NULL) return ERROR;
	for (int i = 0; i < Stock->NombreArticle; i++) { //on ecrit chaque ligne jusqu'a avoir atteint le nombre de ligne du fichier
		sprintf_s(Buffer, 1028, "%d;%d;%d;%d;%d;%d;\n", ((Stock->TabArticleStock)+i)->ArticleID, ((Stock->TabArticleStock) + i)->EntrepotID, ((Stock->TabArticleStock) + i)->Hauteur, ((Stock->TabArticleStock) + i)->Largeur, ((Stock->TabArticleStock) + i)->Profondeur, ((Stock->TabArticleStock) + i)->Quantity); //le format d'une ligne, les element a mettre ATTENTION PEUT ETRE DANS LE MAUVAIS ORDRE ; le tout ecrit dans le buffer
		fputs(Buffer, fichier); //On ecrit le buffer dans le fichier, la ligne est écrite
	}
	fclose(fichier); //FERMETURE
	return OK;
}

int main() {

	ArticleStocksFile TEST;
	TEST.NombreArticle = 3;
	TEST.TabArticleStock = malloc(3 * sizeof(ArticleStocks));
	TEST.TabArticleStock[0].ArticleID = 1;
	TEST.TabArticleStock[1].ArticleID = 2;
	TEST.TabArticleStock[2].ArticleID = 3;
	TEST.TabArticleStock[0].EntrepotID = 101;
	TEST.TabArticleStock[1].EntrepotID = 102;
	TEST.TabArticleStock[2].EntrepotID = 103;
	TEST.TabArticleStock[0].Quantity = 12;
	TEST.TabArticleStock[1].Quantity = 42;
	TEST.TabArticleStock[2].Quantity = 602;
	TEST.TabArticleStock[0].Hauteur = 10;
	TEST.TabArticleStock[1].Hauteur = 11;
	TEST.TabArticleStock[2].Hauteur = 12;
	TEST.TabArticleStock[0].Largeur = 13;
	TEST.TabArticleStock[1].Largeur = 14;
	TEST.TabArticleStock[2].Largeur = 15;
	TEST.TabArticleStock[0].Profondeur = 16;
	TEST.TabArticleStock[1].Profondeur = 17;
	TEST.TabArticleStock[2].Profondeur = 18;



	Ecrire("test.txt", &TEST);


	return OK;
}