#include "ngx_http_qkd_api_status.h"
#include "ngx_http_qkd_api_params.h"
#include "cJSON.h"

/*
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
*/
ngx_http_qkd_api_status_t status_data[] = {
  {
    "KME-KT-RegionCode-SerialNumber1-Version",
    "KME-KT-RegionCode-SerialNumber2-Version",
    "IIIIJJJJKKKKLLLL",
    "123",
    32,
    25000,
    100000,
    128,
    32,
    32,
    0
  }
};

ngx_int_t ngx_http_qkd_api_status_get(ngx_http_request_t* r, u_char* str, int* len)
{
  ngx_http_qkd_api_status_t* status = status_data;
  ngx_http_qkd_api_param_t *params = NULL, *tmp;

  int param_count = ngx_http_qkd_api_get_params(r, &params);
  dd("param_count: %d\n", param_count);
  while (params)
  {
    dd("key: %.*s, value: %.*s\n", 
        (int)params->key.len, params->key.data,
        (int)params->value.len, params->value.data);
    if (ngx_strncmp("sae_id", params->key.data, params->key.len) == 0)
    {
      sprintf(status[0].slave_sae_id, "%.*s", (int)params->value.len, params->value.data);
      dd("%.*s\n", (int)params->value.len, params->value.data);
    }

    if (params->next == params)
    {
      break;
    }
    else
    {
      tmp = params;
      params = params->next;
      ngx_queue_remove(tmp);
      ngx_pfree(r->pool, tmp);
    }
  }

  if (params != NULL)
  {
    ngx_pfree(r->pool, params);
  }
  
  cJSON* root = cJSON_CreateObject();
  
  cJSON_AddStringToObject(root, "source_KME_ID", status[0].source_kme_id);
  cJSON_AddStringToObject(root, "target_KME_ID", status[0].target_kme_id);
  cJSON_AddStringToObject(root, "master_SAE_ID", status[0].master_sae_id);
  cJSON_AddStringToObject(root, "slave_SAE_ID", status[0].slave_sae_id);
  cJSON_AddNumberToObject(root, "key_size", status[0].key_size);
  cJSON_AddNumberToObject(root, "stored_key_count", status[0].stored_key_count);
  cJSON_AddNumberToObject(root, "max_key_count", status[0].max_key_count);
  cJSON_AddNumberToObject(root, "max_key_per_request", status[0].max_key_per_request);
  cJSON_AddNumberToObject(root, "max_key_size", status[0].max_key_size);
  cJSON_AddNumberToObject(root, "min_key_size", status[0].min_key_size);
  cJSON_AddNumberToObject(root, "max_SAE_ID_count", status[0].max_sae_id_count);

  strcpy((char*)str, cJSON_Print(root));
  *len = strlen((const char*)str);

  free(root); 

  dd("status get:\n%s\n", str);
 
  return NGX_OK;
}

