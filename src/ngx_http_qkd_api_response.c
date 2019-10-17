#include "ngx_http_qkd_api_response.h"

ngx_int_t ngx_http_qkd_api_response(ngx_http_request_t *r, u_char* str, int len, ngx_int_t result)
{
  ngx_buf_t *b;
  ngx_chain_t out;

  /* Set the Content-Type header. */
  r->headers_out.content_type.len = sizeof("text/plain") - 1;
  r->headers_out.content_type.data = (u_char *) "text/plain";

  /* Allocate a new buffer for sending out the reply. */
  b = ngx_pcalloc(r->pool, sizeof(ngx_buf_t));

  /* Insertion in the buffer chain. */
  out.buf = b;
  out.next = NULL; /* just one buffer */

  b->pos = str; /* first position in memory of the data */
  b->last = str + len; /* last position in memory of the data */
  b->memory = 1; /* content is in read-only memory */
  b->last_buf = 1; /* there will be no more buffers in the request */

  /* Sending the headers for the reply. */
  r->headers_out.status = (result == NGX_OK) ? NGX_HTTP_OK : result; /* 200 status code */
  /* Get the content length of the body. */
  r->headers_out.content_length_n = len;
  ngx_http_send_header(r); /* Send the headers */

  /* Send the body, and return the status code of the output filter chain. */
  return ngx_http_output_filter(r, &out);
} /* ngx_http_qkd_api_handler */

