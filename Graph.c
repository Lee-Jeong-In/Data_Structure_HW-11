#include <stdio.h>
#include <stdlib.h>


#define MAX_VERTEX 10

#define TRUE 1
#define FALSE 0

typedef struct Node			// 노드 구조체
{
	int vertex;
	struct Node* next;
}node;

typedef struct Adj_List		// 리스트 구조체
{				
	int n;				// 정점개수
	node* adjList_H[MAX_VERTEX];	// 노드 구조체 배열
	int visited[MAX_VERTEX];		// stack용 visited 배열
	int Q_visited[MAX_VERTEX];		// queue용 visited 배열
}adjList;

node* top;						// stack용

typedef struct QNode
{
	int data;
	struct QNode* link;
}QNode;

typedef struct {
	QNode* front, * rear;
}LQueueType;

LQueueType* createQueue();
int Q_isEmpty(LQueueType* LQ);
void enQueue(LQueueType* LQ, int v);
int deQueue(LQueueType* LQ);

node* queue[MAX_VERTEX];
int front = -1;
int rear = -1;
node* Dequeue();
void Enqueue(node* anode);

void InitGraph(adjList* head);
void InsertVertex(adjList* head);
void InsertEdge(adjList* head);
void PrintGraph(adjList* head);

int pop();
void push(int item);
int isEmpty();
void DFS(adjList* head);

void BFS(adjList* head);

void FreeGraph(adjList* head);


