#include <fstream>
#include <iostream>
#include <cstdlib>
using namespace std;

ifstream fin("abce.in");
ofstream fout("abce.out");

struct node
{
    int key, priority;
    node* left;
    node* right;
    node* parent;
};

node* treap;

int succ, prec;

void Insert(node*& root, int y)
{
    int random_priority = rand();

    node* q = new node;

    q -> key = y;
    q -> priority = random_priority;
    q -> left = NULL;
    q -> right = NULL;
    q -> parent = NULL;

    if(root == NULL)
    {
        root = q;
        return;
    }

    node* p = root;
    while(1)
    {
        if(y <= p -> key && p -> left != NULL)
        {
            p = p -> left;
        }
        else if(y <= p -> key && p -> left == NULL)
        {
            q -> parent = p;
            p -> left = q;
            break;
        }
        else if(y > p -> key && p -> right != NULL)
        {
            p = p -> right;
        }
        else if(y > p -> key && p -> right == NULL)
        {
            q -> parent = p;
            p -> right = q;
            break;
        }
    }

    while(q -> parent != NULL && q -> priority < q -> parent -> priority)
    {
        if((q -> parent -> left) == q) // stiu ca sunt fiu stang pt tata
        {

            if((q -> parent -> parent) == NULL)   // ajung sa fiu radacina, nu am bunic
            {
                if((q -> right) != NULL)
                    q -> right -> parent = q -> parent;
                q -> parent -> left = q -> right;
                q -> right = q -> parent;
                q -> parent = NULL;

                root = q;
            }
            else                               // am bunic
            if((q -> parent -> parent -> right) == (q -> parent))    // si tatal e fiu drept pt bunicu
            {
                if(q -> right != NULL)                              // daca am subarbore drept
                    q -> right -> parent = q -> parent;
                    q -> parent -> left = q -> right;
                    q -> right = q -> parent;
                    q -> parent = q -> parent -> parent;
                    q -> parent -> right = q;
                    q -> right -> parent = q;
            }
            else
            if((q -> parent -> parent -> left) == (q -> parent))      // si tatal e fiu stang pt bunicu
            {
                node* fiu = q -> right;
                node* tata = q -> parent ;
                node* bunicu = q -> parent -> parent;

                bunicu -> left = q;
                q -> parent = bunicu;
                tata -> parent = q;
                q -> right = tata;
                if(fiu != NULL)
                    fiu -> parent = tata;
                tata -> left = fiu;
            }
        }
        else if((q -> parent -> right) == q)                     // daca sunt fiu drept pt tata
        {
            if((q -> parent -> parent) == NULL)                   // nu am bunic, ajung sa fiu radacina
            {
                node* tata = q -> parent;
                node* fiu = q -> left;

                if(fiu != NULL)
                    fiu -> parent = tata;
                tata -> right = fiu;
                tata -> parent = q;
                q -> left = tata;
                q -> parent = NULL;

                root = q;
            }
            else                                                  // tata e fiu drept pt bunicu
            if((q -> parent -> parent -> right) == (q -> parent))
            {
                node* tata = q -> parent;
                node* bunicu = q -> parent -> parent;
                node* fiu = q -> left;

                if(fiu != NULL)
                    fiu -> parent = tata;
                tata -> right = fiu;
                tata -> parent = q;
                q -> left = tata;
                q -> parent = bunicu;
                bunicu -> right = q;
            }
            else                                                   // tata e e fiu stang pt bunicu
            if((q -> parent -> parent -> left) == (q -> parent))
            {
                node* tata = q -> parent;
                node* bunicu = q -> parent -> parent;
                node* fiu = q -> left;

                if(fiu != NULL)
                    fiu -> parent = tata;
                tata -> right = fiu;
                tata -> parent = q;
                q -> left = tata;
                q -> parent = bunicu;
                bunicu -> left = q;
            }
        }
    }

}

bool Search(node* root, int y)
{
    if(root == NULL)
        return 0;

    if(root -> key == y)
        return 1;

    if(root -> key < y)
        return Search(root -> right, y);

    return Search(root -> left, y);
}

