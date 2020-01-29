#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        exit();
    }
    int sec = atoi(argv[1]);
    if (sec > 0)
    {
        sleep(sec);
    }
    exit();
}
