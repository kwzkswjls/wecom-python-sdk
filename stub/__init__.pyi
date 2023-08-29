class SDK:
    """
    errcode参考
        返回码	错误说明
        0       ok
        10000	参数错误，请求参数错误
        10001	网络错误，网络请求错误
        10002	数据解析失败
        10003	系统失败
        10004	密钥错误导致加密失败
        10005	fileid错误
        10006	解密失败
        10007   找不到消息加密版本的私钥，需要重新传入私钥对
        10008   解析encrypt_key出错
        10009   ip非法
        10010   数据过期
        10011	证书错误
    """

    def __init__(self, corp_id: str, secret: str, proxy: str = None, proxy_auth: str = None) -> None:
        """

        :param corp_id: 企业id，可以在企业微信管理端--我的企业--企业信息查看 e.g. wwd08c8exxxx5ab44d
        :param secret: 聊天内容存档的Secret，可以在企业微信管理端--管理工具--聊天内容存档查看
        :param proxy: SDK访问的域名是"https://qyapi.weixin.qq.com"，如网络环境无法直连该域名，可通过此参数配置代理 e.g. socks5://10.0.0.1:8081 或者 http://10.0.0.1:8081
        :param proxy_auth: 代理账号密码 e.g. user_name:passwd_123
        """

    def get_chat_data(self, seq: int, limit: int, timeout: int = 5) -> str:
        """
        :param seq: 企业存档消息序号，该序号单调递增，拉取序号建议设置为上次拉取返回结果中最大序号。首次拉取时seq传0，sdk会返回有效期内最早的消息
        :param limit: 本次拉取的最大消息条数，取值范围为1~1000
        :param timeout: 拉取会话存档的超时时间，单位为秒
        :return: 本次拉取消息的数据，JSON字符串，内容包括errcode/errmsg，以及每条消息内容
        e.g.
            {"errcode":0,"errmsg":"ok","chatdata":[{"seq":196,"msgid":"CAQQ2fbb4QUY0On2rYSAgAMgip/yzgs=","publickey_ver":3,"encrypt_random_key":"ftJ+uz3n/z1DsxlkwxNgE+mL38H42/KCvN8T60gbbtPD+Rta1hKTuQPzUzO6Hzne97MgKs7FfdDxDck/v8cDT6gUVjA2tZ/M7euSD0L66opJ/IUeBtpAtvgVSD5qhlaQjvfKJc/zPMGNK2xCLFYqwmQBZXbNT7uA69Fflm512nZKW/piK2RKdYJhRyvQnA1ISxK097sp9WlEgDg250fM5tgwMjujdzr7ehK6gtVBUFldNSJS7ndtIf6aSBfaLktZgwHZ57ONewWq8GJe7WwQf1hwcDbCh7YMG8nsweEwhDfUz+u8rz9an+0lgrYMZFRHnmzjgmLwrR7B/32Qxqd79A==","encrypt_chat_msg":"898WSfGMnIeytTsea7Rc0WsOocs0bIAerF6de0v2cFwqo9uOxrW9wYe5rCjCHHH5bDrNvLxBE/xOoFfcwOTYX0HQxTJaH0ES9OHDZ61p8gcbfGdJKnq2UU4tAEgGb8H+Q9n8syRXIjaI3KuVCqGIi4QGHFmxWenPFfjF/vRuPd0EpzUNwmqfUxLBWLpGhv+dLnqiEOBW41Zdc0OO0St6E+JeIeHlRZAR+E13Isv9eS09xNbF0qQXWIyNUi+ucLr5VuZnPGXBrSfvwX8f0QebTwpy1tT2zvQiMM2MBugKH6NuMzzuvEsXeD+6+3VRqL"}]}
            {"errcode":41001,"errmsg":"access_token missing, hint: [1684401769_399_465504c915f130b50294a4abe4eb82d0], from ip: 117.131.109.99, more info at https://open.work.weixin.qq.com/devtool/query?e=41001","chatdata":[]}
        """

    def get_media_file(self, file_id: str, save_to: str, timeout: int = 10) -> None:
        """
        :param file_id: 媒体文件id，从解密后的会话消息中得到
        :param save_to: 媒体文件保存路径
        :param timeout: 拉取媒体文件的超时时间，单位为秒
        :return: None
        """

    def decrypt_data(self, encrypt_random_key: str, encrypt_msg: str) -> str:
        """
        :param encrypt_random_key: 拉取会话存档返回的encrypt_random_key，使用配置在企业微信管理后台的rsa公钥对应的私钥解密后得到encrypt_key
        :param encrypt_msg: 拉取会话存档返回的encrypt_chat_msg
        :return: 解密的消息明文，JSON字符串，消息格式参考 https://developer.work.weixin.qq.com/document/path/91774
        """
