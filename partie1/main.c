#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <raylib.h>


typedef struct partition{
    int adr;
    int taille;
    int etat;
}partition;
//etat 1 occupe ,0 libre
typedef struct node {
  struct partition data;
  struct node *svt;
}node;

typedef struct processus{
   int id; //identite
   float ia;  //instant arrive
   float te; //temps execution
   int taille;
}processus;

typedef struct nodeF{
   struct processus data;
   struct nodeF *svt;
}nodeF;
//liste de partitions
typedef  node* liste;
typedef struct file{
  nodeF* tete;
  nodeF* queue;
}file;
typedef struct tab{
    int id;
    struct node* affect;
    struct tab* svt;
}tab;




////////////////////////////////////////////////////////typedefs of part 2////////////////////////////////////////////////////////////////




//processus avec priorite
typedef struct process{
   int id; //identite
   float ia;  //instant arrive
   float te; //temps execution
   int taille;
   int prio;
}process;

//noeud de file de processus de priorite
typedef struct nodeFP{
   struct process data;//information
   struct nodeFP *svt;
}nodeFP;

//file de pile
typedef struct fileP{
  nodeFP* tete;
  nodeFP* queue;
}fileP;

//cellule de pile de file avec priorite
typedef struct elem{
int prio;
struct fileP F;
struct elem* svt;
}elem;

typedef  elem* Pile;




///////////////////////////////////////////////////////////functions part 2/////////////////////////////////////////////////////////



// Initialiser la pile (créer une pile vide)
void InitPile(Pile* pile) {
    *pile = NULL;
}

// Vérifier si la pile est vide
int PileVide(Pile pile) {
    if(pile==NULL)
        return 1;
    else
        return 0;
}


// Empiler un élément sur la pile
void Empiler(Pile  *pile,fileP f, int n) {
   //allocation et initialisation des attributs du nouvel element
    elem* nouvelElem = (elem*)malloc(sizeof(elem));
    nouvelElem->prio=n;
   nouvelElem->F.queue=f.queue;
   nouvelElem->F.tete=f.tete;
    //mise a jour pointeur de pile
    nouvelElem->svt = *pile ;
    *pile = nouvelElem ;

}

// Dépiler un élément de la pile
elem Depiler(Pile* pile) {
     elem f;

     if (*pile == NULL) {
        // pile vide, retourner un element avec des atributs nul
         f.prio = 0;
        f.F.tete = NULL;
        f.F.queue = NULL;
        return f;
    }
    //recuperer les attribut du sommet de pile
     f.F.queue= (*pile)->F.queue;
     f.F.tete=(*pile)->F.tete;
     f.prio=(*pile)->prio;
    elem* temp = (*pile)->svt;
//mise a jour du pointeur et liberer la memoire
    free(*pile);
    *pile=temp;
    printf("pile apres %p", *pile);
    return f;
}





// Obtenir le sommet de la pile sans le dépiler
elem SommetPile(Pile* pile) {
     elem r;
     r.F.tete=(*pile)->F.tete;
     r.F.queue=(*pile)->F.queue;
     r.prio=(*pile)->prio;
    return r;
}


//primitives de fileP  (file de processus a priorite)
void initfileP(fileP *f){
  f->tete=NULL;
  f->queue=NULL;

}

//ajouter un processus a la file de processus de priorite
void enfilerP(fileP *f , process x ){
   //allocation et initialisation du nouvel element
    nodeFP* newf=(nodeFP*)malloc(sizeof(nodeFP));
    newf->data.id = x.id;
    newf->data.ia=x.ia;
    newf->data.te= x.te;
    newf->data.prio=x.prio;
    newf->data.taille=x.taille;
    newf->data.prio=x.prio;
   //mise a jour du chainage a la queu de file
   newf->svt=NULL;
    //si la file est vide , la tete et la queu pointent vers le nouvel element
  if ( f->queue == NULL) {
        f->tete=newf; f->queue=newf; }
   else{ f->queue->svt=newf; f->queue=newf;  };

}


