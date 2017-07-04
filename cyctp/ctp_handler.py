# -*- coding: utf-8 -*-

import os, sys, time
import logging, logging.config
import codecs, copy
from ctypes import *
reload(sys)
sys.setdefaultencoding('utf-8')
logger = logging.getLogger()#__name__)

from cyctp.ctp_market import MD_CALLBACK_TYPE
from cyctp.ctp_trader import TD_CALLBACK_TYPE
from cyctp.ctp_struct import *

logger = logging.getLogger()#__name__)

def log_op(ret, name):
    if ret == 0:
        pass
    elif ret == -1:
        logger.error(u"%s 因网络原因发送失败"%name)
    elif ret == -2:
        logger.error(u"%s 未处理请求队列总数量超限"%name)
    elif ret == -3:
        logger.error(u"%s 每秒发送请求数量超限"%name)
    elif ret == -999:
        logger.error(u"%s 网络未连接或未登录"%name)

class PathHandler(object):
    def __init__(self, caller):
        self.caller = caller
    def get_data_path(self):
        return "data" #os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), 'data')
    def get_data_trading_day_path(self):
        trading_day = self.caller.get_trading_day()
        if trading_day:
            path = self.get_data_path()+os.path.sep+trading_day
            if not int(os.path.exists(path)):
                os.makedirs(path)
            return path
        return None
    def get_data_trading_day_xxx_path(self, xxx):
        trading_day_path = self.get_data_trading_day_path()
        if trading_day_path:
            path = trading_day_path+os.path.sep+xxx
            if not int(os.path.exists(path)):
                os.makedirs(path)
            return path
        return None 

class InstrumentHandler(PathHandler):
    def __init__(self, caller):
        super(InstrumentHandler, self).__init__(caller)
        self.sub_iids = set([])
        self.sub_insts_volume_multiple = {}
    def sub_self_instruments(self):
        file_path = self.get_data_path() + os.path.sep + "sub_instruments.csv"
        if int(os.path.exists(file_path)):
            iids = []
            with codecs.open(file_path, 'r', 'utf-8') as fi:
                line = fi.readline()
                while line:
                    line_values = line.split('\t')
                    iids.append(line_values[0])
                    self.sub_insts_volume_multiple[line_values[0]] = int(line_values[12]) if line_values[12] else 1
                    line = fi.readline()
            if iids:
                if len(self.sub_iids)>0: more_iids = list(set(iids)-set(self.sub_iids))
                else: more_iids = iids
                log_op(self.caller.sub_market(more_iids), "sub_market")
                self.sub_iids |= set(more_iids)
    def make_instrument(self, instrument):
        data_path = self.get_data_trading_day_xxx_path('instrument')
        if data_path:
            file_path = data_path + os.path.sep + "%s.csv"%(instrument.InstrumentID)
            if not int(os.path.exists(file_path)):                
                with codecs.open(file_path, 'a', 'utf-8') as fo:
                    pass            
    def save_instrument(self, instrument, last):
        self.caller.add_instrument(copy.deepcopy(instrument))
        if last:
            data_path = self.get_data_trading_day_xxx_path('instrument')
            if data_path:
                file_path = data_path + os.path.sep + "0_all_instruments.csv"
                with codecs.open(file_path, 'w', 'utf-8') as fo:
                    for ins in self.caller.get_instruments():
                        fo.write(dump_struct(ins)+"\n")
                file_path = self.get_data_path() + os.path.sep + "sub_instruments.csv"
                if not int(os.path.exists(file_path)):
                    with codecs.open(file_path, 'w', 'utf-8') as fo:
                        for ins in self.caller.get_instruments():
                            fo.write(dump_struct(ins)+"\n")
            #TODO should call market sub instruments
    def save_instrument_status(self, status):
        data_path = self.get_data_trading_day_xxx_path('instrument')
        if status and data_path:
            file_path = data_path + os.path.sep + "0_status.csv"
            with codecs.open(file_path, 'a', 'utf-8') as fo:
                fo.write(dump_struct(status)+"\n")

