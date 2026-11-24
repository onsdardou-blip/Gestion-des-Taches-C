#include <stdio.h>
#include <stdlib.h>
#include<string.h>
typedef struct
{
    char code[50];
    int type;
    int duree;
    int nbop;
    int idoperateurs[10];
} tache;
typedef struct structNoeud
{
    tache t;
    struct structNoeud *suiv;
} structNoeud;
typedef structNoeud *NOEUD;
typedef struct
{
    NOEUD tete;
    int lg;
} structList;
typedef structList *LISTE;

void Saisir_Tache(tache *t)
{
    printf("saisir code: ");
    scanf("%s",t->code);
    do
    {
        printf("saisir type (entre 1 et 5): ");
        scanf("%d",&t->type);
    }
    while(t->type<1 || t->type>5);
    printf("saisir duree(en minutes): ");
    scanf("%d",&t->duree);
    do
    {
        printf("saisir nombre des operateurs: ");
        scanf("%d",&t->nbop);
    }
    while(t->nbop<1 || t->nbop>10);// validation nb opérateurs (0 à 10)
    for(int i=0 ; i<t->nbop; i++)
    {
        printf("id operateur %d :",i+1);
        scanf("%d",&t->idoperateurs[i]);
    }
}
NOEUD ncreer(tache T)
{
    NOEUD n=(NOEUD)malloc(sizeof(structNoeud));
    if (n!=NULL)
    {
        n->t=T;
        n->suiv=NULL;
    }
    return n;
}
LISTE Creer_Liste()
{
    LISTE L = malloc(sizeof(structList));
    if(L!=NULL)
    {
        L->tete = NULL;
        L->lg = 0;
    }
    return L;
}
LISTE Ajouter_Tache(LISTE L, tache T)
{
    NOEUD n = ncreer(T);
    if(L->tete == NULL) // si liste est vide
    {
        L->tete = n;
    }
    else
    {
        NOEUD p = L->tete;
        while(p->suiv != NULL)
            p = p->suiv;

        p->suiv = n;
    }
    (L->lg)++;
    return L;
}
void Afficher_Liste(LISTE L)
{
    NOEUD p=L->tete;
    while(p!=NULL)
    {
        printf("\nCode : %s",p->t.code);
        printf("\nType : %d",p->t.type);
        printf("\nDuree : %d",p->t.duree);
        printf("\nles Operateurs : ");
        for(int i=0; i<p->t.nbop; i++)
            printf("%d ",p->t.idoperateurs[i]);
        printf("\n----------------");
        p = p->suiv;
    }
}
int Duree_Tot(LISTE L)
{
    int s=0;
    NOEUD p=L->tete;
    while(p!=NULL)
    {
        s+=p->t.duree;
        p=p->suiv;
    }
    return s;
}
NOEUD Recherche (NOEUD p, int type)
{
    if(p==NULL) //type n'existe pas
        return NULL;

    if(p->t.type == type)//si type trouvee
        return p;

    return Recherche(p->suiv,type);

}
LISTE Inserer_Tache( LISTE L, tache T)
{
    NOEUD n = ncreer(T);
    NOEUD p = Recherche(L->tete,T.type);
    if(p==NULL)//ajout a la fin
        return Ajouter_Tache(L,T);

    if(p == L->tete)//insertion dans la tete
    {
        n->suiv = L->tete;
        L->tete = n;
    }
    else //insertion avnt p
    {
        NOEUD q = L->tete;

        while(q->suiv != p)
            q = q->suiv;

        q->suiv = n;
        n->suiv = p;
    }

    (L->lg)++;
    return L;

}
void Ajouter_Op(LISTE L, char code[], int id)
{
    NOEUD p = L->tete;
    while(p!=NULL && strcmp(p->t.code, code) != 0 ) p = p->suiv;
    if(p==NULL)
    {
        printf("code introuvable");
        return;
    }
    int n=p->t.nbop;
    if(n==10)// vérifier si le nombre maximal d'opérateurs est atteint
    {
        printf("Max atteint");
        return;
    }
    for(int i=0; i<n; i++)
    {
        if(p->t.idoperateurs[i]==id)// vérifier si l'opérateur existe déjà
        {
            printf("Operateur existe");
            return;
        }
    }
    // ajout de l'opérateur
    p->t.idoperateurs[n] = id;
    (p->t.nbop)++;


    printf("Operateur ajouter avec succes! \n");
}
LISTE Supprimer_Taches_Type(LISTE L, int type)
{
    NOEUD p=L->tete, q=NULL;
    while(p!=NULL) // parcourir la liste
    {
        if(p->t.type==type)
        {
            // cas 1 : suppression en tête
            if(q==NULL)
            {
                L->tete = p->suiv;
                free(p);
                p = L->tete;
            }
            else // cas 2 : suppression au milieu ou fin
            {
                q->suiv = p->suiv;
                free(p);
                p = q->suiv;
            }
            (L->lg)--;
        }
        else // on fait rien et on avance
        {
            q = p;
            p = p->suiv;
        }
    }
    return L;
}
void Remplacer_Operateur(LISTE L, int ancien_id, int nouveau_id)
{
    NOEUD p=L->tete;
    while(p!= NULL)
    {
        for(int i=0; i<p->t.nbop; i++)
        {
            if(p->t.idoperateurs[i]==ancien_id)
                p->t.idoperateurs[i]=nouveau_id;
        }
        p=p->suiv;
    }
}
void permuter (tache *a, tache *b)
{
    tache x;
    x=*a;
    *a=*b;
    *b=x;
}
LISTE Trier_Liste_Duree(LISTE L)
{
    NOEUD p,q;
    for(p=L->tete; p!=NULL; p=p->suiv)
    {
        for(q=p->suiv; q!=NULL; q=q->suiv)
        {
            if(p->t.type==q->t.type &&
                    p->t.duree < q->t.duree)
            {
                permuter(&p->t,&q->t);

            }
        }
    }

    return L;
}
int Calculer_Duree_Op(LISTE L, int id)
{
    NOEUD p=L->tete;
    int tot=0;
    while(p!=NULL)
    {
        for(int i=0; i<p->t.nbop; i++)
        {
            if(p->t.idoperateurs[i]==id)
                tot+=p->t.duree;
        }
        p=p->suiv;

    }
    return tot;
}
int Verifier_Sequence(LISTE L, int id, int Seq[], int n)
{
    NOEUD p=L->tete;
    int i=0; // position de la séquence
    while(p!=NULL && i<n) // parcourir toutes les tâches tant que la séquence n'est pas terminée
    {
        if(p->t.type==Seq[i])//verification le type
        {
            for(int j=0; j<p->t.nbop; j++)
            {
                if(p->t.idoperateurs[j]== id)//vérifcation que l’opérateur a participé
                {
                    i++;
                    break ;
                }
            }
        }
        p=p->suiv;
    }
    if(i==n)
        return 1;
    else
        return 0;
}
int Verifier_Efficace(LISTE L, int id, int Seq[], int n, float P)
{
    float pr;
    int d1,d2;
    d1 = Calculer_Duree_Op(L,id);
    d2 = Duree_Tot(L);
    pr = ((float)d1/d2))*100;
    if(pr>=P && Verifier_Sequence(L,id,Seq,n)==1)
        return 1;
    return 0;
}
int main()
{
    LISTE L;
    L=Creer_Liste();
    tache t;
    int type;
    char code[50];
    int id,n;
    float P;
    int ancien,nouveau;
    int Seq[20];
    int choix;

    do
    {
        printf ("----------------Gestion des tâches-------------\n");
        printf (" 1-Ajouter une tache\n");
        printf (" 2-Afficher la liste des tâches\n");
        printf (" 3-Afficher la durée totale\n");
        printf (" 4-Insérer une tâche\n");
        printf (" 5-Ajouter un opérateur\n");
        printf (" 6-Supprimer les tâches par type\n");
        printf (" 7-Remplacer un opérateur\n");
        printf (" 8-Trier la liste\n");
        printf (" 9-Verifier l’efficacité d’un opérateur");
        printf("\n 0-Quitter\n");
        printf("\n Choix (0~9): ");
        scanf("%d",&choix);
        switch(choix)
        {
        case 1:
            Saisir_Tache(&t);
            L=Ajouter_Tache(L,t);
            break;
        case 2:
            Afficher_Liste(L);
            break;
        case 3:
            printf("Duree totale = %d minutes ",Duree_Tot(L));
            break;
        case 4:
            Saisir_Tache(&t);
            L = Inserer_Tache(L,t);
            break;
        case 5:
            printf("Code tache : ");
            scanf("%s",code);
            printf("ID operateur : ");
            scanf("%d",&id);
            Ajouter_Op(L,code,id);
            break;
        case 6:
            printf("donner le type a supprimer : ");
            scanf("%d",&type);
            L = Supprimer_Taches_Type(L,type);
            break;
        case 7:
            printf("donner l'id d'operateur a remplacer : \n");
            scanf("%d",&ancien);
            printf("donner le nouveau d'operateur : \n");
            scanf("%d",&nouveau);
            Remplacer_Operateur(L,ancien,nouveau);
            break;
        case 8:
            L=Trier_Liste_Duree(L);
            printf("la liste trier : \n ");
            Afficher_Liste(L);
            break;
        case 9:
            printf("ID operateur : ");
            scanf("%d",&id);
            printf("Taille sequence : ");
            scanf("%d",&n);
            printf("sequence :");
            for(int i=0; i<n; i++)
            {
                printf("Seq[%d]=",i);
                scanf("%d",&Seq[i]);
            }
            printf("donner le Pourcentage : ");
            scanf("%f",&P);
            if(Verifier_Efficace(L,id,Seq,n,P)==1)
                printf("Operateur efficace");
            else
                printf("Operateur non efficace");
            break;
        }
    }
    while(choix!=0);
    return 0;
}