//supprimer et retourner le premier processus de la fileP
process defilerP(fileP *f ){
    nodeFP* temp;
    process x;
   if (f->tete == NULL) {
        // fileP vide, retourner un process avec tout les attributs mis a  0
        x.id = 0;
        x.ia = 0.0;
        x.te = 0.0;
        x.taille = 0;
        x.prio = 0;
        return x;
    }
   //mise a jour du pointeur de tete de file et liberer l'element
    temp = (f->tete);
    x.ia=((f->tete)->data).ia;
    x.id=((f->tete)->data).id;
    x.te=((f->tete)->data).te;
    x.taille=((f->tete)->data).taille;
    x.prio=((f->tete)->data).prio;
    f->tete=(f->tete)->svt;
    if(f->tete==NULL){ f->queue=NULL;  }

    free(temp);
    return x;
}

//retourner le premier element de la file sans le supprimer
 process TeteFileP(fileP *f){
  return ((f->tete)->data);
 }

//verifier si la fileP est vide
int FileVideP(fileP *f){
      if(f->tete==NULL) {return 1;}  else{ return 0;}
}


//affiche la file de process
void AffichefileP(fileP* f){
       fileP tempQueue;
    initfileP(&tempQueue);

    if (FileVideP(f) == 1) {
        printf("La file est vide .\n");
    } else {
        while (FileVideP(f) != 1) {
            process x = defilerP(f);
            printf("_____________________________________________________________________________________________\n");
            printf("|Id= %d  | instant d'arrive=%0.2f s |temps d'execution=%0.2f s | taille= %d ko prio=%d       |\n", x.id, x.ia, x.te, x.taille, x.prio);
            enfilerP(&tempQueue, x);
        }
        *f = tempQueue;
    }
printf("\n \n \n ");
}

//affiche la pile de file
void affichepile(Pile *pile){
        Pile temp;
    InitPile(&temp);

     for(int z=3; z>0; z--){
       printf(" ----------------affichage de la pile de priorite : %d-----------------\n ", z);
         elem depp=Depiler(pile);

            AffichefileP(&depp.F);
            Empiler(&temp, depp.F, depp.prio);
        }

          for(int z=3; z>0; z--){
         elem depp=Depiler(&temp);
            Empiler(pile, depp.F, depp.prio);
        }




    printf("\n\n");
}


//creer une file de processus avec priorite
fileP createfileP(int n, int j, int posdep){
  srand(time(NULL));
  float r_ia=0;
  float r_te;
  fileP F;
  process p;
   initfileP(&F);

  for (int  i=posdep+1 ; i<=n+posdep ; i++ ){
    p.id=i;
    r_ia=r_ia+( rand() % 12 + 0);
    p.ia=r_ia ;
    r_te= rand() % 20 + 5;
    p.te=r_te;
    p.taille= rand()% 200*10;
    p.prio=j;
    enfilerP(&F, p);

      }
return (F);
}


//creer le reste de partition apres allocation si il existe
void createreste2(node* p , process x ){
 node* q=(node*)malloc(sizeof(node));
 q->data.etat=0;
 q->data.taille= p->data.taille - x.taille ;
 //mise a jour taille de p
 p->data.taille= x.taille;
 q->data.adr= (p->data.adr) + (p->data.taille);
 //chainage
 q->svt=p->svt;
 p->svt=q;

}

//allocation en memoire selon la politique Firstfit
void FirstfitP(liste L, fileP *F, int *n, fileP *h, tab **T) {
    // Initialize variables
    int cpt=0;
    liste p;
    int i = 0;
    process x;
    p = L;
    int trouve;
    tab *Q = NULL;

    // boucler pour chaque processus de la file
    while (i < *n) {
        x = defilerP(F);
        enfilerP(h, x);
        p = L;
        trouve = 0;

        // verifier si l'espace est disponible selon la politique FirstFit
        while (p != NULL && trouve == 0) {
            if (p->data.etat == 0 && p->data.taille >= x.taille) {
                Q = (tab *)malloc(sizeof(tab));
                Q->id = x.id;
                Q->affect = p;
                Q->svt = *T;
                *T = Q;
                trouve = 1;
                p->data.etat = 1;

                if (p->data.taille > x.taille) {
                    createreste2(p, x);
                }
            }
            p = p->svt;
        }

        // si pas d'espaces, enfiler le process de nouveau dans sa file
        if (trouve == 0) {
            enfilerP(F, x);
            cpt++; // Incrementer le nombre de processecus non alloue
        }
        i++;
    }
*n=cpt;
}


