#include "yukon.h"

int main(void) {

    yukon_init();

    int ret = start_game();

    yukon_destroy();

    return ret;
}
