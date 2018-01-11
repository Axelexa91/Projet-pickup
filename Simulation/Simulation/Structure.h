#include <stdlib.h>
#include <stdio.h>

#define OK 1
#define ERROR -1
#define SEPARATEUR ';'
#define MAX_IDArticle 4
#define MAX_Dim 3
#define SEPARATEUR ';'
#define MAX_Article 4
#define MAX_ID_PICKUP 3
#define MAX_ID_SLOT 4
#define MAX_STATION_PAR_REGION 20

typedef struct Ventes {
	int ClientID;						//L'ID du client qui passe la commande
	int ArticleID;						//L'ID de l'article que le client a commander
	int Quantity;						//La quantité de c et article que le client à commander
	int CodeRetrait;					//Le code que le client doit taper afin de retirer son colis dans les pickup Station
} Ventes;

/*La structure qui regroupe toutes les commandes sous forme de liste chaînée(UN FICHIER) */

typedef struct SingleVentesElement
{
	Ventes data;
	struct SingleVentesElement * next;
} SingleVentesElement;

typedef struct VentesFile {
	SingleVentesElement * head;
	SingleVentesElement * tail;
	int Size;				//Le nombre de commandes dans ce fichier de commande
} VentesFile;

typedef struct SlotDispo
{
	int ID_PickUP_Station;
	int ID_article;
	int quantite;
	int ID_slot;
} SlotDispo;

typedef struct SingelSlotDispoElement
{
	SlotDispo data;
	struct SingelSlotDispoElement * next;
} SingelSlotDispoElement;

typedef struct SlotDispoFile
{
	SingelSlotDispoElement * head;
	SingelSlotDispoElement * tail;
	int size;
} SlotDispoFile;

typedef struct ArticleDansSlot {
	int IDArticle;
	int quantite;
	int largeur;
	int profondeur;
	int hauteur;
} ArticleDansSlot;

typedef struct SingelArticleDansSlotElement
{
	ArticleDansSlot data;
	struct SingelArticleDansSlotElement * next;
} SingelArticleDansSlotElement;

typedef struct ListArticle
{
	SingelArticleDansSlotElement * head;
	SingelArticleDansSlotElement * tail;
	int Size;
} ListArticle;
//Liste chaînée des articles à mettre dans la station

/****** Client ******/

#define MAX_ID_CLIENT 4
#define CLIENT_PRENOM_MAX 21
#define CLIENT_NOM_MAX 21
#define CLIENT_ADRESSE_MAX 51
#define CLIENT_VILLE_MAX 51
#define CLIENT_CodePostal_MAX 6
#define CLIENT_TELEPHONE_MAX 10
#define CLIENT_MotDePasse_MAX 5

/*La structure qui défini un client (UNE LIGNE) */
typedef struct Client {
	int ID;								//4 chiffres pour l'ID
	char Nom[CLIENT_NOM_MAX];						//Nom de 20 caractère alpha ( +1 pour le \0 de fin de chaine de caractère )
	char Prenom[CLIENT_PRENOM_MAX];					//Prenom de 20 caractère alpha ( +1 pour \0 de fin de chaine de caractère)
	char Adresse[CLIENT_ADRESSE_MAX];					//localisation ( adresse ) du client, 50 caractères ( +1 pour \0 de fin de chainede caractère )
	char Ville[CLIENT_VILLE_MAX];						//La ville du client (+1 pour le \0)
	char CodePostal[CLIENT_CodePostal_MAX];					//Code postal du client 5 chiffres + le \0
	char NumeroTel[CLIENT_TELEPHONE_MAX];					//numéro de téléphone, 10 chiffres dit caractère numérique ( et PAS alphanumérique ) (+1 pour le \0 de fin de chaine de caractère )
	char MotDePasse[CLIENT_MotDePasse_MAX];					//Mot de passe de 4 caractère alphanumérique (+1 pour le \0à de fin de chaine de caractère)
} Client;

/*La structure qui regroupe tous les clients (UN FICHIER) */

typedef struct SingleClientElement  //Déclaration d'un élément individuel de la liste
{
	Client data;
	struct SingleClientElement *next;  //On définit un pointeur sur l'élément suivant
}SingleClientElement;

typedef struct ListClient //Déclaration de la liste en elle-même avec une tête de liste, une queue de liste et la taille de la liste
{
	SingleClientElement *head;
	int Size;
	SingleClientElement *tail;
}ListClient;




/***Structure qui va nous donner la station la plus proche***/
typedef struct RecupDistance {
	int ID_Client;
	int ID_Station;
	int Distance;
}RecupDistance;

typedef struct SingleRecupDistanceElement {
	RecupDistance data;
	struct SingleRecupDistanceElement *next;
}SingleRecupDistanceElement;

typedef struct RecupDistanceList {
	SingleRecupDistanceElement *head;
	SingleRecupDistanceElement *tail;
	int Size;
}RecupDistanceList;


/*Structure qui va nous permettre de renvoyer les slots que l'on réserve avec les articles et la quantité qu'on met dedans*/

typedef struct ReserveSlot {
	int ID_PickUP;
	int ID_slot;
	int ID_article;
	int quantite;
	int numero_tel;
	int login;
}ReserveSlot;

typedef struct SingleReserveSlotElement {
	ReserveSlot data;
	struct SingleReserveSlotElement *next;
}SingleReserveSlotElement;

typedef struct ReserveSlotList {
	SingleReserveSlotElement *head;
	SingleReserveSlotElement *tail;
	int Size;
}ReserveSlotList;

//Un seul fichier pour toutes les réservation dans une région région

/*********************************************************/
/*********************************************************/

typedef struct Station {
	int PickupStationID;		//L'ID de la pickup station
	int SlotID;					//L'ID du slot de cette pickup station
	int Hauteur;				//La hauteur de ce slot
	int Largeur;				//La largeur de ce slot
	int Profondeur;				//La profondeur de ce slot
	int Disponible;				//Si slot disponible mettre a 1, sinon 0
} Station;

typedef struct SingelStationElement
{
	Station data;
	struct SingelStationElement * next;
} SingelStationElement;

typedef struct StationFile
{
	SingelStationElement * head;
	SingelStationElement * tail;
	int Size;
} StationFile;

typedef struct SlotOccuppe
{
	int ID_PickUP;
	int ID_Slot;
	int Tel;
	int CodeRetrait;
} SlotOccuppe;

typedef struct SingleSlotOccuppeElement
{
	SlotOccuppe data;
	struct SingleSlotOccuppeElement *next;
}SingleSlotOccuppeElement;

typedef struct SlotOccuppeList {
	SingleSlotOccuppeElement *head;
	SingleSlotOccuppeElement *tail;
	int Size;
}SlotOccuppeList;