//allocation en memoire selon la politique BestFit
void BestFit2(liste L, fileP *F , int* n , fileP *h, tab** T){
//initialisations
 int cpt=0;
 liste p;
 int i=0;
 liste pmin;
 process x;
 p=L;
 tab* Q=NULL;

  // boucler pour chaque processus de la file
 while(i< *n )
 { x=defilerP(F);  printf("defilina w enfilina \n");
   enfilerP(h, x);
    pmin=NULL;
   p=L;
   while(p!=NULL){
    if( (p->data.etat== 0 ) && ((p->data.taille)>=x.taille) ){
        if(pmin==NULL){pmin=p;   }
        if(pmin!=NULL &&((p->data.taille)<=(pmin->data.taille))){pmin=p; }
        }

       p=p->svt;
    }

   if(pmin!= NULL){
         //creer le lien d'affectation dans la table des affectations
         Q=(tab*)malloc(sizeof(tab));
                   Q->id=x.id; Q->affect=pmin;
                   Q->svt=*T; *T=Q;

                  pmin->data.etat=1;
                  //creer le risidue de partition si taille supeerieur
                if ((pmin->data.taille)>x.taille ){
                        createreste2( pmin , x );}
                        }

       //si pas d'espace , remmetre en queu de file
  if(pmin==NULL){ enfilerP(F, x); cpt++;  }
 i++;
 }
*n=cpt;
}


//allocation en memoire selon la politique WorstFit
void WorstFit2(liste L, fileP *F , int *n , fileP *h, tab** T){

 //initialisation
 int cpt=0;
 liste p;
 int i=0;
 liste pmax;
 process x;
 p=L;
tab* Q=NULL;

//boucler pour chaque processus
  while( i< *n )
   { x=defilerP(F);
     enfilerP(h, x);
      pmax=NULL;
      p=L;
   //verifier la disponibilite de l'espacce selon la politique WorstFIt
   while(p!=NULL){
    if( (p->data.etat== 0 ) && ((p->data.taille)>=x.taille) ){
        if(pmax==NULL){pmax=p;  }
        if(pmax!=NULL&&((p->data.taille)>=(pmax->data.taille))){pmax=p;}
         }

    p=p->svt;
   }
   if(pmax!= NULL){
               //allocation dans la table des allocations
               Q=(tab*)malloc(sizeof(tab));
                   Q->id=x.id; Q->affect=pmax; printf("tc id = %d  tc affect= %p \n", Q->id , Q->affect);
                   Q->svt=*T; *T=Q;//affectation
                  //mise a jour etat de la partition
                  pmax->data.etat=1;
                  //creation du reste de partition si besoin
                  if ((pmax->data.taille)>x.taille ){
                        createreste2( pmax , x );}
                  }
  // si pas d'espace disponible , remmetre en queue de file
  if(pmax==NULL){ enfilerP(F, x); cpt++;  }
   i++;
   }
*n=cpt;
}








/////////////////////////////////////////////////////fonctions part 1////////////////////////////////////////////////////////////////////////
















//creation de la liste de partitons
liste Listepar(){
    srand(time(NULL));
    int i;
    liste L = NULL ;   //initialiser la tete a nill
    liste p,q;
    //creation du premier element
    L=(node*)malloc(sizeof(node));

    //remplicage de premier element
    L->data.adr = rand() % 100 +1;
    do{L->data.taille = (rand()%200)*10; }while(L->data.taille == 0); //si taille=0 on cherche une nouv valeur
    L->data.etat= 0;
    p = L;//p pointe sur la tete
    q = L;//q pointe sur la tete

    for(i=0;i<15;i++){ //boucle de creation des elements
             p=(node *)malloc(sizeof(node));
             p->data.adr= q->data.adr + q->data.taille;
              do{p->data.taille = ( rand()%200 )*10 ; }while(p->data.taille == 0);
             p->data.etat= 0;
             q->svt = p; //chainage d'element
             q=p;
    }
    p->svt = NULL; //dernier element n'a pas un svt
    return(L);
}


//afficher la liste de partitions
 void affichageListepar(liste L){
    liste p;
    p = L;
    while(p != NULL){
            printf("............................................................\n");
            printf(". adresse:%d ko  ||   taille:%d ko  ||   etat:   %d       ||\n", p->data.adr, p->data.taille , p->data.etat);
            p = p->svt;
    }
    printf("\n \n \n ");
}



//initialisation de la file
void initfile(file *f){
  f->tete=NULL;
  f->queue=NULL;

}

