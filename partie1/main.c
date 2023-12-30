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
void Empiler(Pile * pile,fileP f, int n) {
    elem* nouvelElem = (elem*)malloc(sizeof(elem)); printf("newele adress =%p\n", nouvelElem);
    nouvelElem->prio=n; printf("newele prio =%d", nouvelElem->prio);
   nouvelElem->F.queue=f.queue;
   nouvelElem->F.tete=f.tete;printf("newele tete=%p \t", nouvelElem->F.tete);
    nouvelElem->svt = *pile ;
    printf("pile = %p et  newele svt  =%p\n", pile, nouvelElem->svt);
    *pile = nouvelElem ; printf("npile adress =%p \n", pile);
}

// Dépiler un élément de la pile
elem Depiler(Pile* pile) {
     elem f;
     f.F.queue= (*pile)->F.queue;
     f.F.tete=(*pile)->F.tete;
     f.prio=(*pile)->prio;
    elem* temp = *pile;
    *pile = (*pile)->svt;
    free(temp);
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
   if ( f->queue == NULL) { printf("dans pile fait empile \n");
        f->tete=newf; f->queue=newf; }//file vide
   else{ f->queue->svt=newf; f->queue=newf;  };

}

process defilerP(fileP *f ){
    nodeFP* temp;
    process x;
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
    while (FileVideP(&f)!=1){
        x=defilerP(&f);
        printf("id= %d  ia=%0.2f te=%0.2f taille= %d prio=%d \n", x.id , x.ia , x.te , x.taille , x.prio);

        enfilerP(&r , x );
    }
    f=r;
}

void affichepile(Pile p){
     Pile r;
     elem x;
     InitPile(&r);
     while(!PileVide(p)){
        x=Depiler(&p);
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


tab* FirstfitP(liste L, fileP *F , int n , fileP *h){
 liste p;
 int i=0;
 fileP r;
 process x; printf("l= %p", L);
 p=L;printf("p= %p", p);
 initfileP(h);
 initfileP(&r);
 int trouve;
  tab* T=NULL;
 tab* Q=NULL; printf("t= %p", T);
 while( i<n )//normalement i use something like boucle pour cuz i know num of elements
{ x=defilerP(F);  printf("defilina w enfilina \n");
   enfilerP(h, x);
   p=L;
   trouve=0;
   while(p!=NULL && trouve==0){
     if( (p->data.etat== 0 ) && ((p->data.taille)>=x.taille) ){
             Q=(tab*)malloc(sizeof(tab));
                   Q->id=x.id; Q->affect=p; printf("tc id = %d  tc affect= %p \n", Q->id , Q->affect);
                   Q->svt=T; T=Q;//affectation
            trouve=1;
           p->data.etat=1;
             if ((p->data.taille)>x.taille ){createreste2( p , x );}
          }
   p=p->svt;
   }
   if (trouve==0){enfilerP(F, x);}
   i++;
 }

return(T);

}


tab* BestFit2(liste L, fileP *F , int n , fileP *h){
 liste p;
 int i=0;
 liste pmin;
 process x; printf("l= %p", L);
 p=L;printf("p= %p", p);
 initfileP(h);
 tab* T=NULL;
 tab* Q=NULL;
 while(i<n )//normalement i use something like boucle pour cuz i know num of elements
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
                   Q->id=x.id; Q->affect=pmin; printf("tc id = %d  tc affect= %p \n", Q->id , Q->affect);
                   Q->svt=T; T=Q;
                   //affectation
                  pmin->data.etat=1;   //1 c  occupe
                if ((pmin->data.taille)>x.taille ){
                        createreste2( pmin , x );}
                  }
  if(pmin==NULL){ enfilerP(F, x);  }
 i++;}
return (T);
}

