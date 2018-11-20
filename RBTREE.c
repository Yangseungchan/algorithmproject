#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define RED 'R'
#define BLACK 'B'

int chk[50];	//avoid duplicate value

typedef struct node {
	struct node* left;
	struct node* right;
	struct node* parent;
	int key;
	char color;
}Node;

char depth[100];
int di;

void push(char c)
{
	depth[di++] = ' ';
	depth[di++] = c;
	depth[di++] = ' ';
	depth[di++] = ' ';
	depth[di] = 0;
}

void Print_RBT(Node* T) {
	printf("(%2d-%c)", T->key, T->color);
	printf("\n");
	if (T->left != NULL || T->right != NULL) {
		printf("%s '--", depth);
		push('|');
		if (T->right)
			Print_RBT(T->right);
		else printf("\n");
		depth[di -= 4] = 0;
		printf("%s '--", depth);
		push(' ');
		if (T->left)
			Print_RBT(T->left);
		else printf("\n\n");
		depth[di -= 4] = 0;
	}
}

Node* LEFT_Rotate(Node* T, Node *x) {
	Node *tmp;

	tmp = x->right;
	x->right = tmp->left;

	if (tmp->left != NULL)
		(tmp->left)->parent = x;

	tmp->parent = x->parent;

	if (x->parent == NULL)
		T = tmp;
	else if (x == (x->parent)->left)
		(x->parent)->left = tmp;
	else
		(x->parent)->right = tmp;

	tmp->left = x;
	x->parent = tmp;

	return T;
}

Node* RIGHT_Rotate(Node* T, Node *y) {
	Node *tmp;

	tmp = y->left;
	y->left = tmp->right;

	if (tmp->right != NULL)
		(tmp->right)->parent = y;

	tmp->parent = y->parent;

	if (y->parent == NULL)
		T = tmp;
	else if (y == (y->parent)->left)
		(y->parent)->left = tmp;
	else
		(y->parent)->right = tmp;

	tmp->right = y;
	y->parent = tmp;

	return T;
}

Node* RB_Insert_Fixup(Node* T, Node *x) {
	Node *u = NULL;

	while ((x->parent)->color == RED) {
		if (x->parent == (x->parent->parent)->left) {
			u = (x->parent->parent)->right;

			if (u != NULL && u->color == RED) {
				(x->parent)->color = BLACK;
				u->color = BLACK;
				(x->parent->parent)->color = RED;

				x = (x->parent->parent);
			}
			else {
				if (x == (x->parent)->right) {
					x = (x->parent);

					T = LEFT_Rotate(T, x);
				}

				(x->parent)->color = BLACK;
				(x->parent->parent)->color = RED;

				T = RIGHT_Rotate(T, x->parent->parent);
			}
		}
		else {
			u = (x->parent->parent)->left;

			if (u != NULL && u->color == RED) {
				(x->parent)->color = BLACK;
				u->color = BLACK;

				(x->parent->parent)->color = RED;

				x = (x->parent->parent);
			}
			else {
				if (x == (x->parent)->left) {
					x = (x->parent);

					T = RIGHT_Rotate(T, x);
				}

				(x->parent)->color = BLACK;
				(x->parent->parent)->color = RED;

				T = LEFT_Rotate(T, x->parent->parent);
			}
		}
		if (x->parent == NULL) break;
	}
	T->color = BLACK;

	return T;
}

Node* RB_Insert(Node* T, int key, int mode) {
	if (mode) {
		printf("\nInsert key %d\n", key);
	}
	Node *n = (Node*)malloc(sizeof(Node));
	Node *tmp = T;
	Node *p = NULL;

	n->left = NULL;
	n->right = NULL;
	n->color = RED;
	n->key = key;

	while (tmp != NULL) {
		p = tmp;

		if (key < tmp->key)
			tmp = tmp->left;
		else if (key > tmp->key)
			tmp = tmp->right;
		else {
			printf("key %d already inserted in RBT!\n", key);
			return T;
		}
	}

	n->parent = p;
	if (p == NULL) {
		n->color = BLACK;
		T = n;
		return T;
	}
	else if (key < p->key)
		p->left = n;
	else
		p->right = n;

	T = RB_Insert_Fixup(T, n);

	if (mode) {
		Print_RBT(T);
		printf("\n");
	}

	return T;
}

