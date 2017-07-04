# -*- coding: utf-8 -*-

from ctypes import *
import inspect, sys

class LoginUser(Structure):
    _fields_ = [('TradingDay',               c_char * 9),
                ('LoginTime',                c_char * 9),  
                ('BrokerID',                 c_char * 11),          
                ('UserID',                   c_char * 16),      
                ('SystemName',               c_char * 41),   
                ('FrontID',                  c_int32),     
                ('SessionID',                c_int32),           
                ('MaxOrderRef',              c_char * 13),  
                ('SHFETime',                 c_char * 9),          
                ('DCETime',                  c_char * 9),      
                ('CZCETime',                 c_char * 9),    
                ('FFEXTime',                 c_char * 9),               
                ('INETime',                  c_char * 9)             
    ]

class MarketData(Structure):
    _fields_ = [('TradingDay',          c_char * 9),
                ('InstrumentID',        c_char * 31),
                ('ExchangeID',          c_char * 9),
                ('ExchangeInstID',      c_char * 31),
                ('LastPrice',           c_double),
                ('PreSettlementPrice',  c_double),
                ('PreClosePrice',       c_double),
                ('PreOpenInterest',     c_double),
                ('OpenPrice',           c_double),
                ('HighestPrice',        c_double),
                ('LowestPrice',         c_double),
                ('Volume',              c_int),
                ('Turnover',            c_double),
                ('OpenInterest',        c_double),
                ('ClosePrice',          c_double),
                ('SettlementPrice',     c_double),
                ('UpperLimitPrice',     c_double),
                ('LowerLimitPrice',     c_double),
                ('PreDelta',            c_double),
                ('CurrDelta',           c_double),
                ('UpdateTime',          c_char * 9),
                ('UpdateMillisec',      c_int32),
                ('BidPrice1',           c_double),
                ('BidVolume1',          c_int32),
                ('AskPrice1',           c_double),
                ('AskVolume1',          c_int32),
                ('BidPrice2',           c_double),
                ('BidVolume2',          c_int32),
                ('AskPrice2',           c_double),
                ('AskVolume2',          c_int32),
                ('BidPrice3',           c_double),
                ('BidVolume3',          c_int32),
                ('AskPrice3',           c_double),
                ('AskVolume3',          c_int32),
                ('BidPrice4',           c_double),
                ('BidVolume4',          c_int32),
                ('AskPrice4',           c_double),
                ('AskVolume4',          c_int32),
                ('BidPrice5',           c_double),
                ('BidVolume5',          c_int32),
                ('AskPrice5',           c_double),
                ('AskVolume5',          c_int32),
                ('AveragePrice',        c_double),
                ('ActionDay',           c_char * 9)
    ]

class AccountRegister(Structure):
    _fields_ = [('TradingDay',          c_char * 9),
                ('BankID',              c_char * 4),
                ('BankBranchID',        c_char * 5),
                ('BankAccount',         c_char * 41),
                ('BrokerID',            c_char * 11),
                ('BrokerBranchID',      c_char * 31),
                ('AccountID',           c_char * 13),
                ('IdCardType',          c_char),
                ('IdentifiedCardNo',    c_char * 51),
                ('CustomerName',        c_char * 51),
                ('CurrencyID',          c_char * 4),
                ('OpenOrDestroy',       c_char),
                ('RegDate',             c_char * 9),
                ('OutDate',             c_char * 9),
                ('TID',                 c_int32),
                ('CustType',            c_char),
                ('BankAccType',         c_char),
                ('LongCustomerName',    c_char * 161)
    ]

class TransferBank(Structure):
    _fields_ = [('BankID',              c_char * 4),
                ('BankBrchID',          c_char * 5),
                ('BankName',            c_char * 101),
                ('IsActive',            c_int32)
    ]

class ContractBank(Structure):
    _fields_ = [('BrokerID',            c_char * 11),
                ('BankID',              c_char * 4),
                ('BankBrchID',          c_char * 5),
                ('BankName',            c_char * 101)
    ]

class TradeCode(Structure):
    _fields_ = [('InvestorID',          c_char * 13),
                ('BrokerID',            c_char * 11),
                ('ExchangeID',          c_char * 9),
                ('ClientID',            c_char * 11),
                ('IsActive',            c_int32),
                ('ClientIDType',        c_char)
    ]