class QuoteHandler(PathHandler):
    def __init__(self, caller):
        super(QuoteHandler, self).__init__(caller)
        self.for_quote_count = 0
        self.sub_forquotes = set([])
    def sub_self_forquotes(self):
        file_path = self.get_data_path() + os.path.sep + "quote_instruments.csv"
        if int(os.path.exists(file_path)):
            iids = []
            with codecs.open(file_path, 'r', 'utf-8') as fi:
                line = fi.readline()
                while line:
                    if line.split('\t')[1] == 'CZCE': iids.append(line.split('\t')[0])  #only for market maker in CZCE
                    line = fi.readline()
            if iids:
                if len(self.sub_forquotes)>0: more_iids = list(set(iids)-set(self.sub_forquotes))
                else: more_iids = iids
                log_op(self.caller.sub_ask_quote(more_iids), "sub_for_quote")
                self.sub_forquotes |= set(more_iids)
    def save_for_quote(self, data):
        self.for_quote_count += 1 
        data_path = self.get_data_trading_day_xxx_path('quote')
        if data and data_path:
            file_path = data_path + os.path.sep + "%s.csv"%(data.InstrumentID)
            with codecs.open(file_path, 'a', 'utf-8') as fo:
                fo.write(dump_struct(data)+"\n")
        if self.for_quote_count%((2*len(self.sub_iids))*3) == 0:
            logger.info("For quote %s data saved"%self.for_quote_count)

class SettlementHandler(PathHandler):
    def __init__(self, caller):
        super(SettlementHandler, self).__init__(caller)        
    def check_settle_confirm(self, data):
        if data:
            data = SettleInfoConfirm.from_address(data)
            if data.ConfirmDate == self.caller.get_trading_day():
                logger.info("Trader settlement confirmed")
                print_struct(data)
            else:
                logger.info("Trader settlement not confirmed today, last by %s"%data.ConfirmDate)
                time.sleep(1)
                log_op(self.caller.query_settle(), "qry_settle")
        else:
            time.sleep(1)
            logger.info("Trader settlement not confirmed")
            log_op(self.caller.query_settle(), "qry_settle")
    def check_settle(self, data):
        logger.info("Trader settlement info responsed")   
        if data:
            data = SettleInfo.from_address(data)
            print_struct(data)                     
            #user should check this info and confirm the settlement info
        time.sleep(1)
        log_op(self.caller.confirm_settle(), "confirm_settle")

class TradeHandler(PathHandler):
    def __init__(self, caller):
        super(TradeHandler, self).__init__(caller)
    def save_trade_code(self, tradecode, last):
        self.caller.add_trade_code(copy.deepcopy(tradecode))
        if last:
            data_path = self.get_data_trading_day_xxx_path('trade')
            if data_path:
                file_path = data_path + os.path.sep + "0_all_trade_codes.csv"
                with codecs.open(file_path, 'w', 'utf-8') as fo:
                    for ins in self.caller.get_trade_codes():
                        fo.write(dump_struct(ins)+"\n")

class BankHandler(PathHandler):
    def __init__(self, caller):
        super(BankHandler, self).__init__(caller)
    def save_bank_account(self, account, last):
        self.caller.add_bank_account(copy.deepcopy(account))
        if last:
            data_path = self.get_data_trading_day_xxx_path('bank')
            if data_path:
                file_path = data_path + os.path.sep + "0_all_bank_accounts.csv"
                with codecs.open(file_path, 'w', 'utf-8') as fo:
                    for ins in self.caller.get_bank_accounts():
                        fo.write(dump_struct(ins)+"\n")                        

class MarketDataHandler(InstrumentHandler):
    def __init__(self, caller):
        super(MarketDataHandler, self).__init__(caller)   
        self.market_data_count = 0
    def adjust_turnover_averageprice(self, data):
        volume_multiple = self.sub_insts_volume_multiple.get(data.InstrumentID)
        if volume_multiple and volume_multiple != 1:
            if data.ExchangeID == 'SHFE' or data.ExchangeID == 'DCE':
                data.AveragePrice = data.AveragePrice/volume_multiple
            elif data.ExchangeID == 'CZCE':
                data.Turnover = data.Turnover * volume_multiple
    def save_depth_data(self, data):       
        self.market_data_count += 1 
        data_path = self.get_data_trading_day_xxx_path('instrument')
        if data and data_path:
            self.adjust_turnover_averageprice(data)
            file_path = data_path + os.path.sep + "%s.csv"%(data.InstrumentID)
            with codecs.open(file_path, 'a', 'utf-8') as fo:
                fo.write(dump_struct(data)+"\n")
        if self.market_data_count%((2*len(self.sub_iids))*3) == 0:
            logger.info("Market %s data saved"%self.market_data_count)        


