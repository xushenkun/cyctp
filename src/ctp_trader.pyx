#distutils: language = c++

from libc.stdlib cimport malloc, free
from cpython.string cimport PyString_AsString
from libc.string cimport strdup
import logging, logging.config
from multiprocessing.pool import ThreadPool
from threading import current_thread

cimport ctp_trader
#from ctp_struct import *
from ctypes import addressof, memset, pointer, byref, sizeof as csizeof

logger = logging.getLogger()#__name__)

cb_pool = ThreadPool(processes=2)

cdef int td_callback(void *context, ctp_trader.TD_CALLBACK_TYPE t, void* data, int rid, int last) with gil:
    async_result = cb_pool.apply_async(async_callback, args=(<object>context, t, <object>data, rid, last)) 
    async_result.get()
    return 0

def async_callback(context, t, data, rid, last):
    #print '', current_thread().ident, t, 'pycallback'
    context.get_handler().route(t, data, rid, last)
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

cdef class CtpTrader:
    cdef ctp_trader.CTraderCollector *collector
    def __cinit__(self, flowPath=".\\flow\\td\\", *args, **kwargs):
        self.collector = new ctp_trader.CTraderCollector(as_str(flowPath if flowPath else ""))        
    def __dealloc__(self):
        print 'CtpTrader dealloc'
        if self.collector is not NULL: del self.collector
    @staticmethod
    def version():
        return ctp_trader.CTraderCollector.Version()
    def connect(self, frontPaths, privateResume=ctp_trader.THOST_TERT_QUICK, publicResume=ctp_trader.THOST_TERT_RESTART):
        self.collector.RegisterCb(<void*>self, td_callback)
        ids = to_cstring_array(frontPaths)
        ret = self.collector.Connect(ids, len(frontPaths), privateResume, publicResume)
        free(ids)
        return ret
    def auth(self):
        if self.product and self.authcode:
            return self.collector.Auth(as_str(self.broker), as_str(self.user), as_str(self.product), as_str(self.authcode))
        else:
            return self.login()
    def login(self, bbid='0000'):
        return self.collector.Login(as_str(self.broker), as_str(bbid if bbid else '0000'), as_str(self.user), as_str(self.pwd))
    def logout(self):
        return self.collector.Logout()
    def get_trading_day(self):
        return self.collector.GetTradingDay()
    def change_user_pwd(self, opwd, npwd):
        return self.collector.ChangeUserPwd(as_str(opwd if opwd else ""), as_str(npwd if npwd else ""))
    def change_trading_pwd(self, account, opwd, npwd, currency="CNY"):
        return self.collector.ChangeTradingPwd(as_str(opwd if opwd else ""), as_str(npwd if npwd else ""), as_str(currency if currency else ""))    
    def query_settle_confirm(self):
        return self.collector.QrySettleInfoConfirm()
    def query_settle(self, day=None):
        return self.collector.QrySettleInfo(as_str(day if day else self.get_trading_day()))
    def confirm_settle(self):
        return self.collector.ConfirmSettleInfo()
    def query_notice(self):
        return self.collector.QryNotice()
    def query_trading_notice(self):
        return self.collector.QryTradingNotice()
    def query_investor(self):
        return self.collector.QryInvestor()
    def query_exchange(self, eid=None): #CZCE/CFFEX/SHFE/DCE
        return self.collector.QryExchange(as_str(eid if eid else ""))
    def query_product(self, pid=None, pclass=None):  #p/SM/v/JR/WT/PM/cs/y/c/SF/CF/bb/T/RO/ZC/MA
        return self.collector.QryProduct(as_str(pid if pid else ""), as_str(pclass if pclass else ""))
    def query_instrument(self, iid=None, eid=None, pid=None):
        return self.collector.QryInstrument(as_str(iid if iid else ""), as_str(eid if eid else ""), as_str(pid if pid else ""))
    def query_instruments(self):
        self.all_instruments = []
        return self.collector.ReqAllInstrument()
    def add_instrument(self, instrument):
        if instrument:
            find = False
            for i, inst in enumerate(self.all_instruments):    
                if inst.InstrumentID == instrument.InstrumentID and inst.ExchangeID == instrument.ExchangeID:
                    self.all_instruments[i] = instrument; find = True; break
            if not find: self.all_instruments.append(instrument) 
    def get_instruments(self):
        return self.all_instruments
    def query_position(self, iid=None):
        return self.collector.QryInvestorPos(as_str(iid if iid else ""))
    def query_position_detail(self, iid=None):
        return self.collector.QryInvestorPosDetail(as_str(iid if iid else ""))
    def query_position_comb_detail(self, ciid=None):
        return self.collector.QryInvestorPosCombDetail(as_str(ciid if ciid else ""))
    def query_park_order(self, iid=None, eid=None):
        return self.collector.QryParkOrder(as_str(iid if iid else ""), as_str(eid if eid else ""))
    def query_park_order_action(self, iid=None, eid=None):
        return self.collector.QryParkOrderAction(as_str(iid if iid else ""), as_str(eid if eid else ""))
    def query_order(self):
        return self.collector.QryOrder()
    def query_order_max_volume(self, iid=None):
        return self.collector.QryMaxOrderVolume(as_str(iid if iid else ""))
    def query_trade(self, iid=None, eid=None, tid=None):
        return self.collector.QryTrade(as_str(iid if iid else ""), as_str(eid if eid else ""), as_str(tid if tid else ""))
    def query_account_register(self, bid=None):
        return self.collector.QryAccountRegister(as_str(bid if bid else ""))
    def add_bank_account(self, account):
        if account:
            find = False
            for i, inst in enumerate(self.all_bank_accounts):    
                if inst.BankID == account.BankID and inst.BankBranchID == account.BankBranchID and inst.BrokerID == account.BrokerID and inst.BrokerBranchID == account.BrokerBranchID:
                    self.all_bank_accounts[i] = account; find = True; break
            if not find: self.all_bank_accounts.append(account)
    def get_bank_accounts(self):
        return self.all_bank_accounts
    def query_transfer_bank(self, bid=None, bbid=None):
        return self.collector.QryTransferBank(as_str(bid if bid else ""), as_str(bbid if bbid else ""))
    def query_contract_bank(self, bid=None, bbid=None):
        return self.collector.QryContractBank(as_str(bid if bid else ""), as_str(bbid if bbid else ""))
    def query_trade_account(self):
        return self.collector.QryTradingAccount()    
    def query_trade_code(self, eid=None, cid=None, cidt=None):
        return self.collector.QryTradingCode(as_str(eid if eid else ""), as_str(cid if cid else ""), as_str(cidt if cidt else ""))
    def add_trade_code(self, tradecode):
        if tradecode:
            find = False
            for i, inst in enumerate(self.all_trade_codes):    
                if inst.InvestorID == tradecode.InvestorID and inst.BrokerID == tradecode.BrokerID and inst.ExchangeID == tradecode.ExchangeID:
                    self.all_trade_codes[i] = tradecode; find = True; break
            if not find: self.all_trade_codes.append(tradecode)
    def get_trade_codes(self):
        return self.all_trade_codes

    def deposit_money(self, bid, amount, bbid="0000", day=None):
        return self.collector.DepositMoney(as_str(bid if bid else ""), as_str(bbid if bbid else "0000"), as_str(day if day else self.get_trading_day()), amount)
    def withdraw_money(self, bid, amount, bbid="0000", day=None):
        return self.collector.WithdrawMoney(as_str(bid if bid else ""), as_str(bbid if bbid else "0000"), as_str(day if day else self.get_trading_day()), amount)
    def query_transfer(self, bid):
        return self.collector.QryTransferSerial(as_str(bid if bid else ""))
    def query_bank_money(self, bid, bbid="0000", day=None):
        return self.collector.QryBankMoney(as_str(bid if bid else ""), as_str(bbid if bbid else "0000"), as_str(day if day else self.get_trading_day()))
    
    def query_market_data(self, iid):
        return self.collector.QryMarketData(as_str(iid if iid else ""))

    def ask_quote(self, iid, max_ref=None):
        return self.collector.AskQuote(as_str(iid if iid else ""), as_str(max_ref if max_ref else self.inc_max_other_ref()))
    def query_ask_quote(self):
        pass
    def answer_quote(self, iid, buy_price, sell_price, buy_volume, sell_volume):
        return self.collector.AnswerQuote(as_str(iid if iid else ""), buy_price, sell_price, buy_volume, sell_volume)
    def cancel_answer_quote(self, iid):
        pass

    def insert_limit_price_order(self, iid, direction, volume, price, offset_flags, hedge_flags, min_volume=1, volume_condition='1', time_condition='3', force_close_reason='0', auto_suspend=0, user_force_close=0, max_ref=None, park=False):        
        from cyctp.ctp_struct import OrderRequest, ParkOrderRequest
        order_req = OrderRequest() if not park else ParkOrderRequest()
        memset(addressof(order_req), 0, csizeof(order_req))    
        order_req.InstrumentID = iid; order_req.OrderPriceType = '2' #ORDER_PRICE_TYPE_LIMIT
        order_req.OrderRef = max_ref if max_ref else self.inc_max_order_ref()
        order_req.Direction = direction; order_req.CombOffsetFlag = ''.join(offset_flags)
        order_req.CombHedgeFlag = ''.join(hedge_flags); order_req.VolumeTotalOriginal = volume
        order_req.LimitPrice = price; order_req.VolumeCondition = volume_condition #VOLUME_CONDITION_ANY
        order_req.MinVolume = min_volume; order_req.ForceCloseReason = force_close_reason #FORCE_CLOSE_NOT
        order_req.IsAutoSuspend = auto_suspend; order_req.UserForceClose = user_force_close
        order_req.TimeCondition = time_condition; order_req.ContingentCondition = '1' #CONDITION_TRIGGER_IMMEDIATELY
        cdef unsigned int addr = addressof(order_req) #byref(order_req) #
        return self.collector.InsertOrder(<CThostFtdcInputOrderField*>addr) if not park else self.collector.InsertParkOrder(<CThostFtdcParkedOrderField*>addr)

    def insert_market_price_order(self, iid, direction, volume, offset_flags, hedge_flags, min_volume=1, volume_condition='1', time_condition='1', force_close_reason='0', auto_suspend=0, user_force_close=0, max_ref=None, park=False):
        from cyctp.ctp_struct import OrderRequest, ParkOrderRequest
        order_req = OrderRequest() if not park else ParkOrderRequest()
        memset(addressof(order_req), 0, csizeof(order_req))    
        order_req.InstrumentID = iid; order_req.OrderPriceType = '1' #ORDER_PRICE_TYPE_ANY
        order_req.OrderRef = max_ref if max_ref else self.inc_max_order_ref()
        order_req.Direction = direction; order_req.CombOffsetFlag = ''.join(offset_flags)
        order_req.CombHedgeFlag = ''.join(hedge_flags); order_req.VolumeTotalOriginal = volume
        order_req.LimitPrice = 0; order_req.VolumeCondition = volume_condition #VOLUME_CONDITION_ANY
        order_req.MinVolume = min_volume; order_req.ForceCloseReason = force_close_reason #FORCE_CLOSE_NOT
        order_req.IsAutoSuspend = auto_suspend; order_req.UserForceClose = user_force_close
        order_req.TimeCondition = time_condition; order_req.ContingentCondition = '1' #CONDITION_TRIGGER_IMMEDIATELY
        cdef unsigned int addr = addressof(order_req) #byref(order_req) #
        return self.collector.InsertOrder(<CThostFtdcInputOrderField*>addr) if not park else self.collector.InsertParkOrder(<CThostFtdcParkedOrderField*>addr)

    def insert_condition_order(self, iid, direction, volume, stop_price, contingent_condition, time_condition, offset_flags, hedge_flags, date=None, price=None, min_volume=1, volume_condition='1', force_close_reason='0', auto_suspend=0, user_force_close=0, max_ref=None, park=False):
        from cyctp.ctp_struct import OrderRequest, ParkOrderRequest
        order_req = OrderRequest() if not park else ParkOrderRequest()
        memset(addressof(order_req), 0, csizeof(order_req))    
        order_req.InstrumentID = iid; order_req.OrderPriceType = '2' #ORDER_PRICE_TYPE_LIMIT
        order_req.OrderRef = max_ref if max_ref else self.inc_max_order_ref()
        order_req.Direction = direction; order_req.CombOffsetFlag = ''.join(offset_flags)
        order_req.CombHedgeFlag = ''.join(hedge_flags); order_req.VolumeTotalOriginal = volume
        order_req.StopPrice = stop_price; order_req.VolumeCondition = volume_condition #VOLUME_CONDITION_ANY
        order_req.MinVolume = min_volume; order_req.ForceCloseReason = force_close_reason #FORCE_CLOSE_NOT
        order_req.IsAutoSuspend = auto_suspend; order_req.UserForceClose = user_force_close        
        if date and time_condition == '4': order_req.GTDDate = date #TIME_CONDITION_GTILL_DAY
        if price: order_req.LimitPrice = price
        order_req.TimeCondition = time_condition; order_req.ContingentCondition = contingent_condition
        cdef unsigned int addr = addressof(order_req) #byref(order_req) #
        return self.collector.InsertOrder(<CThostFtdcInputOrderField*>addr) if not park else self.collector.InsertParkOrder(<CThostFtdcParkedOrderField*>addr)

    def insert_fok_order(self, iid, direction, volume, price, offset_flags, hedge_flags, force_close_reason='0', auto_suspend=0, user_force_close=0, max_ref=None, park=False):
        return self.insert_limit_price_order(iid, direction, volume, price, offset_flags, hedge_flags, volume_condition='3', time_condition='1', force_close_reason=force_close_reason, auto_suspend=auto_suspend, user_force_close=user_force_close, max_ref=max_ref, park=park)

    def insert_fak_order(self, iid, direction, volume, price, offset_flags, hedge_flags, min_volume=0, force_close_reason='0', auto_suspend=0, user_force_close=0, max_ref=None, park=False):
        volume_condition = '2' if min_volume > 1 else '1'
        return self.insert_limit_price_order(iid, direction, volume, price, offset_flags, hedge_flags, min_volume=min_volume, volume_condition=volume_condition, time_condition='1', force_close_reason=force_close_reason, auto_suspend=auto_suspend, user_force_close=user_force_close, max_ref=max_ref, park=park)

    def delete_order_by_ref(self, fid, sid, order_ref, max_ref=None, park=False):
        from cyctp.ctp_struct import OrderActionRequest, ParkOrderActionRequest
        action_req = OrderActionRequest() if not park else ParkOrderActionRequest()
        memset(addressof(action_req), 0, csizeof(action_req))
        action_req.OrderActionRef = max_ref if max_ref else int(self.inc_max_other_ref())
        action_req.FrontID = fid; action_req.SessionID = sid; action_req.OrderRef = order_ref
        action_req.ActionFlag = '0'
        cdef unsigned int addr = addressof(action_req) #byref(action_req) #
        return self.collector.ActionOrder(<CThostFtdcInputOrderActionField*>addr) if not park else self.collector.ActionParkOrder(<CThostFtdcParkedOrderActionField*>addr)

    def delete_order_by_sysid(self, eid, order_sys_id, max_ref=None, park=False):
        from cyctp.ctp_struct import OrderActionRequest, ParkOrderActionRequest
        action_req = OrderActionRequest() if not park else ParkOrderActionRequest()
        memset(addressof(action_req), 0, csizeof(action_req))
        action_req.OrderActionRef = max_ref if max_ref else int(self.inc_max_other_ref())
        action_req.ExchangeID = eid; action_req.OrderSysID = order_sys_id
        action_req.ActionFlag = '0'
        cdef unsigned int addr = addressof(action_req) #byref(action_req) #
        return self.collector.ActionOrder(<CThostFtdcInputOrderActionField*>addr) if not park else self.collector.ActionParkOrder(<CThostFtdcParkedOrderActionField*>addr)

    def delete_park(self, pid):
        return self.collector.RemovePark(as_str(pid if pid else ""))
    def delete_park_action(self, rpid):
        return self.collector.RemoveParkAction(as_str(rpid if rpid else ""))

    def query_instrument_margin_rate(self, iid, hedge_flag):
        return self.collector.QryInstrumentMarginRate(as_str(iid if iid else ""), as_str(hedge_flag if hedge_flag else ""))
    def query_instrument_commission_rate(self, iid):
        return self.collector.QryInstrumentCommissionRate(as_str(iid if iid else ""))

    def query_exchange_margin_rate(self, iid, hedge_flag):
        return self.collector.QryExchangeMarginRate(as_str(iid if iid else ""), as_str(hedge_flag if hedge_flag else ""))
    def query_exchange_margin_rate_adjust(self, iid, hedge_flag):
        return self.collector.QryExchangeMarginRateAdjust(as_str(iid if iid else ""), as_str(hedge_flag if hedge_flag else ""))
    def query_currency_rate(self, from_id, to_cid):
        return self.collector.QryCurrencyRate(as_str(from_id if from_id else ""), as_str(to_cid if to_cid else ""))
    def query_product_rate(self, pid):
        return self.collector.QryProductRate(as_str(pid if pid else ""))
    def query_product_group(self, pid, eid=None):
        return self.collector.QryProductGroup(as_str(pid if pid else ""), as_str(eid if eid else ""))
    '''
    def insert_park_order(self, iid):
        req = ParkOrder()
        memset(addressof(req), 0, csizeof(req))
        req.BrokerID = self.broker
        req.InvestorID = self.user
        req.InstrumentID = iid
        req.OrderRef = self.inc_max_order_ref()
        req.UserID = self.user
        #req.OrderPriceType = 1
        #req.Direction = 1
        #req.LimitPrice = 1
        #req.VolumeTotalOriginal = 1
    '''

    #def ReqQryInstrument(CThostFtdcQryInstrumentField *pQryInstrument, int nRequestID):
    #    return self.collector.ReqQryInstrument()
    def get_handler(self):
        return self.handler
    #def join(self):        
    #    self._mdApi.Join()    
    def close(self):
        if self.collector: return self.collector.Close()    

#print "Imported CTP Trader Module"
