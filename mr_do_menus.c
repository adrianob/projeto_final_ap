#include "mr_do_menus.h"

void show_menu(void){
  char *choices[] = {
                    "Novo Jogo",
                    "Continuar",
                    "High Scores",
                    "Sair"
  };

  ITEM **menu_items;
  MENU *game_menu;
  int n_choices = ARRAY_SIZE(choices);
  menu_items = (ITEM **)calloc(n_choices + 1, sizeof(ITEM *));

  for(int i = 0; i < n_choices; ++i){
    menu_items[i] = new_item(choices[i], "");
  }

  set_item_userptr(menu_items[0], play);//nivel 1
  set_item_userptr(menu_items[3], exit);//sair

  game_menu = new_menu((ITEM **)menu_items);
  post_menu(game_menu);
  refresh();

  int c;
  while((c = getch()) != KEY_F(1)){
    switch(c){
      case KEY_DOWN:
        menu_driver(game_menu, REQ_DOWN_ITEM);
        break;
      case KEY_UP:
        menu_driver(game_menu, REQ_UP_ITEM);
        break;
      case 10: //enter
        {
        ITEM *cur;
				void (*p)(char *);

        cur = current_item(game_menu);
        p = item_userptr(cur);
        p((char *)item_name(cur));
        pos_menu_cursor(game_menu);
        }
        break;
    }
  }
}