class MdHandler(MarketDataHandler, QuoteHandler):
    def __init__(self, caller):
        super(MdHandler, self).__init__(caller)    
    def route(self, t, data, rid, last):
        if t == MD_CALLBACK_TYPE.MD_FRONT_CONNECTED:
            logger.info("Market front connected")
            log_op(self.caller.login(), "login")
        elif t == MD_CALLBACK_TYPE.MD_FRONT_DISCONNECTED and data:
            data = c_int.from_address(data)
            logger.info("Market front disconnected: %s"%data)           
        elif t == MD_CALLBACK_TYPE.MD_HEART_BEAT_WARN and data:
            data = c_int.from_address(data)
            logger.info("Market heart beat time lapse: %s"%data)
        elif t == MD_CALLBACK_TYPE.MD_RSP_USER_LOGIN and data:
            logger.info("Market user logined")
            data = LoginUser.from_address(data)
            print_struct(data)
            logger.info("Market instrument subscribing...")
            self.sub_self_instruments()
            time.sleep(1)
            self.sub_self_forquotes()
        elif t == MD_CALLBACK_TYPE.MD_RSP_USER_LOGOUT and data:
            logger.info("Market user logouted")
        elif t == MD_CALLBACK_TYPE.MD_RSP_ERROR and data:
            data = RsponseInfo.from_address(data)
            logger.error("Market error responsed: %s %s"%(data.ErrorID, str(data.ErrorMsg)))
        elif t == MD_CALLBACK_TYPE.MD_RSP_SUB_MARKET_DATA and data:
            data = InstrumentID.from_address(data)
            if last:
                logger.info("Market instrument batch subscribed: [:%s]"%data.InstrumentID)
            self.make_instrument(data)
            #logger.info("Market instrument maked: %s"%data.InstrumentID)
        elif t == MD_CALLBACK_TYPE.MD_RSP_UNSUB_MARKET_DATA and data:
            data = InstrumentID.from_address(data)
            if last:
                logger.info("Market instrument batch unsubscribed: [:%s]"%data.InstrumentID)
        elif t == MD_CALLBACK_TYPE.MD_RSP_SUB_FOR_QUOTE and data:
            data = InstrumentID.from_address(data)
            if last:
                logger.info("Market quote batch subscribed: [:%s]"%data.InstrumentID)
        elif t == MD_CALLBACK_TYPE.MD_RSP_UNSUB_FOR_QUOTE and data:
            data = InstrumentID.from_address(data)
            if last:
                logger.info("Market quote batch unsubscribed: [:%s]"%data.InstrumentID)
        elif t == MD_CALLBACK_TYPE.MD_RTN_DEPTH_MARKET_DATA and data:            
            data = MarketData.from_address(data)
            self.save_depth_data(data)
        elif t == MD_CALLBACK_TYPE.MD_RTN_FOR_QUOTE and data:
            data = AskQuoteRsp.from_address(data)
            self.save_for_quote(data)
        else:
            print "Market handler", t
        sys.stdout.flush()
        return None

