#include <uv.h>
#include <stdio.h>
#include <stdlib.h>

uv_tcp_t server;
uv_loop_t *loop;

void read_cb(uv_stream_t *stream, size_t nread, uv_buf_t buf)
{
    uv_write_t *req = (uv_write_t *)malloc(sizeof(uv_write_t));
    uv_write(req, stream, &buf, 1, NULL);
    free(buf.base);
}

uv_buf_t alloc_buf_cb(uv_handle_t *handle, size_t size)
{
    return uv_buf_init((char *)malloc(size), size);
}

void connection_cb(uv_stream_t *server, int status)
{
    uv_tcp_t *client = (uv_tcp_t *)malloc(sizeof(uv_tcp_t));

    uv_tcp_init(loop, client);
    uv_accept(server, (uv_stream_t *) client);
    uv_read_start((uv_stream_t *)client, alloc_buf_cb, read_cb);
}

int main()
{
    loop = uv_default_loop();

    struct sockaddr_in addr;
    uv_ip4_addr("127.0.0.1", 12345, &addr);

    uv_tcp_init(loop, &server);
    uv_tcp_bind(&server, &addr, UV_TCP_IPV6ONLY);
    uv_listen((uv_stream_t *)&server, 128, connection_cb);


    return uv_run(loop, UV_RUN_DEFAULT);
}
