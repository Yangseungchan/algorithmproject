
//Project
#if 1
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <conio.h>

#pragma warning(disable:4996)

#define MAXLINE 300
#define _CRT_NONSTDC_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#define RED 'R'
#define BLACK 'B'
#define MAXDIST 0x7fffffff
#define MAXSITE 100

int chk[50];	//avoid duplicate value

typedef struct node {
	struct node* left;
	struct node* right;
	struct node* parent;
	int key;				//Hotel_ID or Member_ID
	char color;
	//Hotel RB
	int price;
	struct node *reservation;	//호텔 RBT안의 예약정보 RBT의 root
	//Reservation RB
	int time;		//예약 날짜
	int duration;	//머무는 기간
}Node;

typedef struct __city {
	char name[100];
	double x;
	double y;
}City;

typedef struct Member
{
	int  id[10];
}member; 

typedef struct __date {
	int month;
	int day;
}Date;

void getdateinformation(Date *d1, Date *d2, char *start, char *finish) {
	char *token;
	token = strtok(start, "/"); d1->month = atoi(token);
	token = strtok(NULL, "/"); d1->day = atoi(token);
	token = strtok(finish, "/"); d2->month = atoi(token);
	token = strtok(NULL, "/"); d2->day = atoi(token);
}


void exit_prog(void)
{
	system("cls");
	printf("\t\t=================================================\n");
	printf("\t\t|                                               |\n");
	printf("\t\t|        -----------------------------          |\n");
	printf("\t\t|           TOUR RESREVATION SYSTEM             |\n");
	printf("\t\t|        -----------------------------          |\n");
	printf("\t\t|                  THANK YOU                    |\n");
	printf("\t\t|                                               |\n");
	printf("\t\t|                                               |\n");
	printf("\t\t|              BROUGHT TO YOU BY                |\n");
	printf("\t\t|                |  TEAM 9  |                   |\n");
	printf("\t\t|                                               |\n");
	printf("\t\t=================================================\n\n\n");
	_getch();
	exit(1);
}

void login(member *info)
{
	int i;
	int temp = 0, count = 0;

	printf("\n  =======================  LOGIN FORM  =======================\n");
	printf("  1)LOGIN\n");
	printf("  2)EXIT\n");
	printf("  Select the menu: ");
	scanf("%d", &i);
	system("cls");
	if (i == 1)
	{
		printf("\n  =======================  LOGIN FORM  =======================\n  ");
		while (1) {
			printf(" \n                   ENTER USERID(6digit): ");
			scanf("%d", &temp);
			count = (int)log10(temp) + 1;
			if (count == 6)
			{
				printf("\n\n\n       WELCOME TO OUR SYSTEM !! YOUR LOGIN IS SUCCESSFUL");
				printf("\n\n\n\t\tPress any key to continue...");
				//info[index].id = temp;		//
				_getch();
				break;
			}
			else {
				printf("\n                 - PLEASE ENTER 6DIGIT NUMBER -\n");
			}
		}
	}
	if (i == 2) {
		exit_prog();
	}
}
/////////////////shorted path
void dijkstra(int **arr, int *dist, int *path, int a, int b, int n) {
	int i, j, k = 0;
	int res = 0, fg = 0;
	int x = b;


	for (i = 0; i < n; i++) {
		if (dist[i] + arr[i][b] == dist[b] && i != b && i != a) {
			path[k++] = i;
		}
	}
	int ini = 0;
	while (1) {
		int x = k;
		if (ini == k)
			break;
		for (i = ini; i < x; i++) {
			for (j = 0; j < n; j++) {
				if ((dist[j] + arr[j][path[i]] == dist[path[i]]) && j != path[i] && j != a) {
					path[k++] = j;
				}
			}
		}
		ini = x;
	}
}

