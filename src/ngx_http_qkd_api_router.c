#include "ngx_http_qkd_api_router.h"
#include "ngx_http_qkd_api_status.h"
#include "ngx_http_qkd_api_keys.h"

ngx_http_qkd_api_route_tbl_t api_v1_keys_route_tbl[] = {
  {"/status",    NULL,    0,    "GET",     ngx_http_qkd_api_status_get},
  {"/enc_keys",    NULL,    0,    "GET",     ngx_http_qkd_api_enc_keys_get},
  {"/enc_keys",    NULL,    0,    "POST",     ngx_http_qkd_api_enc_keys_post},
  {"/dec_keys",    NULL,    0,    "GET",     ngx_http_qkd_api_dec_keys_get},
  {"/dec_keys",    NULL,    0,    "POST",     ngx_http_qkd_api_dec_keys_post},
};

ngx_http_qkd_api_route_tbl_t api_v1_route_tbl[] = {
  {"/keys",    api_v1_keys_route_tbl,    sizeof(api_v1_keys_route_tbl)/sizeof(ngx_http_qkd_api_route_tbl_t),    "",     NULL},
};

ngx_http_qkd_api_route_tbl_t api_route_tbl[] = {
  {"/v1",    api_v1_route_tbl,    sizeof(api_v1_route_tbl)/sizeof(ngx_http_qkd_api_route_tbl_t),    "",     NULL},
};

ngx_http_qkd_api_route_tbl_t route_tbl[] = {
  {"/api",    api_route_tbl,    sizeof(api_route_tbl)/sizeof(ngx_http_qkd_api_route_tbl_t),    "",     NULL},
};

/**/

ngx_int_t ngx_http_qkd_api_router(ngx_http_request_t *r, u_char* str, int* len)
{
  ngx_http_qkd_api_route_tbl_t* tbl = route_tbl;
  int tbl_count = sizeof(route_tbl)/sizeof(ngx_http_qkd_api_route_tbl_t);

  ngx_str_t path;
  u_char *p, *last, *slash;

  p = r->uri.data;
  last = p + r->uri.len;

  for (; p < last;)
  {
    slash = ngx_strlchr(p + 1, last, '/');
    if (slash == NULL || slash > last)
    {
      slash = last;
    }

    path.data = p;
    path.len = slash - p;
    dd("path: %.*s, slash: %s, last: %s\n", (int)path.len, path.data, slash, last);

    int i;
    char is_find = FALSE;
    for (i = 0; i < tbl_count; i++)
    {
//       dd("i: %d, path: %s : %.*s, method: %s : %.*s\n",
//           i,
//           tbl[i].path, (int)path.len, path.data,
//           tbl[i].method, (int)r->method_name.len, r->method_name.data);

      if (ngx_strncmp(tbl[i].path, path.data, path.len) == 0)
      {
        if (slash == last)
        {
          if (ngx_strncmp(tbl[i].method, r->method_name.data, r->method_name.len) != 0)
          {
            continue;
          }

          if (tbl[i].func == NULL)
          {
            dd("not defined function\n");
            sprintf((char*)str, "not defined function");
            *len = strlen((const char*)str); 
            return NGX_HTTP_NOT_FOUND;
          }

          return tbl[i].func(r, str, len);
        }
        else
        {
          if (tbl[i].child_tbl == NULL)
          {
            dd("not defined next route\n");
            sprintf((char*)str, "not defined next route");
            *len = strlen((const char*)str); 
            return NGX_HTTP_NOT_FOUND;
          }
          tbl_count = tbl[i].child_count;
          tbl = tbl[i].child_tbl;
        }

        is_find = TRUE;
        break;
      }
    }

    if (is_find == FALSE)
      return NGX_ERROR;


    p = slash;
  }

  return NGX_OK;
}

