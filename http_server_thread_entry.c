#include "http_server_thread.h"

void http_server_entry();

/* Http Server Thread entry function */
void http_server_thread_entry(void)
{
    http_server_entry();
    /* TODO: add your own code here */
    while (1)
    {
        tx_thread_sleep (1);
    }
}
