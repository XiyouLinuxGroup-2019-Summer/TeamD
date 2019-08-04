#include<stdio.h>
#include<stdlib.h>
 
#define STACKINITSIZE 100
#define STACKINCREASESIZE 20
 
typedef char ElemType;
//树结构
typedef struct tree
{
    ElemType data;
    struct tree * lchild;
    struct tree * rchild;
    unsigned int isOut;   //专为后序遍历设置的，0为不需要被输出，1为需要被输出
}TreeNode,*Tree;
 
//栈结构
typedef struct stack
{
    Tree * base;
    Tree * top;
    int stacksize;
}Sqstack;
 
 
/*****************栈的操作声明********************/
 
//初始化栈
void InitStack( Sqstack &s );
//元素入栈
void Push( Sqstack &s, Tree e );
//获得栈顶元素
void GetTop( Sqstack s, Tree &e );
//弹出栈顶元素
void Pop( Sqstack &s, Tree &e );
//判断栈是否为空，为空返回1，否则返回0
int StackEmpty( Sqstack s );
 
/*****************栈的操作声明********************/
 
 
/*****************树的操作声明********************/
//创建树,以先序序列建立树
void CreateTree(Tree &t);
//递归先序遍历
void PreOrder(Tree t);
//非递归先序遍历
void PreOrder1(Tree t);
//递归中序遍历
void InOrder(Tree t);
//非递归中序遍历
void InOrder1(Tree t);
//递归后序遍历
void PostOrder(Tree t);
//非递归后序遍历
void PostOrder1(Tree t);
/*****************树的操作声明********************/
 
int main()
{
    Tree T;
    printf("\n按先序序列输入结点序列，'#'代表空：");
    CreateTree(T);
 
    printf("\n非递归先序遍历的结果：");
    PreOrder1(T);
    printf("\n递归先序遍历的结果：  ");
    PreOrder(T);
 
    printf("\n非递归中序遍历的结果：");
    InOrder1(T);
    printf("\n递归中序遍历的结果：  ");
    InOrder(T);
 
    printf("\n非递归后序遍历的结果：");
    PostOrder1(T);
    printf("\n递归后序遍历的结果：  ");
    PostOrder(T);
    printf("\n");

    return 0;
 
}
 
 
/*****************栈的操作定义********************/
 
//初始化栈
void InitStack( Sqstack &s )
{
    s.base = (Tree *)malloc(STACKINITSIZE*sizeof(Tree));
    if ( !s.base )
    {
    printf("InitStack内存分配出错\n");
    }    
    s.top = s.base;
    s.stacksize = STACKINITSIZE;
}
 
//元素入栈
void Push( Sqstack &s, Tree e )
{
    if ( s.top - s.base >= s.stacksize )
    {
    s.base = (Tree *)realloc(s.base,(s.stacksize+STACKINCREASESIZE)*sizeof(Tree));
    if ( !s.base )
        {
        printf("Push内存分配出错\n");
        return ;
        }
    
     s.top = s.base + s.stacksize;
    s.stacksize += STACKINCREASESIZE;
    }
    e->isOut = 0;
    *s.top++ = e;
}
 
//获得栈顶元素
void GetTop( Sqstack s, Tree &e )
{
    e = *(s.top - 1);
}
 
//弹出栈顶元素
void Pop( Sqstack &s, Tree &e )
{
    if ( s.top == s.base )
    {
    printf("栈为空\n");
    return ;
    }
    e = *(--s.top);
}
 
//判断栈是否为空，为空返回1，否则返回0
int StackEmpty( Sqstack s )
{
    if ( s.top == s.base )
    return 1;
    return 0;
}
 
/*****************栈的操作定义********************/
 
/*****************树的操作定义********************/
//创建树,以先序序列建立树
void CreateTree(Tree &t)
{
    char ch;
    scanf("%c",&ch);
    if ( ch == '#' )
    t = NULL;
    else
    {
    t = (Tree)malloc(sizeof(TreeNode));
        if ( !t )
    {
        printf("分配内存出错！");
        return ;
    }
    t->data = ch;
    CreateTree(t->lchild);
    CreateTree(t->rchild);
    }
}
 
 
//递归先序遍历
void PreOrder(Tree t)
{
    if ( t )
    {
    printf("%c",t->data);
    PreOrder(t->lchild);
    PreOrder(t->rchild);
    }
}
 
//非递归先序遍历
void PreOrder1(Tree t)
{
    Tree p = t;
    Sqstack s;
    InitStack(s);
 
    while ( p || !StackEmpty(s) )
    {
    if ( p )
    {
        printf("%c",p->data);
        Push(s,p);
        p = p->lchild;    
    }
    else
    {
        Pop(s,p); 
        p = p->rchild;
    }
    }
 
}
 
 
//递归中序遍历
void InOrder(Tree t)
{
    if ( t )
    {
    InOrder(t->lchild);
    printf("%c",t->data);
    InOrder(t->rchild);
    }
}
 
 
//非递归中序遍历
void InOrder1(Tree t)
{
    Tree p = t;
    Sqstack s;
    InitStack(s);  
    
    while ( p || !StackEmpty(s) )
    {
    if ( p )
        {
        Push(s,p);
        p = p->lchild;
    }    
    else
    {
        Pop(s,p);
        printf("%c",p->data);
        p = p->rchild;
    } 
    }
}
 
//递归后序遍历
void PostOrder(Tree t)
{
    if ( t )
    {
    PostOrder(t->lchild);
     PostOrder(t->rchild);
    printf("%c",t->data);
    }
}
 
 
//非递归后序遍历
void PostOrder1(Tree t)
{
    t->isOut = 0;
    Tree p = t;
    Sqstack s;
    InitStack(s); 
 
    while ( p || !StackEmpty(s) )
    {
    if ( p )
        {
        if ( p->isOut )
            {//左右子树都已输出，则该节点也输出        
            Pop(s,p);
            printf("%c",p->data);
        if (!StackEmpty(s))
                GetTop(s,p); //得到弹出节点元素的父节点
            else
            p = NULL;
        }
        else
        {    
            if ( (p->lchild) && (p->lchild->isOut == 1) )
            {//如果存在左子树,并且左子树已经遍历完，则说明该节点已经入栈，不用再次Push,直接走向右子树          
            p->isOut = 1;
            p = p->rchild;   
              }
            else
            {
            Push(s,p);
            p = p->lchild;
            }          
        }
        }
        else
        {
        if (!StackEmpty(s))
            GetTop(s,p); 
        else
        p = NULL;
 
        if ( p->rchild )
        {
            p = p->rchild;
        }
        else
        {
           Pop(s,p);
            printf("%c",p->data);
            p->isOut = 1;
            if (!StackEmpty(s))
            {
            GetTop(s,p);
            if ( p->lchild == NULL )
                p->isOut = 1; //右子树已输出，将父节点isOut置1
        }
            else
            p = NULL;
        }
        
        }
    }
    
}
 