class TradeAccount(Structure):
    _fields_ = [('BrokerID',                        c_char * 11),
                ('AccountID',                       c_char * 13), 
                ('PreMortgage',                     c_double),
                ('PreCredit',                       c_double),
                ('PreDeposit',                      c_double),
                ('PreBalance',                      c_double),
                ('PreMargin',                       c_double),
                ('InterestBase',                    c_double),
                ('Interest',                        c_double),
                ('Deposit',                         c_double),
                ('Withdraw',                        c_double),
                ('FrozenMargin',                    c_double),
                ('FrozenCash',                      c_double),
                ('FrozenCommission',                c_double),
                ('CurrMargin',                      c_double),
                ('CashIn',                          c_double),
                ('Commission',                      c_double),
                ('CloseProfit',                     c_double),
                ('PositionProfit',                  c_double),
                ('Balance',                         c_double),
                ('Available',                       c_double),
                ('WithdrawQuota',                   c_double),
                ('Reserve',                         c_double),
                ('TradingDay',                      c_char * 9),
                ('SettlementID',                    c_int32),
                ('Credit',                          c_double),
                ('Mortgage',                        c_double),
                ('ExchangeMargin',                  c_double),
                ('DeliveryMargin',                  c_double),
                ('ExchangeDeliveryMargin',          c_double),
                ('ReserveBalance',                  c_double),
                ('CurrencyID',                      c_char * 4),
                ('PreFundMortgageIn',               c_double),
                ('PreFundMortgageOut',              c_double),
                ('FundMortgageIn',                  c_double),
                ('FundMortgageOut',                 c_double),
                ('FundMortgageAvailable',           c_double),
                ('MortgageableFund',                c_double),
                ('SpecProductMargin',               c_double),
                ('SpecProductFrozenMargin',         c_double),
                ('SpecProductCommission',           c_double),
                ('SpecProductFrozenCommission',     c_double),
                ('SpecProductPositionProfit',       c_double),
                ('SpecProductCloseProfit',          c_double),
                ('SpecProductPositionProfitByAlg',  c_double),
                ('SpecProductExchangeMargin',       c_double)
    ]

class RsponseInfo(Structure):
    _fields_ = [('ErrorID',                  c_int32),
                ('ErrorMsg',                 c_char * 81)
    ]    

class InstrumentID(Structure):
    _fields_ = [('InstrumentID',            c_char * 31)
    ]

class InstrumentStatus(Structure):
    _fields_ = [('ExchangeID',              c_char * 9),
                ('ExchangeInstID',          c_char * 31),
                ('SettlementGroupID',       c_char * 9),
                ('InstrumentID',            c_char * 31),
                ('InstrumentStatus',        c_char),
                ('TradingSegmentSN',        c_int32),
                ('EnterTime',               c_char * 9),
                ('EnterReason',             c_char)
    ]

class InstrumentMarginRate(InstrumentID):
    _fields_ = [('InvestorRange',           c_char),
                ('BrokerID',                c_char * 11),
                ('InvestorID',              c_char * 13),
                ('HedgeFlag',               c_char),
                ('LongMarginRatioByMoney',  c_double),
                ('LongMarginRatioByVolume', c_double),
                ('ShortMarginRatioByMoney', c_double),
                ('ShortMarginRatioByVolume',c_double),
                ('IsRelative',              c_int32)
    ]

class InstrumentCommissionRate(InstrumentID):
    _fields_ = [('InvestorRange',           c_char),
                ('BrokerID',                c_char * 11),
                ('InvestorID',              c_char * 13),
                ('OpenRatioByMoney',        c_double),
                ('OpenRatioByVolume',       c_double),
                ('CloseRatioByMoney',       c_double),
                ('CloseRatioByVolume',      c_double),
                ('CloseTodayRatioByMoney',  c_double),
                ('CloseTodayRatioByVolume', c_int32)
    ]

class ExchangeMarginRate(Structure):
    _fields_ = [('BrokerID',                c_char * 11),
                ('InstrumentID',            c_char * 31),
                ('HedgeFlag',               c_char),
                ('LongMarginRatioByMoney',  c_double),
                ('LongMarginRatioByVolume', c_double),
                ('ShortMarginRatioByMoney', c_double),
                ('ShortMarginRatioByVolume',c_double)
    ]    

class ExchangeMarginRateAdjust(ExchangeMarginRate):
    _fields_ = [('ExchLongMarginRatioByMoney',      c_double),
                ('ExchLongMarginRatioByVolume',     c_double),
                ('ExchShortMarginRatioByMoney',     c_double),
                ('ExchShortMarginRatioByVolume',    c_double),
                ('NoLongMarginRatioByMoney',        c_double),
                ('NoLongMarginRatioByVolume',       c_double),
                ('NoShortMarginRatioByMoney',       c_double),
                ('NoShortMarginRatioByVolume',      c_double)
    ]  

class CurrencyRate(Structure):
    _fields_ = [('BrokerID',                c_char * 11),
                ('FromCurrencyID',          c_char * 4),
                ('FromCurrencyUnit',        c_double),
                ('ToCurrencyID',            c_double),
                ('ExchangeRate',            c_char * 4)
    ]      

