#include "main.h"
#include "SPRITEs.h"
#include "lists.h"
#include <stdint.h>
#include <check.h>

START_TEST(test_push){
  SPRITE s = DEFAULT_GHOST;
  SPRITE *head = NULL;
  push(&head, s);
  ck_assert(head->representation == CH_GHOST);
  SPRITE s2 = DEFAULT_FRUIT;
  push(&head, s2);
  //tem que inserir no comeco da lista
  ck_assert(head->next->representation == CH_GHOST);
  ck_assert(head->representation == CH_FRUIT);
}
END_TEST

START_TEST(test_list_size){
  SPRITE s = DEFAULT_GHOST;
  SPRITE *head = NULL;
  ck_assert(list_size(head) == 0);
  push(&head, s);
  ck_assert(list_size(head) == 1);
}
END_TEST

START_TEST(test_count_alive){
  SPRITE s = DEFAULT_GHOST;
  SPRITE *head = NULL;
  push(&head, s);
  ck_assert(count_alive(head) == 1);
  head->alive = 0;
  ck_assert(count_alive(head) == 0);
}
END_TEST


START_TEST(test_collided){
  SPRITE s1 = {.alive = 1, .representation = 'a', .position = {0, 0, 0, 0}};
  SPRITE s2 = {.alive = 1, .representation = 'b', .position = {0, 0, 0, 0}};
  ck_assert_int_eq(collided(&s1, &s2), 1);
  SPRITE s3 = {.alive = 1, .representation = 'a', .position = {0, 0, 1, 0}};
  SPRITE s4 = {.alive = 1, .representation = 'b', .position = {0, 0, 0, 0}};
  ck_assert_int_eq(collided(&s3, &s4), 1);
  SPRITE s5 = {.alive = 1, .representation = 'a', .position = {0, 1, 0, 0}};
  SPRITE s6 = {.alive = 1, .representation = 'b', .position = {0, 0, 0, 1}};
  ck_assert_int_eq(collided(&s5, &s6), 1);
  SPRITE s7 = {.alive = 1, .representation = 'a', .position = {0, 0, 0, 1}};
  SPRITE s8 = {.alive = 1, .representation = 'b', .position = {0, 1, 0, 0}};
  ck_assert_int_eq(collided(&s7, &s8), 1);
  SPRITE s9 = {.alive = 1, .representation = 'a', .position = {1, 0, 0, 0}};
  SPRITE s10 = {.alive = 1, .representation = 'b', .position = {0, 0, 1, 0}};
  ck_assert_int_eq(collided(&s9, &s10), 1);
  SPRITE s11 = {.alive = 1, .representation = 'a', .position = {0, 2, 0, 1}};
  SPRITE s12 = {.alive = 1, .representation = 'b', .position = {0, 1, 0, 0}};
  ck_assert_int_eq(collided(&s11, &s12), 0);

}
END_TEST

Suite * mr_do_suite(void);
Suite * mr_do_suite(void){
  Suite *s;
  TCase *tc_unit;

  s = suite_create("Mr_DO");

  //testa casos unitarios
  tc_unit = tcase_create("Units");

  tcase_add_test(tc_unit, test_push);
  tcase_add_test(tc_unit, test_list_size);
  tcase_add_test(tc_unit, test_count_alive);
  tcase_add_test(tc_unit, test_collided);
  suite_add_tcase(s, tc_unit);

  return s;
}

int main(void){
  int number_failed;
  Suite *s;
  SRunner *sr;

  s = mr_do_suite();
  sr = srunner_create(s);

  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
