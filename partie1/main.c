#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

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
struct elem svt;
}pile;

typedef elem* Pile;

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
void Empiler(Pile* pile, fileP f, int a) {
    elem* nouvelElem = (elem*)malloc(sizeof(elem));
    nouvelElem->prio=n;
    nouvelElem->F.queue=f.queue;
    nouvelElem->F.tete=f.tete;
    nouvelElem->svt = *pile;
    *pile = nouvelElem;
}

// Dépiler un élément de la pile
fileP Depiler(Pile* pile) {
    if (PileVide(*pile)) {
        printf("Erreur : La pile est vide\n");
        return -1;
    }
     fileP f;
     f.queue=(*pile)->queue;
     f.tete=(*pile)->tete;
    elem* temp = *pile;
    *pile = (*pile)->svt;
    free(temp);
    return f;
}


// Obtenir le sommet de la pile sans le dépiler
elem SommetPile(Pile* pile) {
    if (PileVide(*pile)) {
        printf("Erreur : La pile est vide\n");
        return -1;
    }
     elem r;
     r.F.tete=(*pile)->F.tete;
     r.F.queue=(*pile)->F.queue;
     r.prio=(*pile)->prio;
    return r;
}





/////////////////////////////////////////////////////fonctions part 2////////////////////////////////////////////////////////////////////////

















/////////////////////////////////////////////////////fonctions part 1 ////////////////////////////////////////////////////////////////////////




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
             p->data.etat= rand()%2;
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
//affiche et affecte le tout dans r puis f=r
void Affichefile(file f){
    processus x;
    file r;
    printf("affichage begins\n");
    initfile(&r);
    while (FileVide(&f)!=1){
        x=defiler(&f);
        printf("id= %d  ia=%0.2f te=%0.2f taille= %d \n", x.id , x.ia , x.te , x.taille );

        enfiler(&r , x );
    }
    f=r;
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
    r_te= rand() % 60 + 5;
    p.te=r_te;
    p.taille= rand()% 200*10;
    enfiler(&F, p);
      }
return (F);
}





void createreste(node* p , processus x ){
 node* q=(node*)malloc(sizeof(node)); printf("creation node rest \t");
 q->data.etat=0;
 q->data.taille= p->data.taille - x.taille ;
 p->data.taille= x.taille;//mise a jour taille de p
 q->data.adr= (p->data.adr) + (p->data.taille);
 //chainage
 q->svt=p->svt;
 p->svt=q;

}

tab* BestFit(liste L, file *F , int n , file *h){
 liste p;
 int i=0;
 liste pmin;
 processus x; printf("l= %p", L);
 p=L;printf("p= %p", p);
 initfile(h);
 tab* T=NULL;
 tab* Q=NULL;
 while(i<n )//normalement i use something like boucle pour cuz i know num of elements
 { x=defiler(F);  printf("defilina w enfilina \n");
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
                   Q->id=x.id; Q->affect=pmin; printf("tc id = %d  tc affect= %p \n", Q->id , Q->affect);
                   Q->svt=T; T=Q;
                   //affectation
                  pmin->data.etat=1;   //1 c  occupe
                if ((pmin->data.taille)>x.taille ){
                        createreste( pmin , x );}
                  }
  if(pmin==NULL){ enfiler(F, x);  }
 i++;}
return (T);
}

tab* Firstfit(liste L, file *F , int n , file *h){
 liste p;
 int i=0;
 file r;
 processus x; printf("l= %p", L);
 p=L;printf("p= %p", p);
 initfile(h);
 initfile(&r);
 int trouve;
  tab* T=NULL;
 tab* Q=NULL;
 while( i<n )//normalement i use something like boucle pour cuz i know num of elements
{ x=defiler(F);  printf("defilina w enfilina \n");
   enfiler(h, x);
   p=L;
   trouve=0;
   while(p!=NULL && trouve==0){
     if( (p->data.etat== 0 ) && ((p->data.taille)>=x.taille) ){
             Q=(tab*)malloc(sizeof(tab));
                   Q->id=x.id; Q->affect=p; printf("tc id = %d  tc affect= %p \n", Q->id , Q->affect);
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
 liste p;
 int i=0;
 file r;
 liste pmax;
 processus x; printf("l= %p", L);
 p=L;printf("p= %p", p);
 initfile(h);
 initfile(&r);
 tab* T=NULL;
 tab* Q=NULL;
 while( i<n )
 { x=defiler(F);  printf("defilina w enfilina \n");
   enfiler(h, x);
    pmax=NULL;
    p=L;
   while(p!=NULL){
    if( (p->data.etat== 0 ) && ((p->data.taille)>=x.taille) ){
        if(pmax==NULL){pmax=p;  }
        else{
            if((p->data.taille)> (pmax->data.taille)){pmax=p;}
        }

    }
    p=p->svt;
   }
   if(pmax!= NULL){
               Q=(tab*)malloc(sizeof(tab));
                   Q->id=x.id; Q->affect=p; printf("tc id = %d  tc affect= %p \n", Q->id , Q->affect);
                   Q->svt=T; T=Q;//affectation
                  pmax->data.etat=1;//1 devient occupe
                  if ((pmax->data.taille)>x.taille ){
                        createreste( pmax , x );}
                  }
  if(pmax==NULL){ enfiler(F, x);  }
   i++;
   }
return (T);
}


void affichtab(tab *T  ){
   tab* q=T;
   printf("on rentre");
   if(q==NULL){printf("tab est vide");}
   while(q!=NULL){
        printf("id= %d  est affecte a %p  \n", q->id, q->affect);
        q=q->svt;
   }
  }

//avant dupress pointer=L
//on lui donne un element du tableau elle le remet a nul , et prend l'adress de partition et la remt a libre et decale touut pour supprimer l'elem du tableau
void supress(tab *e, liste L){
    node*p= e->affect;
    node* q =L;
    while(q!= p){ //pas besoin de null vue queforcement adress existe dans table
        q=q->svt;
        }
        if(q!=NULL){q->data.etat=0;}
//maybe need to put *L for change to occur?
}

tab* rech_ptit_id(tab* T){
       tab* q;
       tab* p=T;
       tab* pmin=T;
       while(p!=NULL){
        if(p->id<pmin->id){pmin=p;}
        p=p->svt;
        } p=T;
        q=T;
      while(p!=NULL&& p!=pmin){
        q=p; p=p->svt;
      }
       if(p==T){T=T->svt; p->svt=NULL;}
       else{q->svt=p->svt; p->svt=NULL;}

return(p);
}



//rearanger c faire une sorte de decalage te3 les partition apres chaque supression






//////////////////////////////////////////////////////////////main////////////////////////////////////////////////////////////////////////////////



int main(){
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

tab* e=rech_ptit_id(m);
supress(e, L);
affichtab(m );
affichageListepar(L);

//this code allows the function to start once the user presses a button

/*printf("Let the Battle Begin!\n");
printf("Press Any Key to Continue\n");
getchar();*/
//hna we start timer supress and rearange



    return 0;
}

