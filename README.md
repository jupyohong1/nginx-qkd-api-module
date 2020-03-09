# nginx-qkd-api-module
nginx qkd api module

## nginx config
``` shell
server {
  listen 80;
  server_name localhost;

  location / {
    rewrite ^/api/([a-zA-Z0-9_]+)/keys/([a-zA-Z0-9_]+)/([a-zA-Z0-9_]+) /api/$1/keys/$3?sae_id=$2 last;
    qkd_api;
  }
}
```
## test

1. Get status
- Request
``` shell
curl http://localhost:80/api/v1/keys/123/status
```
- Response
``` json
{
  "source_KME_ID": "KME-KT-RegionCode-SerialNumber1-Version",
  "target_KME_ID": "KME-KT-RegionCode-SerialNumber2-Version",
  "master_SAE_ID": "IIIIJJJJKKKKLLLL",
  "slave_SAE_ID": "WNSAE002",
  "key_size": 32,
  "stored_key_count": 25000,
  "max_key_count": 100000,
  "max_key_per_request": 128,
  "max_key_size": 32,
  "min_key_size": 32,
  "max_SAE_ID_count": 0
}
```

2. Get key
- Request
``` shell
curl http://localhost:80/api/v1/keys/123/enc_keys?number=1&size=256
```

- Response
``` json
{
	"keys":	[{
			"key_ID":	"29420130-0296-4370-bab4-d683a5752d86",
			"key":	"MWtUWHZBblhBTEdPWWl2ajRMN0J5UXJjUElRSUNPU2k="
		}]
}
```


