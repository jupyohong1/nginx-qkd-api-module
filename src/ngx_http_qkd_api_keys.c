#include "ngx_http_qkd_api_keys.h"
#include "ngx_http_qkd_api_post.h"
#include "ngx_http_qkd_api_params.h"
#include "cJSON.h"

/*
struct ngx_http_qkd_api_key_s {
  char                     key_id[MAX_UUID_LEN];
  char                     key[MAX_KEY_LEN];
  ngx_http_qkd_api_key_t*  prev;
  ngx_http_qkd_api_key_t*  next;
};
*/

int g_sample_key_idx = 0;
ngx_http_qkd_api_key_t sample_keys[] = {
  {"0bd03cdb-59ec-468b-a35d-75ac96a33cf2", "Y1BIU3JCZ3N0R3BPZzhmd1pKRHF5eHVUMHA3M0VWbVc=", NULL, NULL},
  {"29420130-0296-4370-bab4-d683a5752d86", "MWtUWHZBblhBTEdPWWl2ajRMN0J5UXJjUElRSUNPU2k=", NULL, NULL},
};


// typedef struct ngx_http_qkd_api_enc_keys_req_s ngx_http_qkd_api_enc_keys_req_t;
// struct ngx_http_qkd_api_enc_keys_req_s {
//   int number;
//   int size;
// };

ngx_int_t ngx_http_qkd_api_enc_keys_post(ngx_http_request_t* r, u_char* str, int* plen)
{
  cJSON *json, *json_number, *json_size;
  ngx_str_t body;
  ngx_http_qkd_api_enc_keys_req_t req;
  memset(&req, 0, sizeof(ngx_http_qkd_api_enc_keys_req_t));

  body = ngx_http_qkd_api_post_get_request_body(r);
  dd("request body: len(%d)\n%.*s\n", (int)body.len, (int)body.len, body.data);

  if (body.len > 0)
  {
    json = cJSON_Parse((const char*)body.data);
    if (json == NULL)
    {
      req.number = 1;
      req.size = 256;
    }
    else
    {
      json_number = cJSON_GetObjectItem(json, "number");
      if (json_number != NULL)
        req.number = json_number->valueint;
      else
        req.number = 1;

      json_size = cJSON_GetObjectItem(json, "size");
      if (json_size != NULL)
        req.size = json_size->valueint;
      else
        req.size = 256;
    }

    ngx_pfree(r->pool, body.data);
  }

  return ngx_http_qkd_api_enc_keys(r, str, plen, &req);
}