int main()
{
	char command;

	adjList* head;
	head = (adjList*)malloc(sizeof(adjList));

	printf("[----- [이정인] [2019038015] -----]\n");


	do {
		printf("\n\n");
		printf("---------------------------------------------------------------\n");
		printf("                         Graph Searches                        \n");
		printf("---------------------------------------------------------------\n");
		printf(" Initialize Graph     = z                                      \n");
		printf(" Insert Vertex        = v           Insert Edge            = e \n");
		printf(" Depth First Search   = d           Breath First Search    = b \n");
		printf(" Print Graph          = p           Quit                   = q \n");
		printf("---------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);

switch (command) {
		case 'z': case 'Z':
			InitGraph(head);
			break;
		case 'q': case 'Q':
			FreeGraph(head);
			break;
		case 'v': case 'V':
			InsertVertex(head);
			break;
		case 'e': case 'E':
			InsertEdge(head);
			break;
		case 'd': case 'D':
			DFS(head);
			break;
		case 'b': case 'B':
			BFS(head);
			break;
		case 'p': case 'P':
			PrintGraph(head);
			break;

		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	} while (command != 'q' && command != 'Q');

	return 0;
}

void InitGraph(adjList* head)		// 인접리스트 초기화
{
	head->n = 0;						// 인접 리스트에 연결된 정점 갯수 초기화

	//head = (adjList*)malloc(sizeof(adjList));
						

	for (int i = 0; i < MAX_VERTEX; i++)	// 인접 리스트의 각 index마다의 노드구
	{
		head->adjList_H[i] = NULL;			// 인접리스트 초기화
		head->visited[i] = FALSE;			// stact visit 초기화
		head->Q_visited[i] = FALSE;			// Q_visit 초기화
	}

}

void FreeGraph(adjList* head)			// Graph 해제
{
	node* temp = NULL;
	node* del = NULL;

	for (int i = 0; i < MAX_VERTEX; i++)
	{
		temp = head->adjList_H[i];

		while (temp != NULL)
		{
			del = temp;
			temp = temp->next;

			free(del);
		}
	}
	free(head);

	printf("\n\n >>Free Graph...\n");
}

void InsertVertex(adjList* head)		// input만큼 vertex만들기 (인접리스트에 메모리 할당하기)
{
	int inputNum;
	node* newNode;

	do
	{
		printf("Input Number of Vertex(1~10) : ");		
		scanf("%d", &inputNum);
	} while ((inputNum < 0) || (inputNum > MAX_VERTEX));

	//if (head->adjList_H[inputNum]!= )								인접 리스트에서는 중복 노드가 발생한다.
	//{
	//	printf("\n\t<lready have [%d]Vertex>", inputNum);
	//	return;
	//}

	for (int i = 0; i < inputNum; i++)
	{
		head->n++;

		head->adjList_H[i]=(node*)malloc(sizeof(node));
		head->adjList_H[i]->vertex = i;
		head->adjList_H[i]->next = NULL;

		printf("\nVertex[%d] is generted", i);
	}


//	if (((head->vertex_count) + 1) > MAX_VERTEX)
//	{
//		printf("\n 그래프 정점 개수를 초과\n");
//		return;
//	}
//
//	head->adjList_H[inputNum] = (node*)malloc(sizeof(node));		// 새로운 vertex 생성
//	head->adjList_H[inputNum]->vertex = inputNum;
//	head->adjList_H[inputNum]->next = NULL;
//
//	head->vertex_count++;
//
//	printf("\nMake [%d] vertexes\n", inputNum);
}

void InsertEdge(adjList* head)			// 문제점 vertex의 메모리 leak가 발생 할 수 있다.
{
	int index, input;
	node* newNode = NULL;

	printf("Max Vertex is %d", head->n);
	printf("\nChoice Start Vertex : ");				// 시작 vertex 입력받기
	scanf("%d", &index);

	if ((index < 0) || (index > head->n))
	{
		printf("There is no vertex in Graph!!\n");
		return;
	}

	printf("\nNumber of Vertex to connect : ");		// input받기
	scanf("%d", &input);

	if ((input < 0) || (input > head->n))
	{
		printf("There is no vertex in Graph!!\n");
		return;
	}
	
	// 그래프 중복 확인하기
	if(head->adjList_H[index] != NULL)
	{
		node* temp = head->adjList_H[index]->next;
		while (temp != NULL)
		{
			if (temp->vertex == input)
			{
				printf("Already Connected!!\n");
				return;
			}
			temp = temp->next;
		}
	}

	newNode = (node*)malloc(sizeof(node));			// 새로운 노드 만들고
	newNode->vertex = input;
	newNode->next = NULL;

	//if (head->adjList_H[index]->next == NULL)		// 아무것도 없고+맨 처음일 때
	//{
	//	head->adjList_H[index]->next = newNode;
	//	return;
	//}

	node* temp = head->adjList_H[index];
	node* trail = head->adjList_H[index];

	if (temp == NULL)
	{
		printf("There is no vertex in Graph!!\n");
		return;
	}

	while (temp != NULL)
	{
		if (temp->vertex >= newNode->vertex)
		{
			if (temp == head->adjList_H[index]->next)		// 뒤에 노드가 있는데 처음으로 삽입 해야 할 때
			{
				head->adjList_H[index]->next = newNode;
				newNode->next = temp;
			}
			else										// 중간에 삽입 해야 할때
			{
				newNode->next = temp;
				trail->next = newNode;
			}
			return;
		}
		trail = temp;
		temp = temp->next;
	}
	trail->next = newNode;									// 마지막에 삽입할 때
	return;
}

int isEmpty()				// stact이 비어있는지 확인한다.
{
	if (top == NULL)
		return 1;
	else
		return 0;
}

int pop()
{
	int item;
	node* temp = top;

	if (isEmpty())
	{
		printf("\n\n Stack is Empyh!\n");
		return 0;
	}
	else
	{
		item = temp->vertex;
		top = temp->next;
		free(temp);
		return item;
	}

}

void push(int item)
{
	node* temp = (node*)malloc(sizeof(node));
	temp->vertex = item;
	temp->next = top;
	top = temp;
}

void DFS(adjList* head)
{
	int index;
	node* adj;
	top = NULL;

	printf("\nChoice Start Vertex : ");				// 시작 vertex 입력받기
	scanf("%d", &index);

	if ((index < 0) || (index > MAX_VERTEX))
	{
		printf("There is no vertex in Graph!!\n");
		return;
	}

	push(index);						// dfs 시작
	head->visited[index] = TRUE;
	printf("%d", index);

	while (!isEmpty())
	{
		adj = head->adjList_H[index];
		while (adj)
		{
			if (!(head->visited[adj->vertex]))
			{
				push(adj->vertex);
				head->visited[adj->vertex] = TRUE;
				printf("%d", adj->vertex);
				index = adj->vertex;
				adj = head->adjList_H[index];
			}
			else
				adj = adj->next;
		}

		index = pop();
	}

}

LQueueType* createQueue()
{
	LQueueType* LQ;
	LQ = (LQueueType*)malloc(sizeof(LQueueType));
	LQ->front = NULL;
	LQ->rear = NULL;
	return LQ;
}

int Q_isEmpty(LQueueType* LQ)
{
	if (LQ->front == NULL)
	{
		printf("\n Linked Queue is empty..\n");
		return 1;
	}
	else
		return 0;
}

void enQueue(LQueueType* LQ, int v)
{
	QNode* newNode = (QNode*)malloc(sizeof(QNode));
	newNode->data = v;
	newNode->link = NULL;

	if (LQ->front == NULL)
	{
		LQ->front = newNode;
		LQ->rear = newNode;
	}
	else
	{
		LQ->rear->link = newNode;
		LQ->rear = newNode;
	}
}
int deQueue(LQueueType* LQ)
{
	QNode* old = LQ->front;
	int item;

	if (Q_isEmpty(LQ))
		return 0;

	else
	{
		item = old->data;
		LQ->front = LQ->front->link;
		if (LQ->front == NULL)
			LQ->rear = NULL;

		free(old);
		return item;
	}
}


void BFS(adjList* head)
{
	int index;
	node* w;
	LQueueType* Q;

	printf("\nChoice Start Vertex : ");				// 시작 vertex 입력받기
	scanf("%d", &index);

	if ((index < 0) || (index > MAX_VERTEX))
	{
		printf("There is no vertex in Graph!!\n");
		return;
	}

	Q = createQueue();
	head->Q_visited[index] = TRUE;
	printf("%d", index);
	enQueue(Q, index);

	while (!Q_isEmpty(Q))
	{
		index = deQueue(Q);
		for (w = head->adjList_H[index]; w; w = w->next)
		{
			if (!(head->Q_visited[w->vertex]))
			{
				head->Q_visited[w->vertex] = TRUE;
				printf("%d", w->vertex);
				enQueue(Q, w->vertex);
			}
		}
	}
	//1. 큐생성
	//2. 시작 노드 넣고
	//3. 큐가 비어있을 때 까지 반복
	//	4. 자식노드를 큐에 넣고
	//	5. 큐에서 꺼내고 출력
}

void PrintGraph(adjList* head)
{
	node* temp;

	for (int i = 0; i < MAX_VERTEX; i++)
	{
		printf("\n	정점 [%d]의 인접리스트 : ", i);
		temp = head->adjList_H[i];
		while (temp)
		{
			printf("%d -> ", temp->vertex);
			temp = temp->next;
		}
		printf("END");
	}
}


