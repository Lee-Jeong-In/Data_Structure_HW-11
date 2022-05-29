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
	int n;					// ��������
	int vertex_count;
	node* adjList_H[MAX_VERTEX];	// ��� ����ü �迭
	int visited[MAX_VERTEX];		// stack�� visited �迭
	int Q_visited[MAX_VERTEX];		// queue�� visited �迭
}adjList;

node* top;						// stack��

typedef struct QNode
{
	int data;
	struct QNode* link;
}QNode;

typedef struct {
	QNode* front,*rear;
}LQueueType;

LQueueType* createQueue();
int Q_isEmpty(LQueueType* LQ);
void enQueue(LQueueType* LQ, int v);
int deQueue(LQueueType* LQ);

node* queue[MAX_VERTEX];
int front=-1;
int rear=-1;
node* Dequeue();
void Enqueue(node* anode);

void InitializeGraph(adjList* head);
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

	adjList* head=NULL;

	printf("[----- [������] [2019038015] -----]\n");


	do{
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

		switch(command) {
		case 'z': case 'Z':
			InitializeGraph(head);		
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

	}while(command != 'q' && command != 'Q');

	return 0;
}

void InitializeGraph(adjList* head)		// ��������Ʈ �ʱ�ȭ
{
	while(head!=NULL)
		free(head);

	head=(adjList*)malloc(sizeof(adjList));

	head->n=0;						// ���� ����Ʈ�� ����� ���� ���� �ʱ�ȭ
	head->vertex_count=0;					// ����Ƚ��

	for(int i=0;i<MAX_VERTEX;i++)	// ���� ����Ʈ�� �� index������ ��屸
		head->adjList_H[i]=NULL;

	for(int i=0;i<MAX_VERTEX;i++)	// DFS���� ���
		head->visited[i]=FALSE;

	for(int i=0;i<MAX_VERTEX;i++)	// BFS���� ���
		head->Q_visited[i]=FALSE;	
}

void FreeGraph(adjList* head)			// Graph ����
{
	node* temp=NULL;
	node* del=NULL;

	for(int i=0;i<MAX_VERTEX;i++)
	{
		temp=head->adjList_H[i];

		while(temp!=NULL)
		{
			del=temp;
			temp=temp->next;

			free(del);
		}
	}
	free(head);

	printf("\n\n >>Free Graph...\n");
}

void InsertVertex(adjList* head)		// input��ŭ vertex����� (��������Ʈ�� �޸� �Ҵ��ϱ�)
{
	int inputNum;

	do
	{
		printf("Input Number of Vertex(0~10) : ");
		scanf("%d",&inputNum);
	}while((inputNum<0)||(inputNum>MAX_VERTEX));

	if(head->adjList_H[inputNum]!=NULL)							// �̹� �ִ� vertex
	{
		printf("\n\t<lready have [%d]Vertex>",inputNum);
		return;
	}

	head->adjList_H[inputNum]=(node*)malloc(sizeof(node));		// ���ο� vertex ����
	head->adjList_H[inputNum]->vertex=inputNum;
	head->adjList_H[inputNum]->next=NULL;

	head->vertex_count++;

	printf("\nMake [%d] vertexes\n",inputNum);
}

void InsertEdge(adjList* head)			// ������ vertex�� �޸� leak�� �߻� �� �� �ִ�.
{
	int index,input;

	node* newNode=NULL;

	printf("\nChoice Start Vertex : ");				// ���� vertex �Է¹ޱ�
	scanf("%d",&index);

	if((index<0)||(index>MAX_VERTEX))
	{
		printf("There is no vertex in Graph!!\n");
		return;
	}

	printf("\nNumber of Vertex to connect : ");		// input�ޱ�
	scanf("%d",&input);

	newNode=(node*)malloc(sizeof(node));			// ���ο� ��� �����
	newNode->vertex=index;
	newNode->next=NULL;

	if(head->adjList_H[index]->next==NULL)		// �ƹ��͵� ����+�� ó���� ��
	{
		head->adjList_H[index]->next=newNode;
		return;
	}
	
	node* temp=head->adjList_H[index]->next;
	node* trail=head->adjList_H[index]->next;

	while(temp!=NULL)									
	{
		if(temp->vertex>=newNode->vertex)
		{
			if(temp==head->adjList_H[index]->next)		// �ڿ� ��尡 �ִµ� ó������ ���� �ؾ� �� ��
			{
				head->adjList_H[index]->next=newNode;
				newNode->next=temp;
			}
			else										// �߰��� ���� �ؾ� �Ҷ�
			{
				newNode->next=temp;
				trail->next=newNode;
			}
			return ;
		}
		trail=temp;
		temp=temp->next;
	}
	trail->next=newNode;									// �������� ������ ��
	return;
}