class ProductRate(Structure):
    _fields_ = [('ProductID',               c_char * 31),
                ('QuoteCurrencyID',         c_char * 4),
                ('ExchangeRate',            c_double)
    ]   

class ProductGroup(Structure):
    _fields_ = [('ProductID',               c_char * 31),
                ('ExchangeID',              c_char * 9),
                ('ProductGroupID',          c_char * 31)
    ]               

class Instrument(InstrumentID):
    _fields_ = [('ExchangeID',              c_char * 9),
                ('InstrumentName',          c_char * 21),
                ('ExchangeInstID',          c_char * 31),
                ('ProductID',               c_char * 31),
                ('ProductClass',            c_char),
                ('DeliveryYear',            c_int32),
                ('DeliveryMonth',           c_int32),
                ('MaxMarketOrderVolume',    c_int32),
                ('MinMarketOrderVolume',    c_int32),
                ('MaxLimitOrderVolume',     c_int32),
                ('MinLimitOrderVolume',     c_int32),
                ('VolumeMultiple',          c_int32),
                ('PriceTick',               c_double),
                ('CreateDate',              c_char * 9),
                ('OpenDate',                c_char * 9),
                ('ExpireDate',              c_char * 9),
                ('StartDelivDate',          c_char * 9),
                ('EndDelivDate',            c_char * 9),
                ('InstLifePhase',           c_char),
                ('IsTrading',               c_int32),
                ('PositionType',            c_char),
                ('PositionDateType',        c_char),
                ('LongMarginRatio',         c_double),
                ('ShortMarginRatio',        c_double),
                ('MaxMarginSideAlgorithm',  c_char),
                ('UnderlyingInstrID',       c_char * 31),
                ('StrikePrice',             c_double),
                ('OptionsType',             c_char),
                ('UnderlyingMultiple',      c_double),
                ('CombinationType',         c_char)
    ]

class Position(InstrumentID):
    _fields_ = [('BrokerID',                c_char * 11),
                ('InvestorID',              c_char * 13),
                ('PosiDirection',           c_char),
                ('HedgeFlag',               c_char),
                ('PositionDate',            c_char),
                ('YdPosition',              c_int32),
                ('Position',                c_int32),
                ('LongFrozen',              c_int32),
                ('ShortFrozen',             c_int32),
                ('LongFrozenAmount',        c_double),
                ('ShortFrozenAmount',       c_double),
                ('OpenVolume',              c_int32),
                ('CloseVolume',             c_int32),
                ('OpenAmount',              c_double),
                ('CloseAmount',             c_double),
                ('PositionCost',            c_double),
                ('PreMargin',               c_double),
                ('UseMargin',               c_double),
                ('FrozenMargin',            c_double),
                ('FrozenCash',              c_double),
                ('FrozenCommission',        c_double),
                ('CashIn',                  c_double),
                ('Commission',              c_double),
                ('CloseProfit',             c_double),
                ('PositionProfit',          c_double),
                ('PreSettlementPrice',      c_double),
                ('SettlementPrice',         c_double),
                ('TradingDay',              c_char * 9),
                ('SettlementID',            c_int32),
                ('OpenCost',                c_double),
                ('ExchangeMargin',          c_double),
                ('CombPosition',            c_int32),
                ('CombLongFrozen',          c_int32),
                ('CombShortFrozen',         c_int32),
                ('CloseProfitByDate',       c_double),
                ('CloseProfitByTrade',      c_double),
                ('TodayPosition',           c_int32),
                ('MarginRateByMoney',       c_double),
                ('MarginRateByVolume',      c_double),
                ('StrikeFrozen',            c_int32),
                ('StrikeFrozenAmount',      c_double),
                ('AbandonFrozen',           c_int32)
    ]

class PositionDetail(InstrumentID):
    _fields_ = [('BrokerID',                c_char * 11),
                ('InvestorID',              c_char * 13),                
                ('HedgeFlag',               c_char),
                ('Direction',               c_char),
                ('OpenDate',                c_char * 9),
                ('TradeID',                 c_char * 21),
                ('Volume',                  c_int32),
                ('OpenPrice',               c_double),
                ('TradingDay',              c_char * 9),
                ('SettlementID',            c_int32),
                ('TradeType',               c_char),
                ('CombInstrumentID',        c_char * 31),
                ('ExchangeID',              c_char * 9),
                ('CloseProfitByDate',       c_double),
                ('CloseProfitByTrade',      c_double),
                ('PositionProfitByDate',    c_double),
                ('PositionProfitByTrade',   c_double),
                ('Margin',                  c_double),
                ('ExchMargin',              c_double),
                ('MarginRateByMoney',       c_double),
                ('MarginRateByVolume',      c_double),
                ('LastSettlementPrice',     c_double),
                ('SettlementPrice',         c_double),
                ('CloseVolume',             c_int32),
                ('CloseAmount',             c_double)
    ]    

