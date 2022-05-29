#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTEX 10

#define TRUE 1
#define FALSE 0

typedef struct Node			// ��� ����ü
{
	int vertex;
	struct Node* next;
}node;

typedef struct Adj_List		// ����Ʈ ����ü
{				
	int n;				// ��������
	node* adjList_H[MAX_VERTEX];	// ��� ����ü �迭
	int visited[MAX_VERTEX];		// stack�� visited �迭
	int Q_visited[MAX_VERTEX];		// queue�� visited �迭
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

stackNode* top;						// stack��

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

	printf("[----- [������] [2019038015] -----]\n");


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

void InitGraph(adjList* head)		// ��������Ʈ �ʱ�ȭ
{
	head->n = 0;						// ���� ����Ʈ�� ����� ���� ���� �ʱ�ȭ

						

	for (int i = 0; i < MAX_VERTEX; i++)	// ���� ����Ʈ�� �� index������ ��屸
	{
		head->adjList_H[i] = NULL;			// ��������Ʈ �ʱ�ȭ
		head->visited[i] = FALSE;			// stact visit �ʱ�ȭ
		head->Q_visited[i] = FALSE;			// Q_visit �ʱ�ȭ
	}

}

void FreeGraph(adjList* head)			// Graph ����
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

void InsertVertex(adjList* head)		// input��ŭ vertex����� (��������Ʈ�� �޸� �Ҵ��ϱ�)
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

void InsertEdge(adjList* head)					// <����> vertex�� �޸� leak�� �߻� �� �� �ִ�.
{
	int index, input;
	node* newNode = NULL;

	printf("Max Vertex is %d", head->n);
	printf("\nChoice Start Vertex : ");				// ���� vertex �Է¹ޱ�
	scanf("%d", &index);

	if ((index < 0) || (index > head->n))
	{
		printf("There is no vertex in Graph!!\n");
		return;
	}

	printf("\nNumber of Vertex to connect : ");		// input�ޱ�
	scanf("%d", &input);

	if ((input < 0) || (input > head->n))
	{
		printf("There is no vertex in Graph!!\n");
		return;
	}
	
	// �׷��� �ߺ� Ȯ���ϱ� <���� ó��>
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

	newNode = (node*)malloc(sizeof(node));			// ���ο� edge�����
	newNode->vertex = input;
	newNode->next = head->adjList_H[index];
	head->adjList_H[index] = newNode;

}


int isEmpty()				// stack�� ��� �ִ��� Ȯ��
{
	if (top == NULL)
		return 1;
	else
		return 0;
}

void push(int item)			// �ѹ� ���İ� vertex�� stack�� �ִ´�.
{
	stackNode* temp = (stackNode*)malloc(sizeof(stackNode));
	temp->data = item;
	temp->link = top;
	top = temp;
}

int pop()					// ������ ��尡 ��� True�϶�, stack���� ����.
{
	int item;
	stackNode* temp = top;

	if (isEmpty())			// isEmpty�� true�̸� ��, top==null �϶�
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

	printf("\nChoice Start Vertex : ");				// ���� vertex �Է¹ޱ�
	scanf("%d", &index);

	if ((index < 0) || (index > MAX_VERTEX))
	{
		printf("There is no vertex in Graph!!\n");
		return;
	}
	push(index);
	head->visited[index] = TRUE;
	printf("%d", index);

	while (!isEmpty())								// stack�� ���� ��
	{
		w = head->adjList_H[index];
		while (w)
		{
			if (!(head->visited[w->vertex]))		// ���� vertex ���� ��
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

void enQueue(LQueueType* LQ, int v)						// Q ����
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
int deQueue(LQueueType* LQ)		// Q ����
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

	printf("\nChoice Start Vertex : ");				// ���� vertex �Է¹ޱ�
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
	//1. ť����
	//2. ���� ��� �ְ�
	//3. ť�� ������� �� ���� �ݺ�
	//	4. �ڽĳ�带 ť�� �ְ�
	//	5. ť���� ������ ���
}

void PrintGraph(adjList* head)
{
	node* temp;

	for (int i = 0; i < MAX_VERTEX; i++)
	{
		printf("\n	���� [%d]�� ��������Ʈ : ", i);
		temp = head->adjList_H[i];
		while (temp)
		{
			printf("%d -> ", temp->vertex);
			temp = temp->next;
		}
		printf("END");
	}
}

