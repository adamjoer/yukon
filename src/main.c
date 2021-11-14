#include "yukon.h"

int main() {

    yukon_init();

    int ret = start_game();

    yukon_destroy();

    return ret;
}
