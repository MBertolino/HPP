#include <stdio.h>
#include <stdlib.h>

typedef struct link_node {
	int day;
	double min;
	double max;
	struct link_node *next;
} node_t;

void print_list(node_t *node) {
	if(node == NULL) {
		return;
	} else {
		printf("%i\t%lf\t%lf\n", node->day, node->min, node->max);
	}
		
	if(node->next != NULL) {
		print_list(node->next);
	}
}

void insert(node_t **node, int day, double min, double max) {
	if((*node) == NULL) {
		/* Add new node */
		node_t *new_node = (node_t *)malloc(sizeof(node_t));
		new_node->day = day;
		new_node->min = min;
		new_node->max = max;
		new_node->next = NULL;
		*node = new_node;	
	} else if(day < (*node)->day) {
		/* Push the nodes */
		node_t *new_node = (node_t *)malloc(sizeof(node_t));
    	new_node->day = day;
    	new_node->min = min;
    	new_node->max = max;
    	new_node->next = *node;
   		*node = new_node;
	} else if(day == (*node)->day) {
		/* Update node */
		(*node)->min = min;
		(*node)->max = max;
	} else {
		insert(&(*node)->next, day, min, max);
	}
}

void delete(node_t **node, int day) {
	if(*node == NULL) {
		printf("No such node.\n");	
	} else if(day == (*node)->day) {
		/* If the node is found, delete it */
		(*node)->day = 0;
		(*node)->min = 0;
		(*node)->max = 0;
		node_t *toDelete = *node;
		*node = (*node)->next;
		free(toDelete);
	} else {
		delete(&((*node)->next), day);
	}
}

int main() {
	node_t *head = NULL;
	int day;
	double min, max;
		
	char command;	
	int run = 1;
	while(run) {
		printf("Enter command: ");
		scanf(" %c", &command);
		
		/* For given command do something */
		switch(command) {
			case 'A': case 'a':
				scanf("%i%lf%lf", &day, &min, &max);
				if(day <= 31 && day >= 0) {
					insert(&head, day, min, max);
				} else {
					printf("%i is not a day in January\n", day);
				}
				break;
			case 'D': case 'd':
				scanf("%i", &day);
				if(day <= 31 && day >=0) {
					delete(&head, day);
				} else {
					printf("%i is not a day in January\n", day);
				}
				break;
			case 'P': case 'p':
				printf("day\tmin\t\tmax\n");
				print_list(head);
				printf("\n");
				break;
			case 'Q': case'q':
				run = 0;
				break;
			default :
				printf("Wrong command, use A, D, P or Q\n");
		}
	}	
	
	/* Free the nodes */
	node_t *node = head;
	while(node != NULL) {
		node_t *temp = node;
		node = node->next;
		free(temp);
	}
	free(node);
	
	return 0;
}
