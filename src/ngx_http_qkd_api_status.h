#ifndef __NGX_HTTP_QKD_API_STATUS_H__
#define __NGX_HTTP_QKD_API_STATUS_H__

#include "ngx_http_qkd_api_module.h"

typedef struct ngx_http_qkd_api_status_s ngx_http_qkd_api_status_t;
struct ngx_http_qkd_api_status_s {
  char source_kme_id[255];
  char target_kme_id[255];
  char master_sae_id[255];
  char slave_sae_id[255];
  int key_size;
  int stored_key_count;
  int max_key_count;
  int max_key_per_request;
  int max_key_size;
  int min_key_size;
  int max_sae_id_count;
};

ngx_int_t ngx_http_qkd_api_status_get(ngx_http_request_t* r, u_char* str, int* len);

#endif //__NGX_HTTP_QKD_API_STATUS_H__
