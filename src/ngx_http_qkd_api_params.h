#ifndef __NGX_HTTP_QKD_API_PARAMS_H__
#define __NGX_HTTP_QKD_API_PARAMS_H__

#include "ngx_http_qkd_api_module.h"

typedef struct ngx_http_qkd_api_param_s ngx_http_qkd_api_param_t;
struct ngx_http_qkd_api_param_s {
  ngx_str_t                   key;
  ngx_str_t                   value;
  ngx_http_qkd_api_param_t*   prev;
  ngx_http_qkd_api_param_t*   next;
};

ngx_int_t ngx_http_qkd_api_get_params(ngx_http_request_t *r, ngx_http_qkd_api_param_t** p);

#endif // __NGX_HTTP_QKD_API_PARAMS_H__
