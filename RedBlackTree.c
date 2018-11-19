#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

typedef struct __node{
    int data; 
    int color; /* black is 0, red is 1*/
    struct __node *left;
    struct __node *right;
    struct __node *parent;
}Node; /* Node for BST or RBT */

typedef struct __bst{
    Node *Root;
}BST;

typedef struct __qnode{
    Node *node;
    struct __qnode *next;
}Qnode;

typedef struct _queue{
    Qnode *front;
    Qnode *rear;
}Queue;

void QueueInit(Queue *q){
    q->front = NULL;
    q->rear = NULL;
}

int QisEmpty(Queue *q){
    if(q->front == NULL)
        return 1;
    else
        return 0;
}

void InitBST(BST *T){
    T->Root = NULL;
}

void Enqueue(Queue *q, Node *node){
    Qnode *newNode = (Qnode *)malloc(sizeof(Qnode)); /* allocating new node */
    newNode->next = NULL;
    newNode->node = node;
    if(QisEmpty(q)){ /* when Queue is empty */
        q->front = newNode;
        q->rear = newNode;
    }
    else{ /* Q doesn't Empty */
        q->rear->next = newNode;
        q->rear = newNode;
    }
}

int Dequeue(Queue *q){ /* dequeue the node from q and return the dequeued node data */
    Qnode *DelNode; int retdata;
    if(QisEmpty(q)){
        return -2;
    }
    DelNode = q->front;
    retdata = DelNode->node->data;
    q->front = q->front->next;
    free(DelNode);
    return retdata;
}

Node *AllocNode(void){
    return calloc(1, sizeof(Node));
}

void SetNode(Node *n, int data, Node *n1, Node *n2, Node *parent){
    n->data = data; 
    n->left = n1; 
    n->right = n2;
    n->parent = parent;
    n->color = 0;
}

