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

from cyctp.ctp_trader import CtpTrader, THOST_TE_RESUME_TYPE
from cyctp.ctp_handler import TdHandler
from cyctp.ctp_flag import *

class Trader(CtpTrader):
    def __new__(cls, *args, **kwargs):
        flowPath = kwargs['flowPath'] if kwargs and kwargs.has_key('flowPath') else (args[0] if len(args)>0 else 'flow'+os.path.sep+'td')
        if not int(os.path.exists(flowPath)):
            os.makedirs(flowPath)
        return CtpTrader.__new__(cls, *args, **kwargs)
    #frontPath="tcp://180.168.146.187:10000", broker="9999", user="083775", pwd="xxx"
    def __init__(self, flowPath='flow'+os.path.sep+'td', frontPaths=["tcp://180.168.146.187:10030"], broker="9999", user="083775", pwd="xxx", product="", authcode="", privateResume=THOST_TE_RESUME_TYPE.THOST_TERT_RESUME, publicResume=THOST_TE_RESUME_TYPE.THOST_TERT_RESUME, market=None): 
        self.handler = TdHandler(self)
        self.broker = broker if broker else ""
        self.user = user if user else ""
        self.pwd = pwd if pwd else ""
        self.product = product if product else ""
        self.authcode = authcode if authcode else ""
        self.market = market
        self.front_id = -1
        self.session_id = -1
        self.max_order_ref = 0
        self.max_other_ref = 0
        self.all_instruments = []        
        self.all_trade_codes = []
        self.all_bank_accounts = []
        logger.info("Connecting to trader front server: %s ..."%frontPaths)
        self.connect(frontPaths, privateResume, publicResume)
    def set_market(self, market):
        self.market = market
    def get_marekt(self):
        return self.market
    def inc_max_order_ref(self):
        self.max_order_ref += 1
        return '%012d'%self.max_order_ref
    def inc_max_other_ref(self):
        self.max_other_ref += 1
        return '%012d'%self.max_other_ref
