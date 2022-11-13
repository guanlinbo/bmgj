import base64
import datetime
import json
import sys
import time
from datetime import datetime
from pyzbar.pyzbar import decode
from PIL import Image
import qrcode
import requests


讲座eid = []
讲座map = {}
# 要是你知道eid和你的tocken，本程序自动开始抢
# eid = ''
# tocken = ''
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
    with open('./b.png', 'wb') as f:
        f.write(page_content)
    barcode_url = ''
    barcodes = decode(Image.open('./b.png'))
    for barcode in barcodes:
        barcode_url = barcode.data.decode("utf-8")
    qr = qrcode.QRCode()
    qr.add_data(barcode_url)
    # invert=True白底黑块,有些app不识别黑底白块.
    qr.print_ascii(invert=True)
    return code

def 获取token():
    print("微信扫码二维码！！！！")
    code = 获取二维码()
    if code == '':
        return ''
    url = 'https://api-xcx-qunsou.weiyoubot.cn/xcx/enroll_web/v1/pc_login?code=%s'% code
    while(True):
        ret = requests.get(url, headers=headers).json()
        if ret['msg'] != 'please wait':
            return ret['data']['access_token']
        time.sleep(1)
        print(ret['msg'])
    # return ret['data']['access_token']


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

def 获取讲座开始时间(token, eid):
    print(token,eid)
    url = 'https://api-xcx-qunsou.weiyoubot.cn/xcx/enroll/v1/detail?access_token=%s&eid=%s' % (token, eid)
    ret = requests.get(url, headers=headers).json()
    if ret["msg"] != "ok":
        return
    # print("您的选择是：" + 讲座map[eid] +" 报名时间：" + datetime.fromtimestamp(ret["data"]["start_time"]).strftime("%Y-%m-%d %H:%M:%S") + "\n" )
    print("请检查代码所配置信息是否跟如下信息一样,如不是请自行修改(不一致会导致失败)!!!!!!!!!!!!!!!!")
    # print(ret["data"]["req_info"])
    index = 1
    # 直接一个函数
    for i in ret["data"]["req_info"]:
        # if i['from']:
        print("\n类型:%s"%(i['type_text']))
        print("要求", index, " field_name:", i['field_name'], " field_key:", i['field_key'])
        if 'field_desc' in i.keys():
            print("描述:",i['field_desc'])
        if 'from' not in i.keys():
            pass
        elif i['type_text'] == "下拉框":
            print("min_check:",i['min_check']," max_check:",i['min_check'])
            print("options:\n",i['options'])
        elif i['type_text'] == "单项选择":
            print(i['new_options'])
        elif i['type_text'] == "日期":
            print("开始选择日期:",i['startDate'])
            print("结束选择日期:",i['endDate'])
        index+=1
    return ret["data"]["start_time"]


def clicked_button(token,index):
    data = {
        "access_token": token,
        "eid": 讲座eid[index],
        "info":
            [
                {  # ---------------从这里配置报名信息-----------------
                    "field_name": "姓名",
                    "field_value": "你的姓名",
                    "field_key": 1
                },
                {
                    "field_name": "手机号",
                    "field_value": "你的手机号",
                    "field_key": 6
                },
                {
                    "field_name": "时间",
                    "field_value": "12:12",
                    "field_key": "a0e39bfc-268a-0fba-f12b-1035022fd6f8"
                },
                {
                    "field_name": "下拉框",
                    "field_value": "1",
                    "field_key": "6123c514-af7d-1466-d47c-1c68aeb75ce8"
                },
                {
                    "field_name": "单项选择",
                    "field_value": "33",
                    "field_key": "1d485c5c-609a-1669-5520-58c86cdc0373"
                },
                {
                    "field_name": "日期选择",
                    "field_value": "2022-11-16",
                    "field_key": "ec071083-1c15-558a-04cc-34550797d284"
                },
                {
                    "field_name": "地理位置",
                    "field_value": "广东省深圳市南山区沙河西路",
                    "field_key": "da43e493-a6d0-9642-1fd6-83605aa3aba5"
                },
                {
                    "field_name": "多行文本",
                    "field_value": "多行文本多行文本多行文本多行文本多行文本",
                    "field_key": "97e26dd5-4a2e-8b1a-43fd-c0c995d06f33"
                },
            ],
        "on_behalf": 0,
        "items": [],
        "referer": "",
        "fee_type": ""
    }
    url = "http://api-xcx-qunsou.weiyoubot.cn/xcx/enroll/v5/enroll"
    start = 获取讲座开始时间(token, 讲座eid[index])
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

获取所有讲座(token)
index = int(input("请选择(填编号):"))
clicked_button(token,index - 1)
