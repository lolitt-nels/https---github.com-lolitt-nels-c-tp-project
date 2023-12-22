void decalage(liste L,tab * T){
    node * q;
    node * p;
    tab * t;
    q = L;
    p = L;

    while (q != NULL){
        p = q; printf("p=%p  et q= %p   q.etat=%d \n", p , q, q->data.etat);
        if(q->data.etat == 1){  printf("l9ina \n");
            while(p != q && (p->data.taille)>(q->data.taille)){ printf("dkhel2");p = p->svt;}
            if((p->data.taille)<=(q->data.taille)){ printf("dkhel3");
                p->data.etat = 1;
                q->data.etat = 0;
                processus y;
                y.ia=0; y.id=0; y.te=0;
                y.taille=q->data.taille;
                createreste(p , y);
                t = T;
                while((t != NULL)&&(t->affect != q)){
                    t = t->svt;
                    q = p;
                    if(t != NULL){t->affect = p;}

                }


            }
        }
else {q = q->svt;}
    }
}


/*
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
void decalage(liste L,tab * T){
    node * q;
    node * p;

    tab * t;
    q = L;
    p = L;

    while (q != NULL){
        p = q; printf("p=%p  et q= %p   q.etat=%d \n", p , q, q->data.etat);
        if(q->data.etat == 1){  printf("l9ina \n");
            while(p != q && (p->data.taille)<(q->data.taille)){ printf("dkhel2");p = p->svt;}
            if((p->data.taille)>(q->data.taille)){ printf("dkhel3");
                p->data.etat = 1;
                q->data.etat = 0;
                processus y;
                y.ia=0; y.id=0; y.te=0;
                y.taille=q->data.taille;
                createreste(p , y);
                t = T;
                while((t != NULL)&&(t->affect != q)){
                    t = t->svt;}

                    if(t != NULL){t->affect = p;}


            }
             q=p->svt;
        }
if(p==q) {q = q->svt; printf("we changed cuz libre\n");}
    }
}
*/
