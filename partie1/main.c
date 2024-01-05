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





typedef struct process{
   int id; //identite
   float ia;  //instant arrive
   float te; //temps execution
   int taille;
   int prio;
}process;

typedef struct nodeFP{
   struct process data;
   struct nodeFP *svt;
}nodeFP;

typedef struct fileP{
  nodeFP* tete;
  nodeFP* queue;
}fileP;

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
    elem* nouvelElem = (elem*)malloc(sizeof(elem));
    nouvelElem->prio=n;
   nouvelElem->F.queue=f.queue;
   nouvelElem->F.tete=f.tete;
    nouvelElem->svt = *pile ;
    *pile = nouvelElem ;

}

// Dépiler un élément de la pile
elem Depiler(Pile* pile) {
     elem f;

     if (*pile == NULL) {
        // Stack is empty, return an element with all attributes set to 0
        f.prio = 0;
        f.F.tete = NULL;
        f.F.queue = NULL;
        return f;
    }
     f.F.queue= (*pile)->F.queue;
     f.F.tete=(*pile)->F.tete;
     f.prio=(*pile)->prio;
    elem* temp = (*pile)->svt;
    //*pile = temp->svt;
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


///prefonctions de fileP

void initfileP(fileP *f){
  f->tete=NULL;
  f->queue=NULL;

}

void enfilerP(fileP *f , process x ){
    nodeFP* newf=(nodeFP*)malloc(sizeof(nodeFP));
    newf->data.id = x.id;
    newf->data.ia=x.ia;
    newf->data.te= x.te;
    newf->data.prio=x.prio;
    newf->data.taille=x.taille;
    newf->data.prio=x.prio;
   newf->svt=NULL;
   if ( f->queue == NULL) {
        f->tete=newf; f->queue=newf; }//file vide
   else{ f->queue->svt=newf; f->queue=newf;  };

}

process defilerP(fileP *f ){
    nodeFP* temp;
    process x;
   if (f->tete == NULL) {
        // Queue is empty, return a process with all attributes set to 0
        x.id = 0;
        x.ia = 0.0;
        x.te = 0.0;
        x.taille = 0;
        x.prio = 0;
        return x;
    }
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


 process TeteFileP(fileP *f){
  return ((f->tete)->data);
 }


int FileVideP(fileP *f){
      if(f->tete==NULL) {return 1;}  else{ return 0;}
}
//affiche et affecte le tout dans r puis f=r
void AffichefileP(fileP f){
    process x;
    fileP r;
    printf("debut affichage\n");
    initfileP(&r);
    if (FileVideP(&f)==1){printf("file vide");}
  else{  while (FileVideP(&f)!=1){
        x=defilerP(&f);
        printf("id= %d  ia=%0.2f te=%0.2f taille= %d prio=%d \n", x.id , x.ia , x.te , x.taille , x.prio);

        enfilerP(&r , x );
    }
  f=r;}
}

void affichepile(Pile p){
     Pile r;
     elem x;
     InitPile(&r);
     printf("%d", PileVide(p));
     while(!PileVide(p)){
        x=Depiler(&p); printf("depilina\n");
        printf("x.prio=%d , x.F.tete= %p , x.F.queu=%p ", x.prio, x.F.tete ,x.F.queue);
        Empiler(&r, x.F , x.prio);
        printf("la file est : \n");
        AffichefileP(x.F);
     }
     p=r;
}
//posdep=id lekhr tea queu
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


void createreste2(node* p , process x ){
 node* q=(node*)malloc(sizeof(node)); printf("creation node rest \t");
 q->data.etat=0;
 q->data.taille= p->data.taille - x.taille ;
 p->data.taille= x.taille;//mise a jour taille de p
 q->data.adr= (p->data.adr) + (p->data.taille);
 //chainage
 q->svt=p->svt;
 p->svt=q;

}
void FirstfitP(liste L, fileP *F, int *n, fileP *h, tab **T) {
    // Initialize variables
    int cpt=0;
    liste p;
    int i = 0;
    process x;
    p = L;
    int trouve;
    tab *Q = NULL;

    // Loop through each process in the file
    while (i < *n) {
        x = defilerP(F);
        enfilerP(h, x);
        p = L;
        trouve = 0;

        // Check for available space in the list
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

        // If no space is available, enqueue the process back to the file
        if (trouve == 0) {
            enfilerP(F, x);
            cpt++; // Increment the number of processes not allocated
        }
        i++;
    }
*n=cpt;
}

void BestFit2(liste L, fileP *F , int* n , fileP *h, tab** T){
//initialisations
 int cpt=0;
 liste p;
 int i=0;
 liste pmin;
 process x; printf("l= %p", L);
 p=L;printf("p= %p", p);
 tab* Q=NULL;

 while(i< *n )//normalement i use something like boucle pour cuz i know num of elements
 { x=defilerP(F);  printf("defilina w enfilina \n");
   enfilerP(h, x);
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
                   Q->id=x.id; Q->affect=pmin; printf("tc id = %d  tc affect= %p et q=%p \n", Q->id , Q->affect, Q);
                   Q->svt=*T; *T=Q;
                   //affectation
                  pmin->data.etat=1;   //1 c  occupe
                if ((pmin->data.taille)>x.taille ){
                        createreste2( pmin , x );}
                  }
  if(pmin==NULL){ enfilerP(F, x); cpt++;  }
 i++;
 }
*n=cpt;
}

void WorstFit2(liste L, fileP *F , int *n , fileP *h, tab** T){
 //initialisation
 int cpt=0;
 liste p;
 int i=0;
 liste pmax;
 process x; printf("l= %p", L);
 p=L;printf("p= %p", p);
 tab* Q=NULL;
 while( i< *n )
 { x=defilerP(F);
   enfilerP(h, x);
    pmax=NULL;
    p=L;
   while(p!=NULL){
    if( (p->data.etat== 0 ) && ((p->data.taille)>=x.taille) ){
        if(pmax==NULL){pmax=p;  }
        if(pmax!=NULL&&((p->data.taille)>=(pmax->data.taille))){pmax=p;}
         }

    p=p->svt;
   }
   if(pmax!= NULL){
               Q=(tab*)malloc(sizeof(tab));
                   Q->id=x.id; Q->affect=pmax; printf("tc id = %d  tc affect= %p \n", Q->id , Q->affect);
                   Q->svt=*T; *T=Q;//affectation
                  pmax->data.etat=1;//1 devient occupe
                  if ((pmax->data.taille)>x.taille ){
                        createreste2( pmax , x );}
                  }
  if(pmax==NULL){ enfilerP(F, x); cpt++;  }
   i++;
   }
*n=cpt;
}








/////////////////////////////////////////////////////fonctions part 1////////////////////////////////////////////////////////////////////////

















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
 void affichageListepar(liste L){
    liste p;
    p = L;
    while(p != NULL){
            printf("...........................................................\n");
            printf("adresse:%d ko  ||   taille:%d ko  ||   etat:   %d   ||\n", p->data.adr, p->data.taille , p->data.etat);
            printf("...........................................................\n");
            p = p->svt;
    }
    printf("\n \n \n ");
}




//Procedure InitFile(E/S/ F:File);
void initfile(file *f){
  f->tete=NULL;
  f->queue=NULL;

}

void enfiler(file *f , processus x ){
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
        // Queue is already empty, handle the error or return a default value
        // For example, you can throw an error or return a default processus object.
        // Here, let's assume we return a default processus object with all values set to 0.
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
void Affichefile(file f){
    processus x;
    file r;
    printf("~~~~~~~~~~~~debut de l'affichage de la File de processus:~~~~~~~~~ \n \n");
    initfile(&r);
    while (FileVide(&f)!=1){
        x=defiler(&f);
        printf("**************************************************************************\n");
        printf("* id = %d * instant d'arrive =%0.2f * temps d'exe =%0.2f * taille= %d * \n", x.id , x.ia , x.te , x.taille );

        enfiler(&r , x );
    }
    f=r;
    printf("\n \n \n");
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
        file *r;
printf("to here");


        tab* Q=NULL;
 while(i<*n )//pour chaque element de la file
 { x=defiler(F); printf("on defile");
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
printf("dkhelna worst fit\n");
printf("n= %d", *n);
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
 { printf("i<n\n"); printf("F tete= %p x.id= %d", F->tete, F->tete->data.id);
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
*n=cpt; printf("new n = %d", *n);
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
    node* pres=q;
    //recherche de l'adresse de la partition auquel le processus execute a ete affecte
    while(q!= p){
        pres=q; q=q->svt;
        }  //Mise a jour de cette partiton
        if(q!=NULL){q->data.etat=0;}
            //si partition precedente libre , les ccoller pour creer partition plus grande
            //if (pres->data.etat==0){pres->data.taille= pres->data.taille + q->data.taille; pres->svt=q->svt; free(q);}
            //sinon , si la suivante est libre :faire de meme
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
                    t->affect = p; printf("l7e9na hna\n");
                }
             p=p->svt; printf("l7e9na 2\n");
        }
     q = q->svt;//refaire pour toutes les partitions
    }
}


//fonction de dessin des partitions
void drawRectangles(liste list, tab* t) {
    const int rectangleWidth = 50;
    const int rectangleHeight = 50;
    const int startX = 30;
    const int startY = 200;
    const int spacing = 10;
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
         DrawText("Press Enter to start the restof the program", 10, 300, 20, DARKGRAY);

        EndDrawing();
      if (IsKeyPressed(KEY_ENTER)) {
            // Start the rest of the program here
            // Add your code to start the program after the Enter key is pressed
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


printf("veuillez choisir: \n 1-simulation sans priorite \n 2-simulation avec priorite \n");
    scanf("%d", &partie);
switch(partie){

     fileP g;
fileP H;
tab* table_2;

 int n;
   file f;
   file h;
   tab* m;
   tab *e;
  case 1:
 //partie file


   printf("Veuillez donner le nombre de processsus : \n"); //n = nombre de processus
   scanf("%d", &n);
   f=createfile(n);
   Affichefile(f);



//partie liste

    L= Listepar();
    printf("l'affichage de la liste : \n");
    affichageListepar(L);
 printf("appuiez sur un bouton pour avoir l'affichage graphique\n");
        getchar();
        m=NULL;
        dessineeer(L, m);

    m=NULL;

    printf("Veulliez choisir la politique d'affectation qui vous convient : \n 1-Firstfit 2-BestFit 3-WorstFit \n");
         scanf("%d",&politique);
    scanf("%d", &politique);
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
 Affichefile(f);
 //printf("h jidida\n");
 //Affichefile(*h);
  affichtab(m );

  dessineeer(L, m);

   file r;

    do{
             e=rech_ptit_id(&m);
            initfile(&r);
            processus proc;
            //file h c la file qui sauvgarde la toute premiere file avec toutes les valeur

            //recherche du processus afin d'avoir son temps d'execution
             while((!FileVide(&h)) ){
            proc=defiler(&h);
            printf("on defile et enfile \n");
            if(proc.id==e->id){ break;}
            enfiler(&r, proc);
            }
            printf("execution du processus en cours..\n");
            supress(e, L);
            proc.te=(int)proc.te %60;
            fflush(stdout);
            sleep(proc.te);
            fflush(stdout);
           printf("\n file vide h apres sleep %d", FileVide(&h));
            if(!FileVide(&h)){
            while(!FileVide(&h) ){
            proc=defiler(&h);
            enfiler(&r, proc);
            printf("on continue de defiler \n");
            }}

            h=r;
            printf("\n file vide h apre remplissage de r %d", FileVide(&h));
            printf("affichage de h\n");
         //   Affichefile(h);
           //  printf("la 2eme \n");
             //Affichefile(h);



             printf("Fin d'execution ,processus supprime..\n");
            affichtab(m ); //affichage de la table
            affichageListepar(L);//affichage de la liste de partition
             printf("Appuiyez sur un bouton pour avoir l'affichage de la liste apres supression\n");
             getchar();
            dessineeer(L,m);
            decalage(L, m);
            affichageListepar(L);
             printf("Appuiez sur un bouton pour voir l'etat de la liste apres rearangement:\n");
             getchar();
            dessineeer(L,m);
         //   Affichefile(h);
            printf("file vide h=%d  ",FileVide(&h));
                         if(!FileVide(&f)){ printf("dkhelna f switch 2eme\n");
                                switch(politique){

                                          case 1 :  Firstfit(L, &f, &n , &h, &m);
                                                  break;
                                          case 2 : printf("jina hna"); BestFit(L, &f, &n,&h, &m);
                                                  break;
                                          case 3 : printf("jina l worstfit dakhel switch 2\n"); WorstFit(L, &f, &n , &h, &m);
                                                  break;
                                          }

                         }

                            }while (!FileVide(&h));


                printf("execution termine!");
                   break;
                // end
    case 2 :
      initfileP(&H);
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
                 //AffichefileP(g);
                 Empiler(&pile,g, 1);

                 printf("donnez le nombre de processes de priorite 2 \n");
                 scanf("%d", &nbr2);
                 g=createfileP(nbr2, 2, posdeb);
                 posdeb=g.queue->data.id;
                 //AffichefileP(g);
                 Empiler(&pile,g, 2);

                 printf("donnez le nombre de processes de priorite 3 \n");
                 scanf("%d", &nbr3);
                 g=createfileP(nbr3, 3, posdeb);
                 //AffichefileP(g);
                 Empiler(&pile,g, 3);



     Pile intermedo;
     InitPile(&intermedo);

 /*    //affichage de la pile
       for(int z=3; z>0; z--){
       printf(" ----------------affichage de la pile de priorite : %d-----------------\n ", z);
         elem depp=Depiler(&pile);

            AffichefileP(depp.F);
            Empiler(&intermedo, depp.F, depp.prio);
        }
        printf("\n \n on va remmtre dans p \n");
          for(int z=3; z>0; z--){
         elem depp=Depiler(&intermedo);
            Empiler(&pile, depp.F, depp.prio);
        }
  */        printf("vide= %d  et pile =%p \n", PileVide(pile), pile);



        //choix des politique
        printf("Veulliez choisir la politique d'affectation qui vous convient : \n 1-Firstfit 2-BestFit 3-WorstFit \n");
        scanf("%d",&politique);
        elem element;
         Pile pile_inter;
         elem element_inter;
         fileP inter;
         elem element2;
         elem element3;
         initfileP(&element_inter.F);
         initfileP(&element.F);
         int count;
         int nbr=0;
         printf("\n pile est %d", PileVide(pile));
            do{
                   elem p=Depiler(&pile);
                    switch(p.prio){
                                         case 1: nbr= nbr+nbr1; printf("on est la et nbr= %d Nbr1= %d", nbr, nbr1); nbr1=nbr; break;
                                         case 2: nbr= nbr+nbr2; printf("on est la nbr= %d Nbr1= %d", nbr, nbr2); nbr2=nbr; break;
                                         case 3: nbr=nbr+nbr3; printf("on est la nbr= %d Nbr1= %d", nbr, nbr3); nbr3=nbr; break;
                                         }
                     printf("khrejna mel p prio switch \n");
                 //  if(element_inter.F.tete!= NULL){ printf("on est la aussi \n"); element_inter.F.queue->svt=p.F.tete; p.F.tete=element.F.tete;}
     switch (politique){
            case 1 : printf("we're here\n"); FirstfitP(L, &p.F, &nbr, &element.F ,&table_2); printf("we finished \n"); break;
            case 2 : BestFit2(L, &p.F, &nbr, &element.F ,&table_2); break;
            case 3 : WorstFit2(L, &p.F, &nbr, &element.F ,&table_2); break;
            default : printf("vous n'avez pas choisi de politique convenable");}
            //initialiser le compteur avec le nombre de processus affecte
             switch(p.prio){
                                         case 1: count=nbr1-nbr; printf("count= %d \n", count); break;
                                         case 2: count=nbr2-nbr; printf("count= %d \n", count); break;
                                         case 3: count=nbr3-nbr; printf("count= %d \n", count); break;
                                         }

               affichtab(table_2 );
                    printf("appuiez sur un bouton pour avoir l'affichage graphique\n");
                getchar();
                dessineeer(L, table_2);
        while(count>0){ printf("\n iteration number %d \n", count);
           fileP ri;
            e=rech_ptit_id(&table_2);
            initfileP(&ri);
            process pro;
              while((!FileVideP(&element.F)) ){
            pro=defilerP(&element.F);
            printf("on defile et enfile \n");
            if(pro.id==e->id){ break;}
            enfilerP(&ri, pro);
            }
            printf("execution du processus en cours..\n");
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
            affichtab(table_2 ); //affichage de la table
            affichageListepar(L);//affichage de la liste de partition
             printf("Appuiyez sur un bouton pour avoir l'affichage de la liste apres supression\n");
             getchar();
            dessineeer(L,table_2);
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



//partie pile et files
/*
fileP g;
fileP h;
tab* u=NULL;
initfileP(&h);
initfileP(&g);
printf("donnez le nombre de processes\n");
int nbr;
scanf("%d", &nbr);
int j=1;
g=createfileP(nbr, j);
AffichefileP(g);
//u=FirstfitP(L, &g, nbr, &h);
u=FirstfitP(L, &g, nbr, &h);
affichtab(u);
*/


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

