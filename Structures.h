#include <stdio.h>
#include <stdlib.h>


/*--------------------------------------------------------------------*/
/*                          DONNEES STOCKEES                          */
/*--------------------------------------------------------------------*/

/*La structure qui d�fini un client (UNE LIGNE) */ 
typedef struct Client {	
	int ID;								//4 chiffres pour l'ID
	char Nom[21];						//Nom de 20 caract�re alpha ( +1 pour le \0 de fin de chaine de caract�re )
	char Prenom[21];					//Prenom de 20 caract�re alpha ( +1 pour \0 de fin de chaine de caract�re)
	char Adresse[51];					//localisation ( adresse ) du client, 50 caract�res ( +1 pour \0 de fin de chainede caract�re )
	char Ville[21];						//La ville du client (+1 pour le \0)
	char CodePostal[6];					//Code postal du client 5 chiffres + le \0
	char NumeroTel[11];					//num�ro de t�l�phone, 10 chiffres dit caract�re num�rique ( et PAS alphanum�rique ) (+1 pour le \0 de fin de chaine de caract�re )
	char MotDePasse[5];					//Mot de passe de 4 caract�re alphanum�rique (+1 pour le \0� de fin de chaine de caract�re)
} Client;
/*La structure qui regroupe tous les clients (UN FICHIER) */
typedef struct ClientFile {
	Client *TableauDeClient;			//Un pointeur sur un tableau de Client (N�c�ssite un malloc et des realloc)
	int NombreDeClients;				//Nombre de client enregistrer pour le moment

} ClientFile;





/*La structure qui definit un Article (UNE LIGNE) */
typedef struct Article {
	int ID;								//4 chiffres pour l'ID
	char Nom[21];						//Nom de l'article de 20 caract�re ( +1 pour le \0 de fin de chaine de caract�re )
	char Description[21];				//Description de l'article de 20 caract�re de long ( +1 pour le \0 de fin de chaine de caract�re )
	char Type[21];						//Type de l'article de 20 caract�re de long
} Article;
/*La structure qui regroupe tous les articles d�fini (UN FICHIER) */
typedef struct ArticleFile {	
	Article *TableauDesArticles;		//Un pointeur sur un tableau d'article (Necessite un malloc et des realloc)
	int NombreArticles;					//Nombre d'article dans ce fichier d'article
} ArticleFile;




/*La structure qui defini une commande (UNE LIGNE) */
typedef struct Commande {
	int ClientID;						//L'ID du client qui passe la commande
	int ArticleID;						//L'ID de l'article que le client a commander
	int Quantity;						//La quantit� de c et article que le client � commander
	int CodeRetrait;					//Le code que le client doit taper afin de retirer son colis dans les pickup Station
} Commande;
/*La structure qui regroupe toutes les commandes (UN FICHIER) */
typedef struct CommandeFile {
	Commande *TableauDesCommandes;		//Un pointeur qui pointe sur un tableau de toutes les commandes
	int NombreDeCommande;				//Le nombre de commandes dans ce fichier de commande
} CommandeFile;




/*strucutre qui d�fini les stocks d'un article dans un entrepot (UNE LIGNE) */
typedef struct ArticleStocks {
	int EntrepotID;					//ID de l'entrepot (USELESS)
	int ArticleID;					//ID de l'article
	int Quantity;					//Quantit� en stock de cet article
	int Hauteur;					//Hauteur de l'article	
	int Largeur;					//Largeur de l'article
	int Profondeur;					//Profondeur de l'article
} ArticleStocks;
/*structure qui defini tous les article stock�es dans un entrepot (UN FICHIER) */
typedef struct StocksEntrepot {
	ArticleStocks *TabArticleStock;	//Un pointeur qui pointe sur un tableau qui liste tous les articles en stocks dans CET entrepot
	int NombreArticle;				//Le nombre d'article DIFFERENT en stock
} StocksEntrepot;

/*structure qui defini un entrepot (le dossier ou il faut aller voir son stock) (UNE LIGNE) */
typedef struct Entrepot {
	int EntrepotID;					//ID de l'entrepot
	char NomDuDossier[16];			//Nom du dossier dans lequel se trouve le dossier stock de cet entrepot
} Entrepot;
/*structure qui contient tous les emplacements des dossier des entrepots (UN FICHIER) */
typedef struct EntrepotFile {
	Entrepot *TabEntrepot;			//Un poiteur qui pointe sur un tableau listant chaquun des entrepots
	int NombreEntrepot;				//Nombre d'entrepot a disposition
} EntrepotFile;





/*structure qui defini une station (UNE LIGNE) */
typedef struct Station {
	int StationID;		//L'ID de la pickup station
	int SlotID;					//L'ID du slot de cette pickup station
	int Hauteur;				//La hauteur de ce slot
	int Largeur;				//La largeur de ce slot
	int Profondeur;				//La profondeur de ce slot
	int Disponible;				//Si slot disponible mettre a 1, sinon 0
} Station;
/*structure qui regroupe toutes nos stations*/
typedef struct StationFile {
	Station *TableauDesStation;	//pointeur qui pointe vers un tableau qui liste les stations a disposition
	int NombreDeStation;		//Le Nombre de station a disposition
} StationFile;





/*structure achat*/
typedef struct Achat {
	int AchatID;				//L'ID de l'achat... ? Pas compris c'etait pendant la pause y avais plus personne pour me repondre... :(
} Achat;





/*structure dispatching*/
typedef struct Dispatching {
	int DispatchingID;			//L'id du dispatching... Pareil :'(
} Dispatching;


/*--------------------------------------------------------------------*/
/*                          DONNEES ENVOYEES                          */
/*--------------------------------------------------------------------*/
















