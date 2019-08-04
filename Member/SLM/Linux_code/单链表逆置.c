#include <stdio.h>
#include <stdlib.h>

struct haha{
	int data;
	struct haha *next;	
};

struct haha * creat()
{
	int n;
	int i;
	struct haha *head, *p, *p1;
	head = NULL;
	p = (struct haha*)malloc(sizeof(struct haha));
	printf("请输入链表的长度:\n");
	scanf("%d",&n);
	for(i = 0; i < n; i++)
	{
		scanf("%d",&p->data);
		if(head == NULL)
			head = p;
		else
			p1->next = p;
		p1 = p;
		p = (struct haha*)malloc(sizeof(struct haha));
	}
	free(p);
	return head;
}

void print(struct haha *head)
{
	struct haha *p;
	p = head;
	while(p != NULL)
	{
		printf("%d ",p->data);
		p = p->next;
	}
	printf("\n");
}

struct haha *nizhi(struct haha *head)
{
	struct haha *p, *p1, *p2;
	p = head;
	p1 = p->next;
	p2 = p1->next;
	if(p == NULL || p->next ==NULL)
		return head;
	while(p2->next != NULL)
	{
		if(p == head)
		{
			p->next = NULL;
			p1->next = p;
			p = p1;
			p1 = p2;
			p2 = p1->next;
		}
		else
		{
			p1->next = p;
			p = p1;
			p1 = p2;
			p2 = p1->next;
		}
	}
	p1->next = p;
	p2->next = p1;
	head = p2;
	return head;
}

int main()
{
	struct haha *p;
	p = creat();
	print(p);
	p = nizhi(p);
	print(p);
}
