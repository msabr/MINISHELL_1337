#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("Usage: %s <string_to_test>\n", argv[0]);
        return 1;
    }

    if (check_closedqt(argv[1]))
        printf("Input: quotes fermées ✅\n");
    else
        printf("Input: quotes non fermées n");

    return 0;
}
