#pragma once

//Node for linkedlist

typedef struct node{
	int data;
	struct node* next;
};

//PriorityQueue is a class which is an implementation of element of highest priority to be removed first using linkedlist.
//For a MaxPriorityQueue(Greater element has more priority) initialize queue with 1
//PriorityQueue queue(1);
//For a MinPriorityQueue(Lesser element has more priority) initialize queue with -1
//PriorityQueue queue(-1);

//Space complexity is O(n), but to make enqueue more efficient can use array but will make space less efficient
//Enqueue O(n) can be O(logn), but to use O(logn) we need binary search which can only be done using array
//Dequeue and top O(1)


class PriorityQueue{
	private:
		struct node* front;
		int priority;
	public:
		PriorityQueue(int priority_param)
		{
			front = NULL;
			priority = priority_param;
		}
		void enqueue(int data)
		{
			struct node* temp;
			temp = new struct node;
			temp->data = data;
			if (front == NULL || (data - front->data)*priority > 0)
			{
				temp->next = front;
				front = temp;
			}
			else
			{
				struct node* node = front;
				while (node->next != NULL && (node->next->data - data)*priority >=0) node = node->next;
				temp->next = node->next;
				node->next = temp;
			}
		}
		void dequeue()
		{
			struct node* temp;
			if (front == NULL) return;
			temp = front;
			front = temp->next;
			free(temp);
		}
		int top()
		{
			if (front == NULL) return NULL;
			return front->data;
		}

};