class PositionCombDetail(Structure):
    _fields_ = [('TradingDay',              c_char * 9),
                ('OpenDate',                c_char * 9),
                ('ExchangeID',              c_char * 9),
                ('SettlementID',            c_int32),
                ('BrokerID',                c_char * 11),
                ('InvestorID',              c_char * 13),
                ('ComTradeID',              c_char * 21),
                ('TradeType',               c_char),
                ('InstrumentID',            c_char * 31),
                ('HedgeFlag',               c_char),
                ('Direction',               c_char),                
                ('TotalAmt',                c_int32),
                ('Margin',                  c_double),
                ('ExchMargin',              c_double),
                ('MarginRateByMoney',       c_double),
                ('MarginRateByVolume',      c_double),
                ('LegID',                   c_int32),
                ('LegMultiple',             c_int32),                
                ('CombInstrumentID',        c_char * 31),
                ('TradeGroupID',            c_int32)
    ]

class AskQuoteRsp(Structure):
    _fields_ = [('TradingDay',               c_char * 9), 
                ('InstrumentID',             c_char * 31),
                ('ForQuoteSysID',            c_char * 21),
                ('ForQuoteTime',             c_char * 9),
                ('ActionDay',                c_char * 9),
                ('ExchangeID',               c_char * 9)
    ]

class AskQuote(Structure):
    _fields_ = [('BrokerID',                 c_char * 11),
                ('InvestorID',               c_char * 13),
                ('InstrumentID',             c_char * 31),
                ('ForQuoteRef',              c_char * 13),
                ('UserID',                   c_char * 16),
                ('ExchangeID',               c_char * 9),
                ('InvestUnitID',             c_char * 17),
                ('IPAddress',                c_char * 16), 
                ('MacAddress',               c_char * 21) 
    ]   

class AnswerQuote(Structure):
    _fields_ = [('BrokerID',                 c_char * 11),
                ('InvestorID',               c_char * 13),
                ('InstrumentID',             c_char * 31),
                ('QuoteRef',                 c_char * 13),
                ('UserID',                   c_char * 16),
                ('AskPrice',                 c_double),
                ('BidPrice',                 c_double),
                ('AskVolume',                c_int32),
                ('BidVolume',                c_int32),
                ('RequestID',                c_int32),
                ('BusinessUnit',             c_char * 21),
                ('AskOffsetFlag',            c_char),
                ('BidOffsetFlag',            c_char),
                ('AskHedgeFlag',             c_char),
                ('BidHedgeFlag',             c_char),
                ('AskOrderRef',              c_char * 13),
                ('BidOrderRef',              c_char * 13),
                ('ForQuoteSysID',            c_char * 21), 
                ('ExchangeID',               c_char * 9),
                ('InvestUnitID',             c_char * 17),
                ('ClientID',                 c_char * 11),
                ('IPAddress',                c_char * 16), 
                ('MacAddress',               c_char * 21) 
    ] 

class SettleInfoConfirm(Structure):
    _fields_ = [('BrokerID',                 c_char * 11),
                ('InvestorID',               c_char * 13),
                ('ConfirmDate',              c_char * 9),
                ('ConfirmTime',              c_char * 9)
    ]    

class SettleInfo(Structure):
    _fields_ = [('TradingDay',               c_char * 9),
                ('SettlementID',             c_int32),
                ('BrokerID',                 c_char * 11),
                ('InvestorID',               c_char * 13),
                ('SequenceNo',               c_int32),
                ('Content',                  c_char * 501)
    ]

class Notice(Structure):
    _fields_ = [('BrokerID',                 c_char * 11),
                ('Content',                  c_char * 501),
                ('SequenceLabel',            c_char * 2)
    ]

class TradingNotice(Structure):
    _fields_ = [('BrokerID',                 c_char * 11),
                ('InvestorRange',            c_char),
                ('InvestorID',               c_char * 13),
                ('SequenceSeries',           c_short),
                ('UserID',                   c_char * 16),
                ('SendTime',                 c_char * 9), 
                ('SequenceNo',               c_int32),
                ('FieldContent',             c_char * 501)                
    ] 

class TradingNoticeInfo(Structure):
    _fields_ = [('BrokerID',                 c_char * 11),
                ('InvestorID',               c_char * 13),
                ('SendTime',                 c_char * 9), 
                ('FieldContent',             c_char * 501),
                ('SequenceSeries',           c_short),
                ('SequenceNo',               c_int32)
    ]    

