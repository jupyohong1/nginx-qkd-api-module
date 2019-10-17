#ifndef __NGX_HTTP_QKD_API_ROUTER_H__
#define __NGX_HTTP_QKD_API_ROUTER_H__

#include "ngx_http_qkd_api_module.h"

typedef struct ngx_http_qkd_api_route_tbl_s ngx_http_qkd_api_route_tbl_t;
struct ngx_http_qkd_api_route_tbl_s {
    char                            path[20];
    ngx_http_qkd_api_route_tbl_t    *child_tbl;
    int                             child_count;
    char                            method[10];
    ngx_int_t                       (*func)(ngx_http_request_t*, u_char*, int*);
};

ngx_int_t ngx_http_qkd_api_router(ngx_http_request_t *r, u_char* str, int* len);


#endif // __NGX_HTTP_QKD_API_ROUTER_H__
