#define _CRT_SECURE_NO_WARNINGS
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

/* Graph */
void InitGraph(adjList* head);
void InsertVertex(adjList* head);
void InsertEdge(adjList* head);
void PrintGraph(adjList* head);


/* Stack */
typedef int element;
typedef struct stackNode
{
	int data;
	struct stackNode* link;
}stackNode;

stackNode* top;						// stack용

int isEmpty();
void push(int item);
int pop();
void DFS(adjList* head);
void FreeGraph(adjList* head);


/* Queue */
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
void BFS(adjList* head);

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


	for (int i = 0; i < inputNum; i++)
	{
		head->n++;

		head->adjList_H[i]=(node*)malloc(sizeof(node));
		head->adjList_H[i]->vertex = i;
		head->adjList_H[i]->next = NULL;

		printf("\nVertex[%d] is generted", i);
	}

}

void InsertEdge(adjList* head)					// <주의> vertex의 메모리 leak가 발생 할 수 있다.
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
	
	// 그래프 중복 확인하기 <예외 처리>
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

	newNode = (node*)malloc(sizeof(node));			// 새로운 edge만들기
	newNode->vertex = input;
	newNode->next = head->adjList_H[index];
	head->adjList_H[index] = newNode;

}


int isEmpty()				// stack이 비어 있는지 확인
{
	if (top == NULL)
		return 1;
	else
		return 0;
}

void push(int item)			// 한번 걷쳐간 vertex를 stack에 넣는다.
{
	stackNode* temp = (stackNode*)malloc(sizeof(stackNode));
	temp->data = item;
	temp->link = top;
	top = temp;
}

int pop()					// 인접한 노드가 모두 True일때, stack에서 뺀다.
{
	int item;
	stackNode* temp = top;

	if (isEmpty())			// isEmpty가 true이면 즉, top==null 일때
	{
		printf("\n Stack is empty!\n");
		return 0;
	}
	else
	{
		item = temp->data;
		top = temp->link;
		free(temp);
		return item;
	}

}
void DFS(adjList* head)
{
	int index;
	node* w;
	top = NULL;

	printf("\nChoice Start Vertex : ");				// 시작 vertex 입력받기
	scanf("%d", &index);

	if ((index < 0) || (index > MAX_VERTEX))
	{
		printf("There is no vertex in Graph!!\n");
		return;
	}
	push(index);
	head->visited[index] = TRUE;
	printf("%d", index);

	while (!isEmpty())								// stack에 있을 때
	{
		w = head->adjList_H[index];
		while (w)
		{
			if (!(head->visited[w->vertex]))		// 인접 vertex 있을 때
			{
				push(w->vertex);
				head->visited[w->vertex] = TRUE;
				printf(" %d", w->vertex);
				index = w->vertex;
				w = head->adjList_H[index];
			}
			else
				w = w->next;
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

void enQueue(LQueueType* LQ, int v)						// Q 삽입
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
int deQueue(LQueueType* LQ)		// Q 삭제
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