class Investor(Structure):
    _fields_ = [('InvestorID',               c_char * 13),
                ('BrokerID',                 c_char * 11),
                ('InvestorGroupID',          c_char * 13),
                ('InvestorName',             c_char * 81),
                ('IdentifiedCardType',       c_char),
                ('IdentifiedCardNo',         c_char * 51),
                ('IsActive',                 c_int32),
                ('Telephone',                c_char * 41),
                ('Address',                  c_char * 101),
                ('OpenDate',                 c_char * 9),
                ('Mobile',                   c_char * 41),
                ('CommModelID',              c_char * 13),
                ('MarginModelID',            c_char * 13)
    ]     

class Exchange(Structure):
    _fields_ = [('ExchangeID',               c_char * 9),
                ('ExchangeName',             c_char * 61),
                ('ExchangeProperty',         c_char)
    ]         

class Product(Structure):
    _fields_ = [('ProductID',                c_char * 31),
                ('ProductName',              c_char * 21),
                ('ExchangeID',               c_char * 9),
                ('ProductClass',             c_char),
                ('VolumeMultiple',           c_int32),
                ('PriceTick',                c_double),
                ('MaxMarketOrderVolume',     c_int32),
                ('MinMarketOrderVolume',     c_int32),
                ('MaxLimitOrderVolume',      c_int32),
                ('MinLimitOrderVolume',      c_int32),
                ('PositionType',             c_char),
                ('PositionDateType',         c_char),
                ('CloseDealType',            c_char),
                ('TradeCurrencyID',          c_char * 4),
                ('MortgageFundUseRange',     c_char),
                ('ExchangeProductID',        c_char * 31),
                ('UnderlyingMultiple',       c_double)
    ]      

class MaxVolume(Structure):
    _fields_ = [('BrokerID',                 c_char * 11), 
                ('InvestorID',               c_char * 13), 
                ('InstrumentID',             c_char * 31),                 
                ('Direction',                c_char),  
                ('OffsetFlag',               c_char),   
                ('HedgeFlag',                c_char),     
                ('MaxVolume',                c_int32)    
    ]

class Orderend(Structure):
    _fields_ = [('BrokerID',                 c_char * 11), 
                ('InvestorID',               c_char * 13), 
                ('InstrumentID',             c_char * 31), 
                ('OrderRef',                 c_char * 13), 
                ('UserID',                   c_char * 16), 
                ('OrderPriceType',           c_char),  
                ('Direction',                c_char),  
                ('CombOffsetFlag',           c_char * 5),  
                ('CombHedgeFlag',            c_char * 5),  
                ('LimitPrice',               c_double),   
                ('VolumeTotalOriginal',      c_int),     
                ('TimeCondition',            c_char),  
                ('GTDDate',                  c_char * 9),   
                ('VolumeCondition',          c_char),  
                ('MinVolume',                c_int),      
                ('ContingentCondition',      c_char),  
                ('StopPrice',                c_double),   
                ('ForceCloseReason',         c_char),  
                ('IsAutoSuspend',            c_int)                                
    ]

class OrderActend(Structure):
    _fields_ = [('BrokerID',                 c_char * 11), 
                ('InvestorID',               c_char * 13), 
                ('OrderActionRef',           c_int), 
                ('OrderRef',                 c_char * 13), 
                ('RequestID',                c_int), 
                ('FrontID',                  c_int),  
                ('SessionID',                c_int),  
                ('ExchangeID',               c_char * 9),  
                ('OrderSysID',               c_char * 21),  
                ('ActionFlag',               c_char),   
                ('LimitPrice',               c_double)
    ]    

class OrderRequest(Orderend):
    _fields_ = [('BusinessUnit',             c_char * 21),
                ('RequestID',                c_int),
                ('UserForceClose',           c_int), 
                ('IsSwapOrder',              c_int), 
                ('ExchangeID',               c_char * 9), 
                ('InvestUnitID',             c_char * 17),                 
                ('AccountID',                c_char * 13), 
                ('CurrencyID',               c_char * 4), 
                ('ClientID',                 c_char * 11), 
                ('IPAddress',                c_char * 16), 
                ('MacAddress',               c_char * 21) 
    ]

class OrderActionRequest(OrderActend):
    _fields_ = [('VolumeChange',             c_int),  
                ('UserID',                   c_char * 16),   
                ('InstrumentID',             c_char * 31),
                ('InvestUnitID',             c_char * 17),
                ('IPAddress',                c_char * 16), 
                ('MacAddress',               c_char * 21) 
    ]

