#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

typedef struct treeNode{
	int data;
	struct treeNode*left;
	struct treeNode*right;
}Node;

int searchNode (Node * node, int data){
	int height=0;
	if (node==NULL){
		return 0;
	}
	if(data==(node->data)){
		++height;
		return height;
	}
	if (data > node->data){
		if (node->right==NULL){
			return 0;
		}
		height+=searchNode(node->right, data);
		if (height==0){
			return 0;
		}
		++height;

	}
	else if(data<node->data){
		if (node->left==NULL){
			return 0;
		}	
		height+=searchNode(node->left, data);
		if (height==0){
			return 0;
		}
		++height;
	}
	return height;
}
Node* createNewNode(int data, Node* left, Node*right){
	Node *newNode=(Node*)malloc(sizeof(Node));
	newNode->data=data;
	newNode->left=left;
	newNode->right=right;
	return newNode;
	free(newNode);
}
Node* minNode(Node*node){
	if (node==NULL){
		return NULL;
	}
	if(node->left!=NULL){
		return minNode(node->left);
	}
	else{
		return node;
	}
}
Node* deleteNode(Node* node, int data, int*success){
	Node *target;
	int tempSuccess;
	if (node==NULL){
		tempSuccess=0;
	}
	else if (data<node->data){
		node->left=deleteNode(node->left, data, &tempSuccess);
	}
	else if (data>node->data){
		node->right=deleteNode(node->right, data, &tempSuccess);
	}
	else{
		if(node->left!=NULL && node->right!=NULL){	
			target=minNode(node->right);
			node->data=target->data;
			node->right=deleteNode(node->right, target->data, &tempSuccess);
			tempSuccess=1;
		}
		else{
			target=node;
			if(node->left==NULL){
				node=node->right;
			}
			else if(node->right==NULL){
				node=node->left;
			}
			tempSuccess=1;
		}
	}
	*success=tempSuccess;
	return node;
}
	
int insertNode(Node * node, int data){
	int height=0;
	if (node==NULL){
		Node* newNode=createNewNode(data, node, node);
		++height;
		node=newNode;	
	}
	else{
		Node*current=node;
		Node*prev=NULL;
		int right=0;
		while(current!=NULL){
			prev=current;
			if(data==current->data){
				return 0;
			}
			else if(data>current->data){
				++height;
				right=1;
				current=current->right;
			}
			else{
				++height;
				right=0;
				current=current->left;
			}
		}
		if(right==1){
			prev->right=createNewNode(data, NULL, NULL);
			++height;
		}
		else{
			prev->left=createNewNode(data, NULL, NULL);
			++height;
		}
	}
	return height;
}

int main (int argc, char*argv[]){
	int fd = open (argv[1], O_RDONLY);
	if (fd==-1){
		return -1;
	}
	int i=0;
	int numBytes=200;
	int numRead=0;
	char*buffer=(char*)malloc(sizeof(char)*numBytes);
	for(i=0; i<99; ++i){
		buffer[i]='\0';
	}
	Node*root=(Node*)malloc(sizeof(Node));
	root=NULL;
	while((numRead=read(fd, buffer, numBytes))!=0){
		if(numRead==-1){
			return -1;
		}
		int a=0;
		int b=0;
		char*line=(char*)malloc(sizeof(char)*(numRead));
		while(b<numRead){
			while(b<numRead && buffer[b]!='\n'){
				line[a]=buffer[b];
				++a;
				++b;
			}
			if (b>=numRead-1&& buffer[b]!='\n'){
				exit(0);
			}
			line[a]='\0';
			char op=line[0];
			char*num=(char*)malloc(sizeof(char)*b);
			int k=0;
			i=0;
			for (i=2; i<a; ++i){
				num [k]=line[i];
				++k;
			}
			int height=-1;
			int success=0;
			int target= atoi(num);
			free(num);
			switch(op){
				case 'i':
					if(root==NULL){
						root=createNewNode(target, NULL, NULL);
						height=1;
						printf("inserted 1\n");
					}
					else{
						height=insertNode(root,target);
						if(height==0){
							printf("duplicate\n");
						}
						else if(height>0){
							printf("inserted %d\n", height);
						}
						else{
							printf("ERROR\n");
						}
					}
					break;
				case 's':
					height=searchNode(root, target);
					if (height>0){
						printf("present %d\n", height);
					}
					else{
						printf("absent\n");
					}
					break;
				case 'd':
					root = deleteNode(root, target, &success);
					if (success==1){
						printf("success\n");
					}
					else{
						printf("fail\n");
					}
					break;
				default:
					printf("error\n");
					return -1;
			}
			a=0;
			++b;
			i=0;
			while(line[i]!='\0'){
				line[i]='\0';
				i++;
			}
		}
	}
	free(root);
	free(buffer);
	close(fd);
	return 0;
}