tab* WorstFit2(liste L, fileP *F , int n , fileP *h){ //j'ai changé les parametres seulement
 liste p;
 int i=0;
 liste pmax;
 process x; printf("l= %p", L);
 p=L;printf("p= %p", p);
 initfileP(h);

 tab* T=NULL;
 tab* Q=NULL;
 while( i<n )
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
                   Q->id=x.id; Q->affect=p; printf("tc id = %d  tc affect= %p \n", Q->id , Q->affect);
                   Q->svt=T; T=Q;//affectation
                  pmax->data.etat=1;//1 devient occupe
                  if ((pmax->data.taille)>x.taille ){
                        createreste2( pmax , x );}
                  }
  if(pmax==NULL){ enfilerP(F, x);  }
   i++;
   }
return (T);
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
            printf(" %d %d %d \n", p->data.adr, p->data.taille , p->data.etat);
            p = p->svt;
    }
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

tab* BestFit(liste L, file *F , int n , file *h){
 //initialisations

        liste p;
        int i=0;
        liste pmin;
        processus x;
        p=L;
          initfile(h);
        tab* T=NULL;
        tab* Q=NULL;
 while(i<n )//pour chaque element de la file
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
                   Q->svt=T; T=Q;
                   //mise a jour de la table des affectations
                  pmin->data.etat=1;   //mettre la partition a  occupe
                if ((pmin->data.taille)>x.taille ){
                        createreste( pmin , x );}
                  }
  if(pmin==NULL){ enfiler(F, x);  }
 i++;}
return (T);
}

tab* Firstfit(liste L, file *F , int n , file *h){

 //initialisations
        liste p;
        int i=0;
        file r;
        processus x;
        p=L;
            initfile(h);
          //  initfile(&r);
        int trouve;
     tab* T=NULL;
     tab* Q=NULL;


 while( i<n )//pour chaque element de la file
{ x=defiler(F);
   enfiler(h, x);
   p=L;
   trouve=0;
   //trouver la premiere partiton vide dont l'espace est suffisant
   while(p!=NULL && trouve==0){
     if( (p->data.etat== 0 ) && ((p->data.taille)>=x.taille) ){
            //mettre a jour la table des affectation
             Q=(tab*)malloc(sizeof(tab));
                   Q->id=x.id; Q->affect=p;
                   Q->svt=T; T=Q;//affectation
            trouve=1;
           p->data.etat=1;
             if ((p->data.taille)>x.taille ){createreste( p , x );}
          }
   p=p->svt;
   }
   if (trouve==0){enfiler(F, x);}
   i++;
 }

return(T);

}

tab* WorstFit(liste L, file *F , int n , file *h){

 //initialisations
       liste p;
       int i=0;
       liste pmax;
       processus x;
       p=L;
       initfile(h);
       tab* T=NULL;
       tab* Q=NULL;

 while( i<n )
 { x=defiler(F);
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
                   Q->svt=T; T=Q;//affectation
                  pmax->data.etat=1;// devient occupe
                  //creer le risidu s'il y en as
                  if ((pmax->data.taille)>x.taille ){
                        createreste( pmax , x );}
                  }
  if(pmax==NULL){ enfiler(F, x);  }
   i++;
   }
return (T);
}

//afficher la table des affectations
void affichtab(tab *T  ){
   tab* q=T;
   printf("\t affichage de la table des affectations:\n");

   if(q==NULL){printf("Aucun processus affecte");}

   while(q!=NULL){
        printf("---------------------------------------------------\n");
        printf(" | processus %d  est affecte a l'adresse: %d ko | \n", q->id, q->affect->data.adr);
        q=q->svt;
   }
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
        if(q!=NULL){q->data.etat=0;
            //si partition precedente libre , les ccoller pour creer partition plus grande
            if (pres->data.etat==0){pres->data.taille +=q->data.taille; pres->svt=q->svt; free(q);}
            //sinon , si la suivante est libre :faire de meme
           else{ if (q->svt->data.etat==0){q->data.taille+=q->svt->data.taille; q->svt=q->svt->svt; free(q->svt);}   }
        }//liberer l'element de la table des affectations
        free(e);
}

