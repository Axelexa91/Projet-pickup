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
//Liste caînée des slots (caractéristiques)

int Capacite(int P_article,int L_article, int H_article, int P_boite, int L_boite, int H_boite);
int RecCapacite(int P_article, int L_article, int H_article, int P_boite, int L_boite, int H_boite);