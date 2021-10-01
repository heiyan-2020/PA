#include "sdb.h"

#define NR_WP 32

typedef struct watchpoint {
  int NO;
	word_t preValue;
	char expression[128];
  struct watchpoint *next;

  /* TODO: Add more members if necessary */

} WP;

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = &wp_pool[i + 1];
  }
  wp_pool[NR_WP - 1].next = NULL;

  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */
void new_wp(char *expression) {
	if (free_ == NULL) {
		printf("There are no more watchpoints!\n");
		assert(0);
	} else {
		WP* active_wp = free_;
		free_ = free_->next;
		
		active_wp->next = head;
		strcpy(active_wp->expression, expression);
		bool success = true;
		active_wp->preValue = expr(expression, &success);
		head = active_wp;
	}
}

void free_wp(WP* wp) {
	if (wp == NULL) {
		assert(0);
	} else {
		wp->next = free_;
		free_ = wp;
	}
}


bool iter_wp() {
	WP* iter = head;
	bool success = true;
	while (iter != NULL) {
		word_t newValue = expr(iter->expression, &success);
		if (newValue != iter->preValue) {
			return false;
		}
		iter = iter->next;
	}
	return true;
}

bool delete_wp(int index) {
	WP* iter = head;
	if (head->NO == index) {
		head = head->next;
	} else {
	while(iter->next != NULL) {
		if (iter->next->NO == index) {
			WP* tmp = iter->next->next;
			free_wp(iter->next);
			iter->next = tmp;
			return true;			
		}
	}
	}
	return false;
}

void print_wp() {
	WP* iter = head;
	if (head == NULL) {
		printf("No watchpoints.\n");
	} else {
		printf("Num\t\tWhat\n");
		while (iter != NULL) {
			printf("%d\t\t%s", iter->NO, iter->expression);
		}
	}
}
