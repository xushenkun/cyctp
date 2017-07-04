#distutils: language = c++

from libc.stdlib cimport malloc, free
from cpython.string cimport PyString_AsString
from libc.string cimport strdup
import logging, logging.config
from multiprocessing.pool import ThreadPool
from threading import current_thread

cimport ctp_market
cimport ctp_trader

from ctp_market import *

print 'MD_CALLBACK_TYPE', MD_RSP_USER_LOGIN

logger = logging.getLogger()#__name__)

cb_pool = ThreadPool(processes=2)

cdef int md_callback(void *context, ctp_market.MD_CALLBACK_TYPE t, void* data, int rid, int last):# with gil:
    if t == -1:#ctp_market.MD_RTN_DEPTH_MARKET_DATA:
         (<object>context).get_handler().handle(t, <object>data, rid, last)
    else:
        async_result = cb_pool.apply_async(async_callback, args=(<object>context, t, <object>data, rid, last))
        async_result.get()   
        #logger.info('md callback: %s'%t)
    return 0

cdef int td_callback(void *context, ctp_trader.TD_CALLBACK_TYPE t, void* data, int rid, int last) with gil:
    async_result = cb_pool.apply_async(async_callback, args=(<object>context, t, <object>data, rid, last)) 
    async_result.get()
    return 0

def async_callback(context, t, data, rid, last):
    #print '', current_thread().ident, t, 'pycallback'
    context.get_handler().handle(t, data, rid, last)
    return 0

cdef bytes as_str(data):
    if isinstance(data, bytes):
        return data
    elif isinstance(data, unicode):
        return data.encode('utf8')
    raise TypeError('Cannot convert %s to string' % type(data))

cdef char ** to_cstring_array(datas):
    cdef char **ret = <char **>malloc(len(datas)*sizeof(char *))
    for i in xrange(len(datas)):
        ret[i] = strdup(datas[i])#PyString_AsString(datas[i])
    return ret

#because of cpdef enum bugs in cython
class MarketCallbackType:
    MD_FRONT_CONNECTED = ctp_market.MD_FRONT_CONNECTED
    MD_FRONT_DISCONNECTED = ctp_market.MD_FRONT_DISCONNECTED
    MD_HEART_BEAT_WARN = ctp_market.MD_HEART_BEAT_WARN
    MD_RSP_USER_LOGIN = ctp_market.MD_RSP_USER_LOGIN
    MD_RSP_USER_LOGOUT = ctp_market.MD_RSP_USER_LOGOUT
    MD_RSP_ERROR = ctp_market.MD_RSP_ERROR
    MD_RSP_SUB_MARKET_DATA = ctp_market.MD_RSP_SUB_MARKET_DATA
    MD_RSP_UNSUB_MARKET_DATA = ctp_market.MD_RSP_UNSUB_MARKET_DATA
    MD_RSP_SUB_QUOTE = ctp_market.MD_RSP_SUB_QUOTE
    MD_RSP_UNSUB_QUOTE = ctp_market.MD_RSP_UNSUB_QUOTE
    MD_RTN_DEPTH_MARKET_DATA = ctp_market.MD_RTN_DEPTH_MARKET_DATA
    MD_RTN_QUOTE = ctp_market.MD_RTN_QUOTE