//enfiler un processus dans chaque file
void enfiler(file *f , processus x ){
   //allocation en memoire et initialisation
    nodeF* newf=(nodeF*)malloc(sizeof(nodeF));
    newf->data.id = x.id;
    newf->data.ia=x.ia;
    newf->data.te= x.te;
    newf->data.taille=x.taille;
   newf->svt=NULL;
   if ( f->queue == NULL) {f->tete=newf; f->queue=newf;}//file vide
   else{ (f->queue)->svt=newf; f->queue=newf; };

}

processus defiler(file *f ){
    nodeF* temp;
    processus x;
    if (f->tete == NULL) {
    //creer un processus a 0 si vide pour le retourner
        processus emptyProcessus;
        emptyProcessus.ia = 0;
        emptyProcessus.id = 0;
        emptyProcessus.te = 0;
        emptyProcessus.taille = 0;
        return emptyProcessus;
    }
    temp = (f->tete);
    x.ia=((f->tete)->data).ia;
    x.id=((f->tete)->data).id;
    x.te=((f->tete)->data).te;
    x.taille=((f->tete)->data).taille;
    f->tete=(f->tete)->svt;
    if(f->tete==NULL){ f->queue=NULL;  }
    free(temp);
    return x;
}


 processus TeteFile(file *f){
  return ((f->tete)->data);
 }


int FileVide(file *f){
      if(f->tete==NULL) {return 1;}  else{ return 0;}
}
//affiche la file
void Affichefile( file* f) {
       file tempQueue;
    initfile(&tempQueue);

    printf("~~~~~~~~~~~~ Displaying the Queue ~~~~~~~~~~~~\n\n");

    while (FileVide(f) != 1) {
        processus x = defiler(f);
        printf("*****************************************************************************\n");
        printf("* id = %d * instant d'arrive = %0.2f * temps d'exe = %0.2f * taille = %d *\n", x.id, x.ia, x.te, x.taille);
        enfiler(&tempQueue, x);
    }

    while (FileVide(&tempQueue) != 1) {
        processus x = defiler(&tempQueue);
        enfiler(f, x);
    }

    printf("\n\n");
}


file createfile(int n){
  srand(time(NULL));
  float r_ia=0;
  float r_te;
  file F;
  processus p;
   initfile(&F);
  for (int i=1; i<=n ; i++ ){
    p.id=i;
    r_ia=r_ia+( rand() % 12 + 0);
    p.ia=r_ia ;
    r_te= rand() % 20 + 5;
    p.te=r_te;
    p.taille= rand()% 200*10;
    enfiler(&F, p);
      }
return (F);
}




//creer une nouvelle partition avec le risidu apres affectation
void createreste(node* p , processus x ){
 node* q=(node*)malloc(sizeof(node));

 //allocation du nouveau noeud et remplissage des champs
 q->data.etat=0;
 q->data.taille= p->data.taille - x.taille ;
 p->data.taille= x.taille;//mise a jour taille de p

 q->data.adr= (p->data.adr) + (p->data.taille);


 //chainage
 q->svt=p->svt;
 p->svt=q;

}


void BestFit(liste L, file *F ,int *n, file *h, tab** T){
 //initialisations
        int cpt=0;
        liste p;
        int i=0;
        liste pmin;
        processus x;
        p=L;




        tab* Q=NULL;
 while(i<*n )//pour chaque element de la file
 { x=defiler(F);
   enfiler(h, x);
    pmin=NULL;      //initialiser min
   p=L;
   while(p!=NULL){
    if( (p->data.etat== 0 ) && ((p->data.taille)>=x.taille) ){
        if(pmin==NULL){pmin=p;   }
        if(pmin!=NULL &&((p->data.taille)<=(pmin->data.taille))){pmin=p; }
        }

       p=p->svt;
    }

   if(pmin!= NULL){
         Q=(tab*)malloc(sizeof(tab));
                   Q->id=x.id; Q->affect=pmin;
                   Q->svt=*T; *T=Q;
                   //mise a jour de la table des affectations
                  pmin->data.etat=1;   //mettre la partition a  occupe
                if ((pmin->data.taille)>x.taille ){
                        createreste( pmin , x );}
                  }
  if(pmin==NULL){ enfiler(F, x); cpt++;  }
 i++;}
*n=cpt;
}

