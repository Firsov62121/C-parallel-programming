#include <ev.h>
#include <stdio.h>
#include <netinet/in.h>
#include <stdlib.h>

static void read_cb(struct ev_loop *loop, ev_io *w, int revents)
{
    char buf[1024];
    size_t r = recv(w->fd, buf, 1024, MSG_NOSIGNAL);
    if(r == 0)
    {
        ev_io_stop(loop, w);
        free(w);
    }
    else if(r > 0)
    {
        send(w->fd, buf, r, MSG_NOSIGNAL);
    }
}

static void accept_cb(struct ev_loop *loop, ev_io *w, int revents)
{
    int client_sd = accept(w->fd, 0, 0); //create connection
    struct ev_io *w_client = (struct ev_io*)
        malloc(sizeof(struct ev_io));
    ev_io_init(w_client, read_cb, client_sd, EV_READ);
    ev_io_start(loop, w_client);
}

int main()
{
    struct ev_loop *loop = ev_default_loop(0);
    int sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(12345);

    bind(sd, (struct sockaddr *) &addr, sizeof(addr));
    listen(sd, SOMAXCONN);

    struct ev_io w_accept;
    ev_io_init(&w_accept, accept_cb, sd, EV_READ);
    while(1) 
        ev_loop(loop, 0);
}
