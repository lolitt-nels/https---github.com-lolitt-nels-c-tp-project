void decalage(node* L,tab * T){
    node * q;
    node * p;

    tab * t;
    q = L;
    p = L;

    while (q != NULL){
        p = q;
        if(q->data.taille == 1){
            while(p != q){p = p->svt;}
            if(p!=q){
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