int isEmpty()				// stact�� ����ִ��� Ȯ���Ѵ�.
{
	if(top==NULL)
		return 1;
	else
		return 0;
}

int pop()
{
	int item;
	node* temp=top;

	if(isEmpty())
	{
		printf("\n\n Stack is Empyh!\n");
		return 0;
	}
	else
	{
		item=temp->vertex;
		top=temp->next;
		free(temp);
		return item;
	}

}

void push(int item)
{
	node* temp=(node*)malloc(sizeof(node));
	temp->vertex=item;
	temp->next=top;
	top=temp;
}

void DFS(adjList* head)
{
	int index;
	node* adj;
	top=NULL;

	printf("\nChoice Start Vertex : ");				// ���� vertex �Է¹ޱ�
	scanf("%d",&index);

	if((index<0)||(index>MAX_VERTEX))
	{
		printf("There is no vertex in Graph!!\n");
		return;
	}

	push(index);						// dfs ����
	head->visited[index]=TRUE;
	printf("%d",index);

	while(!isEmpty())
	{
		adj=head->adjList_H[index];
			while(adj)
			{
				if(!(head->visited[adj->vertex]))
				{
					push(adj->vertex);
					head->visited[adj->vertex]=TRUE;
					printf("%d",adj->vertex);
					index=adj->vertex;
					adj=head->adjList_H[index];
				}
				else
					adj=adj->next;
			}

		index=pop();
	}

}

LQueueType* createQueue()
{
	LQueueType* LQ;
	LQ=(LQueueType*)malloc(sizeof(LQueueType));
	LQ->front=NULL;
	LQ->rear=NULL;
	return LQ;
}

int Q_isEmpty(LQueueType* LQ)
{
	if(LQ->front==NULL)
	{
		printf("\n Linked Queue is empty..\n");
		return 1;
	}
	else
		return 0;
}

void enQueue(LQueueType* LQ,int v)
{
	QNode* newNode=(QNode*)malloc(sizeof(QNode));
	newNode->data=v;
	newNode->link=NULL;

	if(LQ->front==NULL)
	{
		LQ->front=newNode;
		LQ->rear=newNode;
	}
	else
	{
		LQ->rear->link=newNode;
		LQ->rear=newNode;
	}
}
int deQueue(LQueueType* LQ)
{
	QNode *old=LQ->front;
	int item;

	if(Q_isEmpty(LQ))
		return 0;

	else
	{
		item=old->data;
		LQ->front=LQ->front->link;
		if(LQ->front==NULL)
			LQ->rear=NULL;
		
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
	scanf("%d",&index);

	if((index<0)||(index>MAX_VERTEX))
	{
		printf("There is no vertex in Graph!!\n");
		return;
	}

	Q=createQueue();
	head->Q_visited[index]=TRUE;
	printf("%d",index);
	enQueue(Q,index);

	while(!Q_isEmpty(Q))
	{
		index=deQueue(Q);
		for(w=head->adjList_H[index];w;w=w->next)
		{
			if(!(head->Q_visited[w->vertex]))
			{
				head->Q_visited[w->vertex]=TRUE;
				printf("%d",w->vertex);
				enQueue(Q,w->vertex);
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
	
	for(int i=0;i<MAX_VERTEX;i++)
	{
		printf("\n	���� [%d]�� ��������Ʈ : ",i);
		temp=head->adjList_H[i];
		while(temp)
		{
			printf("%d -> ",temp->vertex);
			temp=temp->next;
		}
	}
}