ngx_int_t ngx_http_qkd_api_enc_keys_get(ngx_http_request_t* r, u_char* str, int* len)
{
  ngx_http_qkd_api_enc_keys_req_t req;
  memset(&req, 0, sizeof(ngx_http_qkd_api_enc_keys_req_t));

  req.number = 1;
  req.size = 256;

  ngx_http_qkd_api_param_t *params = NULL, *tmp;
  ngx_http_qkd_api_get_params(r, &params);
  while (params)
  {
//     dd("key: %.*s, value: %.*s\n", 
//         (int)params->key.len, params->key.data,
//         (int)params->value.len, params->value.data);

    if (ngx_strncmp("number", params->key.data, params->key.len) == 0)
      req.number = ngx_atoi(params->value.data, params->value.len);
    else if (ngx_strncmp("size", params->key.data, params->key.len) == 0)
      req.size = ngx_atoi(params->value.data, params->value.len);

    if (params->next == params)
      break;
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

  return ngx_http_qkd_api_enc_keys(r, str, len, &req);
}

ngx_int_t ngx_http_qkd_api_enc_keys(ngx_http_request_t* r, u_char* str, int* len, ngx_http_qkd_api_enc_keys_req_t *req)
{
  ngx_http_qkd_api_key_t *key = NULL;
  int i;
  int sample_keys_count = sizeof(sample_keys) / sizeof(ngx_http_qkd_api_key_t);

  dd("req.number: %d, req.size: %d\n", req->number, req->size);

  cJSON* root, *array, *info;
  
  root = cJSON_CreateObject();
  cJSON_AddItemToObject(root, "keys", array = cJSON_CreateArray());

  for (i = 0; i < req->number; i++)
  {
    int key_idx = g_sample_key_idx++ % sample_keys_count;
//    dd("g_sample_key_idx: %d, key_idx: %d, sample_keys_count: %d\n", g_sample_key_idx, key_idx, sample_keys_count);

    key = &sample_keys[key_idx];
   
    info = cJSON_CreateObject();
    cJSON_AddStringToObject(info, "key_ID", key->key_id);
    cJSON_AddStringToObject(info, "key", key->key);

    cJSON_AddItemToArray(array, info);
  }

  strcpy((char*)str, cJSON_Print(root));
  *len = strlen((const char*)str);

  cJSON_Delete(root);

  dd("enc_keys:\n%s\n", str);
 
  return NGX_OK;
}

// typedef struct ngx_http_qkd_api_dec_keys_req_s ngx_http_qkd_api_dec_keys_req_t;
// struct ngx_http_qkd_api_dec_keys_req_s {
//   char                              key_id[MAX_UUID_LEN];
//   ngx_http_qkd_api_dec_keys_req_t*  prev;
//   ngx_http_qkd_api_dec_keys_req_t*  next;
// };

ngx_int_t ngx_http_qkd_api_dec_keys_post(ngx_http_request_t* r, u_char* str, int* plen)
{
  cJSON *json, *json_arr, *json_arr_item, *json_key_id;
  int i = 0;
  ngx_str_t body;
  ngx_http_qkd_api_dec_keys_req_t *req = NULL, *key_id;

  body = ngx_http_qkd_api_post_get_request_body(r);
  dd("request body: len(%d)\n%.*s\n", (int)body.len, (int)body.len, body.data);

  if (body.len > 0)
  {
    json = cJSON_Parse((const char*)body.data);
    if (json != NULL)
    {
      json_arr = cJSON_GetObjectItem(json, "key_IDs");
      if (json_arr != NULL)
      {
        for (i = 0; i < cJSON_GetArraySize(json_arr); i++)
        {
          json_arr_item = cJSON_GetArrayItem(json_arr, i);
          if (json_arr_item != NULL)
          {
            json_key_id = cJSON_GetObjectItem(json_arr_item, "key_ID");
            if (json_key_id != NULL)
            {
              key_id = ngx_pcalloc(r->pool, sizeof(ngx_http_qkd_api_dec_keys_req_t));
              if (key_id == NULL)
                return NGX_ERROR;


              dd("key_id: %s\n", json_key_id->valuestring);
              strcpy(key_id->key_id, json_key_id->valuestring);

              if (req == NULL)
              {
                ngx_queue_init(key_id);
                req = key_id;
              }
              else
              {
                ngx_queue_insert_tail(req, key_id);
              }
            }
          }
        }
      }
    }

    ngx_pfree(r->pool, body.data);
  }

  return ngx_http_qkd_api_dec_keys(r, str, plen, req);
}

ngx_int_t ngx_http_qkd_api_dec_keys_get(ngx_http_request_t* r, u_char* str, int* len)
{
  ngx_http_qkd_api_dec_keys_req_t *req = NULL, *key_id;
  ngx_http_qkd_api_param_t *params = NULL, *tmp;
  ngx_http_qkd_api_get_params(r, &params);
  
  while (params)
  {
    dd("key: %.*s, value: %.*s\n", 
        (int)params->key.len, params->key.data,
        (int)params->value.len, params->value.data);

    if (ngx_strncmp("key_ID", params->key.data, params->key.len) == 0)
    {
      key_id = ngx_pcalloc(r->pool, sizeof(ngx_http_qkd_api_dec_keys_req_t));
      if (key_id == NULL)
        return NGX_ERROR;
      
      strncpy(key_id->key_id, (const char*)params->value.data, params->value.len);

      if (req == NULL)
      {
        ngx_queue_init(key_id);
        req = key_id;
      }
    }

    if (params->next == params)
      break;
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

  return ngx_http_qkd_api_dec_keys(r, str, len, req);
}

ngx_int_t ngx_http_qkd_api_dec_keys(ngx_http_request_t* r, u_char* str, int* len, ngx_http_qkd_api_dec_keys_req_t *req)
{
  int i, sample_keys_count = sizeof(sample_keys) / sizeof(ngx_http_qkd_api_key_t);
  ngx_http_qkd_api_key_t *key = NULL;
  ngx_http_qkd_api_dec_keys_req_t *tmp;

  cJSON *root, *array, *info;

  root = cJSON_CreateObject();
  cJSON_AddItemToObject(root, "keys", array = cJSON_CreateArray());

  while (req)
  {
    dd("req->key_id: %s\n", req->key_id);

    for (i = 0; i < sample_keys_count; i++)
    {
      if (strncmp(req->key_id, sample_keys[i].key_id, strlen(sample_keys[i].key_id)) == 0)
      {
        key = &sample_keys[i];
        break;
      }
    }

    if (key != NULL)
    {
      info = cJSON_CreateObject();
      cJSON_AddStringToObject(info, "key_ID", key->key_id);
      cJSON_AddStringToObject(info, "key", key->key);

      cJSON_AddItemToArray(array, info);
    }

    if (req->next == req)
      break;
    else
    {
      tmp = req;
      req = req->next;
      ngx_queue_remove(tmp);
      ngx_pfree(r->pool, tmp);
    }
  }
  if (req != NULL)
  {
    ngx_pfree(r->pool, req);
  }

  strcpy((char*)str, cJSON_Print(root));
  *len = strlen((const char*)str);

  cJSON_Delete(root);

  dd("dec_keys:\n%s\n", str);
   
  
  return NGX_OK;
}