void TREELEFTROTATE(BST *T, Node *x){
    Node *y;
    if(x == NULL){
        printf("the given node doesn't exist\n"); exit(-1);
    }
    y = x->right;
    x->right = y->left;
    if(y->left != NULL){
        y->left->parent = x;
    }
    y->parent = x->parent;
    if(x->parent == NULL){
        T->Root = y;
    }
    else if(x == x->parent->left){
        x->parent->left = y;
    }
    else{
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

void TREERIGHTROTATE(BST *T, Node *x){
    Node *y;
    if(x == NULL){
        printf("the given node doesn't exist\n"); exit(-1);
    }
    y = x->left;
    x->left = y->right;
    if(y->right != NULL){
        y->right->parent = x;
    }
    y->parent = x->parent;
    if(x->parent == NULL){
        T->Root = y;
    }
    else if(x == x->parent->left){
        x->parent->left = y;
    }
    else{
        x->parent->right = y;
    }
    y->right = x;
    x->parent = y;
}

Node *TREESEARCH(BST *T, int data){
    Node *p; p = T->Root;
    while(p){
        if(p->data > data){
            p = p->left;
        }
        else if(p->data < data){
            p = p->right;
        }
        else{
            return p;
        }
    }
    return NULL;
}


void RBTREEINSERTFIX(BST *T, Node *x){
    Node *y;
    while(x != T->Root && x->parent->color == 1){
        if(x->parent == x->parent->parent->left){
            y = x->parent->parent->right;
            if(y->color == 1){
                x->parent->color = 0;
                y->color = 0;
                x->parent->parent->color = 1;
                x = x->parent->parent;
            }
            else if(x == x->parent->right){
                x = x->parent;
                TREELEFTROTATE(T, x);
            }
            x->parent->color = 0;
            x->parent->parent->color = 1;
            TREERIGHTROTATE(T, x->parent->parent);
        }
        else{
            y = x->parent->parent->left;
            if(y->color == 1){
                x->parent->color = 0;
                y->color = 0;
                x->parent->parent->color = 1;
                x = x->parent->parent;
            }
            else if(x == x->parent->left){
                x = x->parent;
                TREERIGHTROTATE(T, x);
            }
            x->parent->color = 0;
            x->parent->parent->color = 1;
            TREELEFTROTATE(T, x->parent->parent);
        }
    }
    T->Root->color = 0;
}



void TREEINSERT(BST *T, int data){
    Node *x; Node *y; Node *z;
    y = NULL; x = T->Root; z = AllocNode(); SetNode(z, data, NULL, NULL, NULL); // allocating z
    while(x != NULL){
        y = x;
        if(z->data < x->data){
            x = x->left;
        }
        else{
            x = x->right;
        }
    }
    z->parent = y;
    if(y == NULL){
        T->Root = z;
    }
    else if(z->data < y->data){
        y->left = z;
    }
    else{
        y->right = z;
    }
    z->left = NULL;
    z->right = NULL;
    z->color = 1;
    RBTREEINSERTFIX(T, z);
}

Node *MAXIMUM(Node *n){
    Node *p = n;
    while(p->right){
        p = p->right;
    }
    return p;
}

Node *MINIMUM(Node *n){
    Node *p = n;
    while(p->left){
        p = p->left;
    }
    return p;
}

Node *FindSuccessor(BST *T, int data){
    Node *p; Node *pp;
    p = TREESEARCH(T, data);
    if(!p){
        return NULL;
    }
    if(p->right){ /* when right of p does exist */
        return MINIMUM(p->right);
    }
    pp = p->parent;
    while((pp != NULL) && (p == pp->right)){
        p = pp;
        pp = pp->parent;
    }
    return pp;
}

Node *FindPredecessor(BST *T, int data){
    Node *p; Node *pp;
    p = TREESEARCH(T, data);
    if(!p){
        return NULL;
    }
    if(p->left){
        return MAXIMUM(p->left);
    }
    pp = p->parent;
    while((pp != NULL) && (p == pp->left)){
        p = pp;
        pp = pp->parent;
    }
    return pp;
}

/*Node *NEAREST_NEIGHBOR(BST *T, int data){ 
    Node *Self; Node *Predecessor; Node *Successor; int difference2 = 0; int difference3 = 0;
    Self = TREESEARCH(T, data);
    Predecessor = FindPredecessor(T, data);
    Successor = FindSuccessor(T, data);
    if(Self){
        return Self;
    }
    if(Predecessor){
        difference2 = data - Predecessor->data; if(difference2 < 0){difference2 *= -1;}
    }
    if(Successor){
        difference3 = data - Successor->data; if(difference3 < 0){difference3 *= -1;}
    }
    if(difference2 == 0 && difference3){
        return NULL;
    }
    else if(differnce2 == 0){
        return Predecessor;
    }
    else if(differnce3 == 0){
        return Successor;
    }
    else{
        if(difference2 < difference3){
            return Predecessor;
        }
        else{
            return Successor;
        }
    }
}*/

void RBTREEDELETEFIX(BST *T, Node *x){
    Node *w; Node *NIL; NIL = (Node *)malloc(sizeof(Node));
    NIL->color = 0; NIL->data = -1; NIL->left = NIL; NIL->right = NIL; NIL->parent = NULL;
    while(x != T->Root && x->color == 0){
        if(x == x->parent->left){
            w = x->parent->right;
            if(w == NULL){
                w = NIL;
            }
            if(w->color == 1){
                w->color = 0;
                if(x->parent != NULL){
                    x->parent->color = 1;
                    TREELEFTROTATE(T, x->parent);
                }
                w = x->parent->right;
            }
            if(w == NULL){
                w = NIL;
            }
            if(w->left->color == 0 && w->right->color == 0){
                if(w != NIL){
                    w->color = 1;
                }
                x = x->parent;
            }
            else if(w->right->color == 0){
                w->left->color = 0;
                if(w != NIL){
                    w->color = 1;
                    TREERIGHTROTATE(T, w);
                }
                w = x->parent->right;
            }
            if(x->parent != NULL){
                if(w != NIL){
                    w->color = x->parent->color;
                }
                x->parent->color = 0;
            }
            w->right->color = 0;
            if(x->parent != NULL){
                TREELEFTROTATE(T, x->parent);
            }
            x = T->Root;
        }
        else{
            w = x->parent->left;
            if(w == NULL){
                w = NIL;
            }
            if(w->color == 1){
                w->color = 0;
                if(x->parent != NULL){
                    x->parent->color = 1;
                    TREERIGHTROTATE(T, x->parent);
                }
                w = x->parent->left;
            }
            if(w == NULL){
                w = NIL;
            }
            if(w->left->color == 0 && w->right->color == 0){
                if(w != NIL){
                    w->color = 1; 
                }
                x = x->parent;
            }
            else if(w->left->color == 0){
                w->right->color = 0;
                if(w != NIL){
                     w->color = 1;
                     TREELEFTROTATE(T, w);
                }
                w = x->parent->left;
            }
            if(x->parent != NULL){
                if(w != NIL){
                    w->color = x->parent->color;
                }
                x->parent->color = 0;
            }
            w->left->color = 0;
            if(x->parent != NULL){
                TREERIGHTROTATE(T, x->parent);
            }
            x = T->Root;
        }    
    }
    x->color = 0; 
}


Node *TREEDELETE(BST *T, int data){
    Node *x; Node *y;
    Node *z; z = TREESEARCH(T, data);
    if(z == NULL){
        return NULL;
    }
    if(z->left == NULL || z->right == NULL){
        y = z;
    }
    else{
        y = FindSuccessor(T, data);
    }
    if(y->left != NULL){
        x = y->left;
    }
    else{
        x = y->right;
    }
    if(x != NULL){
        x->parent = y->parent;
    }
    if(y->parent == NULL){
        T->Root = x;
    }
    else if(y == y->parent->left){
        y->parent->left = x;
    }
    else{
        y->parent->right = x;
    }
    if(y != z){
        z->data = y->data;
    }
    if(y->color == 0){
        if(x != NULL){
            RBTREEDELETEFIX(T, x);
        }
    }
    return y;
}

int max(int a, int b){ 
    if(a>=b){
        return a;
    }
    return b;
}

int get_height(Node *bstnode){
    int height = 0;
    if(bstnode != NULL){
        height = 1 + max(get_height(bstnode->left), get_height(bstnode->right));
    }
    return height;
}


/*void PrintTree(BST *T){ // level order visiting using Queue 
    int data; Queue *q; Node *BSTnode; Node *EmptyNode; int count = 0; int level = 1;
    int tree_level = get_height(T->Root); 
    printf("tree_level : %d\n", tree_level);
    q = (Queue *)malloc(sizeof(Queue));
    EmptyNode = (Node *)malloc(sizeof(Node));
    EmptyNode->data = -1; EmptyNode->left = NULL; EmptyNode->right = NULL; EmptyNode->parent = NULL;
    QueueInit(q); Enqueue(q, T->Root);
    while(!QisEmpty(q)){
        if(level == tree_level + 1){
            break;
        }
        data = Dequeue(q);
        if(data == -2){
            break;
        }
        else if(data == -1){
            if(count == (int)pow(2 ,(double)(level-1)) - 1){
                for(int i=0; i < (int)pow(2, (double)(tree_level - 2)) - (level-1); i++){
                    putchar(' '); putchar(' ');
                }
            }
            printf("__ "); ++count;
            if(count == pow(2, (double)level) - 1){
                putchar('\n'); ++level;
            }
            Enqueue(q, EmptyNode);
            Enqueue(q, EmptyNode);
        }
        else{
            if(count == (int)pow(2 ,(double)(level-1)) - 1){
                for(int i=0; i < (int)pow(2, (double)(tree_level - 2)) - (level-1); i++){
                    putchar(' '); putchar(' ');
                }
            }
            printf("%d ", data); ++count;
            if(count == pow(2, (double)level) - 1){
                putchar('\n'); ++level;
            }
            BSTnode = TREESEARCH(T, data);
            if(BSTnode->left != NULL){
                Enqueue(q, BSTnode->left);
            }
            else{
                Enqueue(q, EmptyNode);
            }
            if(BSTnode->right != NULL){
                Enqueue(q, BSTnode->right);
            }
            else{
                Enqueue(q, EmptyNode);
            }
        }
    }
} */

void padding(char ch, int n){
  int i;
  for (i = 0; i < n; i++)
    putchar(ch);
}

void PrintTree(Node *root, int level ){
    int i;
    if(root == NULL){
        padding ( '\t', level );
        puts ( "|" );
    }
    else{
        PrintTree(root->right, level+1);
        padding('\t', level);
        printf("%d\n", root->data);
        PrintTree(root->left, level+1);
    }
}


int main(){
    BST *BSTree; InitBST(BSTree); srand(time(NULL)); int randnum;
    Node *x; int i;
    for(i=0; i<50; i++){
        randnum = rand() % 100;
        if(TREESEARCH(BSTree, randnum)){
            i--;
            continue;
        }
        printf("num : %d\n", randnum);
        TREEINSERT(BSTree, randnum);
        //PrintTree(BSTree->Root, 0);
    }
    printf("insertion is end\n");
    PrintTree(BSTree->Root, 0);
    printf("\n\n\n\n");
    //TREEDELETE(BSTree, randnum);
    //PrintTree(BSTree->Root, 0);
    return 0;
}