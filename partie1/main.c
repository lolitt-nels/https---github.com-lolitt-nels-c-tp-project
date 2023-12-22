void decalage(node* L,tab * T,nodeF * F){
    node * q;
    node * p;
    nodeF * f;
    tab * t;
    q = L;
    p = L;
    f = F;
    while (q != NULL){
        p = q;
        if(q->data.taille == 1){
            while(p != q){p = p->svt;}
            if(p!=q){
                p->data.etat = 1;
                q->data.etat = 0;
                createreste(p , f->data);
                t = T;
                while((t != NULL)&&(t->affect != q)){
                    t = t->svt;
                    q = p;
                    if(t != NULL){t->affect = p;}
                    else {q = q->svt; f = f->svt;}
                }


            }
        }

    }
}












