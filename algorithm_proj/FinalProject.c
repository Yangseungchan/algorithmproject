
//Project
#if 1
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
//#include <conio.h>

#pragma warning(disable:4996)

#define MAXLINE 300
#define _CRT_NONSTDC_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#define RED 'R'
#define BLACK 'B'

int chk[50];	//avoid duplicate value

typedef struct node {
	struct node* left;
	struct node* right;
	struct node* parent;
	int key;				//Hotel_ID or Member_ID
	char color;
	//Hotel RB
	int price;
	struct node *reservation;	//»£≈⁄ RBTæ»¿« øπæ‡¡§∫∏ RBT¿« root
	//Reservation RB
	int time;		//호텔 숙박 시작날짜
	int duration;	//숙박일
}Node;

typedef struct __city {
	char name[100];
	double x;
	double y;
}City;

typedef struct Member
{
	int  id[100];
	int index;
}member; 

typedef struct __date {
	int month;
	int day;
}Date;

typedef struct __dayinfo{
	int startday; /* Day 1 */
	int lastday;
	int site_index; /* 첫째날 머무르는 지역의 인덱스 */
	int hotel_index;   /* 해당 site에서 머무를 호텔 인덱스 */
	int checkin_hour; /* checkin 시간, 날짜기록 */
	int checkout_hour;/* checkout 시간, 날짜기록 */
	int transport_cost; int hotel_cost;
}DayInfo;

int user_id;
DayInfo TourInfo[100]; /* index 0 = 첫째날 ~ */
int **arr;
Node **hotel;

void InitTourInfo(){
	for(int i=0; i<100; i++){
		TourInfo[i].site_index = -1;
		TourInfo[i].hotel_index = -1;
		TourInfo[i].checkin_hour = -1;
		TourInfo[i].checkout_hour = -1; 
		TourInfo[i].hotel_cost = 0;
		TourInfo[i].transport_cost = 0;
	}
}

void getdateinformation(Date *d1, Date *d2, char *start, char *finish) {
	char *token;
	token = strtok(start, "/"); d1->month = atoi(token);
	token = strtok(NULL, "/"); d1->day = atoi(token);
	token = strtok(finish, "/"); d2->month = atoi(token);
	token = strtok(NULL, "/"); d2->day = atoi(token);
}


void exit_prog(void)
{
	char ch;
	//system("cls");
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
	scanf("%c",&ch);
	exit(1);
}

