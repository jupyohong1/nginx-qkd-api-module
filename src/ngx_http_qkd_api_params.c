#include "ngx_http_qkd_api_params.h"

/*
typedef struct ngx_http_qkd_api_param_s ngx_http_qkd_api_param_t;
struct ngx_http_qkd_api_param_s {
  ngx_str_t                   key;
  ngx_str_t                   value;
  ngx_http_qkd_api_param_t*   prev;
  ngx_http_qkd_api_param_t*   next;
};
*/

ngx_int_t ngx_http_qkd_api_get_params(ngx_http_request_t *r, ngx_http_qkd_api_param_t** params)
{
  ngx_http_qkd_api_param_t* param;
  u_char *p, *last, *ampersand, *equal;
  int count = 0;

  if (r->args.len == 0)
    return NGX_OK;


  p = r->args.data;
  last = p + r->args.len;

  for (; p < last; p++)
  {
    param = ngx_pcalloc(r->pool, sizeof(ngx_http_qkd_api_param_t));
    if (param == NULL)
      return NGX_ERROR;

    ampersand = ngx_strlchr(p, last, '&');
    if (ampersand == NULL)
      ampersand = last;

    equal = ngx_strlchr(p, last, '=');
    if (equal == NULL || equal > ampersand)
      equal = ampersand;

    param->key.data = p;
    param->key.len = equal - p;

    param->value.data = equal + 1;
    param->value.len = ampersand - equal - 1;

    if (*params == NULL)
    {
      ngx_queue_init(param);
      *params = param;
    }
    else
    {
      ngx_queue_insert_tail(*params, param);
    }

    p = ampersand;
    count++;
  }

  return count;
}

