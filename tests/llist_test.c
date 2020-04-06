#include <stdlib.h>
#include <check.h>

#include "../lib/llist.h"



START_TEST(test_create_and_destroy_llist)
{
    llist *list = llist_create();
    ck_assert_ptr_eq(list->head, NULL);
    llist_destroy(&list);
    ck_assert_ptr_eq(list, NULL);
}
END_TEST


START_TEST(destory_single_node)
{
    llist *list = llist_create();
    int* data = malloc(sizeof(int));
    *data = 1;
    llist_node* node = llist_add_node(list, ((void*) data));
    llist_destroy_node(list, &node);
    ck_assert_ptr_eq(node, NULL);
    ck_assert_ptr_eq(list->head, NULL);
    llist_destroy(&list);
}
END_TEST

START_TEST(test_add_single_node)
{
    llist *list = llist_create();
    int* data = malloc(sizeof(int));
    *data = 1;
    llist_node* node = llist_add_node(list, (void*) data);    

    ck_assert_ptr_eq(node->data, data);
    ck_assert_ptr_eq(node->next, node);
    ck_assert_ptr_eq(list->head, node);
    
    llist_destroy(&list);
}
END_TEST



START_TEST(test_add_multiple_nodes)
{
    llist *list = llist_create();
    int* data1 = malloc(sizeof(int));
    *data1 = 1;
    llist_node* node1 = llist_add_node(list, ((void*) data1));    
    int* data2 = malloc(sizeof(int));
    *data2 = 1;
    llist_node* node2 = llist_add_node(list, ((void*) data2));    
    int* data3 = malloc(sizeof(int));
    *data3 = 1;
    llist_node* node3 = llist_add_node(list, ((void*) data3));  
    ck_assert_ptr_eq((int*)node1->data, data1);
    ck_assert_ptr_eq((int*)node2->data, data2);
    ck_assert_ptr_eq((int*)node3->data, data3);
    ck_assert_ptr_eq(list->head, node3);
    ck_assert_ptr_eq(node1->next, node2);
    ck_assert_ptr_eq(node2->next, node3);
    ck_assert_ptr_eq(node3->next, node1);

    llist_destroy(&list);
}
END_TEST

START_TEST(destory_one_node_multiple_nodes)
{
    llist *list = llist_create();
    int* data1 = malloc(sizeof(int));
    *data1 = 1;
    llist_node* node1 = llist_add_node(list, ((void*) data1));    
    int* data2 = malloc(sizeof(int));
    *data2 = 1;
    llist_node* node2 = llist_add_node(list, ((void*) data2));    
    int* data3 = malloc(sizeof(int));
    *data3 = 1;
    llist_node* node3 = llist_add_node(list, ((void*) data3));   
    llist_destroy_node(list, &node2);
    ck_assert_ptr_eq(node2, NULL);
    ck_assert_ptr_eq(list->head, node3);
    ck_assert_ptr_eq(node1->next, node3);
    ck_assert_ptr_eq(node3->next, node1);

    llist_destroy_node(list, &node1);
    llist_destroy_node(list, &node3);
    ck_assert_ptr_eq(node1, NULL);
    ck_assert_ptr_eq(node3, NULL);

    llist_destroy(&list);
}
END_TEST

START_TEST(destory_head_node_multiple_nodes)
{
    llist *list = llist_create();
    int* data1 = malloc(sizeof(int));
    *data1 = 1;
    llist_node* node1 = llist_add_node(list, ((void*) data1));    
    int* data2 = malloc(sizeof(int));
    *data2 = 1;
    llist_node* node2 = llist_add_node(list, ((void*) data2));    
    int* data3 = malloc(sizeof(int));
    *data3 = 1;
    llist_node* node3 = llist_add_node(list, ((void*) data3));
    llist_destroy_node(list, &node3);
    ck_assert_ptr_eq(node3, NULL);
    ck_assert_ptr_eq(list->head, node2);
    ck_assert_ptr_eq(node1->next, node2);
    ck_assert_ptr_eq(node2->next, node1);

    llist_destroy(&list);
}
END_TEST



Suite * llist_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("Linked list test");

    /* Core test case */
    tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_create_and_destroy_llist);
    tcase_add_test(tc_core, destory_single_node);
    tcase_add_test(tc_core, test_add_single_node);
    tcase_add_test(tc_core, test_add_multiple_nodes);
    tcase_add_test(tc_core, destory_one_node_multiple_nodes);
    tcase_add_test(tc_core, destory_head_node_multiple_nodes);
    suite_add_tcase(s, tc_core);

    return s;
}

int main(void)
{
  int number_failed;
  Suite *s;
  SRunner *sr;

  s = llist_suite();
  sr = srunner_create(s);

  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}