void login(member *info)
{
	int i; char ch;
	int temp = 0, count = 0;

	printf("\n  =======================  LOGIN FORM  =======================\n");
	printf("  1)LOGIN\n");
	printf("  2)EXIT\n");
	printf("  Select the menu: ");
	scanf("%d", &i);
	//system("cls");
	if (i == 1)
	{
		printf("\n  =======================  LOGIN FORM  =======================\n  ");
		while (1) {
			printf(" \n                   ENTER USERID(6digit): ");
			scanf("%d", &temp);
			count = (int)log10(temp) + 1;
			if (count == 6)
			{
				info->id[info->index++] = temp;
				printf("\n\n\n       WELCOME TO OUR SYSTEM !! YOUR LOGIN IS SUCCESSFUL");
				printf("\n\n\n\t\tPress any key to continue...");
				//info[index].id = temp;		//
				scanf("%c",&ch);
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

	//printf("Delete the key %d\n", key);
	//Print_RBT(T);
	//printf("\n");

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
	,{"Hochimin",10.842,106.692}, {"Kuala_lumpur",3.152,101.678}, {"Kota_manado",1.494,124.858}, {"Perth",-31.956,115.871}
	,{"Melbourne",-37.776,144.929}, {"Sydney",-33.730,151.155}, {"Canberra",-35.313,149.117}, {"Brisbane",-27.461,153.029}
	,{"Gold_coast",-28.375,153.399}, {"Wellington",-41.281,174.778}, {"Christchurch",-43.521,172.601}, {"Maynila",14.593,120.982}
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
		if(i == 0){
			printf("  ");
		}
		else if (i % 8 == 0)
			printf("    ");
		if(i==0){
			printf("[Departure]%s", citylist[i].name);
		}
		else{
			printf("%14s", citylist[i].name);
		}
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
			Site_Graph[i][j] = 0;
		}
	}
	srand(time(NULL));
	for (i = 0; i < 300;) {
		x = rand() % 100;
		y = rand() % 100;
		if (x <= y || Site_Graph[y][x]) continue;
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

/* siteidx = 지역의 인덱스, duration = 해당 지역에 머물 기억, cost = 최대 가격, infoidx = */
int hotel_reservation(int siteidx, int startday, int userid, int duration, int cost, int infoidx){ /* 사이트 인덱스와 비용을 입력받아 해당 사이트의 호텔 인덱스를 반환받는다 */
	Node *node; int hotelidx; int i; 
	node = hotel[siteidx];
	if(cost < duration * (siteidx)){ /* 가격 상한선보다 호텔의 최소가격이 더 큰 경우 -1반환; 호텔 가격 = duration * (siteidx + hotelidx) */
		return -1;
	}
	else if(cost > duration * (siteidx + 99)){ /* 가격의 최갯값보다 가격의 상한선이 더 큰 경우 = 최대 가격이 가능한 경우 */
		cost = duration * (siteidx + 99);
		hotelidx = 99;
	}
	else{
		for(i=0; i<100; i++){
			if(cost == duration * (siteidx + i)){
				cost = duration * (siteidx + i);
				break;
			}
		}
		hotelidx = i;
	}
	RB_Insert(node, userid, 0, startday, duration, siteidx);
	return hotelidx;
}

int *time_partition(int destination, int duration){ // destination : 출발지, 도착지를 포함한 거점의 개수, duration : 여행의 기간
	if(destination > duration){
		return NULL;
	}
	int i; int result = duration;
	int *timetable; timetable = (int *)malloc(sizeof(int) * (destination - 1));
	for(i=0; i < destination - 2; i++){
		timetable[i] = duration / (destination - 1);
		result -= timetable[i];
	}
	timetable[i] = result;
	return timetable;
}

/* 전역변수 구조체배열로 생성, 할당된 DayInfo에 저장된 정보들 저장 */
int reserve_schedule(member *info, Date *start, Date *finish, int minroute[], int totalbudget, int numofroute, int duration){ 
	/* minroute = 최소경로의 인덱스를 담은 배열; 
	totalbudget = 사용자에게 입력받은 총 예산; 
	numofdest = 최소경로 배열에 포함되어 있는 거점들의 개수(출발지 목적지 포함), numofdest - 1 = tour course의 개수
	duration = 몇일동안 여행하는지에 대한 변수 */
	int total_budget = totalbudget; int numofcourse;
	int part_budget = totalbudget / (numofroute - 1); /* 각각 마다 쓸 수 있는 총 비용(교통비용 + 호텔비용) */
	int *timetable;			/* 시간 테이블 = 각각의 사이트에서 머무를 일 수를 분배한 표 */
	int temp_transcost;     /* index마다의 교통비용 */
	int temp_hotelcost;     /* index마다의 호텔비용 */
	int transport_cost = 0; /* 총 교통비용 */ 
	int hotel_cost = 0;     /* 총 호텔비용 */
	int decrement = 1;
	if(numofroute - 1 > duration){ /* destination number is bigger than duration */
		return -1;
	}
	timetable = time_partition(numofroute, duration);
	for(int i=0; i < numofroute - 1; i++){
		printf("%dth\n", i+1);
		temp_transcost = (int)(0.5 * GetDistance(citylist[minroute[i]], citylist[minroute[i+1]])); /* getting cost by transportation */
		transport_cost += temp_transcost; /* 총 교통수단에 temp_transcost 추가 */
		part_budget -= temp_transcost;
		if(temp_transcost > part_budget){
			printf("You don't have enough money for your transportation.\n");
			return -1;
		}
		if(i == 0){
			TourInfo[i].startday = i+1;
		}
		else{
			TourInfo[i].startday = TourInfo[i-1].lastday + 1;
		}
		TourInfo[i].lastday = TourInfo[i].startday + timetable[i] - 1;
		TourInfo[i].site_index = minroute[i];
		TourInfo[i].checkin_hour = 10 + rand() % 2;
		TourInfo[i].checkout_hour = 15 + rand() % 2;
		TourInfo[i].transport_cost = temp_transcost;
		TourInfo[i].hotel_index = hotel_reservation(minroute[i+1],start->day, info->id[info->index], duration, part_budget, i);
		if(TourInfo[i].hotel_index == -1){
			printf("You don't have enough money for your hotel reservation\n");
			return -1;
		}
		temp_hotelcost = duration * (TourInfo[i].hotel_index + TourInfo[i].site_index);
		hotel_cost += temp_hotelcost;
		TourInfo[i].hotel_cost = temp_hotelcost;
		TourInfo[i].transport_cost = temp_transcost;

		/* part_budget 설정 */
		total_budget = total_budget - temp_transcost - temp_hotelcost;
		if(i == numofroute - 2){
			break;
		}
		part_budget = total_budget / (numofroute - 1 - (decrement++));
		printf("part budget : %d\n", part_budget);
		printf("total budget : %d\n", total_budget);
	}
	return (transport_cost + hotel_cost); /* 정상적으로 예약 및 스케줄링 완료 */
}

int main() {
	arr = InsertSite(); /* 2-dimensional matrix for containing connectivity and weights of node */
	int cnt = 0;
	for (int i = 0; i < 100; i++) {
		for (int j = 0; j < 100; j++) {
			if (arr[i][j]) cnt++;
		}
	}
    hotel = Hotel_insert(); /* hotel[i] -> hotel RBTREE in ith site */
	InitTourInfo();
	//Print_RBT(hotel[5]);
	//Node *c = RB_Search(hotel[5], 96);
	//printf("%d\n", c->price);

	char ch;
	char c, *str;
	char startdate[MAXLINE];
	char finishdate[MAXLINE];
	int budget, numofdest, choice, tmp, duration;  
	int month_day[13] = { 0, 31, 59, };			//count month as day
	int minroute[100]; int result, routenum;
	int i, j;
	int *destination;
	member *info;
	Date *start;
	Date *finish;
	start = (Date *)malloc(sizeof(Date));
	finish = (Date *)malloc(sizeof(Date));

	routenum = 0;
	for(i=0; i<100; i++){
		minroute[i] = -1;
	}

	for (i = 3; i <= 12; i++) {
		month_day[i] = month_day[i - 1] + 30 + ((i / 8) + i) % 2;
	}
	info = (member *)malloc(sizeof(member)); info->index = 0;
	//system("cls");
	printf("\t\t=================================================\n");
	printf("\t\t|                                               |\n");
	printf("\t\t|        -----------------------------          |\n");
	printf("\t\t|           TOUR RESREVATION SYSTEM             |\n");
	printf("\t\t|        -----------------------------          |\n");
	printf("\t\t|                                               |\n");
	printf("\t\t|                                               |\n");
	printf("\t\t|                                               |\n");
	printf("\t\t|              BROUGHT TO YOU BY                |\n");
	printf("\t\t|               |  TEAM 12  |                   |\n");
	printf("\t\t|                                               |\n");
	printf("\t\t=================================================\n\n\n");
	printf(" \n Press any key to continue..");
	scanf("%c", &ch);
	//system("cls");
	login(info);
	scanf("%c", &ch);
	//system("cls");
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
				//system("cls");
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
							break;
						}
					}
				}
				minroute[0] = 0; minroute[1] = 1; minroute[2] = 2; minroute[3] = 3; minroute[4] = 4;
				for(int i=0; i<100; i++){
					if(minroute[i] == -1){
						break;
					}
					++routenum;
				}
				printf("\t\t                                                 \n");
				printf("\t\t           |  Team 12 Travel Agency  |             \n");
				printf("\t\t                                                 \n");
				printf("\t\t=================================================\n\n\n");

				printf("routenum : %d\n", routenum);
				result = reserve_schedule(info, start, finish, minroute, budget, routenum, duration);
				if(result == -1){
					printf("there is no proper route\n");
				}
				printf("result : %d\n", result);
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

#if 0
//BROUGHT TO YOU BY code-projects.org//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _CRT_NONSTDC_NO_WARNINGS    // ∫Ò«•¡ÿ «‘ºˆ∏¶ ªÁøÎ«ﬂ¿ª ∂ß ø°∑Ø πÊ¡ˆ
#define _CRT_SECURE_NO_WARNINGS

char ch[10][130] = { "Cardiff Express","Belfast Express","Derby Express","Chester Express","Newport Express","Truro Express" };
char name[32][100] = { '\0' };
char number[32][2] = { '\0' };
int num1[32] = { 0 };
int trno;
void login();
void bus();//for list of bus
void name_number(int booking, char numstr[100]);
void booking();//for booking the tickets
int read_number(int trno);//for reading the number from the file
void read_name(int trno);//for reading the name from the file
void status();//for printing the status by user input
void status_1(int trno);//for printing the status while booking ticket
void cancle();

int main()
{
	//login();
	int num, i;
	do {
		system("cls");
		printf("\n\n\n");
		printf("====================================== WELCOME TO BUS RESERVATION SYSTEM ======================================\n\n\n");
		printf("\t\t\t\t\t[1]=> View Bus List\n");
		printf("\n");
		printf("\t\t\t\t\t[2]=> Book Tickets\n");
		printf("\n");
		printf("\t\t\t\t\t[3]=> Cancle Booking\n");
		printf("\n");
		printf("\t\t\t\t\t[4]=> Bus Status Board\n");
		printf("\n");
		printf("\t\t\t\t\t[5]=> Exit\n\n");
		printf("===============================================================================================================\n\n");
		printf("\t\t\tEnter Your Choice:: ");
		scanf("%d", &num);
		switch (num)
		{
		case 1:
			bus();//for list of bus
			break;
		case 2:
			booking();//for booking the tickets
			break;
		case 3:
			cancle();
			break;
		case 4:
			status();
			break;
		}
		getch();
	} while (num != 5);
	system("CLS");
	printf("\t----------------------------------------------------------------------------------------------------------\n");
	printf("\t\t\t\t\tThank You For Using This System\t\t\t\t\t\t\n");
	printf("\t----------------------------------------------------------------------------------------------------------\n");
	printf("\t\t\t Brought To You By code-projects.org");
	getch();
	return 0;
}


void bus()
{
	system("cls");
	printf("\n\n\n");
	printf("=========================================== BUS RESERVATION SYSTEM ============================================\n\n\n");
	printf("\t\t\t\t\t[1]  =>  %s\n", ch[0]);
	printf("\n");
	printf("\t\t\t\t\t[2]  =>  %s\n", ch[1]);
	printf("\n");
	printf("\t\t\t\t\t[3]  =>  %s\n", ch[2]);
	printf("\n");
	printf("\t\t\t\t\t[4]  =>  %s\n", ch[3]);
	printf("\n");
	printf("\t\t\t\t\t[5]  =>  %s\n", ch[4]);
}

void booking()
{

	int i = 0;
	char numstr[100];
	system("cls");
	printf("=========================================== BUS RESERVATION SYSTEM ============================================\n\n\n");//for entering train number
	bus();//for seeing train least
	printf("Enter the Bus number:--->");
	scanf("%d", &trno);
	system("cls");
	printf("=========================================== BUS RESERVATION SYSTEM ============================================\n\n\n");//for selecting coach
	printf("Your Bus Number is %d ********** %s", trno, ch[trno - 1]);
	status_1(trno);
	FILE *f1, *fopen();//for reading the seats from the user.
	char str1[80] = "32", str2[4], str3[4];
	int seat1, seat2, booking = 0;
	if (trno == 1)
	{
		f1 = fopen("tr1.txt", "r+");
		fgets(str1, 80, f1);
		fclose(f1);
	}
	else if (trno == 2)
	{
		f1 = fopen("tr2.txt", "r+");
		fgets(str1, 80, f1);
		fclose(f1);
	}
	else if (trno == 3)
	{
		f1 = fopen("tr3.txt", "r+");
		fgets(str1, 80, f1);
		fclose(f1);
	}
	else if (trno == 4)
	{
		f1 = fopen("tr4.txt", "r+");
		fgets(str1, 80, f1);
		fclose(f1);
	}
	else if (trno == 5)
	{
		f1 = fopen("tr5.txt", "r+");
		fgets(str1, 80, f1);
		fclose(f1);
	}
	seat1 = atoi(str1);//covert the string into number
	if (seat1 <= 0)
	{
		printf("There is no blank seat in this bus ");
	}
	else
	{
		printf("\n\n\n\t\t\t\tAvailable Seats:------>%d\n", seat1);
		printf("\n\t\t\t\tNumber of Tickets:----->");
		scanf("%d", &booking);
		printf("\n");

		seat1 = seat1 - booking;
		itoa(trno, numstr, 10);
		name_number(booking, numstr);
		printf("\n\t\t\t\tThe Total booking amount is %d", 200 * booking);
		itoa(seat1, str1, 10);
		//for reading the seats from the user.
		if (trno == 1)
		{
			f1 = fopen("tr1.txt", "w");
			fputs(str1, f1);
			fclose(f1);
		}
		else if (trno == 2)
		{
			f1 = fopen("tr2.txt", "w");
			fputs(str1, f1);
			fclose(f1);
		}
		else if (trno == 3)
		{
			f1 = fopen("tr3.txt", "w");
			fputs(str1, f1);
			fclose(f1);
		}
		else if (trno == 4)
		{
			f1 = fopen("tr4.txt", "w");
			fputs(str1, f1);
			fclose(f1);
		}
		else if (trno == 5)
		{
			f1 = fopen("tr5.txt", "w");
			fputs(str1, f1);
			fclose(f1);
		}
	}
}



void name_number(int booking, char numstr[100])
{
	char tempstr[100], tempstr1[12] = "status", tempstr2[12] = "number";
	int number;
	FILE *a, *b;
	int i = 0;
	strcat(numstr, ".txt");
	strcat(tempstr1, numstr);
	strcat(tempstr2, numstr);
	a = fopen(tempstr1, "a");//for open the file to write the name in the file
	b = fopen(tempstr2, "a");//for open the file for writing the number in the file
	for (i = 0; i < booking; i++)//for entering the person name and seat number in the file
	{
		printf("============================Enter the details for ticket no %d============================\n\n\n", i + 1);
		printf("\t\t\t\tEnter the seat number:--->");
		scanf("%d", &number);
		printf("\t\t\t\tEnter the name of person:--->");
		scanf("%s", name[number - 1]);
		printf("\n======================================================================================================\n\n");
		printf("\n");
		itoa(number, tempstr, 10);
		fprintf(a, "%s ", name[number - 1]);
		fprintf(b, "%s ", tempstr);

	}
	fclose(a);
	fclose(b);
}



int read_number(int trno)//for putting the numeric value in the array
{
	char tempstr[100], tempstr2[12] = "number";
	FILE *a, *b;
	char numstr[100];
	int i = 0, j = 0, k;
	itoa(trno, numstr, 10);
	strcat(numstr, ".txt");
	strcat(tempstr2, numstr);
	a = fopen(tempstr2, "a+");//for open the file to write the name in the file
	while (!feof(a))
	{
		number[i][j] = fgetc(a);

		if (number[i][j] == ' ')
		{
			j = 0;
			i++;
		}
		else
		{
			j++;
		}
	}
	k = i;
	for (i = 0; i < k; i++)
	{
		num1[i] = atoi(number[i]);
	}
	fclose(a);
	return k;
}


void read_name(int trno)//for putting the numeric value in the array
{
	char tempstr1[12] = "status";
	FILE *b;
	char numstr[100];
	int i = 0, j = 0, k = 0;
	itoa(trno, numstr, 10);
	strcat(numstr, ".txt");
	strcat(tempstr1, numstr);
	b = fopen(tempstr1, "a+");//for open the file to write the name in the file
	while (!feof(b))
	{
		name[i][j] = fgetc(b);

		if (name[i][j] == ' ')
		{
			j = 0;
			i++;
		}
		else
		{
			j++;
		}

	}
	name[i][j] = '\0';
	k = i;
	fclose(b);
}

void status()
{
	system("cls");
	printf("=========================================== BUS RESERVATION SYSTEM ============================================\n\n\n");
	int i, trno, index = 0, j;
	printf("Enter the number of bus:---->");
	scanf("%d", &trno);
	j = read_number(trno);
	read_name(trno);
	printf("____________________________________________________________________________________________________________________\n");
	printf("                                      Bus.no-->%d---->%s                                                            \n", trno, ch[trno - 1]);
	printf("____________________________________________________________________________________________________________________\n");
	char tempname[33][10] = { "Empty ","Empty ","Empty ","Empty ","Empty ","Empty ","Empty ","Empty ","Empty ","Empty ","Empty ","Empty ","Empty ","Empty ","Empty ","Empty ","Empty ","Empty ","Empty ","Empty ","Empty ","Empty ","Empty ","Empty ","Empty ","Empty ","Empty ","Empty ","Empty ","Empty ","Empty ","Empty ","Empty " };
	for (i = 0; i < j; i++)
	{
		strcpy(tempname[num1[i]], name[i]);
	}
	for (i = 0; i < 8; i++)
	{
		printf("\t\t\t\t");
		for (j = 0; j < 4; j++)
		{
			printf("%d.%s\t", index + 1, tempname[index + 1]);
			index++;
		}
		printf("\n");
	}
}

void status_1(int trno)
{
	printf("Your Bus Number is %d ********** %s", trno, ch[trno - 1]);
	system("cls");
	printf("=========================================== BUS RESERVATION SYSTEM ============================================\n\n\n");
	int i, index = 0, j;
	j = read_number(trno);
	read_name(trno);
	char tempname[33][10] = { "Empty ","Empty ","Empty ","Empty ","Empty ","Empty ","Empty ","Empty ","Empty ","Empty ","Empty ","Empty ","Empty ","Empty ","Empty ","Empty ","Empty ","Empty ","Empty ","Empty ","Empty ","Empty ","Empty ","Empty ","Empty ","Empty ","Empty ","Empty ","Empty ","Empty ","Empty ","Empty ","Empty " };
	for (i = 0; i < j; i++)
	{
		strcpy(tempname[num1[i]], name[i]);
	}
	for (i = 0; i < 8; i++)
	{
		printf("\t\t\t\t");
		for (j = 0; j < 4; j++)
		{
			printf("%d.%s\t", index + 1, tempname[index + 1]);
			index++;
		}
		printf("\n");
	}
}


void cancle()
{
	int seat_no, i, j;
	char numstr[100], tempstr2[15] = "number", tempstr1[15] = "status";
	printf("Enter the bus number:---->");
	scanf("%d", &trno);
	itoa(trno, numstr, 10);
	strcat(numstr, ".txt");
	strcat(tempstr1, numstr);
	strcat(tempstr2, numstr);
	read_number(trno);
	read_name(trno);
	status_1(trno);
	printf("Enter the seat number:--->");
	scanf("%d", &seat_no);
	FILE *a, *b;
	a = fopen(tempstr1, "w+");
	b = fopen(tempstr2, "w+");
	for (i = 0; i < 32; i++)
	{
		if (num1[i] == seat_no)
		{
			for (j = 0; j < 32; j++)
			{
				if (num1[j] != seat_no && num1[j] != 0)
				{
					fprintf(b, "%d ", num1[j]);
					fprintf(a, "%s", name[j]);
				}
				else if (num1[j] == seat_no && num1[j] != 0)
				{
					strcpy(name[j], "Empty ");
				}
			}
		}
	}
	fclose(a);
	fclose(b);
	printf("\n\n");
	printf("======================================================================================================\n");
	printf("\t\t\t\tYour 200 rupees has been Returned\t\t\t\n");
	printf("======================================================================================================\n");
}


void login()
{
	int a = 0, i = 0;
	char uname[10], c = ' ';
	char pword[10], code[10];
	char user[10] = "user";
	char pass[10] = "pass";
	do
	{
		system("cls");

		printf("\n  =========================  LOGIN FORM  =========================  ");
		printf(" \n                       ENTER USERNAME:-");
		scanf("%s", &uname);
		printf(" \n                       ENTER PASSWORD:-");
		while (i < 10)
		{
			pword[i] = getch();
			c = pword[i];
			if (c == 13) break;
			else printf("*");
			i++;
		}
		pword[i] = '\0';
		//char code=pword;
		i = 0;
		//scanf("%s",&pword); 
		if (strcmp(uname, "user") == 0 && strcmp(pword, "pass") == 0)
		{
			printf("  \n\n\n       WELCOME TO OUR SYSTEM !!!! LOGIN IS SUCCESSFUL");
			printf("\n\n\n\t\t\t\tPress any key to continue...");
			getch();//holds the screen
			break;
		}
		else
		{
			printf("\n        SORRY !!!!  LOGIN IS UNSUCESSFUL");
			a++;

			getch();//holds the screen

		}
	} while (a <= 2);
	if (a > 2)
	{
		printf("\nSorry you have entered the wrong username and password for four times!!!");

		getch();

	}
	system("cls");
}//bus ticket
#endif


//Train reservation
#if 0		//Train Reservation
/**********************************************PREPROCESSORS**********************************************************/
//Train Reservation System - BROUGHT TO YOU BY : code-projects.org
//Working on basic structure
//including all libraries for now
#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<string.h>

#define _CRT_NONSTDC_NO_WARNINGS    // ∫Ò«•¡ÿ «‘ºˆ∏¶ ªÁøÎ«ﬂ¿ª ∂ß ø°∑Ø πÊ¡ˆ
#define _CRT_SECURE_NO_WARNINGS

#pragma warning(disable : 4996)


/*******************************************GLOBAL VARIABLES**********************************************************/
//ALl the globle variables and the composite data types will be declared here
typedef struct {
	char name[50];
	int train_num;
	int num_of_seats;
}pd;




/*******************************************FUNCTION PROTOTYPE**********************************************************/
//function prototypes to be used
void reservation(void);							//main reservation function
void viewdetails(void);							//view details of all the trains
void cancel(void);
void printticket(char name[], int, int, float);	//print ticket 
void specifictrain(int);						//print data related to specific train
float charge(int, int);							//charge automatically w.r.t number of seats and train
void login();


/******************************************FUNCTION DECLARATION**********************************************************/

/*********************************************MAIN()*************************************************/

int main()

{
	system("cls");
	printf("\t\t=================================================\n");
	printf("\t\t|                                               |\n");
	printf("\t\t|        -----------------------------          |\n");
	printf("\t\t|           TRAIN TICKET RERS. SYSTEM           |\n");
	printf("\t\t|        -----------------------------          |\n");
	printf("\t\t|                                               |\n");
	printf("\t\t|                                               |\n");
	printf("\t\t|                                               |\n");
	printf("\t\t|              BROUGHT TO YOU BY                |\n");
	printf("\t\t|           |  code-projects.org  |             |\n");
	printf("\t\t|                                               |\n");
	printf("\t\t=================================================\n\n\n");


	printf(" \n Press any key to continue:");

	getch();
	system("cls");
	//login();
	int menu_choice, choice_return;
start:
	system("cls");
	printf("\n=================================\n");
	printf("    TRAIN RESERVATION SYSTEM");
	printf("\n=================================");
	printf("\n1>> Reserve A Ticket");
	printf("\n------------------------");
	printf("\n2>> View All Available Trains");
	printf("\n------------------------");
	printf("\n3>> Cancel Reservation");
	printf("\n------------------------");
	printf("\n4>> Exit");
	printf("\n------------------------");
	printf("\n\n-->");
	scanf("%d", &menu_choice);
	switch (menu_choice)
	{
	case 1:
		reservation();		//Fucntion still not added
		break;
	case 2:
		viewdetails();
		printf("\n\nPress any key to go to Main Menu..");
		getch();
		break;
	case 3:
		cancel();
		//function not added. code has been removed due to some errors
		break;
	case 4:
		return(0);
	default:
		printf("\nInvalid choice");
	}
	goto start;
	return(0);
}

/*********************************************VIEWDETAILS()*************************************************/

//The function is yet not completed, need more details to be added!
//timings of the trains are still missing 

void viewdetails(void)
{
	system("cls");
	printf("-----------------------------------------------------------------------------");
	printf("\nTr.No\tName\t\t\tDestinations\t\tCharges\t\tTime\n");
	printf("-----------------------------------------------------------------------------");
	printf("\n1001\tRed Lines Express\tBoston to Manhattan\tRs.5000\t\t9am");
	printf("\n1002\tRed Lines Express\tManhattan To Boston\tRs.5000\t\t12pm");
	printf("\n1003\tLA City Express\t\tBoston To L.A\t\tRs.4500\t\t8am");
	printf("\n1004\tLA City Express\t\tL.A To Boston\t\tRs.4500\t\t11am");
	printf("\n1005\tIron City Express\tBoston To Chicago\tRs.4000\t\t7am");
	printf("\n1006\tIron City Express\tChicago To Boston\tRs.4000\t\t9.30am");
	printf("\n1007\tKeystone Express\tBoston To Washington\tRs.3500\t\t1pm");
	printf("\n1008\tKeystone Express\tWashington To Boston\tRs.3500\t\t4pm");
	printf("\n1009\tMeteor Express\t\tBoston To Miami\t\tRs.6000\t\t3.35pm");
	printf("\n1009\tMeteor Express\t\tMiami To Boston\t\tRs.6000\t\t4.15pm");

}

/*********************************************RESERVATION()*************************************************/

void reservation(void)
{
	char confirm;
	int i = 0;
	float charges;
	pd passdetails;
	FILE *fp;
	fp = fopen("seats_reserved.txt", "a");
	system("cls");

	printf("\nEnter Your Name:> ");
	fflush(stdin);
	gets(passdetails.name);
	//error here have to take input of the name 
	printf("\nEnter Number of seats:> ");
	scanf("%d", &passdetails.num_of_seats);
	printf("\n\n>>Press Enter To View Available Trains<< ");
	getch();
	system("cls");
	viewdetails();
	printf("\n\nEnter train number:> ");
start1:
	scanf("%d", &passdetails.train_num);
	if (passdetails.train_num >= 1001 && passdetails.train_num <= 1010)
	{
		charges = charge(passdetails.train_num, passdetails.num_of_seats);
		printticket(passdetails.name, passdetails.num_of_seats, passdetails.train_num, charges);
	}
	else
	{
		printf("\nInvalid train Number! Enter again--> ");
		goto start1;
	}

	printf("\n\nConfirm Ticket (y/n):>");
start:
	scanf(" %c", &confirm);
	if (confirm == 'y')
	{
		fprintf(fp, "%s\t\t%d\t\t%d\t\t%.2f\n", &passdetails.name, passdetails.num_of_seats, passdetails.train_num, charges);
		printf("==================");
		printf("\n Reservation Done\n");
		printf("==================");
		printf("\nPress any key to go back to Main menu");
	}
	else
	{
		if (confirm == 'n') {
			printf("\nReservation Not Done!\nPress any key to go back to  Main menu!");
		}
		else
		{
			printf("\nInvalid choice entered! Enter again-----> ");
			goto start;
		}
	}
	fclose(fp);
	getch();
}

/*********************************************CHARGE()*************************************************/

float charge(int train_num, int num_of_seats)
{
	if (train_num == 1001)
	{
		return(5000.0*num_of_seats);
	}
	if (train_num == 1002)
	{
		return(5000.0*num_of_seats);
	}
	if (train_num == 1003)
	{
		return(4500.0*num_of_seats);
	}
	if (train_num == 1004)
	{
		return(4500.0*num_of_seats);
	}
	if (train_num == 1005)
	{
		return(4000.0*num_of_seats);
	}
	if (train_num == 1006)
	{
		return(4000.0*num_of_seats);
	}
	if (train_num == 1007)
	{
		return(3500.0*num_of_seats);
	}
	if (train_num == 1008)
	{
		return(3500.0*num_of_seats);
	}
	if (train_num == 1009)
	{
		return(6000.0*num_of_seats);
	}
	if (train_num == 1010)
	{
		return(6000.0*num_of_seats);
	}
}


/*********************************************PRINTTICKET()*************************************************/

void printticket(char name[], int num_of_seats, int train_num, float charges)
{
	system("cls");
	printf("-------------------\n");
	printf("\tTICKET\n");
	printf("-------------------\n\n");
	printf("Name:\t\t\t%s", name);
	printf("\nNumber Of Seats:\t%d", num_of_seats);
	printf("\nTrain Number:\t\t%d", train_num);
	specifictrain(train_num);
	printf("\nCharges:\t\t%.2f", charges);
}

/*********************************************SPECIFICTRAIN()*************************************************/

void specifictrain(int train_num)
{

	if (train_num == 1001)
	{
		printf("\nTrain:\t\t\tRed Lines Express");
		printf("\nDestination:\t\tBoston to Manhattan");
		printf("\nDeparture:\t\t9am ");
	}
	if (train_num == 1002)
	{
		printf("\nTrain:\t\t\tRed Lines Express");
		printf("\nDestination:\t\tManhattan to Boston");
		printf("\nDeparture:\t\t12pm");
	}
	if (train_num == 1003)
	{
		printf("\nTrain:\t\t\tLA City Express");
		printf("\nDestination:\t\tBoston to L.A");
		printf("\nDeparture:\t\t8am");
	}
	if (train_num == 1004)
	{
		printf("\nTrain:\t\t\tLA City Express");
		printf("\nDestination:\t\tL.A to Boston");
		printf("\nDeparture:\t\t11am ");
	}
	if (train_num == 1005)
	{
		printf("\nTrain:\t\t\tIron City Express");
		printf("\nDestination:\t\tBoston to Chicago");
		printf("\nDeparture:\t\t7am");
	}
	if (train_num == 1006)
	{
		printf("\ntrain:\t\t\tIron City Express");
		printf("\nDestination:\t\tChicago to Boston");
		printf("\nDeparture:\t\t9.30am ");
	}
	if (train_num == 1007)
	{
		printf("\ntrain:\t\t\tKeystone Express");
		printf("\nDestination:\t\tBoston to Washington");
		printf("\nDeparture:\t\t1pm ");
	}
	if (train_num == 1008)
	{
		printf("\ntrain:\t\t\tKeystone Express");
		printf("\n Destination:\t\tWashington to Boston");
		printf("\nDeparture:\t\t4pm ");
	}
	if (train_num == 1009)
	{
		printf("\ntrain:\t\t\tMeteor Express");
		printf("\nDestination:\t\tBoston to Miami");
		printf("\nDeparture:\t\t3.35pm ");
	}
	if (train_num == 1010)
	{
		printf("\ntrain:\t\t\tMeteor Express");
		printf("\nDestination:\t\tMiami to Boston");
		printf("\nDeparture:\t\t1.15 ");
	}
}

void login()
{
	int a = 0, i = 0;
	char uname[10], c = ' ';
	char pword[10], code[10];
	char user[10] = "user";
	char pass[10] = "pass";
	do
	{

		printf("\n  =======================  LOGIN FORM  =======================\n  ");
		printf(" \n                       ENTER USERNAME:-");
		scanf("%s", &uname);
		printf(" \n                       ENTER PASSWORD:-");
		while (i < 10)
		{
			pword[i] = getch();
			c = pword[i];
			if (c == 13) break;
			else printf("*");
			i++;
		}
		pword[i] = '\0';
		//char code=pword;
		i = 0;
		//scanf("%s",&pword); 
		if (strcmp(uname, "user") == 0 && strcmp(pword, "pass") == 0)
		{
			printf("  \n\n\n       WELCOME TO OUR SYSTEM !! YOUR LOGIN IS SUCCESSFUL");
			printf("\n\n\n\t\t\t\tPress any key to continue...");
			getch();//holds the screen
			break;
		}
		else
		{
			printf("\n        SORRY !!!!  LOGIN IS UNSUCESSFUL");
			a++;

			getch();//holds the screen
			system("cls");
		}
	} while (a <= 2);
	if (a > 2)
	{
		printf("\nSorry you have entered the wrong username and password for four times!!!");

		getch();

	}
	system("cls");
}

void cancel(void)   /* Sorry this function does not work. Coding is not completed. Codes have been removed due to some errors  */
{
	/*FILE *f,*t;
	int i=0;
	int trainnum;
	t=fopen("seats_reserved.txt","w");
	if ((f=fopen("seats_reserved.txt","r"))==NULL)
	{
		printf("NO RECORD ADDED.");
		main();
		* * * * *
		*
		*
		*
		*
		*
		*
		*  /  missing codes  /
	}
	else*/
	system("cls");
	int trainnum;
	printf("-----------------------\n");
	printf("Enter the train number: \n");
	printf("-----------------------\n");
	fflush(stdin);
	scanf("%i", &trainnum);
	printf("\n\nCancelled");
	getch();
}

#endif