void Firstfit(liste L, file *F, int *n, file *h, tab **T) {
    // Initializations
    int cpt = 0;
    liste p;
    int i = 0;
    file r;
    processus x;
    p = L;
    initfile(h);
    initfile(&r);
    int trouve;

    tab *Q = NULL;
    printf("Starting the loop");

    while (i < *n) {
        x = defiler(F);
        enfiler(h, x);
        p = L;
        trouve = 0;

        // Find the first empty partition with sufficient space
        while (p != NULL && trouve == 0) {
            if ((p->data.etat == 0) && ((p->data.taille) >= x.taille)) {
                // Update the assignment table
                Q = (tab *)malloc(sizeof(tab));
                Q->id = x.id;
                Q->affect = p;
                Q->svt = *T;
                *T = Q;
                trouve = 1;
                p->data.etat = 1;
                if ((p->data.taille) > x.taille) {
                    createreste(p, x);
                }
            }
            p = p->svt;
        }

        if (trouve == 0) {
            enfiler(F, x);
            cpt++;
        }
        i++;
    }

    *n = cpt;
}


void WorstFit(liste L, file *F , int* n , file *h ,tab **T){

 //initialisations
       liste p;
       int i=0;
       liste pmax;
       processus x;
       p=L;
       initfile(h);
       tab* Q=NULL;
       int cpt=0;

 while( i< *n )
 {

   x=defiler(F); printf("on defile f  et x.id= %d x.taille= %d\n", x.id, x.taille);
   enfiler(h, x);
   pmax=NULL;
   p=L;

    //recherche de la aprtition convenable selon la politique worstfit
   while(p!=NULL){
    if( (p->data.etat== 0 ) && ((p->data.taille)>=x.taille) ){ //trouver la partiton libre avec la plus grande taille
        if(pmax==NULL){pmax=p;  }
        if(pmax!=NULL &&((p->data.taille)>=(pmax->data.taille))){pmax=p;}
         }

    p=p->svt;
   }
   //mettre a jour la table des affectations
   if(pmax!= NULL){
               //affectation dans la table
               Q=(tab*)malloc(sizeof(tab));
                   Q->id=x.id; Q->affect=pmax ; printf("tc id = %d  tc affect= %p \n", Q->id , Q->affect);
                   Q->svt=*T; *T=Q;//affectation
                  pmax->data.etat=1;// devient occupe
                  //creer le risidu s'il y en as
                  if ((pmax->data.taille)>x.taille ){
                        createreste( pmax , x );}
                  }
  if(pmax==NULL){ enfiler(F, x); cpt++;  }
   i++;
   }
*n=cpt;
}

//afficher la table des affectations
void affichtab(tab *T  ){
   tab* q=T;
   printf("\t affichage de la table des affectations:\n");

   if(q==NULL){printf("Aucun processus affecte");}
else{
   while(q!=NULL){
        printf("---------------------------------------------------\n");
        printf(" | processus %d  est affecte a l'adresse: %d ko | \n", q->id, q->affect->data.adr);
        q=q->svt;
   }}
  }


//supression du processus , on casse le lien de l'affectation et met a jour la table
void supress(tab *e, liste L){
    node*p = e->affect;
    node* q =L;

    //recherche de l'adresse de la partition auquel le processus execute a ete affecte
    while(q!= p){
         q=q->svt;
        }  //Mise a jour de cette partiton
        if(q!=NULL){q->data.etat=0;}
            //si partition suivante est libre , fusionner pour creer partition plus grande
       node* suiv=q->svt;
       if (suiv->data.etat==0){q->data.taille=q->data.taille + suiv->data.taille; q->svt=suiv->svt; free(suiv);printf("we freed q.svt taille de q jdida %d\n", q->data.taille);}
        //liberer l'element de la table des affectations
        free(e); printf("we freed e too \n");
}

//recherche du processus arrive en premier pour l'executer et le detache de la table des affectations
tab* rech_ptit_id(tab** T) {
    tab* q;
    tab* p = *T;
    tab* pmin = *T;

    // Find the process with the smallest id among those assigned to partitions
    while (p != NULL) {
        if (p->id < pmin->id) {
            pmin = p;
        }
        p = p->svt;
    }

    p = *T;
    q = *T;

    // Detach the process from the list of assignments
    while (p != NULL && p != pmin) {
        q = p;
        p = p->svt;
    }

    if (p == *T) {
        *T = (*T)->svt;
        p->svt = NULL;
    } else {
        q->svt = p->svt;
        p->svt = NULL;
    }

    // Check if p is the last element and set T to NULL
    if (p == *T && p->svt == NULL) {
        *T = NULL;
        printf("T has been set to NULL   t= %p ", *T);
    }

    printf("Process to execute: %d", p->id);
    return p;
}


