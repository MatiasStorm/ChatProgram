#include "../src/server/socket_node.h";
#include <stdlib.h>
#include <check.h>

 	

START_TEST (test_name)
{
  ck_assert_int_eq(1,1);
}
END_TEST

Suite *socket_node_suite(void){
    Suite *s;
    TCase *tc_core;

    s = suite_create("Socket");
    tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_name);
    suite_add_tcase(s, tc_core);

    return s;
}



int main(int argc, char const *argv[])
{
    int number_failed;
    Suite *s;
    SRunner *sr;
    s = socket_node_suite();
    SRunner *sr;

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;


    // socket_node_t *head = NULL; 
    // socket_node_t *node1 = malloc(sizeof(socket_node_t));
    // socket_node_t *node2 = malloc(sizeof(socket_node_t));
    // socket_node_t *node3 = malloc(sizeof(socket_node_t));
}
