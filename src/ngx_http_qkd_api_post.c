#include "ngx_http_qkd_api_post.h"

void ngx_http_qkd_api_post_read_request_body_handler(ngx_http_request_t *r)
{
  //dd("ngx_http_qkd_api_post_read_request_body\n");
  ngx_http_finalize_request(r, NGX_OK);
}

ngx_int_t ngx_http_qkd_api_post_read_request_body(ngx_http_request_t *r)
{
  ngx_int_t ret;

  ret = ngx_http_read_client_request_body(r, ngx_http_qkd_api_post_read_request_body_handler);
  if (ret >= NGX_HTTP_SPECIAL_RESPONSE)
    return ret;
  return NGX_DONE;
}

ngx_str_t ngx_http_qkd_api_post_get_request_body(ngx_http_request_t *r)
{
  u_char *p, *data;
  size_t len = 0;
  ngx_buf_t *buf, *next;
  ngx_chain_t *cl;
  ngx_str_t body = ngx_null_string;

  if (r->request_body == NULL || r->request_body->bufs == NULL || r->request_body->temp_file) 
  {
    return body;
  }
  else
  {
    cl = r->request_body->bufs;
    buf = cl->buf;

    if (cl->next == NULL) {
      len = (buf->last - buf->pos);
      //dd("get request body size: %d\n", (int)len);
      p = ngx_pnalloc(r->pool, len + 1); 
      if (p == NULL) {
        return body;
      }   
      data = p;
      ngx_memcpy(p, buf->pos, len);
      data[len] = 0;

      //dd("ngx_cpymeme: %s\n", p); 
    } else {
      //dd("has next buf request body\n");
      next = cl->next->buf;
      len = (buf->last - buf->pos) + (next->last - next->pos);
      p = ngx_pnalloc(r->pool, len + 1); 
      data = p;
      if (p == NULL) {
        return body;
      }   
      p = ngx_cpymem(p, buf->pos, buf->last - buf->pos);
      ngx_memcpy(p, next->pos, next->last - next->pos);
      data[len] = 0;
    }
  }
  
  body.len = len;
  body.data = data;

  return body;
}