//rearanger les partition apres chaque supression de processus si il y a moyen
void decalage(liste L,tab * T){
    node * q;
    node * p;

    tab * t;
    q = L;
    p = L;

    while (q != NULL){

        if(q->data.etat == 1){//partiton occupee trouvee
            //recherche d'une partition entre elle et la partiton occupe  precedente ET qui est libre et de taille suffisante
            while( (p->svt!=q)&&(p->data.taille)<(q->data.taille)){ p = p->svt;}
            if((p->data.etat==0)&&(p->data.taille)>=(q->data.taille)){

                p->data.etat = 1; //rendre la partiton dans laquelle on va decaler occupee
                q->data.etat = 0;//remettre l'etat de la partiton a libre

                processus y;//creer un processus temporaire qui a la meme taille pour le passer en parametre
                y.ia=0; y.id=0; y.te=0;
                y.taille=q->data.taille;
                createreste(p , y); //creer le residu s'il existe

                //mettre a jour la table des affectations
                t = T;
                while((t != NULL)&&(t->affect != q)){
                    t = t->svt;}
                    t->affect = p;
                }
             p=p->svt; printf("l7e9na 2\n");
        }
     q = q->svt;//refaire pour toutes les partitions
    }
}


//fonction de dessin des partitions
void drawRectangles(liste list, tab* t) {
    const int rectangleWidth = 50;
    const int rectangleHeight = 40;
    const int startX = 30;
    const int startY = 200;
    const int spacing = 5;
     int i=1;
     int staart=startX +rectangleWidth + spacing;;
    liste current = list;
    while (current != NULL) {
        Rectangle rectangle = { startX + i * (rectangleWidth + spacing), startY, rectangleWidth, rectangleHeight };
        Color color = (current->data.etat == 0) ? GREEN : RED;
        DrawRectangleRec(rectangle, color);
        DrawText("adresses", startX -5, startY - 30, 12, BLUE);
        DrawText("tailles", startX -5 , startY - 45, 12, BLUE);

            char text[10];
            char text2[10];
            sprintf(text, "%d K", current->data.adr);
            sprintf(text2,  "%d K", current->data.taille );
            DrawText(text, startX + i * (rectangleWidth + spacing) , startY - 30, 16, BLUE);
            DrawText(text2, startX + i * (rectangleWidth + spacing) , startY - 45, 16, BLUE);


        staart += rectangleWidth + spacing;

        current = current->svt;
        i++;
    }
}


 void dessineeer(liste L , tab* t){
 const int screenWidth = 1200;
    const int screenHeight = 500;


SetTraceLogLevel(LOG_NONE);
    InitWindow(screenWidth, screenHeight, "Example");

    // Main game loop
    while (!WindowShouldClose()) {
        BeginDrawing();

        // Clear the background
        ClearBackground(RAYWHITE);

        // Draw rectangles based on the list
        drawRectangles(L, t);
         DrawText("Press Enter to start the rest of the program", 10, 300, 20, DARKGRAY);

        EndDrawing();
     //debuter le reste du programme une fois avoir appuiyer sur entrer
      if (IsKeyPressed(KEY_ENTER)) {

            break;  // Exit the loop to start the program
        }

    }
    // De-Initialization

    CloseWindow();

 }




//////////////////////////////////////////////////////////////main////////////////////////////////////////////////////////////////////////////////



