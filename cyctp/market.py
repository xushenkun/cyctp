# -*- coding: utf-8 -*-

import os, sys
import logging, logging.config
#import ctp library
PKG_PATH = os.path.dirname(os.path.abspath(__file__))
LIB_PATH = os.path.join(PKG_PATH, 'lib') #find ctp dll
if sys.platform == 'win32': os.environ['PATH'] = LIB_PATH + (os.pathsep + os.environ['PATH'] if os.environ['PATH'] else "")
elif sys.platform == 'darwin': os.environ['DYLD_LIBRARY_PATH'] = LIB_PATH + (os.pathsep + os.environ['DYLD_LIBRARY_PATH'] if os.environ.has_key('DYLD_LIBRARY_PATH') else "")
else: 
    os.environ['LD_LIBRARY_PATH'] = LIB_PATH + (os.pathsep + os.environ['LD_LIBRARY_PATH'] if os.environ.has_key('LD_LIBRARY_PATH') else "")  #no effect, should: export LD_LIBRARY_PATH=./lib
    #os.execve(os.path.realpath(__file__), sys.argv, os.environ)

logger = logging.getLogger()#__name__)

from cyctp.ctp_market import CtpMarket
from cyctp.ctp_handler import MdHandler
from cyctp.ctp_flag import *

class Market(CtpMarket):
    def __new__(cls, *args, **kwargs):
        flowPath = kwargs['flowPath'] if kwargs and kwargs.has_key('flowPath') else (args[0] if len(args)>0 else 'flow'+os.path.sep+'md')
        if not int(os.path.exists(flowPath)):
            os.makedirs(flowPath)
        return CtpMarket.__new__(cls, *args, **kwargs)
    #frontPath="tcp://180.168.146.187:10010", broker="9999", user="083775", pwd="xxx"
    def __init__(self, flowPath='flow'+os.path.sep+'md', frontPaths=["tcp://180.168.146.187:10031"], broker="9999", user="083775", pwd="xxx"):  
        self.handler = MdHandler(self)
        self.broker = broker if broker else ""
        self.user = user if user else ""
        self.pwd = pwd if pwd else ""
        logger.info("Connecting to market front server: %s ..."%frontPaths)
        self.connect(frontPaths)

    @staticmethod
    def version():
        return ctp_market.CMarketCollector.Version()
    def connect(self, frontPaths):
        return super(Market, self).connect(frontPaths)
    def login(self):
        return super(Market, self).login()
    def logout(self):
        return super(Market, self).logout()
    def get_trading_day(self):
        return super(Market, self).get_trading_day()
    def sub_market(self, instrument_ids):           
        ids = to_cstring_array(instrument_ids)
        ret = self.collector.SubMarket(ids, len(instrument_ids))
        free(ids)
        return ret
    def unsub_market(self, instrument_ids):
        ids = to_cstring_array(instrument_ids)
        ret = self.collector.UnSubMarket(ids, len(instrument_ids))
        free(ids)
        return ret
    def sub_ask_quote(self, instrument_ids):
        ids = to_cstring_array(instrument_ids)
        ret = self.collector.SubForQuote(ids, len(instrument_ids))
        free(ids)
        return ret
    def unsub_ask_quote(self, instrument_ids):
        ids = to_cstring_array(instrument_ids)
        ret = self.collector.UnSubForQuote(ids, len(instrument_ids))
        free(ids)
        return ret
    def get_handler(self):
        return self.handler
    #def join(self):        
    #    self._mdApi.Join() 
    def close(self):
        if self.collector: return self.collector.Close()