#include "ngx_http_qkd_api_module.h"
#include "ngx_http_qkd_api_response.h"
#include "ngx_http_qkd_api_router.h"
#include "ngx_http_qkd_api_post.h"

static char *ngx_http_qkd_api(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
static ngx_int_t ngx_http_qkd_api_handler(ngx_http_request_t *r);

/**
 * This module provided directive: hello world.
 *
 */
static ngx_command_t ngx_http_qkd_api_commands[] = {

  { ngx_string("qkd_api"), /* directive */
    NGX_HTTP_LOC_CONF|NGX_CONF_NOARGS, /* location context and takes
                                          no arguments*/
    ngx_http_qkd_api, /* configuration setup function */
    0, /* No offset. Only one context is supported. */
    0, /* No offset when storing the module configuration on struct. */
    NULL},

  ngx_null_command /* command termination */
};

/* The module context. */
static ngx_http_module_t ngx_http_qkd_api_module_ctx = {
  NULL, /* preconfiguration */
  NULL, /* postconfiguration */

  NULL, /* create main configuration */
  NULL, /* init main configuration */

  NULL, /* create server configuration */
  NULL, /* merge server configuration */

  NULL, /* create location configuration */
  NULL /* merge location configuration */
};

/* Module definition. */
ngx_module_t ngx_http_qkd_api_module = {
  NGX_MODULE_V1,
  &ngx_http_qkd_api_module_ctx, /* module context */
  ngx_http_qkd_api_commands, /* module directives */
  NGX_HTTP_MODULE, /* module type */
  NULL, /* init master */
  NULL, /* init module */
  NULL, /* init process */
  NULL, /* init thread */
  NULL, /* exit thread */
  NULL, /* exit process */
  NULL, /* exit master */
  NGX_MODULE_V1_PADDING
};

/**
 * Content handler.
 *
 * @param r
 *   Pointer to the request structure. See http_request.h.
 * @return
 *   The status of the response generation.
 */
static ngx_int_t ngx_http_qkd_api_handler(ngx_http_request_t *r)
{
  u_char response[2048];
  int len = 0;

  // for debug
  strncpy((char*)response, (const char*)r->uri.data, r->uri.len);
  len = r->uri.len;
  // end of for debug

  if (ngx_strncmp("POST", r->method_name.data, r->method_name.len) == 0)
  {
    ngx_http_qkd_api_post_read_request_body(r);
  }

  ngx_int_t result = ngx_http_qkd_api_router(r, response, &len);
  if (result != NGX_OK || len <= 0)
  {
    sprintf((char*)response, "unknown uri, %.*s", (int)r->uri.len, r->uri.data);
    len = strlen((const char*)response);
  }

  return ngx_http_qkd_api_response(r, response, len, result);
} /* ngx_http_qkd_api_handler */

/**
 * Configuration setup function that installs the content handler.
 *
 * @param cf
 *   Module configuration structure pointer.
 * @param cmd
 *   Module directives structure pointer.
 * @param conf
 *   Module configuration structure pointer.
 * @return string
 *   Status of the configuration setup.
 */
static char *ngx_http_qkd_api(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
  ngx_http_core_loc_conf_t *clcf; /* pointer to core location configuration */

  /* Install the hello world handler. */
  clcf = ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);
  clcf->handler = ngx_http_qkd_api_handler;

  return NGX_CONF_OK;
} /* ngx_http_qkd_api */
