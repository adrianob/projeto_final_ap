#ifndef SCORES
#define SCORES

int compare(struct score *elem1, struct score *elem2);
void check_score(int score);
void high_scores(void);
void print_score(WINDOW *w, struct score *sc);
#endif