class Order(Orderend):
    _fields_ = [('BusinessUnit',             c_char * 21),
                ('RequestID',                c_int),
                ('OrderLocalID',             c_char * 13),  
                ('ExchangeID',               c_char * 9),  
                ('ParticipantID',            c_char * 11),  
                ('ClientID',                 c_char * 11),  
                ('ExchangeInstID',           c_char * 31),  
                ('TraderID',                 c_char * 21), 
                ('InstallID',                c_int32),     
                ('OrderSubmitStatus',        c_char),   
                ('NotifySequence',           c_int32),      
                ('TradingDay',               c_char * 9),  
                ('SettlementID',             c_int32),    
                ('OrderSysID',               c_char * 21),  
                ('OrderSource',              c_char),  
                ('OrderStatus',              c_char),  
                ('OrderType',                c_char),  
                ('VolumeTraded',             c_int32),     
                ('VolumeTotal',              c_int32),      
                ('InsertDate',               c_char * 9),  
                ('InsertTime',               c_char * 9),  
                ('ActiveTime',               c_char * 9),  
                ('SuspendTime',              c_char * 9),  
                ('UpdateTime',               c_char * 9),   
                ('CancelTime',               c_char * 9),   
                ('ActiveTraderID',           c_char * 21),  
                ('ClearingPartID',           c_char * 11),
                ('SequenceNo',               c_int32), 
                ('FrontID',                  c_int32),     
                ('SessionID',                c_int32),     
                ('UserProductInfo',          c_char * 11),   
                ('StatusMsg',                c_char * 81),  
                ('UserForceClose',           c_int32),     
                ('ActiveUserID',             c_char * 16), 
                ('BrokerOrderSeq',           c_int32),         
                ('RelativeOrderSysID',       c_char * 21), 
                ('ZCETotalTradedVolume',     c_int32),     
                ('IsSwapOrder',              c_int32),     
                ('BranchID',                 c_char * 9),  
                ('InvestUnitID',             c_char * 17),  
                ('AccountID',                c_char * 13),  
                ('CurrencyID',               c_char * 4),  
                ('IPAddress',                c_char * 16),  
                ('MacAddress',               c_char * 21)    
    ] 

class OrderAction(OrderActend):
    _fields_ = [('VolumeChange',             c_int),  
                ('ActionDate',               c_char * 9),   
                ('ActionTime',               c_char * 9),
                ('TraderID',                 c_char * 21),
                ('InstallID',                c_int32), 
                ('OrderLocalID',             c_char * 13),
                ('ActionLocalID',            c_char * 13),   
                ('ParticipantID',            c_char * 11),
                ('ClientID',                 c_char * 11),
                ('BusinessUnit',             c_char * 21), 
                ('OrderActionStatus',        c_char),
                ('UserID',                   c_char * 16),
                ('StatusMsg',                c_char * 81),   
                ('InstrumentID',             c_char * 31),
                ('BranchID',                 c_char * 9),
                ('InvestUnitID',             c_char * 17), 
                ('IPAddress',                c_char * 16),  
                ('MacAddress',               c_char * 21)
    ]

class ParkOrderRequest(Orderend):
    _fields_ = [('BusinessUnit',             c_char * 21),
                ('RequestID',                c_int),
                ('UserForceClose',           c_int), 
                ('ExchangeID',               c_char * 9), 
                ('ParkedOrderID',            c_char * 13), 
                ('UserType',                 c_char), 
                ('Status',                   c_char), 
                ('ErrorID',                  c_int), 
                ('ErrorMsg',                 c_char * 81), 
                ('IsSwapOrder',              c_int), 
                ('AccountID',                c_char * 13), 
                ('CurrencyID',               c_char * 4), 
                ('ClientID',                 c_char * 11), 
                ('InvestUnitID',             c_char * 17), 
                ('IPAddress',                c_char * 16), 
                ('MacAddress',               c_char * 21) 
    ]

class ParkOrderActionRequest(OrderActend):
    _fields_ = [('VolumeChange',             c_int),  
                ('UserID',                   c_char * 16),   
                ('InstrumentID',             c_char * 31),
                ('ParkedOrderActionID',      c_char * 13),      
                ('UserType',                 c_char),  
                ('Status',                   c_char),   
                ('ErrorID',                  c_int), 
                ('ErrorMsg',                 c_char * 81), 
                ('InvestUnitID',             c_char * 17), 
                ('IPAddress',                c_char * 16), 
                ('MacAddress',               c_char * 21) 
    ]

class RemovePark(Structure):
    _fields_ = [('BrokerID',                 c_char * 11), 
                ('InvestorID',               c_char * 13), 
                ('ParkedOrderID',            c_char * 13)
    ] 

class RemoveParkAction(Structure):
    _fields_ = [('BrokerID',                 c_char * 11), 
                ('InvestorID',               c_char * 13), 
                ('ParkedOrderActionID',      c_char * 13)
    ]          

