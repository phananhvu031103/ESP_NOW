#ifndef __APP_HTTP_SERVER_H__
#define __APP_HTTP_SERVER_H__

typedef void (*http_server_handle)(char *data, int len);

void start_webserver(void);
void stop_webserver(void);
void http_get_set_callback(void *cb);
void http_post_set_callback(void *cb);
void rgb_post_set_callback(void *cb);
void http_send_response(char *data, int len);

#endif // __APP_HTTP_SERVER_H__