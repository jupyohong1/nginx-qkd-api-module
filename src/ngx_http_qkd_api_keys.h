#ifndef __NGX_HTTP_QKD_API_KEYS_H__
#define __NGX_HTTP_QKD_API_KEYS_H__

#include "ngx_http_qkd_api_module.h"

#define MAX_UUID_LEN  36
#define MAX_KEY_LEN   1024

typedef struct ngx_http_qkd_api_enc_keys_req_s ngx_http_qkd_api_enc_keys_req_t;
struct ngx_http_qkd_api_enc_keys_req_s {
  int number;
  int size;
};

typedef struct ngx_http_qkd_api_dec_keys_req_s ngx_http_qkd_api_dec_keys_req_t;
struct ngx_http_qkd_api_dec_keys_req_s {
  char                              key_id[MAX_UUID_LEN+1];
  ngx_http_qkd_api_dec_keys_req_t*  prev;
  ngx_http_qkd_api_dec_keys_req_t*  next;
};

typedef struct ngx_http_qkd_api_key_s ngx_http_qkd_api_key_t;
struct ngx_http_qkd_api_key_s {
  char                     key_id[MAX_UUID_LEN+1];
  char                     key[MAX_KEY_LEN+1];
  ngx_http_qkd_api_key_t*  prev;
  ngx_http_qkd_api_key_t*  next;
};

ngx_int_t ngx_http_qkd_api_enc_keys_post(ngx_http_request_t* r, u_char* str, int* len);
ngx_int_t ngx_http_qkd_api_enc_keys_get(ngx_http_request_t* r, u_char* str, int* len);
ngx_int_t ngx_http_qkd_api_enc_keys(ngx_http_request_t* r, u_char* str, int* len, ngx_http_qkd_api_enc_keys_req_t *req);

ngx_int_t ngx_http_qkd_api_dec_keys_post(ngx_http_request_t* r, u_char* str, int* len);
ngx_int_t ngx_http_qkd_api_dec_keys_get(ngx_http_request_t* r, u_char* str, int* len);
ngx_int_t ngx_http_qkd_api_dec_keys(ngx_http_request_t* r, u_char* str, int* len, ngx_http_qkd_api_dec_keys_req_t *req);

#endif // __NGX_HTTP_QKD_API_KEYS_H__