class Trade(Structure):
    _fields_ = [('BrokerID',                 c_char * 11),  
                ('InvestorID',               c_char * 13),   
                ('InstrumentID',             c_char * 31), 
                ('OrderRef',                 c_char * 13),  
                ('UserID',                   c_char * 16),  
                ('ExchangeID',               c_char * 9),   
                ('TradeID',                  c_char * 21),  
                ('Direction',                c_char),  
                ('OrderSysID',               c_char * 21),   
                ('ParticipantID',            c_char * 11),    
                ('ClientID',                 c_char * 11),       
                ('TradingRole',              c_char),   
                ('ExchangeInstID',           c_char * 31),  
                ('OffsetFlag',               c_char),   
                ('HedgeFlag',                c_char),        
                ('Price',                    c_double),   
                ('Volume',                   c_int32),     
                ('TradeDate',                c_char * 9),   
                ('TradeTime',                c_char * 9),        
                ('TradeType',                c_char), 
                ('PriceSource',              c_char),         
                ('TraderID',                 c_char * 21),   
                ('OrderLocalID',             c_char * 13), 
                ('ClearingPartID',           c_char * 11),  
                ('BusinessUnit',             c_char * 21),   
                ('SequenceNo',               c_int32),  
                ('TradingDay',               c_char * 9),  
                ('SettlementID',             c_int32),      
                ('BrokerOrderSeq',           c_int32),   
                ('TradeSource',              c_char)
    ]

class Bankend(Structure):
    _fields_ = [('TradeCode',                c_char * 7), 
                ('BankID',                   c_char * 4), 
                ('BankBranchID',             c_char * 5), 
                ('BrokerID',                 c_char * 11), 
                ('BrokerBranchID',           c_char * 31), 
                ('TradeDate',                c_char * 9),  
                ('TradeTime',                c_char * 9),  
                ('BankSerial',               c_char * 13),  
                ('TradingDay',               c_char * 9),  
                ('PlateSerial',              c_int),   
                ('LastFragment',             c_char),     
                ('SessionID',                c_int),  
                ('CustomerName',             c_char * 51),   
                ('IdCardType',               c_char),  
                ('IdentifiedCardNo',         c_char * 51),      
                ('CustType',                 c_char),  
                ('BankAccount',              c_char * 41),   
                ('BankPassWord',             c_char * 41), 
                ('AccountID',                c_char * 13), 
                ('Password',                 c_char * 41)
    ]

class Bankount(Bankend):
    _fields_ = [('FutureSerial',             c_int),
                ('InstallID',                c_int),                 
                ('UserID',                   c_char * 16),  
                ('VerifyCertNoFlag',         c_char),   
                ('CurrencyID',               c_char * 4), 
                ('Digest',                   c_char * 36),                 
                ('BankAccType',              c_char), 
                ('DeviceID',                 c_char * 3),
                ('BankSecuAccType',          c_char),
                ('BrokerIDByBank',           c_char * 33),  
                ('BankSecuAcc',              c_char * 41),   
                ('BankPwdFlag',              c_char), 
                ('SecuPwdFlag',              c_char), 
                ('OperNo',                   c_char * 17), 
                ('RequestID',                c_int), 
                ('TID',                      c_int)
    ]

class BankRequest(Bankount):
    _fields_ = [('LongCustomerName',         c_char * 161) 
    ]

class BankBalance(Bankount):
    _fields_ = [('BankUseAmount',            c_double),
                ('BankFetchAmount',          c_double),
                ('ErrorID',                  c_int),
                ('ErrorMsg',                 c_char * 81),
                ('LongCustomerName',         c_char * 161) 
    ]

class Transfer(Bankend):
    _fields_ = [('InstallID',                c_int), 
                ('FutureSerial',             c_int),
                ('UserID',                   c_char * 16),  
                ('VerifyCertNoFlag',         c_char),   
                ('CurrencyID',               c_char * 4), 
                ('TradeAmount',              c_double), 
                ('FutureFetchAmount',        c_double), 
                ('FeePayFlag',               c_char), 
                ('CustFee',                  c_double),
                ('BrokerFee',                c_double), 
                ('Message',                  c_char * 129), 
                ('Digest',                   c_char * 36), 
                ('BankAccType',              c_char), 
                ('DeviceID',                 c_char * 3),
                ('BankSecuAccType',          c_char),
                ('BrokerIDByBank',           c_char * 33),  
                ('BankSecuAcc',              c_char * 41),   
                ('BankPwdFlag',              c_char), 
                ('SecuPwdFlag',              c_char), 
                ('OperNo',                   c_char * 17), 
                ('RequestID',                c_int), 
                ('TID',                      c_int)                 
    ] 

class TransferResponse(Transfer):
    _fields_ = [('TransferStatus',           c_char),
                ('ErrorID',                  c_int),
                ('ErrorMsg',                 c_char * 81),
                ('LongCustomerName',         c_char * 161)   
    ]