#because of cpdef enum bugs in cython
class TraderCallbackType:
    TD_FRONT_CONNECTED = ctp_trader.TD_FRONT_CONNECTED
    TD_FRONT_DISCONNECTED = ctp_trader.TD_FRONT_DISCONNECTED
    TD_HEART_BEAT_WARN = ctp_trader.TD_HEART_BEAT_WARN
    TD_RSP_AUTH = ctp_trader.TD_RSP_AUTH
    TD_RSP_USER_LOGIN = ctp_trader.TD_RSP_USER_LOGIN
    TD_RSP_USER_LOGOUT = ctp_trader.TD_RSP_USER_LOGOUT
    TD_RSP_PWD_UPDATE = ctp_trader.TD_RSP_PWD_UPDATE
    TD_RSP_TRADING_PWD_UPDATE = ctp_trader.TD_RSP_TRADING_PWD_UPDATE
    TD_RSP_ORDER_INSERT = ctp_trader.TD_RSP_ORDER_INSERT
    TD_RSP_PACK_ORDER_INSERT = ctp_trader.TD_RSP_PACK_ORDER_INSERT
    TD_RSP_PACK_ORDER_ACTION = ctp_trader.TD_RSP_PACK_ORDER_ACTION
    TD_RSP_ORDER_ACTION = ctp_trader.TD_RSP_ORDER_ACTION
    TD_RSP_QRY_MAX_ORDER_VOLUME = ctp_trader.TD_RSP_QRY_MAX_ORDER_VOLUME
    TD_RSP_SETTLE_INFO_CONFIRM = ctp_trader.TD_RSP_SETTLE_INFO_CONFIRM
    TD_RSP_REMOVE_PACK_ORDER = ctp_trader.TD_RSP_REMOVE_PACK_ORDER
    TD_RSP_REMOVE_PACK_ORDER_ACTION = ctp_trader.TD_RSP_REMOVE_PACK_ORDER_ACTION
    TD_RSP_EXEC_ORDER_INSERT = ctp_trader.TD_RSP_EXEC_ORDER_INSERT
    TD_RSP_EXEC_ORDER_ACTION = ctp_trader.TD_RSP_EXEC_ORDER_ACTION
    TD_RSP_FOR_QUOTE_INSERT = ctp_trader.TD_RSP_FOR_QUOTE_INSERT
    TD_RSP_QUOTE_INSERT = ctp_trader.TD_RSP_QUOTE_INSERT
    TD_RSP_QUOTE_ACTION = ctp_trader.TD_RSP_QUOTE_ACTION
    TD_RSP_BATCH_ORDER_ACTION = ctp_trader.TD_RSP_BATCH_ORDER_ACTION
    TD_RSP_COMB_ACTION_INSERT = ctp_trader.TD_RSP_COMB_ACTION_INSERT
    TD_RSP_QRY_ORDER = ctp_trader.TD_RSP_QRY_ORDER
    TD_RSP_QRY_TRADE = ctp_trader.TD_RSP_QRY_TRADE
    TD_RSP_QRY_INVESTOR_POS = ctp_trader.TD_RSP_QRY_INVESTOR_POS
    TD_RSP_QRY_TRADING_ACCOUNT = ctp_trader.TD_RSP_QRY_TRADING_ACCOUNT
    TD_RSP_QRY_INVESTOR = ctp_trader.TD_RSP_QRY_INVESTOR
    TD_RSP_QRY_TRADING_CODE = ctp_trader.TD_RSP_QRY_TRADING_CODE
    TD_RSP_QRY_INSTRUMENT_MARGIN_RATE = ctp_trader.TD_RSP_QRY_INSTRUMENT_MARGIN_RATE
    TD_RSP_QRY_INSTRUMENT_COMMISSION_RATE = ctp_trader.TD_RSP_QRY_INSTRUMENT_COMMISSION_RATE
    TD_RSP_QRY_EXCHANGE = ctp_trader.TD_RSP_QRY_EXCHANGE
    TD_RSP_QRY_PRODUCT = ctp_trader.TD_RSP_QRY_PRODUCT
    TD_RSP_QRY_INSTRUMENT = ctp_trader.TD_RSP_QRY_INSTRUMENT
    TD_RSP_QRY_DEPTH_MARKET_DATA = ctp_trader.TD_RSP_QRY_DEPTH_MARKET_DATA
    TD_RSP_QRY_SETTLE_INFO = ctp_trader.TD_RSP_QRY_SETTLE_INFO
    TD_RSP_QRY_TRANSFER_BANK = ctp_trader.TD_RSP_QRY_TRANSFER_BANK
    TD_RSP_QRY_INVESTOR_POS_DETAIL = ctp_trader.TD_RSP_QRY_INVESTOR_POS_DETAIL
    TD_RSP_QRY_NOTICE = ctp_trader.TD_RSP_QRY_NOTICE
    TD_RSP_QRY_SETTLE_INFO_CONFIRM = ctp_trader.TD_RSP_QRY_SETTLE_INFO_CONFIRM
    TD_RSP_QRY_INVESTOR_POS_COMB_DETAIL = ctp_trader.TD_RSP_QRY_INVESTOR_POS_COMB_DETAIL
    TD_RSP_QRY_CFMMC_TRADING_ACCOUNT_KEY = ctp_trader.TD_RSP_QRY_CFMMC_TRADING_ACCOUNT_KEY
    TD_RSP_QRY_EWARRANT_OFFSET = ctp_trader.TD_RSP_QRY_EWARRANT_OFFSET
    TD_RSP_QRY_INVESTOR_PRODUCT_GROUP_MARGIN = ctp_trader.TD_RSP_QRY_INVESTOR_PRODUCT_GROUP_MARGIN
    TD_RSP_QRY_EXCHANGE_MARGIN_RATE = ctp_trader.TD_RSP_QRY_EXCHANGE_MARGIN_RATE
    TD_RSP_QRY_EXCHANGE_MARGIN_RATE_ADJUST = ctp_trader.TD_RSP_QRY_EXCHANGE_MARGIN_RATE_ADJUST
    TD_RSP_QRY_EXCHANGE_RATE = ctp_trader.TD_RSP_QRY_EXCHANGE_RATE
    TD_RSP_QRY_SEC_AGENT_ACID_MAP = ctp_trader.TD_RSP_QRY_SEC_AGENT_ACID_MAP
    TD_RSP_QRY_PRODUCT_EXCH_RATE = ctp_trader.TD_RSP_QRY_PRODUCT_EXCH_RATE
    TD_RSP_QRY_PRODUCT_GROUP = ctp_trader.TD_RSP_QRY_PRODUCT_GROUP
    TD_RSP_QRY_MM_INSTRUMENT_COMMISSION_RATE = ctp_trader.TD_RSP_QRY_MM_INSTRUMENT_COMMISSION_RATE
    TD_RSP_QRY_MM_OPTION_INSTR_COMM_RATE = ctp_trader.TD_RSP_QRY_MM_OPTION_INSTR_COMM_RATE
    TD_RSP_QRY_INSTRUMENT_ORDER_COMM_RATE = ctp_trader.TD_RSP_QRY_INSTRUMENT_ORDER_COMM_RATE
    TD_RSP_QRY_OPTION_INSTR_TRADE_COST = ctp_trader.TD_RSP_QRY_OPTION_INSTR_TRADE_COST
    TD_RSP_QRY_OPTION_INSTR_COMM_RATE = ctp_trader.TD_RSP_QRY_OPTION_INSTR_COMM_RATE
    TD_RSP_QRY_EXEC_ORDER = ctp_trader.TD_RSP_QRY_EXEC_ORDER
    TD_RSP_QRY_FOR_QUOTE = ctp_trader.TD_RSP_QRY_FOR_QUOTE
    TD_RSP_QRY_QUOTE = ctp_trader.TD_RSP_QRY_QUOTE
    TD_RSP_QRY_COMB_INSTRUMENT_GUARD = ctp_trader.TD_RSP_QRY_COMB_INSTRUMENT_GUARD
    TD_RSP_QRY_COMB_ACTION = ctp_trader.TD_RSP_QRY_COMB_ACTION
    TD_RSP_QRY_TRANSFER_SERIAL = ctp_trader.TD_RSP_QRY_TRANSFER_SERIAL
    TD_RSP_QRY_ACCOUNT_REGISTER = ctp_trader.TD_RSP_QRY_ACCOUNT_REGISTER
    TD_RSP_ERROR = ctp_trader.TD_RSP_ERROR
    TD_RTN_ORDER = ctp_trader.TD_RTN_ORDER
    TD_RTN_TRADE = ctp_trader.TD_RTN_TRADE
    TD_ERR_RTN_ORDER_INSERT = ctp_trader.TD_ERR_RTN_ORDER_INSERT
    TD_ERR_RTN_ORDER_ACTION = ctp_trader.TD_ERR_RTN_ORDER_ACTION
    TD_RTN_INSTRUMENT_STATUS = ctp_trader.TD_RTN_INSTRUMENT_STATUS
    TD_RTN_BULLETIN = ctp_trader.TD_RTN_BULLETIN
    TD_RTN_TRADING_NOTICE = ctp_trader.TD_RTN_TRADING_NOTICE
    TD_RTN_ERROR_CONDITIONAL_ORDER = ctp_trader.TD_RTN_ERROR_CONDITIONAL_ORDER
    TD_RTN_EXEC_ORDER = ctp_trader.TD_RTN_EXEC_ORDER
    TD_ERR_RTN_EXEC_ORDER_INSERT = ctp_trader.TD_ERR_RTN_EXEC_ORDER_INSERT
    TD_ERR_RTN_EXEC_ORDER_ACTION = ctp_trader.TD_ERR_RTN_EXEC_ORDER_ACTION
    TD_ERR_RTN_FOR_QUOTE_INSERT = ctp_trader.TD_ERR_RTN_FOR_QUOTE_INSERT
    TD_RTN_QUOTE = ctp_trader.TD_RTN_QUOTE
    TD_ERR_RTN_QUOTE_INSERT = ctp_trader.TD_ERR_RTN_QUOTE_INSERT
    TD_ERR_RTN_QUOTE_ACTION = ctp_trader.TD_ERR_RTN_QUOTE_ACTION
    TD_RTN_FOR_QUOTE_RSP = ctp_trader.TD_RTN_FOR_QUOTE_RSP
    TD_RTN_CFMMC_TRADING_ACCOUNT_TOKEN = ctp_trader.TD_RTN_CFMMC_TRADING_ACCOUNT_TOKEN
    TD_ERR_RTN_BATCH_ORDER_ACTION = ctp_trader.TD_ERR_RTN_BATCH_ORDER_ACTION
    TD_RTN_COMB_ACTION = ctp_trader.TD_RTN_COMB_ACTION
    TD_ERR_RTN_COMB_ACTION_INSERT = ctp_trader.TD_ERR_RTN_COMB_ACTION_INSERT
    TD_RSP_QRY_CONTRACT_BANK = ctp_trader.TD_RSP_QRY_CONTRACT_BANK
    TD_RSP_QRY_PACK_ORDER = ctp_trader.TD_RSP_QRY_PACK_ORDER
    TD_RSP_QRY_PACK_ORDER_ACTION = ctp_trader.TD_RSP_QRY_PACK_ORDER_ACTION
    TD_RSP_QRY_TRADING_NOTICE = ctp_trader.TD_RSP_QRY_TRADING_NOTICE
    TD_RSP_QRY_BROKER_TRADING_PARAMS = ctp_trader.TD_RSP_QRY_BROKER_TRADING_PARAMS
    TD_RSP_QRY_BROKER_TRADING_ALGOS = ctp_trader.TD_RSP_QRY_BROKER_TRADING_ALGOS
    TD_RSP_QRY_CFMMC_TRADING_ACCOUNT_TOKEN = ctp_trader.TD_RSP_QRY_CFMMC_TRADING_ACCOUNT_TOKEN
    TD_RTN_FROM_BANK_TO_FUTURE_BY_BANK = ctp_trader.TD_RTN_FROM_BANK_TO_FUTURE_BY_BANK
    TD_RTN_FROM_FUTURE_TO_BANK_BY_BANK = ctp_trader.TD_RTN_FROM_FUTURE_TO_BANK_BY_BANK
    TD_RTN_REPEAL_FROM_BANK_TO_FUTURE_BY_BANK = ctp_trader.TD_RTN_REPEAL_FROM_BANK_TO_FUTURE_BY_BANK
    TD_RTN_REPEAL_FROM_FUTURE_TO_BANK_BY_BANK = ctp_trader.TD_RTN_REPEAL_FROM_FUTURE_TO_BANK_BY_BANK
    TD_RTN_FROM_BANK_TO_FUTURE_BY_FUTURE = ctp_trader.TD_RTN_FROM_BANK_TO_FUTURE_BY_FUTURE
    TD_RTN_FROM_FUTURE_TO_BANK_BY_FUTURE = ctp_trader.TD_RTN_FROM_FUTURE_TO_BANK_BY_FUTURE
    TD_RTN_REPEAL_FROM_BANK_TO_FUTURE_BY_FUTURE_MANUAL = ctp_trader.TD_RTN_REPEAL_FROM_BANK_TO_FUTURE_BY_FUTURE_MANUAL
    TD_RTN_REPEAL_FROM_FUTURE_TO_BANK_BY_FUTURE_MANUAL = ctp_trader.TD_RTN_REPEAL_FROM_FUTURE_TO_BANK_BY_FUTURE_MANUAL
    TD_RTN_QUERY_BANK_BALANCE_BY_FUTURE = ctp_trader.TD_RTN_QUERY_BANK_BALANCE_BY_FUTURE
    TD_ERR_RTN_BANK_TO_FUTURE_BY_FUTURE = ctp_trader.TD_ERR_RTN_BANK_TO_FUTURE_BY_FUTURE
    TD_ERR_RTN_FUTURE_TO_BANK_BY_FUTURE = ctp_trader.TD_ERR_RTN_FUTURE_TO_BANK_BY_FUTURE
    TD_ERR_RTN_REPEAL_BANK_TO_FUTURE_BY_FUTURE_MANUAL = ctp_trader.TD_ERR_RTN_REPEAL_BANK_TO_FUTURE_BY_FUTURE_MANUAL
    TD_ERR_RTN_REPEAL_FUTURE_TO_BANK_BY_FUTURE_MANUAL = ctp_trader.TD_ERR_RTN_REPEAL_FUTURE_TO_BANK_BY_FUTURE_MANUAL
    TD_ERR_RTN_QRY_BANK_BALANCE_BY_FUTURE = ctp_trader.TD_ERR_RTN_QRY_BANK_BALANCE_BY_FUTURE
    TD_RTN_REPEAL_FROM_BANK_TOFUTURE_BY_FUTURE = ctp_trader.TD_RTN_REPEAL_FROM_BANK_TOFUTURE_BY_FUTURE
    TD_RTN_REPEAL_FROM_FUTURE_TO_BANK_BY_FUTURE = ctp_trader.TD_RTN_REPEAL_FROM_FUTURE_TO_BANK_BY_FUTURE
    TD_RSP_FROM_BANK_TO_FUTURE_BY_FUTURE = ctp_trader.TD_RSP_FROM_BANK_TO_FUTURE_BY_FUTURE
    TD_RSP_FROM_FUTURE_TO_BANK_BY_FUTURE = ctp_trader.TD_RSP_FROM_FUTURE_TO_BANK_BY_FUTURE
    TD_RSP_QRY_BANK_ACCOUNT_MONEY_BY_FUTURE = ctp_trader.TD_RSP_QRY_BANK_ACCOUNT_MONEY_BY_FUTURE
    TD_RTN_OPEN_ACCOUNT_BY_BANK = ctp_trader.TD_RTN_OPEN_ACCOUNT_BY_BANK
    TD_RTN_CANCEL_ACCOUNT_BY_BANK = ctp_trader.TD_RTN_CANCEL_ACCOUNT_BY_BANK
    TD_RTN_CHANGE_ACCOUNT_BY_BANK = ctp_trader.TD_RTN_CHANGE_ACCOUNT_BY_BANK