int *Shortest_Path(int **arr, int n, int s, int d) {		// n = number of sites, s = start site, d = destination
	int i, j, min, bk;

	int *flag = (int*)malloc(sizeof(int)*n);
	int *dist = (int*)malloc(sizeof(int)*n);

	int *path = (int*)malloc(sizeof(int)*n);
	for (j = 0; j < n; j++)
		path[j] = MAXSITE;

	for (i = 0; i < n; i++) {
		flag[i] = 0;
		dist[i] = MAXDIST;
	}

	*(dist + s) = 0;
	for (i = 0; i < n; i++) {
		min = MAXDIST;
		for (j = 0; j < n; j++) {
			if (min > dist[i] && flag[j] == 0) {
				min = dist[i] + arr[i][j];
				bk = j;
			}
		}

		flag[bk] = 1;
		for (j = 0; j < n; j++) {
			if (dist[j] > arr[bk][j] + dist[bk] && arr[bk][j] != 1001)
				dist[j] = arr[bk][j] + dist[bk];
		}
	}

	dijkstra(arr, dist, path, s, d, n);

	free(dist);
	free(flag);

	return path;
}



//////////////////////////////////

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

Node* RB_Insert(Node* T, int key, int mode, int time, int duration, int site_index) {		//key = ID, mode 1 : hotel, mode 0 : information on reservation

	Node *n = (Node*)malloc(sizeof(Node));
	if (mode) {
		n->price = 1 + key + site_index;		//accommodation fee = Hotel_ID + Site_index
		n->reservation = (Node*)malloc(sizeof(Node));
	}
	else {
		n->time = time;
		n->duration = duration;
	}
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
			//printf("key %d already inserted in RBT!\n", key);
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

Node* RB_Delete(Node* T, int key) {			//cancel resevation only
	Node *target = T;
	Node *tmp = NULL;
	Node *x = NULL;
	char t_color;

	while (1) {
		if (target == NULL) {
			//printf("Can't find the key %d!\n", key);
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
	
	return T;
}

Node *RB_Search(Node *T, int key) {
	Node *p = T;

	while (p) {
		if (p->key > key) {
			p = p->left;
		}
		else if (p->key < key) {
			p = p->right;
		}
		else {
			return p;
		}
	}
	return NULL;
}

City citylist[100] = { {"Seoul", 37.566, 126.978}, {"Beijing", 39.900, 116.401}, {"Tokyo", 35.689, 139.69}
	,{"Busan", 35.179, 129.074},{"Jeju", 33.499, 126.531}, {"Kyoto", 35.011, 135.768}, {"Sapporo", 43.062, 141.354}
	,{"Sydney", -33.873, 151.206}, {"Macau", 22.180, 113.537}, {"Bangkok", 13.753, 100.501},{"Jakarta",-6.216,106.854}
	,{"Bandung",-6.903,107.633}, {"Bali",-8.672,115.213}, {"Singapore",1.371,103.855}, {"Bangkok",13.784,100.523}
	,{"Hochimin",10.842,106.692}, {"Kuala lumpur",3.152,101.678}, {"Kota manado",1.494,124.858}, {"Perth",-31.956,115.871}
	,{"Melbourne",-37.776,144.929}, {"Sydney",-33.730,151.155}, {"Canberra",-35.313,149.117}, {"Brisbane",-27.461,153.029}
	,{"Gold coast",-28.375,153.399}, {"Wellington",-41.281,174.778}, {"Christchurch",-43.521,172.601}, {"Maynila",14.593,120.982}
	,{"Taipei",25.057,121.575}, {"ChungChing",29.426,106.913}, {"HongKong",22.292,114.132}, {"Shanghai",31.341,121.516}
	,{"Yokohama",35.461,139.632}, {"Osaka",34.703,135.487}, {"Hiroshima",34.393,132.461}, {"Indore",22.713,75.876}
	,{"Agra",27.203,78.014}, {"Dakah",23.825,90.413}, {"Kolakata",22.581,88.351}, {"Myanmar",21.868,96.871}
	,{"Ottawa", 45.420, -75.717}, {"Montreal", 45.529, -73.586}, {"Quebec", 46.802, -71.276}, {"Vancouver", 49.285, -123.11}
	,{"Toronto", 43.998, -79.418}, {"Detroit", 42.438, -82.933}, {"Chicago", 42.188, -87.381}, {"Cleveland", 41.519, -81.738}
	, {"Pittsburgh", 40.752, -79.859}, {"New York", 41.016, -73.987}, {"Washington", 38.912, -77.032}, {"Seattle", 47.602, -122.35}
	,{"San Francisco", 38.187, -122.56}, {"Los Angeles", 34.748, -117.45}, {"Las Vegas", 36.536, -113.20}, {"San Diego", 31.347, -119.62}
	,{"Kansas City", 39.370, -95.173}, {"St.Louis", 38.754, -90.399}, {"Cincinnati", 39.100, -84.520}, {"Atlanta", 33.763, -84.323}
	,{"Miami", 25.761, -80.193}, {"Habana", 23.061, -82.332}, {"Mexico city", 19.394, -99.103}, {"Guatemala", 14.660, -90.505}
	,{"Panama", 8.934, -79.506}, {"Bogota", 4.573, -74.138}, {"Brasilia", -15.779, -47.849}, {"Sao Paulo", -23.590, -46.641}
	,{"Buenos Aires", -34.606, -58.392}, {"Santiago", -33.407, -70.708}, {"Santa Cruz", -17.807, -63.178}
	,{"London", 51.507, -0.127}, {"Paris", 48.859, 2.352}, {"Berlin", 52.514, 13.409}, {"Frankfurt", 50.110, 8.681}
	,{"Zurich", 47.370, 8.541}, {"Wien", 48.208, 16.372}, {"Oslo", 59.915, 10.751},
	{"Stockholm", 59.331, 18.069}, {"Helsinki", 60.172, 24.936}, {"Reykjavik", 64.148, -21.943}, {"Dublin", 53.351, -6.263},
	{"Kobenhavn", 55.677, 12.567}, {"Lisboa", 38.722, -9.139}, {"Madrid", 40.421, -3.704}, {"Barcelona", 41.384, 2.172},
	{"Roma", 41.904, 12.497}, {"Athenae", 37.982, 23.727}, {"Bucharest", 44.428, 26.101}, {"Kiev", 50.453, 30.517},
	{"Praha", 50.074, 14.437}, {"Bruxelles", 50.851, 4.349}, {"Bern", 46.947, 7.447}, {"Amsterdam", 52.368, 4.896},
	{"Warszawa", 52.229, 21.007}, {"Budapest", 47.498, 19.042},
	{"Bratislava", 48.148, 17.103}, {"Sofia", 42.698, 23.321}, {"Zagreb", 45.817, 15.978}, {"Cape Town", -33.927, 18.414 }, {"Cairo", 30.046, 31.234},
};

double GetDistance(City pt1, City pt2) {
	double pi = 0.017453292519943295;
	double a = 0.5 - cos((pt2.x - pt1.x)*pi) / 2 + cos(pt1.x * pi) * cos(pt2.x * pi) * (1 - cos((pt2.y - pt1.y) * pi)) / 2;
	return 12742 * asin(sqrt(a));
}

void Print_Destination(void) {
	int i;
	printf("\n\t\t\t\t\t\t\t< City List >");
	printf("\n     ==================================================================================================================\n");
	for (i = 0; i < 100; i++) {
		if (i % 8 == 0)
			printf("    ");
		printf("%14s", citylist[i].name);
		if (i % 8 == 7)
			printf("\n");
	}
	printf("\n     ==================================================================================================================\n\n\n");
}

int **InsertSite(void) {
	int i, j, x, y;
	int **Site_Graph = (int**)malloc(sizeof(int*) * 100);	/* 2 dimensional matrix for graph */
	for (i = 0; i < 100; i++) {
		Site_Graph[i] = (int*)malloc(sizeof(int) * 100);
		for (j = 0; j < 100; j++) {
			if (i == j) Site_Graph[i][j] = 0;
			else Site_Graph[i][j] = MAXDIST;
		}
	}
	srand(time(NULL));
	for (i = 0; i < 300;) {
		x = rand() % 100;
		y = rand() % 100;
		if (x <= y || (Site_Graph[y][x] != MAXDIST && Site_Graph[y][x] != 0)) continue;
		Site_Graph[y][x] = (int)GetDistance(citylist[y], citylist[x]);
		Site_Graph[x][y] = Site_Graph[y][x];
		i++;
	}

	return Site_Graph;
}

Node** Hotel_insert(void)
{
	int i, j;
	Node **Site_hotel = (Node**)malloc(sizeof(Node*) * 100);

	for (i = 0; i < 100; i++) {		// i = site index
		Node *T = NULL;
		for (j = 0; j < 100; j++) {
			T = RB_Insert(T, j, 1, 0, 0, i);		// j = hotel index
		}
		Site_hotel[i] = T;
	}

	return Site_hotel;
}

int main() {
	int **arr = InsertSite();
	Node **hotel = Hotel_insert();

	//Print_RBT(hotel[5]);
	//Node *c = RB_Search(hotel[5], 96);
	//printf("%d\n", c->price);


	char c, *str;
	char startdate[MAXLINE];
	char finishdate[MAXLINE];
	int budget, numofdest, choice, duration;
	int month_day[13] = { 0, 31, 59, };			//count month as day
	int i, j;
	int *destination;
	member info[100];
	Date *start;
	Date *finish;
	start = (Date *)malloc(sizeof(Date));
	finish = (Date *)malloc(sizeof(Date));

	for (i = 3; i <= 12; i++) {
		month_day[i] = month_day[i - 1] + 30 + ((i / 8) + i) % 2;
	}

	system("cls");
	printf("\t\t=================================================\n");
	printf("\t\t|                                               |\n");
	printf("\t\t|        -----------------------------          |\n");
	printf("\t\t|           TOUR RESREVATION SYSTEM             |\n");
	printf("\t\t|        -----------------------------          |\n");
	printf("\t\t|                                               |\n");
	printf("\t\t|                                               |\n");
	printf("\t\t|                                               |\n");
	printf("\t\t|              BROUGHT TO YOU BY                |\n");
	printf("\t\t|               |  TEAM 9  |                    |\n");
	printf("\t\t|                                               |\n");
	printf("\t\t=================================================\n\n\n");
	printf(" \n Press any key to continue..");
	_getch();
	system("cls");
	login(info);
	_getch();
	system("cls");
	printf("\n  =================================\n");
	printf("      TOUR RESERVATION SYSTEM");
	printf("\n  =================================");
	printf("\n  1>> Reservation Recommendation");
	printf("\n  ------------------------");
	printf("\n  2>> View Reservation State");
	printf("\n  ------------------------");
	printf("\n  3>> Exit");
	printf("\n  ------------------------");
	printf("\n\n  --> ");
	while (1) {
		scanf("%d", &choice);
		switch (choice)
		{
		case 1:
			while (1) {
				system("cls");
				printf("\t\t============== Reservation System ===============\n");
				printf("\t\t         Start  Date [ ex) '10/26' ] : "); scanf("%s", startdate);
				printf("\t\t         Finish Date [ ex) '10/29' ] : "); scanf("%s", finishdate);
				getdateinformation(start, finish, startdate, finishdate);

				duration = month_day[finish->month] - month_day[start->month] + finish->day - start->day + 1;
				if (duration <= 0)
					duration += 365;

				printf("\t\t                                               \n");
				printf("\t\t         Budget($) : "); scanf("%d", &budget);
				printf("\t\t         Num of Destinations : "); scanf("%d", &numofdest);
				if (numofdest > duration) {
					printf("\n\t\t\t Invalid Number\n\t\t\t Press 'Enter' to continue..");
					scanf("%c", &c);
					scanf("%c", &c);
					continue;
				}
				destination = (int *)malloc(sizeof(int *) * numofdest);
				str = (char *)malloc(sizeof(char) * MAXLINE);
				
				printf("\t\t         1) See Destinations List   2) Input Destination\n\t\t         : ");
				scanf("%d", &i);
				if (i == 1) {
					Print_Destination();
				}

				for (i = 0; i < numofdest; i++) {
					printf("\t\t         Destination %d : ", i + 1);
					scanf("%s", str);
					for (j = 0; j < 100; j++) {
						if (!strcmp(str, citylist[j].name)) {
							destination[i] = j;
							////////////////////////////////
							int *path = Shortest_Path(arr, 100, 0, destination[i]);
							printf("\n(%s)", citylist[0].name);
							for (int k = 0; k < 100; k++) {
								if (path[k] == MAXSITE) break;
								printf("->(%s)", citylist[path[k]].name);
							}
							printf("->(%s)\n", citylist[destination[i]].name);
							/////////////////////////////////
							free(path);
							break;
						}
					}
				}
				printf("\t\t                                                 \n");
				printf("\t\t           |  SKKU Travel Agency  |              \n");
				printf("\t\t                                                 \n");
				printf("\t\t=================================================\n\n\n");
				break;
			}
			break;
		case 2:
			; break;
		case 3:			//exit
			return 0;
			break;
		default:
			printf("Invalid Value\n--> ");
			continue;
		}
		break;
	}


	//system("cls");
	return 0;

}
#endif