//DELETION
Node* tree_transplant(Node* T, Node* t, Node* c) {
	if (t->parent == NULL)
		T = c;
	else if (t == (t->parent)->left)
		(t->parent)->left = c;
	else
		(t->parent)->right = c;

	if (c != NULL)
		c->parent = t->parent;

	return T;
}

Node* RB_Delete_Fixup(Node* T, Node* x) {
	Node *s = NULL;

	while ((x != T) && x != NULL && (x->color == BLACK)) {	//x != NULL
		if (x == (x->parent)->left) {
			s = (x->parent)->right;

			if (s->color == RED) {
				s->color = BLACK;
				(x->parent)->color = RED;
				T = LEFT_Rotate(T, x->parent);
				s = (x->parent)->right;
			}

			if ((s->left)->color == BLACK && (s->right)->color == BLACK) {
				s->color = RED;
				x = x->parent;
			}
			else if ((s->left)->color == RED && (s->right)->color == BLACK) {
				s->color = RED;
				(s->left)->color = BLACK;
				T = RIGHT_Rotate(T, s);
				s = (x->parent)->right;
			}

			if ((s->right)->color == RED) {
				s->color = (x->parent)->color;
				(s->right)->color = BLACK;
				(x->parent)->color = BLACK;
				T = LEFT_Rotate(T, x->parent);

				x = T;
			}
		}
		else {
			s = (x->parent)->left;
			
			if (s->color == RED) {
				s->color = BLACK;
				(x->parent)->color = RED;
				T = RIGHT_Rotate(T, x->parent);
				s = (x->parent)->left;
			}

			if ((s->left)->color == BLACK && (s->right)->color == BLACK) {
				s->color = RED;
				x = x->parent;
			}
			else if ((s->right)->color == RED && (s->left)->color == BLACK) {
				s->color = RED;
				(s->right)->color = BLACK;
				T = LEFT_Rotate(T, s);
				s = (x->parent)->left;
			}

			if ((s->left)->color == RED) {
				s->color = (x->parent)->color;
				(s->left)->color = BLACK;
				(x->parent)->color = BLACK;
				T = RIGHT_Rotate(T, x->parent);

				x = T;
			}
		}
	}
	if (x != NULL)
		x->color = BLACK;

	return T;
}

Node* RB_Delete(Node* T, int key) {
	Node *target = T;
	Node *tmp = NULL;
	Node *x = NULL;
	char t_color;

	while (1) {
		if (target == NULL) {
			printf("Can't find the key %d!\n", key);
			return T;
		}
		if (key < target->key)
			target = target->left;
		else if (key > target->key)
			target = target->right;
		else
			break;
	}
	t_color = target->color;
	if (target->left == NULL) {
		x = target->right;
		T = tree_transplant(T, target, target->right);
	}
	else if (target->right == NULL) {
		x = target->left;
		T = tree_transplant(T, target, target->left);
	}
	else {
		tmp = target->right;

		while (tmp->left != NULL)
			tmp = tmp->left;
		
		if (tmp->right != NULL || tmp->left != NULL)
			t_color = tmp->color;
		x = tmp->right;

		T = tree_transplant(T, tmp, tmp->right);
		tmp->right = target->right;
		if (tmp->right != NULL)
			(tmp->right)->parent = tmp;

		T = tree_transplant(T, target, tmp);
		tmp->left = target->left;
		if (tmp->left != NULL)
			(tmp->left)->parent = tmp;
		tmp->color = target->color;
	}
	free(target);

	printf("Delete the key %d\n", key);
	Print_RBT(T);
	printf("\n");

	return T;
}

int main(void) {
	int i, tmp;

	Node* T = NULL;

	//2.
	printf("\n2.\n");
	srand(time(NULL));
	for (i = 0; i < 100; i++) {
		tmp = rand() % 100;
		if (chk[tmp] == 1) {
			i--;
		}
		else {
			chk[tmp] = 1;
			T = RB_Insert(T, tmp, 0);
		}
	}
	Print_RBT(T);

	
	//3.
	printf("\n3.\n");
	T = RB_Insert(T, 5, 1);
	T = RB_Insert(T, 7, 1);
	T = RB_Insert(T, 17, 1);
	T = RB_Insert(T, 51, 1);

	//4.
	printf("\n4.\n");
	T = RB_Delete(T, 17);
	T = RB_Delete(T, 5);
	T = RB_Delete(T, 51);
	T = RB_Delete(T, 17);

	return 0;
}