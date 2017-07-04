# -*- coding: utf-8 -*-

import os, sys, re, time, itertools
import logging, logging.config
import requests
from collections import OrderedDict

logger = logging.getLogger()#__name__)
reload(sys)
sys.setdefaultencoding('utf-8')

PREFIX_URL = 'http://qt.gtimg.cn/q='
MARKET_PATTERN = re.compile(r'(.*)=\"(.*)\";')

class Stocker:
    def __init__(self, codes=None): 
        self.codes = codes
    def market(self, interval=500):        
        if self.codes:
            market_url = PREFIX_URL + ','.join(self.codes)
            try:
                r = requests.get(market_url, timeout=1)
            except requests.exceptions.ConnectTimeout:
                r = None
                print('ConnectTimeout')
            except requests.exceptions.Timeout:
                r = None
                print('Timeout')
            except requests.exceptions.ConnectionError:
                r = None
                print('ConnectionError')
            if r and r.status_code == 200:
                ms = MARKET_PATTERN.finditer(r.text)
                datas = []
                for m in ms: datas.append(self._m2d(m.group(2).split('~')))
                return datas
        return []
    def _m2d(self, ml):
        md = OrderedDict()
        md['name'] = ml[1]
        md['code'] = ml[2]
        md['price'] = ml[3]
        md['last'] = ml[4]
        md['open'] = ml[5]
        md['amount'] = ml[6]
        #md[''] = ml[7]
        #md[''] = ml[8]
        md['buy1p'] = ml[9]
        md['buy1a'] = ml[10]
        md['sell1p'] = ml[19]
        md['sell1a'] = ml[20]
        md['diff'] = ml[31]
        md['diff%'] = ml[32]
        md['high'] = ml[33]
        md['low'] = ml[34]
        md['hstop'] = ml[47]
        md['lstop'] = ml[48]
        return md

def spin():
    spinner = itertools.cycle(['-', '/', '|', '\\'])
    count = 12
    while count>0:
        sys.stdout.write(spinner.next())
        sys.stdout.flush()
        sys.stdout.write('\b')
        time.sleep(0.1)
        count=count-1

if __name__ == '__main__':    
    #logging.config.fileConfig(os.path.dirname(os.path.abspath(__file__))+os.path.sep+"logger.conf")
    from colorama import init, Fore, Back, Style
    init(autoreset=True)
    stocker = Stocker(["sz002467", "sh600830", "sz002432", "sz002261"])
    while True:
        datas = stocker.market()
        for data in datas:  
            notice_color = Back.RED if float(data['price'])>=float(data['high']) else Back.GREEN if float(data['price'])<=float(data['low']) else ''
            updown_color = Back.RED if float(data['diff'])>0 else Back.GREEN if float(data['diff'])<0 else ''
            price_color = Fore.YELLOW if float(data['price']) <= float(data['open'])*0.95 else Fore.RED if float(data['price']) >= float(data['open'])*1.05 else ''
            print(notice_color + u'%s[%s-%s]\t'%(data['name'][0], data['lstop'], data['hstop'])),
            print(updown_color + price_color + Style.BRIGHT + u'%s'%data['price']),
            print(notice_color + u'[%s-%s]\t'%(data['low'], data['high'])),
        print ''
        spin()