//recherche du processus arrive en premier pour l'executer
tab* rech_ptit_id(tab* T){
       tab* q;
       tab* p=T;
       tab* pmin=T;
       //trouve le id du processus arrive en premier parmis ceux affecte a des partitions
       while(p!=NULL){
        if(p->id<pmin->id){pmin=p;}
        p=p->svt;
        } p=T;
        q=T;
        //affecter un pointeur qui pointe vers la partition auquel il q ete affecte
      while(p!=NULL&& p!=pmin){
        q=p; p=p->svt;
      }
        //detacher ,sans supprimer, le processus du tableau des affectations (liste)
       if(p==T){T=T->svt; p->svt=NULL;}
       else{q->svt=p->svt; p->svt=NULL;}
      printf("processus a executer : %d ", p->id);
return(p);
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
             p=p->svt;
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
 
switch{
    cas 1 [part1 sans priorite]
       ask user donnez le nombre de processus 
       creer la liste  + affichage textuelle + affichage raylib
       creation de la file +affichage textuelle.

        ask user politique switch {
                                   cas 1 [first fit] (utilisation la fonction)
                                   cas 2 [best fit] (utilisation la fonction)
                                   cas 3 [worst fit](utilisation la fonction)
                                   }
             affichage de la table des affectations
             afficher les partitions (textuelle + graphic)
             do{  recherche ptit id ;
                  supress ;
                  sleep;
                  affiche tables des affectations+ affiche file + affiche partitons (textuelle +graphic)
                  decalage (rearanger);
                  affichage partitions
             }while (non file vide (F))
    
//use keyborad thingy before every affichage graphique
//this code allows the function to start once the user presses a button

/*printf("Let the Battle Begin!\n");
printf("Press Any Key to Continue\n");
getchar();*/
    cas 2 [part2 avec priorite]
            create liste de partition affichage textuelle+ graphique
            ask user for every file de priorite , creation de files et pile
            affichage de la pile et ses files           
              
           ask user politique switch {
                                
                                   cas 1 [first fit] (utilisation la fonction):depiler Pile
                                                                               use function
                                                                               depiler pile 2+enfiler le reste de la file
                                                                               use function 
                                                                               depiler pile 3 +enfiler le reste de la file
                                                                               use function
                                                                              affichage liste et tab et 

                                   }
                                                                                                     
                                   cas 2 [best fit] (utilisation la fonction)
                                   cas 3 [worst fit](utilisation la fonction)
                                   }

                      affichage de la table des affectations
             afficher les partitions (textuelle + graphic)
     do{  recherche ptit id ;
                  supress ;
                  sleep;
                  affiche tables des affectations+ affiche file + affiche partitons (textuelle +graphic)
                  decalage (rearanger);
                  affichage partitions
             }while (non file vide (F))//vu que te93ed que la derniere file

}






 //partie file
   int n;
   file f;
   file *h;
   tab* m;
   printf("donnez n \n");
   scanf("%d", &n);
   f=createfile(n);
   Affichefile(f);


//partie liste
liste L;
    L= Listepar();
    printf("l'affichage de la liste : \n");
    affichageListepar(L);

    h->queue=NULL;
    h->tete=NULL;
m=BestFit(L, &f, n , h);
 //m=Firstfit(L, &f, n, h);
 //m=WorstFit(L, &f, n, h );
 affichageListepar(L);
 printf("f jdida\n");
 Affichefile(f);
 printf("h jidida\n");
 Affichefile(*h);
  affichtab(m );

  dessineeer(L, m);

  printf("Press Enter to start the timer\n");
    getchar();










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


//supression procedure en details

printf("i am gonna supress once");
tab* e=rech_ptit_id(m);
int pos=e->id;
file r;
initfile(&r);
processus proc;
//file h c la file qui sauvgarde la toute premiere file avec toutes les valeurs
printf("%d", FileVide(h));
//recherche du temps d'execution 
while((!FileVide(h)) ){

    proc=defiler(h);  printf("on defile procid=%d \t", proc.id);
     enfiler(&r, proc);
    if(proc.id==e->id){ break;}

}
printf("%d  and %d ", proc.id, (int)proc.te);


supress(e, L);
 proc.te=(int)proc.te %60;
 sleep(proc.te);
 fflush(stdout);
printf("its done \n");
 decalage(L, m);
 affichageListepar(L); 
 affichtab(m );




printf("well good job!");


dessineeer(L, m);


   printf("its good \n");



    return 0;
}
