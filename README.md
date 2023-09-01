# 企业微信-会话内容存档 PythonSDK

获取企业微信会话存档仅能通过SDK调用，无WebAPI提供，本项目为C-SDK的Python封装，支持Linux/Windows

官方文档详见 https://developer.work.weixin.qq.com/document/path/91774


## Example Usage

### 安装
```shell
pip install .
```

### 初始化SDK
`CORP_ID`/`SECRET`从企业微信管理后台获取
```python
import wecom

sdk = wecom.SDK("CORP_ID", "SECRET")

# use proxy
sdk = wecom.SDK("CORP_ID", "SECRET", proxy="http://10.0.0.1:8081", proxy_auth="username:password")
```

### 获取聊天对话数据
```python
data = sdk.get_chat_data(seq=0, limit=1) # JSON format
data = json.loads(data) 

# sample data:
# {
#   "errcode": 0,
#   "errmsg": "ok",
#   "chatdata": [
#      {
#         "seq": 0,
#         "msgid": "CAQ...gs=,
#         "publickey_ver": 3,
#         "encrypt_random_key": "ftJ...A==",
#         "encrypt_chat_msg": "898...RqL"
#      }
#   ]
# }
```

### 解密聊天消息
使用前需要生成RSA密钥对，并在企业微信管理后台配置公钥

sdk获取的聊天消息被随机密钥加密，随机密钥被配置的公钥加密

解密时需要先通过RSA私钥解密随机密钥，再调用sdk使用随机密钥解密聊天消息

```python
from base64 import b64decode
from Crypto.PublicKey import RSA  # pycryptodome
from Crypto.Cipher import PKCS1_v1_5

# load RSA private key
rsa_key = RSA.import_key(open("private.key", "r").read())
cipher = PKCS1_v1_5.new(rsa_key)

chat_data = data["chatdata"][0]

# decrypt random key
random_key = chat_data["encrypt_random_key"]
random_key = b64decode(random_key)
random_key = cipher.decrypt(random_key, b'') 
random_key = str(random_key, encoding="utf-8")

encrypt_chat_msg = chat_data["encrypt_chat_msg"]

# decrypt chat message
chat_msg = sdk.decrypt_data(encrypt_random_key=random_key, encrypt_msg=encrypt_chat_msg) # JSON format
chat_msg = json.loads(chat_msg)

# Sample data:
# {
#   "msgid": "CAQ...AE=",
#   "action": "send",
#   "from": "XuJinSheng",
#   "tolist": ["icefog"],
#   "roomid": "",
#   "msgtime": 1547087894783,
#   "msgtype": "text",
#   "text": {"content": "test"}
# }
```


### 获取媒体文件

```python
sdk_file_id = chat_msg["file"]["sdkfileid"] # 
sdk.get_media_file(file_id=sdk_file_id, save_to="/path/to/file")
```

### 手动释放SDK占用内存

```python
del sdk
```
