#ifndef __NGX_HTTP_QKD_API_POST_H__
#define __NGX_HTTP_QKD_API_POST_H__

#include "ngx_http_qkd_api_module.h"

ngx_int_t ngx_http_qkd_api_post_read_request_body(ngx_http_request_t *r);
ngx_str_t ngx_http_qkd_api_post_get_request_body(ngx_http_request_t *r);


#endif // __NGX_HTTP_QKD_API_POST_H__
