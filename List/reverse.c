#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

typedef struct list {
        int value;
        struct list *next;
} list_t;

/*
  add_to_list - add an element to a single linked list

  The function adds a new element to the given list with value being the given
  value. It returns then new head of the list which is the old head unless the
  head was empty. The last element may be the tail of the list to speed up
  repeated adding. If tail is empty, the whole list will be traversed. The
  given head will be left untouched.

 */
list_t *add_to_list(int value, list_t *head)
{
        list_t *new_element, *runner = NULL;
        new_element = malloc(sizeof(*new_element));
        if(!new_element){
                perror("Allocate memory in add_to_list");
                exit(-1);
        }
        new_element->value = value;
        new_element->next  = NULL;
        if(!head){
                return new_element;
        } else {
                runner=head;
                while(runner->next){
                        runner = runner->next;
                }
                runner->next = new_element;
        }
        return head;
}

/* reverse_list_recursive - reverse a single linked list

   As the name says the function works recursively. This is fairly simple from
   an implementation perspective but has two major drawbacks:
   * every additional function call takes additional time and memory
   * we need to provide an additional parameter for the previous element
 */

list_t *reverse_list_recursive(list_t *head, list_t *prev)
{
        list_t *new_head;
        if(!head)
                return NULL;
        if (head->next){
                new_head = reverse_list_recursive(head->next, head);
        } else {
                new_head = head;
        }
        head->next = prev;
        return new_head;
}

/* reverse_list_iterative - reverse a single linked list

   As the name implies the function works iteratively.
 */

list_t *reverse_list_iterative(list_t *head)
{
        list_t *previous, *next;
        previous = NULL;
        while(head) {
                next = head->next;
                head->next = previous;
                previous = head;
                head = next;
        }
        return previous;
}

/*
  reverse_list - reverse a single linked list

  This is a wrapper function that allows to test different
  implementation without having to change the main function every time.

  Furthermore, the common wrapper allows us to have different implementation
  that expect different parameters and still export the same parameters to the
  caller each time.

 */

list_t *reverse_list(list_t *head)
{
        return(reverse_list_iterative(head));
}

/* ask for list - generate list from user input

   This function asks the user to enter numbers for a list into STDIN. This
   continues until the user presses CTRL-D.
 */

list_t *ask_for_list()
{
        list_t *head = NULL;
        int value, success;

        while(1){
                success = scanf("%d", &value);
                if(success == EOF){
                        break;
                } else {
                        head = add_to_list(value, head);
                }
        }
        return head;
}

/* print_elapsed_time - print time in seconds between given time structs */

void print_elapsed_time(struct timeval tv_start, struct timeval tv_end)
{
        time_t seconds;
        suseconds_t useconds;
        seconds = tv_end.tv_sec - tv_start.tv_sec;
        useconds = tv_end.tv_usec - tv_start.tv_usec;
        if(useconds < 0){
                seconds += 1;
                useconds += 1000000;
        }
        printf("%ld.%ld seconds\n", seconds, useconds);
        return;
}

list_t *generate_list(int amount)
{
        list_t *head = NULL;
        for(int i = 0; i < amount; i++){
                head = add_to_list(i, head);
        }
        return head;
}



void print_list(list_t *head)
{
        list_t *next;
        next = head;
        while(next){
                printf("%d->",next->value);
                next = next->next;
        }
        printf("*\n");

}




int main(int argc, char **argv)
{
        list_t *head;
        struct timeval tv_start, tv_end;

        head = NULL;
        if(argc == 2){
                gettimeofday(&tv_start, NULL);
                head = generate_list(atoi(argv[1]));
                gettimeofday(&tv_end, NULL);
                printf("Generate list: ");
                print_elapsed_time(tv_start, tv_end);
                if(!head){
                        perror("Can not generate a list");
                }
        } else {
                printf("Please enter data for a list. End with Ctrl-D\n");
                head = ask_for_list();
        }

        gettimeofday(&tv_start, NULL);
        head = reverse_list_iterative(head);
        gettimeofday(&tv_end, NULL);
        printf("Iterative reversal: ");
        print_elapsed_time(tv_start, tv_end);

        gettimeofday(&tv_start, NULL);
        head = reverse_list_recursive(head, NULL);
        gettimeofday(&tv_end, NULL);
        printf("Recursively reversal: ");
        print_elapsed_time(tv_start, tv_end);

}
