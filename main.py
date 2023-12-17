import base64
import datetime
import json
import sys
import time
from datetime import datetime
import cv2 as cv
import requests


讲座eid = []
讲座map = {}

headers = {
    "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:105.0) Gecko/20100101 Firefox/105.0",
    "Accept": "*/*",
    "Accept-Language": "zh-CN,zh;q=0.8,zh-TW;q=0.7,zh-HK;q=0.5,en-US;q=0.3,en;q=0.2",
    "Origin": "http://baominggongju.com",
    "Connection": "keep-alive",
    "Referer": "http://baominggongju.com/",
    "Sec-Fetch-Dest": "empty",
    "Sec-Fetch-Mode": "cors",
    "Sec-Fetch-Site": "cross-site",
}


def 获取二维码():
    url = 'https://api-xcx-qunsou.weiyoubot.cn/xcx/enroll_web/v1/pc_code'
    ret = requests.get(url,headers=headers).json()
    if ret['msg'] != 'ok':
        return ''
    img_imf = ret['data']['qrcode'].replace('data:image/jpg;base64,', '')
    code = ret['data']['code']
    page_content = base64.b64decode(img_imf)
    with open('./b.jpg', 'wb') as f:
        f.write(page_content)
    src = cv.imread('./b.jpg')
    cv.imshow('微信扫码'.encode("gbk").decode(errors="ignore"), src)
    cv.waitKey(0)
    return code

def 获取token():
    code = 获取二维码()
    if code == '':
        return ''
    url = 'https://api-xcx-qunsou.weiyoubot.cn/xcx/enroll_web/v1/pc_login?code=%s'% code
    ret = requests.get(url, headers=headers).json()
    # return ret['data']['access_token']
    if ret['msg'] == 'ok':
        return ret['data']['access_token']
    else:
        return ''

def 获取常用信息(token):
    url = 'https://api-xcx-qunsou.weiyoubot.cn/xcx/enroll/v1/userinfo?access_token=%s'%token
    ret = requests.get(url, headers=headers).json()
    if ret["msg"] != "ok":
        return
    for i in ret["data"]["extra_info"]:
        pass

def 获取所有讲座(token):
    url = 'https://api-xcx-qunsou.weiyoubot.cn/xcx/enroll/v1/user/history?access_token=%s'%token
    ret = requests.get(url, headers=headers).json()
    # print(ret)
    # status:1 进行中 2结束
    index = 1
    for i in ret['data']:
        if i['status'] == 0 or i['status'] == 1:
            #已满
            if i['count'] >= i['limit']:
                continue
            print("编号：%d 讲座：%s"%(index,i["title"]))
            index+=1
            讲座eid.append(i['eid'])
            讲座map[i['eid']] = i["title"]

def 获取讲座开始时间(token, eid,unid):
    url = 'https://api-xcx-qunsou.weiyoubot.cn/xcx/enroll/v3/detail?eid=%s&access_token=%s&referer=%s&spider=h5' % (eid,token,unid)
    ret = requests.get(url, headers=headers).json()
    # print(ret)
    if ret["msg"] != "ok":
        return
    print("您的选择是：" + 讲座map[eid] +" 报名时间：" + datetime.fromtimestamp(ret["data"]["start_time"]).strftime("%Y-%m-%d %H:%M:%S") + "\n" )
    print("请检查代码所配置信息是否跟如下信息一样,如不是请自行修改(不一致会导致失败)!!!!!!!!!!!!!!!!")
    index = 1
    # 直接一个函数
    for i in ret["data"]["req_info"]:
        # if i['from']:
        print("\n类型:%s" % (i['type_text']))
        print("要求", index, " field_name:", i['field_name'], " field_key:", i['field_key'])
        if 'field_desc' in i.keys():
            print("描述:", i['field_desc'])
        if 'from' not in i.keys():
            pass
        elif i['type_text'] == "下拉框":
            print("min_check:", i['min_check'], " max_check:", i['min_check'])
            print("options:\n", i['options'])
        elif i['type_text'] == "单项选择":
            print(i['new_options'])
        elif i['type_text'] == "日期":
            print("开始选择日期:", i['startDate'])
            print("结束选择日期:", i['endDate'])
        index += 1
    return ret["data"]["start_time"]


def 获取unid(access_token):
    url = 'https://api-xcx-qunsou.weiyoubot.cn/xcx/enroll/v1/userinfo?access_token=%s' % access_token
    ret = requests.get(url, headers=headers).json()
    return ret['data']['unionid']

def 开始抢(token,unid,index):
    data = {
        "access_token": token,
        "eid": 讲座eid[index],
        "info":
            [
                {    #---------------从这里配置报名信息-----------------
                    "field_name": "姓名",
                    "field_value": "你的姓名",
                    "field_key": 1
                },
                {
                    "field_name": "年级",
                    "field_value": "小学6年级",
                    "field_key": 11
                },
                {
                    "field_name": "1+1等于几？",
                    "field_value": "2",
                    "field_key": "de1717d9-10bc-24b0-deda-a3b30c4627a7"
                }
            ],
        "on_behalf": 0,
        "items": [],
        "referer": "",
        "fee_type": ""
    }
    url = "http://api-xcx-qunsou.weiyoubot.cn/xcx/enroll/v5/enroll"
    start = 获取讲座开始时间(token, 讲座eid[index],unid)
    input("\n讲座要求是否已经配置要求，如是请回车!")
    while 1:
        t = time.time()
        # 多次测试提前2秒开始抢没问题，当然你觉得不行，也可以提前1秒开始
        if t >= start-2:
            while(1):
                ret = requests.post(url, headers=headers, data=json.dumps(data), verify=False, timeout=2).json()
                if ret['msg'] == '':
                    print("成功了兄弟!!!!!!!")
                    return
                elif ret['msg'] == '报名未开始':
                    time.sleep(0.3)
                    continue
                elif ret['msg'] == '请求过于频繁，请稍后重试':
                    time.sleep(0.7)
                    continue
                else:
                    print(ret)
                    return
            return
        time.sleep(1)
        print("当前时间：" + datetime.now().strftime("%Y-%m-%d %H:%M:%S"))

token = 获取token()
if token != '':
        print("登录成功!")
else:
        print("未扫码，程序退出......")
        exit(1)
unid = 获取unid(token)
获取所有讲座(token)
index = int(input("请选择(填编号):"))
开始抢(token,unid,index - 1)