class TdHandler(InstrumentHandler, QuoteHandler, SettlementHandler, TradeHandler, BankHandler):
    def __init__(self, caller):
        super(TdHandler, self).__init__(caller)    
    def route(self, t, data, rid, last):
        if t == TD_CALLBACK_TYPE.TD_FRONT_CONNECTED:
            logger.info("Trader front connected")
            log_op(self.caller.auth(), "auth")            
        elif t == TD_CALLBACK_TYPE.TD_FRONT_DISCONNECTED and data:
            data = c_int.from_address(data)
            logger.info("Trader front disconnected: %s"%data)          
        elif t == TD_CALLBACK_TYPE.TD_HEART_BEAT_WARN and data:
            data = c_int.from_address(data)
            logger.info("Trader heart beat time lapse: %s"%data)
        elif t == TD_CALLBACK_TYPE.TD_RSP_AUTH and data:
            logger.info("Trader user authed")
            log_op(self.caller.login(), "login")
        elif t == TD_CALLBACK_TYPE.TD_RSP_USER_LOGIN and data:
            logger.info("Trader user logined")
            data = LoginUser.from_address(data)
            print_struct(data)
            self.caller.front_id = data.FrontID
            self.caller.session_id = data.SessionID
            self.caller.max_order_ref = int(data.MaxOrderRef) if data.MaxOrderRef else 0
            logger.info("Trader settlement confirm querying...")
            #log_op(self.caller.confirm_settle(), "confirm_settle")
            log_op(self.caller.query_settle_confirm(), "qry_settle_confirm")              
        elif t == TD_CALLBACK_TYPE.TD_RSP_USER_LOGOUT and data:
            logger.info("Trader user logouted")
        elif t == TD_CALLBACK_TYPE.TD_RSP_PWD_UPDATE and data:
            logger.info("Trader user password changed")
        elif t == TD_CALLBACK_TYPE.TD_RSP_TRADING_PWD_UPDATE and data:
            logger.info("Trader user trading password changed")
        elif t == TD_CALLBACK_TYPE.TD_RSP_ORDER_INSERT and data:
            data = OrderRequest.from_address(data)
            logger.info("Trader order insert responsed")
            print_struct(data)
        elif t == TD_CALLBACK_TYPE.TD_RSP_PACK_ORDER_INSERT and data:
            data = ParkOrderRequest.from_address(data)
            logger.info("Trader park order insert responsed")
            print_struct(data)
        elif t == TD_CALLBACK_TYPE.TD_RSP_PACK_ORDER_ACTION and data:
            data = ParkOrderActionRequest.from_address(data)
            logger.info("Trader park order action responsed")
            print_struct(data)
        elif t == TD_CALLBACK_TYPE.TD_RSP_ORDER_ACTION and data:
            data = OrderActionRequest.from_address(data)
            logger.info("Trader order action responsed")
            print_struct(data)
        elif t == TD_CALLBACK_TYPE.TD_RSP_QRY_MAX_ORDER_VOLUME and data:
            data = MaxVolume.from_address(data)
            logger.info("Trader order max volume responsed")
            print_struct(data)
        elif t == TD_CALLBACK_TYPE.TD_RSP_SETTLE_INFO_CONFIRM and data:
            data = SettleInfoConfirm.from_address(data)
            logger.info("Trader settlement confirmed rightnow")
            print_struct(data)
        elif t == TD_CALLBACK_TYPE.TD_RSP_REMOVE_PACK and data:
            data = RemovePark.from_address(data)
            logger.info("Trader remove park responsed")
            print_struct(data)
        elif t == TD_CALLBACK_TYPE.TD_RSP_REMOVE_PACK_ACTION and data:
            data = RemoveParkAction.from_address(data)
            logger.info("Trader remove park action responsed")
            print_struct(data)
        elif t == TD_CALLBACK_TYPE.TD_RSP_EXEC_ORDER_INSERT and data:
            pass
        elif t == TD_CALLBACK_TYPE.TD_RSP_EXEC_ORDER_ACTION and data:
            pass
        elif t == TD_CALLBACK_TYPE.TD_RSP_FOR_QUOTE_INSERT and data:
            data = AskQuote.from_address(data)
            logger.info("Trader ask quote insert responsed")
            print_struct(data)
        elif t == TD_CALLBACK_TYPE.TD_RSP_QUOTE_INSERT and data:
            data = AnswerQuote.from_address(data)
            logger.info("Trader answer quote insert responsed")
            print_struct(data)
        elif t == TD_CALLBACK_TYPE.TD_RSP_QUOTE_ACTION and data:
            pass
        elif t == TD_CALLBACK_TYPE.TD_RSP_BATCH_ORDER_ACTION and data:
            pass
        elif t == TD_CALLBACK_TYPE.TD_RSP_COMB_ACTION_INSERT and data:
            pass
        elif t == TD_CALLBACK_TYPE.TD_RSP_QRY_ORDER:
            logger.info("Trader order responsed")
            if data:
                data = Order.from_address(data)            
                print_struct(data)
        elif t == TD_CALLBACK_TYPE.TD_RSP_QRY_TRADE:
            logger.info("Trader trade responsed")
            if data:
                data = Trade.from_address(data)
                print_struct(data)
        elif t == TD_CALLBACK_TYPE.TD_RSP_QRY_INVESTOR_POS:
            logger.info("Trader position responsed")
            if data:
                data = Position.from_address(data)
                print_struct(data)
        elif t == TD_CALLBACK_TYPE.TD_RSP_QRY_TRADING_ACCOUNT and data:
            data = TradeAccount.from_address(data)
            logger.info("Trader trading account responsed")
            print_struct(data)
        elif t == TD_CALLBACK_TYPE.TD_RSP_QRY_INVESTOR and data:
            data = Investor.from_address(data)
            logger.info("Trader investor responsed")
            print_struct(data)
        elif t == TD_CALLBACK_TYPE.TD_RSP_QRY_TRADING_CODE and data:
            data = TradeCode.from_address(data)
            self.save_trade_code(data, last)
        elif t == TD_CALLBACK_TYPE.TD_RSP_QRY_INSTRUMENT_MARGIN_RATE and data:
            data = InstrumentMarginRate.from_address(data)
            logger.info("Trader instrument margin rate responsed")
            print_struct(data)
        elif t == TD_CALLBACK_TYPE.TD_RSP_QRY_INSTRUMENT_COMMISSION_RATE and data:
            data = InstrumentCommissionRate.from_address(data)
            logger.info("Trader instrument commission rate responsed")
            print_struct(data)
        elif t == TD_CALLBACK_TYPE.TD_RSP_QRY_EXCHANGE and data:
            data = Exchange.from_address(data)
            logger.info("Trader exchange responsed")
            print_struct(data)
        elif t == TD_CALLBACK_TYPE.TD_RSP_QRY_PRODUCT and data:
            data = Product.from_address(data)
            logger.info("Trader product responsed")
            print_struct(data)
        elif t == TD_CALLBACK_TYPE.TD_RSP_QRY_INSTRUMENT and data:
            data = Instrument.from_address(data)
            #logger.info("Trader instrument responsed")
            self.save_instrument(data, last)
        elif t == TD_CALLBACK_TYPE.TD_RSP_QRY_DEPTH_MARKET_DATA and data:
            data = MarketData.from_address(data)
            logger.info("Trader market data responsed")
            print_struct(data)
        elif t == TD_CALLBACK_TYPE.TD_RSP_QRY_SETTLE_INFO:
            self.check_settle(data)                        
        elif t == TD_CALLBACK_TYPE.TD_RSP_QRY_TRANSFER_BANK and data:
            data = TransferBank.from_address(data)
            logger.info("Trader transfer bank responsed")
            print_struct(data)
        elif t == TD_CALLBACK_TYPE.TD_RSP_QRY_INVESTOR_POS_DETAIL:
            logger.info("Trader investor position detail responsed")
            if data:
                data = PositionDetail.from_address(data)            
                print_struct(data)
        elif t == TD_CALLBACK_TYPE.TD_RSP_QRY_NOTICE and data:
            data = Notice.from_address(data)
            logger.info("Trader notice responsed")
            print_struct(data)
        elif t == TD_CALLBACK_TYPE.TD_RSP_QRY_SETTLE_INFO_CONFIRM:
            self.check_settle_confirm(data)            
        elif t == TD_CALLBACK_TYPE.TD_RSP_QRY_INVESTOR_POS_COMB_DETAIL:
            logger.info("Trader investor position comb detail responsed")
            if data:
                data = PositionCombDetail.from_address(data)
                print_struct(data)
        elif t == TD_CALLBACK_TYPE.TD_RSP_QRY_CFMMC_TRADING_ACCOUNT_KEY and data:
            pass
        elif t == TD_CALLBACK_TYPE.TD_RSP_QRY_EWARRANT_OFFSET and data:
            pass
        elif t == TD_CALLBACK_TYPE.TD_RSP_QRY_INVESTOR_PRODUCT_GROUP_MARGIN and data:
            pass
        elif t == TD_CALLBACK_TYPE.TD_RSP_QRY_EXCHANGE_MARGIN_RATE and data:
            data = ExchangeMarginRate.from_address(data)
            logger.info("Trader exchange margin rate responsed")
            print_struct(data)
        elif t == TD_CALLBACK_TYPE.TD_RSP_QRY_EXCHANGE_MARGIN_RATE_ADJUST and data:
            data = ExchangeMarginRateAdjust.from_address(data)
            logger.info("Trader exchange margin rate adjust responsed")
            print_struct(data)
        elif t == TD_CALLBACK_TYPE.TD_RSP_QRY_EXCHANGE_RATE:
            logger.info("Trader currency rate responsed")
            if data:
                data = CurrencyRate.from_address(data)
                print_struct(data)
        elif t == TD_CALLBACK_TYPE.TD_RSP_QRY_SEC_AGENT_ACID_MAP and data:
            pass
        elif t == TD_CALLBACK_TYPE.TD_RSP_QRY_PRODUCT_EXCH_RATE:            
            logger.info("Trader product rate responsed")
            if data:
                data = ProductRate.from_address(data)
                print_struct(data)
        elif t == TD_CALLBACK_TYPE.TD_RSP_QRY_PRODUCT_GROUP:
            logger.info("Trader product group responsed")
            if data:
                data = ProductGroup.from_address(data)
                print_struct(data)
        elif t == TD_CALLBACK_TYPE.TD_RSP_QRY_MM_INSTRUMENT_COMMISSION_RATE and data:
            pass
        elif t == TD_CALLBACK_TYPE.TD_RSP_QRY_MM_OPTION_INSTR_COMM_RATE and data:
            pass
        elif t == TD_CALLBACK_TYPE.TD_RSP_QRY_INSTRUMENT_ORDER_COMM_RATE and data:
            pass
        elif t == TD_CALLBACK_TYPE.TD_RSP_QRY_OPTION_INSTR_TRADE_COST and data:
            pass
        elif t == TD_CALLBACK_TYPE.TD_RSP_QRY_OPTION_INSTR_COMM_RATE and data:
            pass
        elif t == TD_CALLBACK_TYPE.TD_RSP_QRY_EXEC_ORDER and data:
            pass
        elif t == TD_CALLBACK_TYPE.TD_RSP_QRY_FOR_QUOTE and data:
            pass
        elif t == TD_CALLBACK_TYPE.TD_RSP_QRY_QUOTE and data:
            pass
        elif t == TD_CALLBACK_TYPE.TD_RSP_QRY_COMB_INSTRUMENT_GUARD and data:
            pass
        elif t == TD_CALLBACK_TYPE.TD_RSP_QRY_COMB_ACTION and data:
            pass
        elif t == TD_CALLBACK_TYPE.TD_RSP_QRY_TRANSFER_SERIAL and data:
            data = TransferSerial.from_address(data)
            logger.info("Trader rsp transfer serial responsed")
            print_struct(data)
        elif t == TD_CALLBACK_TYPE.TD_RSP_QRY_ACCOUNT_REGISTER:
            logger.info("Trader account register responsed%s"%('' if data else ' [no bank]'))
            if data:
                data = AccountRegister.from_address(data)            
                self.save_bank_account(data, last)
        elif t == TD_CALLBACK_TYPE.TD_RSP_ERROR and data:
            data = RsponseInfo.from_address(data)
            logger.error("Trader error responsed: %s %s"%(data.ErrorID, str(data.ErrorMsg)))
        elif t == TD_CALLBACK_TYPE.TD_RTN_ORDER and data:
            data = Order.from_address(data)
            logger.info("Trader rtn order responsed")
            print_struct(data)
        elif t == TD_CALLBACK_TYPE.TD_RTN_TRADE and data:
            data = Trade.from_address(data)
            logger.info("Trader rtn trade responsed")
            print_struct(data)
        elif t == TD_CALLBACK_TYPE.TD_ERR_RTN_ORDER_INSERT and data:
            data = OrderRequest.from_address(data)
            logger.info("Trader err rtn order insert responsed")
            print_struct(data)
        elif t == TD_CALLBACK_TYPE.TD_ERR_RTN_ORDER_ACTION and data:
            data = OrderAction.from_address(data)
            logger.info("Trader err rtn order action responsed")
            print_struct(data)
        elif t == TD_CALLBACK_TYPE.TD_RTN_INSTRUMENT_STATUS and data:
            data = InstrumentStatus.from_address(data)
            self.save_instrument_status(data)
        elif t == TD_CALLBACK_TYPE.TD_RTN_BULLETIN and data:
            pass
        elif t == TD_CALLBACK_TYPE.TD_RTN_TRADING_NOTICE and data:
            data = TradingNoticeInfo.from_address(data)
            logger.info("Trader rtn trading notice info responsed")
            print_struct(data)
        elif t == TD_CALLBACK_TYPE.TD_RTN_ERROR_CONDITIONAL_ORDER and data:
            pass
        elif t == TD_CALLBACK_TYPE.TD_RTN_EXEC_ORDER and data:
            pass
        elif t == TD_CALLBACK_TYPE.TD_ERR_RTN_EXEC_ORDER_INSERT and data:
            pass
        elif t == TD_CALLBACK_TYPE.TD_ERR_RTN_EXEC_ORDER_ACTION and data:
            pass
        elif t == TD_CALLBACK_TYPE.TD_ERR_RTN_FOR_QUOTE_INSERT and data:
            data = AskQuote.from_address(data)
            logger.info("Trader err rtn ask quote insert responsed")
            print_struct(data)
        elif t == TD_CALLBACK_TYPE.TD_RTN_QUOTE and data:
            pass
        elif t == TD_CALLBACK_TYPE.TD_ERR_RTN_QUOTE_INSERT and data:
            pass
        elif t == TD_CALLBACK_TYPE.TD_ERR_RTN_QUOTE_ACTION and data:
            pass
        elif t == TD_CALLBACK_TYPE.TD_RTN_FOR_QUOTE_RSP and data:
            pass
        elif t == TD_CALLBACK_TYPE.TD_RTN_CFMMC_TRADING_ACCOUNT_TOKEN and data:
            pass
        elif t == TD_CALLBACK_TYPE.TD_ERR_RTN_BATCH_ORDER_ACTION and data:
            pass
        elif t == TD_CALLBACK_TYPE.TD_RTN_COMB_ACTION and data:
            pass
        elif t == TD_CALLBACK_TYPE.TD_ERR_RTN_COMB_ACTION_INSERT and data:
            pass
        elif t == TD_CALLBACK_TYPE.TD_RSP_QRY_CONTRACT_BANK and data:
            data = ContractBank.from_address(data)
            logger.info("Trader contract bank responsed")
            print_struct(data)
        elif t == TD_CALLBACK_TYPE.TD_RSP_QRY_PACK_ORDER:
            logger.info("Trader park order responsed")
            if data:
                data = ParkOrder.from_address(data)            
                print_struct(data)
        elif t == TD_CALLBACK_TYPE.TD_RSP_QRY_PACK_ORDER_ACTION:
            logger.info("Trader park order action responsed")
            if data:
                data = ParkOrderAction.from_address(data)            
                print_struct(data)
        elif t == TD_CALLBACK_TYPE.TD_RSP_QRY_TRADING_NOTICE:
            logger.info("Trader trading notice responsed")
            if data:
                data = TradingNotice.from_address(data)            
                print_struct(data)
        elif t == TD_CALLBACK_TYPE.TD_RSP_QRY_BROKER_TRADING_PARAMS and data:
            pass
        elif t == TD_CALLBACK_TYPE.TD_RSP_QRY_BROKER_TRADING_ALGOS and data:
            pass
        elif t == TD_CALLBACK_TYPE.TD_RSP_QRY_CFMMC_TRADING_ACCOUNT_TOKEN and data:
            pass
        elif t == TD_CALLBACK_TYPE.TD_RTN_FROM_BANK_TO_FUTURE_BY_BANK and data:
            data = TransferResponse.from_address(data)
            logger.info("Trader transfer rtn bank to future by bank responsed")
            print_struct(data)    
        elif t == TD_CALLBACK_TYPE.TD_RTN_FROM_FUTURE_TO_BANK_BY_BANK and data:
            data = TransferResponse.from_address(data)
            logger.info("Trader transfer rtn future to bank by bank responsed")
            print_struct(data)
        elif t == TD_CALLBACK_TYPE.TD_RTN_REPEAL_FROM_BANK_TO_FUTURE_BY_BANK and data:
            data = RepealResponse.from_address(data)
            logger.info("Trader rtn repeal bank to future by bank responsed")
            print_struct(data)
        elif t == TD_CALLBACK_TYPE.TD_RTN_REPEAL_FROM_FUTURE_TO_BANK_BY_BANK and data:
            data = RepealResponse.from_address(data)
            logger.info("Trader rtn repeal future to bank by bank responsed")
            print_struct(data)
        elif t == TD_CALLBACK_TYPE.TD_RTN_FROM_BANK_TO_FUTURE_BY_FUTURE and data:
            data = TransferResponse.from_address(data)
            logger.info("Trader transfer rtn bank to future responsed")
            print_struct(data)            
        elif t == TD_CALLBACK_TYPE.TD_RTN_FROM_FUTURE_TO_BANK_BY_FUTURE and data:
            data = TransferResponse.from_address(data)
            logger.info("Trader transfer rtn future to bank responsed")
            print_struct(data)
        elif t == TD_CALLBACK_TYPE.TD_RTN_REPEAL_FROM_BANK_TO_FUTURE_BY_FUTURE_MANUAL and data:
            data = RepealResponse.from_address(data)
            logger.info("Trader rtn repeal bank to future by manual responsed")
            print_struct(data)
        elif t == TD_CALLBACK_TYPE.TD_RTN_REPEAL_FROM_FUTURE_TO_BANK_BY_FUTURE_MANUAL and data:
            data = RepealResponse.from_address(data)
            logger.info("Trader rtn repeal future to bank by manual responsed")
            print_struct(data)
        elif t == TD_CALLBACK_TYPE.TD_RTN_QUERY_BANK_BALANCE_BY_FUTURE and data:
            data = BankBalance.from_address(data)
            logger.info("Trader transfer rtn bank balance responsed")
            print_struct(data)
        elif t == TD_CALLBACK_TYPE.TD_ERR_RTN_BANK_TO_FUTURE_BY_FUTURE and data:
            data = TransferRequest.from_address(data)
            logger.info("Trader transfer err rtn bank to future responsed")
            print_struct(data) 
        elif t == TD_CALLBACK_TYPE.TD_ERR_RTN_FUTURE_TO_BANK_BY_FUTURE and data:
            data = TransferRequest.from_address(data)
            logger.info("Trader transfer err rtn future to bank responsed")
            print_struct(data) 
        elif t == TD_CALLBACK_TYPE.TD_ERR_RTN_REPEAL_BANK_TO_FUTURE_BY_FUTURE_MANUAL and data:
            data = RepealRequest.from_address(data)
            logger.info("Trader err rtn repeal bank to future responsed")
            print_struct(data)
        elif t == TD_CALLBACK_TYPE.TD_ERR_RTN_REPEAL_FUTURE_TO_BANK_BY_FUTURE_MANUAL and data:
            data = RepealRequest.from_address(data)
            logger.info("Trader err rtn repeal future to bank responsed")
            print_struct(data)
        elif t == TD_CALLBACK_TYPE.TD_ERR_RTN_QRY_BANK_BALANCE_BY_FUTURE and data:
            data = BankRequest.from_address(data)
            logger.info("Trader err rtn bank balance responsed")
            print_struct(data)
        elif t == TD_CALLBACK_TYPE.TD_RTN_REPEAL_FROM_BANK_TO_FUTURE_BY_FUTURE and data:
            data = RepealResponse.from_address(data)
            logger.info("Trader rtn repeal bank to future responsed")
            print_struct(data)
        elif t == TD_CALLBACK_TYPE.TD_RTN_REPEAL_FROM_FUTURE_TO_BANK_BY_FUTURE and data:
            data = RepealResponse.from_address(data)
            logger.info("Trader rtn repeal future to bank responsed")
            print_struct(data)
        elif t == TD_CALLBACK_TYPE.TD_RSP_FROM_BANK_TO_FUTURE_BY_FUTURE and data:
            data = TransferRequest.from_address(data)
            logger.info("Trader transfer rsp bank to future responsed")
            print_struct(data)           
        elif t == TD_CALLBACK_TYPE.TD_RSP_FROM_FUTURE_TO_BANK_BY_FUTURE and data:
            data = TransferRequest.from_address(data)
            logger.info("Trader transfer rsp future to bank responsed")
            print_struct(data) 
        elif t == TD_CALLBACK_TYPE.TD_RSP_QRY_BANK_ACCOUNT_MONEY_BY_FUTURE and data:
            data = BankRequest.from_address(data)
            logger.info("Trader transfer rsp bank account money responsed")
            print_struct(data)
        elif t == TD_CALLBACK_TYPE.TD_RTN_OPEN_ACCOUNT_BY_BANK and data:
            pass
        elif t == TD_CALLBACK_TYPE.TD_RTN_CANCEL_ACCOUNT_BY_BANK and data:
            pass
        elif t == TD_CALLBACK_TYPE.TD_RTN_CHANGE_ACCOUNT_BY_BANK and data:
            pass
        else:
            print "Trader handler", t
        return None