int main(){

    printf("fait par: \n Bedrane Melissa et Abbas Imene \n \n \n \n \n ");
    int partie;
    int politique;
    liste L;
    fileP g;
    tab* table_2;
    int n;
    file f;
    file h;
    tab* m;
    tab *e;

    printf("veuillez choisir: \n 1-simulation sans priorite \n 2-simulation avec priorite \n");
    scanf("%d", &partie);


switch(partie){


 //sans priorites
  case 1:

 printf(".........................................................................\n");
 printf("debut de la simulation de l'allocation sans priorite : \n \n \n");

   //partie file

   printf("Veuillez donner le nombre de processsus : \n"); //n = nombre de processus
   scanf("%d", &n);
   f=createfile(n);
   Affichefile(&f);



    //partie liste

    L= Listepar();
    printf("l'affichage de la liste : \n");
    affichageListepar(L);
    printf("appuiez sur un bouton pour avoir l'affichage graphique\n");
        getchar();
        m=NULL;
        dessineeer(L, m);

  //choix de la politique
    printf("Veulliez choisir la politique d'affectation qui vous convient : \n 1-Firstfit 2-BestFit 3-WorstFit \n");
         scanf("%d",&politique);

        initfile(&h);
           switch(politique){

            case 1 :
                Firstfit(L, &f, &n, &h, &m);
                break;
            case 2 :  BestFit(L, &f, &n, &h, &m);
                break;
            case 3 : WorstFit(L, &f, &n , &h, &m);
                break;
            default : printf("vous n'avez pas choisi de politique convenable");
                    }



  getchar();
 affichageListepar(L);
 printf("Affichage de la file apres affectation \n");
 Affichefile(&f);


  affichtab(m );

  dessineeer(L, m);

   file r;

    do{      //recherche du processus a suprimer
             e=rech_ptit_id(&m);
            initfile(&r);
            processus proc;


            //recherche du processus dans la copie de la file afin d'avoir son temps d'execution
             while((!FileVide(&h)) ){
            proc=defiler(&h);
            if(proc.id==e->id){ break;}
            enfiler(&r, proc);
            }
            printf("execution du processus en cours..\n");
            supress(e, L);
            proc.te=(int)proc.te %60;

            //attendre l'execution
            fflush(stdout);
            sleep(proc.te);
            fflush(stdout);
           //re-remplir la file
            if(!FileVide(&h)){
            while(!FileVide(&h) ){
            proc=defiler(&h);
            enfiler(&r, proc);
            }}

            h=r;


            printf("Fin d'execution ,processus supprime..\n");
            //affichage des elements apres execution

            affichtab(m ); //affichage de la table
            affichageListepar(L);//affichage de la liste de partition
             printf("    Appuiyez sur un bouton pour avoir l'affichage de la liste apres supression\n");
             getchar();
            dessineeer(L,m);
               printf("   affichage de la copie de la file apres supression \n ");
            Affichefile(&h);
            //rearanger les partiton et affichages des elements
            decalage(L, m);
            affichageListepar(L);
             printf("  Appuiez sur un bouton pour voir l'etat de la liste apres rearangement:\n");
             getchar();
            dessineeer(L,m);



                       //allouer les processus restants s'il y en a
                         if(!FileVide(&f)){
                                switch(politique){

                                          case 1 :  Firstfit(L, &f, &n , &h, &m);
                                                  break;
                                          case 2 :  BestFit(L, &f, &n,&h, &m);
                                                  break;
                                          case 3 :  WorstFit(L, &f, &n , &h, &m);
                                                  break;
                                          }

                         }


                            }while (!FileVide(&h));


                printf("execution termine!");
                   break;
                // fin de la partie 1

 //partie avec priorite

 case 2 :


 printf(".........................................................................\n");
 printf("debut de la simulation de l'allocation avec priorite : \n \n \n");



      initfileP(&g);
      Pile pile;
      InitPile(&pile);
      int posdeb=0;
      int nbr3, nbr2, nbr1;
      table_2=NULL;
           //creation et affichage de la liste
           L= Listepar();
           affichageListepar(L);
           printf("appuiez sur un bouton pour avoir l'affichage graphique\n");
           getchar();
           dessineeer(L, table_2);

           //remplissage de la pile selon les priorites
                 printf("donnez le nombre de processes de priorite 1 \n");
                 scanf("%d", &nbr1);
                 g=createfileP(nbr1, 1, posdeb);
                 posdeb=g.queue->data.id;
                 AffichefileP(&g);
                 Empiler(&pile,g, 1);

                 //remplir les files une a une


                 printf("donnez le nombre de processes de priorite 2 \n");
                 scanf("%d", &nbr2);
                 g=createfileP(nbr2, 2, posdeb);
                 posdeb=g.queue->data.id;
                 AffichefileP(&g);
                 Empiler(&pile,g, 2);

                 printf("donnez le nombre de processes de priorite 3 \n");
                 scanf("%d", &nbr3);
                 g=createfileP(nbr3, 3, posdeb);
                 AffichefileP(&g);
                 Empiler(&pile,g, 3);
                   printf("encore file \n ");




   //affichage de la pile
                           affichepile(&pile);



        //choix des politique
        printf("Veulliez choisir la politique d'affectation qui vous convient : \n 1-Firstfit 2-BestFit 3-WorstFit \n");
        scanf("%d",&politique);

         elem element;
         elem element_inter;

         initfileP(&element_inter.F);
         initfileP(&element.F);
         int count;
         int nbr=0;
         printf("\n pile est %d", PileVide(pile));
         element_inter.F.tete=NULL;

            do{  //depiler le sommet de la pile
                   elem p=Depiler(&pile);

                    //nombre de processus pour chaque file de priorite
                    switch(p.prio){
                                         case 1: nbr= nbr+nbr1;  nbr1=nbr; break;
                                         case 2: nbr= nbr+nbr2;  nbr2=nbr; break;
                                         case 3: nbr=nbr+nbr3;  nbr3=nbr; break;
                                         }

                   //remettre les processus de taille superieur non allouer en queu de file de la taille inferieur suivante
                   if(element_inter.F.tete!= NULL){  element_inter.F.queue->svt=p.F.tete; p.F.tete=element.F.tete;}
     switch (politique){
            case 1 : FirstfitP(L, &p.F, &nbr, &element.F ,&table_2); break;
            case 2 : BestFit2(L, &p.F, &nbr, &element.F ,&table_2); break;
            case 3 : WorstFit2(L, &p.F, &nbr, &element.F ,&table_2); break;
            default : printf("vous n'avez pas choisi de politique convenable");}
            //initialiser le compteur avec le nombre de processus affecte
             switch(p.prio){
                                         case 1: count=nbr1-nbr; break;
                                         case 2: count=nbr2-nbr; break;
                                         case 3: count=nbr3-nbr; break;
                                         }

               printf(" \n affichage de la table des affectations apres allocation : \n \n");

               affichtab(table_2 );
                    printf("appuiez sur un bouton pour avoir l'affichage graphique\n");
                getchar();
                dessineeer(L, table_2);
        while(count>0){ printf("\n iteration numero %d \n", count);
           fileP ri;
            e=rech_ptit_id(&table_2); printf("priorite =%d \n", p.prio);
            initfileP(&ri);
            process pro;
              while((!FileVideP(&element.F)) ){
            pro=defilerP(&element.F);
            printf("on defile et enfile \n");
            if(pro.id==e->id){ break;}
            enfilerP(&ri, pro);
            }
            printf("execution du processus en cours..\n");
            //supression du processus execute
            supress(e, L);
            pro.te=(int)pro.te %60;
            fflush(stdout);
            sleep(pro.te);
            fflush(stdout);
           printf("\n file vide h apres sleep %d", FileVideP(&element.F));
            if(!FileVideP(&element.F)){
            while(!FileVideP(&element.F) ){
            pro=defilerP(&element.F);
            enfilerP(&ri, pro);
            printf("on continue de defiler \n");
            }}

            element.F.tete=ri.tete;
            element.F.queue=ri.queue;
            printf("\n file vide h apre remplissage de r %d", FileVideP(&element.F));
              printf("Fin d'execution ,processus supprime..\n");

            //afficage apres la supression
            affichtab(table_2 ); //affichage de la table
            affichageListepar(L);//affichage de la liste de partition
             printf("Appuiyez sur un bouton pour avoir l'affichage de la liste apres supression\n");
             getchar();
            dessineeer(L,table_2);

            //declage et affichage des elements apres decalage
            decalage(L, table_2);
            affichageListepar(L);
             printf("Appuiez sur un bouton pour voir l'etat de la liste apres rearangement:\n");
             getchar();
            dessineeer(L,table_2);
            count--; //decrementer le compteur
        };

              element_inter.F.tete=p.F.tete;
              element_inter.F.queue=p.F.queue;

     }while(!PileVide(pile));
     printf("la pile est enfin vide\n");

       printf("fin execution\n");

       break;
        }




/*Pile pile;
InitPile(&pile);
printf("pile vide= %d \n \n", PileVide(pile));
for(int k=1;k<=3;k++){
  printf("donnez le nombre de processes de priorite %d \n", k);
  int nbr;
  scanf("%d", &nbr);
  //fileP placeholder;
g=createfileP(nbr, k);
AffichefileP(g);
//placeholder.queue=g.queue;
//placeholder.tete=g.tete;
printf("g.tete= %p et g.queu= %p \n", g.tete, g.queue);
Empiler(&pile,g, k);
printf("dans main pile= %p", pile);
printf("pile vide= %d \n \n", PileVide(pile));


}

affichepile(pile);*/


    return 0;

}

