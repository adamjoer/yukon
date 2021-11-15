#include "yukon.h"

int main(int argc, char *argv[]) {

    yukon_init(argc, argv);

    int ret = start_game();

    yukon_destroy();

    return ret;
}