void Succesor(node* root, int y)
{
    if(root == NULL)
        return;
    if(root -> key == y)
    {
        succ = y;
        return;
    }
    else if(root -> key < y)
    {
        Succesor(root -> right, y);
    }
    else if(root -> key > y)
    {
        succ = root -> key;
        Succesor(root -> left, y);
    }
}

void Predecesor(node* root, int y)
{
    if(root == NULL)
        return;
    if(root -> key == y)
    {
        prec = y;
        return;
    }
    else if(root -> key < y)
    {
        prec = root -> key;
        Predecesor(root -> right, y);
    }
    else if(root -> key > y)
    {
        Predecesor(root -> left, y);
    }
}

void AfisareNumere(node* root, int y, int z)
{
    if(root == NULL)
        return;
    else if(root -> key <= y)
    {
        AfisareNumere(root -> right, y, z);
    }
    else if(root -> key >= z)
    {
        AfisareNumere(root -> left, y, z);
    }
    else
    {
        AfisareNumere(root -> left, y, root -> key);
        fout << root -> key << " ";
        AfisareNumere(root -> right, root -> key, z);
    }
}

void Delete(node*& root, int y)
{
    node* q = root;
    while(q -> key != y)              // Gasesc primul nod cu valoarea y
    {
        if(q -> key < y)
            q = q -> right;
        else
            q = q -> left;
    }

    while(q -> right != NULL || q -> left != NULL)
    {
        node* tata = q -> parent;
        node* fiu_st = q -> left;
        node* fiu_dr = q -> right;

        if(q == root)                   // daca sunt radacina
        {
            if(fiu_st != NULL && fiu_dr != NULL)
                if(fiu_st -> priority <= fiu_dr -> priority)
                {
                    node* nepot_stanga_dreapta = fiu_st -> right;

                    q -> parent = fiu_st;
                    q -> left = fiu_st -> right;
                    fiu_st -> right = q;
                    fiu_st -> parent = NULL;

                    if(nepot_stanga_dreapta != NULL)
                        nepot_stanga_dreapta -> parent = q;

                    root = fiu_st;
                }
                else
                {
                    node* nepot_dreapta_stanga = fiu_dr -> left;

                    q -> parent = fiu_dr;
                    fiu_dr -> left = q;
                    fiu_dr -> parent = NULL;
                    q -> right = nepot_dreapta_stanga;

                    if(nepot_dreapta_stanga != NULL)
                        nepot_dreapta_stanga -> parent = q;

                    root = fiu_dr;
                }
            else if(fiu_st == NULL)
            {
                    node* nepot_dreapta_stanga = fiu_dr -> left;

                    q -> parent = fiu_dr;
                    fiu_dr -> left = q;
                    fiu_dr -> parent = NULL;
                    q -> right = nepot_dreapta_stanga;

                    if(nepot_dreapta_stanga != NULL)
                        nepot_dreapta_stanga -> parent = q;

                    root = fiu_dr;
            }
            else if(fiu_dr == NULL)
            {
                node* nepot_stanga_dreapta = fiu_st -> right;

                q -> parent = fiu_st;
                q -> left = fiu_st -> right;
                fiu_st -> right = q;
                fiu_st -> parent = NULL;

                if(nepot_stanga_dreapta != NULL)
                    nepot_stanga_dreapta -> parent = q;

                root = fiu_st;
            }
        }

        else                                                            // daca nu e radacina
        if((q -> parent -> left) == q)                                  // si nodul de sters e fiu stanga pentru tata
        {
            tata = q -> parent;

            if(fiu_st != NULL && fiu_dr != NULL)
                if(fiu_st -> priority <= fiu_dr -> priority)
                {
                    node* nepot_stanga_dreapta = fiu_st -> right;

                    q -> parent = fiu_st;
                    q -> left = fiu_st -> right;
                    fiu_st -> right = q;
                    fiu_st -> parent = tata;
                    tata -> left = fiu_st;

                    if(nepot_stanga_dreapta != NULL)
                        nepot_stanga_dreapta -> parent = q;

                }
                else
                {
                    node* nepot_dreapta_stanga = fiu_dr -> left;

                    q -> parent = fiu_dr;
                    fiu_dr -> left = q;
                    fiu_dr -> parent = tata;
                    tata -> left = fiu_dr;
                    q -> right = nepot_dreapta_stanga;

                    if(nepot_dreapta_stanga != NULL)
                        nepot_dreapta_stanga -> parent = q;
                }
            else if(fiu_st == NULL)
            {
                node* nepot_dreapta_stanga = fiu_dr -> left;

                q -> parent = fiu_dr;
                fiu_dr -> left = q;
                fiu_dr -> parent = tata;
                tata -> left = fiu_dr;
                q -> right = nepot_dreapta_stanga;

                if(nepot_dreapta_stanga != NULL)
                    nepot_dreapta_stanga -> parent = q;

            }
            else if(fiu_dr == NULL)
            {
                node* nepot_stanga_dreapta = fiu_st -> right;

                q -> parent = fiu_st;
                q -> left = fiu_st -> right;
                fiu_st -> right = q;
                fiu_st -> parent = tata;
                tata -> left = fiu_st;

                if(nepot_stanga_dreapta != NULL)
                    nepot_stanga_dreapta -> parent = q;

            }
        }
        else
        if((q -> parent -> right) == q)                                  // si nodul de sters e fiu dreapta pentru tata
        {
            tata = q -> parent;

            if(fiu_st != NULL && fiu_dr != NULL)
                if(fiu_st -> priority <= fiu_dr -> priority)
                {
                    node* nepot_stanga_dreapta = fiu_st -> right;

                    q -> parent = fiu_st;
                    q -> left = fiu_st -> right;
                    fiu_st -> right = q;
                    fiu_st -> parent = tata;
                    tata -> right = fiu_st;

                    if(nepot_stanga_dreapta != NULL)
                        nepot_stanga_dreapta -> parent = q;

                }
                else
                {
                    node* nepot_dreapta_stanga = fiu_dr -> left;

                    q -> parent = fiu_dr;
                    fiu_dr -> left = q;
                    fiu_dr -> parent = tata;
                    tata -> right = fiu_dr;
                    q -> right = nepot_dreapta_stanga;

                    if(nepot_dreapta_stanga != NULL)
                        nepot_dreapta_stanga -> parent = q;
                }
            else if(fiu_st == NULL)
            {
                node* nepot_dreapta_stanga = fiu_dr -> left;

                q -> parent = fiu_dr;
                fiu_dr -> left = q;
                fiu_dr -> parent = tata;
                tata -> right = fiu_dr;
                q -> right = nepot_dreapta_stanga;

                if(nepot_dreapta_stanga != NULL)
                    nepot_dreapta_stanga -> parent = q;

            }
            else if(fiu_dr == NULL)
            {
                node* nepot_stanga_dreapta = fiu_st -> right;

                q -> parent = fiu_st;
                q -> left = fiu_st -> right;
                fiu_st -> right = q;
                fiu_st -> parent = tata;
                tata -> right = fiu_st;

                if(nepot_stanga_dreapta != NULL)
                    nepot_stanga_dreapta -> parent = q;

            }
        }
    }

    if(q -> parent == NULL)
    {
        delete q;
        root = NULL;
    }
    else if((q -> parent -> left) == q)     // daca radacina ajunge sa fie frunza stanga
    {
        q -> parent -> left = NULL;
        delete q;
    }
    else if((q -> parent -> right) == q)    // daca radacina ajunge sa fie frunza dreapta
    {
        q -> parent -> right = NULL;
        delete q;
    }
}


int main()
{

    int n, x, y, i, z;

    fin >> n;
    for(i = 1; i <= n; i++)
    {
        fin >> x >> y;
        if(x == 1)
        {
            Insert(treap, y);
        }
        else if(x == 2)
        {
            int nr_stergeri = 0;
            while(Search(treap, y) == 1)
            {
                Delete(treap, y);
                nr_stergeri++;
            }
            fout << y << " a fost sters de " << nr_stergeri << " ori.\n";
        }
        else if(x == 3)
        {
            fout << Search(treap, y) << "\n";
        }
        else if(x == 4)
        {
            succ = 0;
            Succesor(treap, y);
            fout << succ << "\n";
        }
        else if(x == 5)
        {
            prec = 0;
            Predecesor(treap, y);
            fout << prec << "\n";
        }
        else if(x == 6)
        {
            fin >> z;
            AfisareNumere(treap, y, z);
            fout << "\n";
        }
    }

    return 0;
}