class TransferRequest(Transfer):
    _fields_ = [('TransferStatus',           c_char),
                ('LongCustomerName',         c_char * 161)   
    ]

class TransferSerial(Structure):
    _fields_ = [('PlateSerial',              c_int),   
                ('TradeDate',                c_char * 9),  
                ('TradingDay',               c_char * 9),  
                ('TradeTime',                c_char * 9),  
                ('TradeCode',                c_char * 7), 
                ('SessionID',                c_int),  
                ('BankID',                   c_char * 4), 
                ('BankBranchID',             c_char * 5), 
                ('BankAccType',              c_char), 
                ('BankAccount',              c_char * 41), 
                ('BankSerial',               c_char * 13),  
                ('BrokerID',                 c_char * 11), 
                ('BrokerBranchID',           c_char * 31), 
                ('FutureAccType',            c_char), 
                ('AccountID',                c_char * 13), 
                ('InvestorID',               c_char * 13), 
                ('FutureSerial',             c_int),
                ('IdCardType',               c_char),  
                ('IdentifiedCardNo',         c_char * 51),  
                ('CurrencyID',               c_char * 4),  
                ('TradeAmount',              c_double), 
                ('CustFee',                  c_double),
                ('BrokerFee',                c_double), 
                ('AvailabilityFlag',         c_char), 
                ('OperatorCode',             c_char * 17), 
                ('BankNewAccount',           c_char * 41), 
                ('ErrorID',                  c_int),
                ('ErrorMsg',                 c_char * 81)
    ]

class Repeal(Structure):
    _anonymous_=('_transfer',)
    _fields_ = [('RepealTimeInterval',       c_int),   
                ('RepealedTimes',            c_int),  
                ('BankRepealFlag',           c_char),  
                ('BrokerRepealFlag',         c_char),  
                ('PlateRepealSerial',        c_int), 
                ('BankRepealSerial',         c_char * 13),  
                ('FutureRepealSerial',       c_int), 
                ('_transfer',                Transfer)                
    ]

class RepealRequest(Repeal):
    _fields_ = [('TransferStatus',           c_char),
                ('LongCustomerName',         c_char * 161)   
    ]

class RepealResponse(Repeal):
    _fields_ = [('TransferStatus',           c_char),
                ('ErrorID',                  c_int),
                ('ErrorMsg',                 c_char * 81),
                ('LongCustomerName',         c_char * 161)   
    ]   

def print_struct_cannot_inherent(s):
    if s:
        for field in s._fields_:
            print field[0], ":", getattr(s, field[0])

def dump_struct_cannot_inherent(s):
    content = u""
    if s:
        content = u"\t".join([str(getattr(s, field[0])).decode("gb2312") for field in s._fields_])
    return content

def print_struct_1(s):
    if s:
        for field in dir(s):
            if not field.startswith("_"):
                print field, ":", getattr(s, field)

def dump_struct_1(s):
    content = u""
    if s:
        content = u"\t".join([str(getattr(s, field)).decode("gb2312") for field in dir(s) if not field.startswith("_")])
    return content    

def print_struct(s):
    if s:
        fields = inspect.getmembers(type(s), lambda x: hasattr(x, 'offset'))
        fields.sort(key=lambda x: x[1].offset)
        for field in fields:
            if not field[0].startswith("_"):
                print field[0], ":", str(getattr(s, field[0])).decode("gb2312")

def dump_struct(s):
    content = u""
    if s:
        fields = inspect.getmembers(type(s), lambda x: hasattr(x, 'offset'))
        fields.sort(key=lambda x: x[1].offset)
        content = u"\t".join([str(getattr(s, field[0])).decode("gb2312") for field in fields if not field[0].startswith("_")])
    return content        

def tailing_padding(S):
    fields = inspect.getmembers(S, lambda x: hasattr(x, 'offset'))
    fields.sort(key=lambda x: x[1].offset)
    real_fields = [field for field in fields if not field[0].startswith("_")]
    if not real_fields:
        return 0
    last_field = real_fields[-1][1]
    return sizeof(S)-last_field.offset-last_field.size

def check_tailing_padding():
    Structs = [obj for _, obj in inspect.getmembers(sys.modules[__name__]) if inspect.isclass(obj) and issubclass(obj, Structure) and not inspect.isabstract(obj)]
    Parent_Structs = []
    for Si in Structs:
        for Sj in Structs:
            if Si != Sj and issubclass(Sj, Si):
                Parent_Structs.append(Si)
                break
    for S in Parent_Structs:
        pads = tailing_padding(S)
        if pads != 0:
            raise RuntimeError("Parent Structure %s tailing padding is %d not zero"%(S, pads))

check_tailing_padding()