cdef class CtpMarket:
    cdef ctp_market.CMarketCollector *collector
    def __cinit__(self, flowPath=".\\flow\\", *args, **kwargs): 
        self.collector = new ctp_market.CMarketCollector(as_str(flowPath if flowPath else ""))            
    def __dealloc__(self):
        if self.collector is not NULL: del self.collector 
    @staticmethod
    def version():
        return ctp_market.CMarketCollector.Version()
    def connect(self, frontPath=""):
        self.collector.RegisterCb(<void*>self, md_callback) 
        if self.collector: self.collector.Connect(as_str(frontPath if frontPath else ""))
    def login(self):
        if self.collector: return self.collector.Login(as_str(self.broker), as_str(self.user), as_str(self.pwd))
    def logout(self):
        if self.collector: return self.collector.Logout(as_str(self.broker), as_str(self.user))
    def get_trading_day(self):
        return self.collector.GetTradingDay()
    def sub_market(self, instrument_ids):
        ret = None
        if self.collector:             
            ids = to_cstring_array(instrument_ids)
            ret = self.collector.SubMarket(ids, len(instrument_ids))
            free(ids)
        return ret
    def unsub_market(self, instrument_ids):
        ret = None
        if self.collector: 
            ids = to_cstring_array(instrument_ids)
            ret = self.collector.UnSubMarket(ids, len(instrument_ids))
            free(ids)
        return ret
    def sub_quote(self, instrument_ids):
        ret = None
        if self.collector: 
            ids = to_cstring_array(instrument_ids)
            ret = self.collector.SubQuote(ids, len(instrument_ids))
            free(ids)
        return ret
    def unsub_quote(self, instrument_ids):
        ret = None
        if self.collector: 
            ids = to_cstring_array(instrument_ids)
            ret = self.collector.UnSubQuote(ids, len(instrument_ids))
            free(ids)
        return ret
    def get_handler(self):
        return self.handler
    #def join(self):        
    #    self._mdApi.Join()  

