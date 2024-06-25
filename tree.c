#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct node
{
	char *token;
	struct node **sons;
	int size;
	char type[15];
	int count_arg;
 
} node;


void AddType(node *obj,char *Type);
node* mknode(char *token, node **sons, int size);
void printtree(node *tree,int indent);
char* ntos(node *n);
node** sonArr(int size);


void AddType(node *obj,char *Type){
	strcpy(obj->type,Type);
}

node *mknode(char *token,node **sons, int size)
{
	node *newnode = (node*)malloc(sizeof(node));
	newnode->token = (char*)malloc(sizeof(token) + 1);
	newnode->count_arg = 0;

	strcpy(newnode->token,token);
	int countNulls=0;
	for(int i=0;i<size;i++)
		if(!(sons[i]))	countNulls++;
	if(size-countNulls>0)	newnode->sons = (node**)malloc((size-countNulls)*sizeof(node*));
	for(int i=0,j=0;i<size;i++)
		if(sons[i])
			(newnode->sons)[j++]=sons[i];
	newnode->size = size-countNulls;
	return newnode;
}

node** sonArr(int size){
	return (node**)malloc(size*sizeof(node*));
}

void printtree(node *tree,int indent)
{
	if(tree){
		if(tree->sons) printf("(");
		printf("%s", tree->token);
		if(tree->sons){
			printf("\n");
			for(int i=0;i<tree->size;i++){
				for(int i = 0; i<indent;i++)	printf("\t");
				printtree((tree->sons)[i],indent+1);
			}
			for(int i = 1; i<indent;i++)	printf("\t");
			printf(")");
		}
		printf("\n");
	}
}

char* ntos(node *n){
	if(n)
		return n->token;
}