cdef class CtpTrader:
    cdef ctp_trader.CTraderCollector *collector
    def __cinit__(self, flowPath=".\\flow\\", *args, **kwargs):
        self.collector = new ctp_trader.CTraderCollector(as_str(flowPath if flowPath else ""))        
    def __dealloc__(self):
        if self.collector is not NULL: del self.collector
    @staticmethod
    def version():
        return ctp_trader.CTraderCollector.Version()
    def connect(self, frontPath="", privateResume=ctp_trader.THOST_TERT_QUICK, publicResume=ctp_trader.THOST_TERT_QUICK):
        self.collector.RegisterCb(<void*>self, td_callback)
        if self.collector: self.collector.Connect(as_str(frontPath if frontPath else ""), privateResume, publicResume)
    def auth(self):
        if self.product and self.authcode:
            return self.collector.Auth(as_str(self.broker), as_str(self.user), as_str(self.product), as_str(self.authcode))
        else:
            return self.login()
    def login(self):
        return self.collector.Login(as_str(self.broker), as_str(self.user), as_str(self.pwd))
    def logout(self):
        return self.collector.Logout(as_str(self.broker), as_str(self.user))
    def get_trading_day(self):
        return self.collector.GetTradingDay()
    def change_user_pwd(self, opwd, npwd):
        return self.collector.ChangeUserPwd(as_str(self.broker), as_str(self.user), as_str(opwd if opwd else ""), as_str(npwd if npwd else ""))
    def change_trading_pwd(self, account, opwd, npwd, currency="CNY"):
        return self.collector.ChangeTradingPwd(as_str(self.broker), as_str(account if account else ""), as_str(opwd if opwd else ""), as_str(npwd if npwd else ""), as_str(currency if currency else ""))    
    def query_settle_confirm(self):
        return self.collector.QrySettleInfoConfirm(as_str(self.broker), as_str(self.user))
    def query_settle(self, day=None):
        return self.collector.QrySettleInfo(as_str(self.broker), as_str(self.user), as_str(day if day else self.get_trading_day()))
    def confirm_settle(self):
        return self.collector.ConfirmSettleInfo(as_str(self.broker), as_str(self.user))
    def query_investor(self):
        return self.collector.QryInvestor(as_str(self.broker), as_str(self.user))
    def query_exchange(self, eid=None): #CZCE/CFFEX/SHFE/DCE
        return self.collector.QryExchange(as_str(eid if eid else ""))
    def query_product(self, pid=None, pclass=None):  #p/SM/v/JR/WT/PM/cs/y/c/SF/CF/bb/T/RO/ZC/MA
        return self.collector.QryProduct(as_str(pid if pid else ""), as_str(pclass if pclass else ""))
    def query_instrument(self, iid=None, eid=None, pid=None):
        return self.collector.QryInstrument(as_str(iid if iid else ""), as_str(eid if eid else ""), as_str(pid if pid else ""))
    def query_position(self, iid=None):
        return self.collector.QryInvestorPos(as_str(self.broker), as_str(self.user), as_str(iid if iid else ""))
    def query_order(self):
        return self.collector.QryOrder(as_str(self.broker), as_str(self.user))
    def query_trade(self, iid=None, eid=None, tid=None):
        return self.collector.QryTrade(as_str(self.broker), as_str(self.user), as_str(iid if iid else ""), as_str(eid if eid else ""), as_str(tid if tid else ""))

    def query_instruments(self):
        self.all_instruments = []
        return self.collector.ReqAllInstrument()
    def add_instrument(self, instrument):
        if instrument:
            find = False
            for i,ins in enumerate(self.all_instruments):
                if ins.InstrumentID == instrument.InstrumentID and ins.ExchangeID == instrument.ExchangeID:
                    self.all_instruments[i] = instrument, find = True
                    break
            if not find:
                self.all_instruments.append(instrument) 
    def get_instruments(self):
        return self.all_instruments

    #def ReqQryInstrument(CThostFtdcQryInstrumentField *pQryInstrument, int nRequestID):
    #    return self.collector.ReqQryInstrument()
    def get_handler(self):
        return self.handler
    #def join(self):        
    #    self._mdApi.Join()        


print "Imported CTP Module"