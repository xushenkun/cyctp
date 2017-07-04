#distutils: language = c++

cdef extern from "ThostFtdcUserApiDataType.h":
    cpdef enum THOST_TE_RESUME_TYPE:
        THOST_TERT_RESTART = 0
        THOST_TERT_RESUME
        THOST_TERT_QUICK

cdef extern from "Util.h":
    cpdef enum TD_CALLBACK_TYPE:
        TD_FRONT_CONNECTED = 0
        TD_FRONT_DISCONNECTED
        TD_HEART_BEAT_WARN
        TD_RSP_AUTH
        TD_RSP_USER_LOGIN
        TD_RSP_USER_LOGOUT
        TD_RSP_PWD_UPDATE
        TD_RSP_TRADING_PWD_UPDATE
        TD_RSP_ORDER_INSERT
        TD_RSP_PACK_ORDER_INSERT
        TD_RSP_PACK_ORDER_ACTION
        TD_RSP_ORDER_ACTION
        TD_RSP_QRY_MAX_ORDER_VOLUME
        TD_RSP_SETTLE_INFO_CONFIRM
        TD_RSP_REMOVE_PACK
        TD_RSP_REMOVE_PACK_ACTION
        TD_RSP_EXEC_ORDER_INSERT
        TD_RSP_EXEC_ORDER_ACTION
        TD_RSP_FOR_QUOTE_INSERT
        TD_RSP_QUOTE_INSERT
        TD_RSP_QUOTE_ACTION
        TD_RSP_BATCH_ORDER_ACTION
        TD_RSP_COMB_ACTION_INSERT
        TD_RSP_QRY_ORDER
        TD_RSP_QRY_TRADE
        TD_RSP_QRY_INVESTOR_POS
        TD_RSP_QRY_TRADING_ACCOUNT
        TD_RSP_QRY_INVESTOR
        TD_RSP_QRY_TRADING_CODE
        TD_RSP_QRY_INSTRUMENT_MARGIN_RATE
        TD_RSP_QRY_INSTRUMENT_COMMISSION_RATE
        TD_RSP_QRY_EXCHANGE
        TD_RSP_QRY_PRODUCT
        TD_RSP_QRY_INSTRUMENT
        TD_RSP_QRY_DEPTH_MARKET_DATA
        TD_RSP_QRY_SETTLE_INFO
        TD_RSP_QRY_TRANSFER_BANK
        TD_RSP_QRY_INVESTOR_POS_DETAIL
        TD_RSP_QRY_NOTICE
        TD_RSP_QRY_SETTLE_INFO_CONFIRM
        TD_RSP_QRY_INVESTOR_POS_COMB_DETAIL
        TD_RSP_QRY_CFMMC_TRADING_ACCOUNT_KEY
        TD_RSP_QRY_EWARRANT_OFFSET
        TD_RSP_QRY_INVESTOR_PRODUCT_GROUP_MARGIN
        TD_RSP_QRY_EXCHANGE_MARGIN_RATE
        TD_RSP_QRY_EXCHANGE_MARGIN_RATE_ADJUST
        TD_RSP_QRY_EXCHANGE_RATE
        TD_RSP_QRY_SEC_AGENT_ACID_MAP
        TD_RSP_QRY_PRODUCT_EXCH_RATE
        TD_RSP_QRY_PRODUCT_GROUP
        TD_RSP_QRY_MM_INSTRUMENT_COMMISSION_RATE
        TD_RSP_QRY_MM_OPTION_INSTR_COMM_RATE
        TD_RSP_QRY_INSTRUMENT_ORDER_COMM_RATE
        TD_RSP_QRY_OPTION_INSTR_TRADE_COST
        TD_RSP_QRY_OPTION_INSTR_COMM_RATE
        TD_RSP_QRY_EXEC_ORDER
        TD_RSP_QRY_FOR_QUOTE
        TD_RSP_QRY_QUOTE
        TD_RSP_QRY_COMB_INSTRUMENT_GUARD
        TD_RSP_QRY_COMB_ACTION
        TD_RSP_QRY_TRANSFER_SERIAL
        TD_RSP_QRY_ACCOUNT_REGISTER
        TD_RSP_ERROR
        TD_RTN_ORDER
        TD_RTN_TRADE
        TD_ERR_RTN_ORDER_INSERT
        TD_ERR_RTN_ORDER_ACTION
        TD_RTN_INSTRUMENT_STATUS
        TD_RTN_BULLETIN
        TD_RTN_TRADING_NOTICE
        TD_RTN_ERROR_CONDITIONAL_ORDER
        TD_RTN_EXEC_ORDER
        TD_ERR_RTN_EXEC_ORDER_INSERT
        TD_ERR_RTN_EXEC_ORDER_ACTION
        TD_ERR_RTN_FOR_QUOTE_INSERT
        TD_RTN_QUOTE
        TD_ERR_RTN_QUOTE_INSERT
        TD_ERR_RTN_QUOTE_ACTION
        TD_RTN_FOR_QUOTE_RSP
        TD_RTN_CFMMC_TRADING_ACCOUNT_TOKEN
        TD_ERR_RTN_BATCH_ORDER_ACTION
        TD_RTN_COMB_ACTION
        TD_ERR_RTN_COMB_ACTION_INSERT
        TD_RSP_QRY_CONTRACT_BANK
        TD_RSP_QRY_PACK_ORDER
        TD_RSP_QRY_PACK_ORDER_ACTION
        TD_RSP_QRY_TRADING_NOTICE
        TD_RSP_QRY_BROKER_TRADING_PARAMS
        TD_RSP_QRY_BROKER_TRADING_ALGOS
        TD_RSP_QRY_CFMMC_TRADING_ACCOUNT_TOKEN
        TD_RTN_FROM_BANK_TO_FUTURE_BY_BANK
        TD_RTN_FROM_FUTURE_TO_BANK_BY_BANK
        TD_RTN_REPEAL_FROM_BANK_TO_FUTURE_BY_BANK
        TD_RTN_REPEAL_FROM_FUTURE_TO_BANK_BY_BANK
        TD_RTN_FROM_BANK_TO_FUTURE_BY_FUTURE
        TD_RTN_FROM_FUTURE_TO_BANK_BY_FUTURE
        TD_RTN_REPEAL_FROM_BANK_TO_FUTURE_BY_FUTURE_MANUAL
        TD_RTN_REPEAL_FROM_FUTURE_TO_BANK_BY_FUTURE_MANUAL
        TD_RTN_QUERY_BANK_BALANCE_BY_FUTURE
        TD_ERR_RTN_BANK_TO_FUTURE_BY_FUTURE
        TD_ERR_RTN_FUTURE_TO_BANK_BY_FUTURE
        TD_ERR_RTN_REPEAL_BANK_TO_FUTURE_BY_FUTURE_MANUAL
        TD_ERR_RTN_REPEAL_FUTURE_TO_BANK_BY_FUTURE_MANUAL
        TD_ERR_RTN_QRY_BANK_BALANCE_BY_FUTURE
        TD_RTN_REPEAL_FROM_BANK_TO_FUTURE_BY_FUTURE
        TD_RTN_REPEAL_FROM_FUTURE_TO_BANK_BY_FUTURE
        TD_RSP_FROM_BANK_TO_FUTURE_BY_FUTURE
        TD_RSP_FROM_FUTURE_TO_BANK_BY_FUTURE
        TD_RSP_QRY_BANK_ACCOUNT_MONEY_BY_FUTURE
        TD_RTN_OPEN_ACCOUNT_BY_BANK
        TD_RTN_CANCEL_ACCOUNT_BY_BANK
        TD_RTN_CHANGE_ACCOUNT_BY_BANK
    ctypedef int (*CbTdFunc)(void *context, TD_CALLBACK_TYPE type, void* data, int rid, int last)

cdef extern from "ThostFtdcUserApiStruct.h":
    struct CThostFtdcInputOrderField:
        pass
    struct CThostFtdcParkedOrderField:
        pass
    struct CThostFtdcInputOrderActionField:
        pass
    struct CThostFtdcParkedOrderActionField:
        pass

cdef extern from "TraderCollector.h" nogil:
    cdef cppclass CThostFtdcTraderSpi:
        pass
    cdef cppclass CTraderCollector(CThostFtdcTraderSpi):
        CTraderCollector(const char *flowPath)
        int Connect(char *frontPaths[], size_t nCount, THOST_TE_RESUME_TYPE priResume, THOST_TE_RESUME_TYPE pubResume)
        @staticmethod
        const char *Version()
        int Auth(const char *broker, const char *user, const char *product, const char *code)
        int Login(const char *broker, const char *brokerBranch, const char *user, const char *pwd)
        int Logout() 
        const char *GetTradingDay()       
        int ChangeUserPwd(const char *opwd, const char *npwd)
        int ChangeTradingPwd(const char *opwd, const char *npwd, const char *currency)
        int InsertOrder(CThostFtdcInputOrderField *pInputOrder)
        int ActionOrder(CThostFtdcInputOrderActionField *pInputOrderAction)
        int InsertParkOrder(CThostFtdcParkedOrderField *pParkedOrder)
        int ActionParkOrder(CThostFtdcParkedOrderActionField *pParkedOrderAction)        
        int QryMaxOrderVolume(const char *iid)
        int ConfirmSettleInfo()
        
        int RemovePark(const char *pid)
        int RemoveParkAction(const char *rpid)
        #int ExecOrderInsert(CThostFtdcInputExecOrderField *pInputExecOrder)
        #int ExecOrderAction(CThostFtdcInputExecOrderActionField *pInputExecOrderAction)

        int AskQuote(const char *iid, const char *ref)
        int AnswerQuote(const char *iid, double buy_price, double sell_price, int buy_volume, int sell_volume)

        #int ActionQuote(CThostFtdcInputQuoteActionField *pInputQuoteAction)
        #int BatchOrderAction(CThostFtdcInputBatchOrderActionField *pInputBatchOrderAction)
        #int InsertCombAction(CThostFtdcInputCombActionField *pInputCombAction)

        int QryOrder()
        int QryTrade(const char *iid, const char *eid, const char *tid)
        int QryInvestorPos(const char *iid)
        int QryTradingAccount()
        int QryInvestor()
        int QryTradingCode(const char *eid, const char *cid, const char *cidt)
        int QryInstrumentMarginRate(const char *iid, const char *hedgeFlag)
        int QryInstrumentCommissionRate(const char *iid)
        int QryExchange(const char *eid)
        int QryProduct(const char *pid, const char *pcls)
        int QryInstrument(const char *iid, const char *eid, const char *pid)
        int QryMarketData(const char *iid)
        int QrySettleInfo(const char *day)
        int QryTransferBank(const char *bid, const char *bbid)
        int QryInvestorPosDetail(const char *iid)
        int QryNotice()
        int QrySettleInfoConfirm()
        int QryInvestorPosCombDetail(const char *ciid)

        #int QryCFMMCTradingAccountKey(CThostFtdcQryCFMMCTradingAccountKeyField *pQryCFMMCTradingAccountKey)
        #int QryEWarrantOffset(CThostFtdcQryEWarrantOffsetField *pQryEWarrantOffset)
        #int QryInvestorProductGroupMargin(CThostFtdcQryInvestorProductGroupMarginField *pQryInvestorProductGroupMargin)
        int QryExchangeMarginRate(const char *iid, const char *hedgeFlag)
        int QryExchangeMarginRateAdjust(const char *iid, const char *hedgeFlag)
        int QryCurrencyRate(const char *fcid, const char *tcid)
        #int QrySecAgentACIDMap(CThostFtdcQrySecAgentACIDMapField *pQrySecAgentACIDMap)
        int QryProductRate(const char *pid)
        int QryProductGroup(const char *pid, const char *eid)
        #int QryMMInstrumentCommissionRate(CThostFtdcQryMMInstrumentCommissionRateField *pQryMMInstrumentCommissionRate)
        #int QryMMOptionInstrCommRate(CThostFtdcQryMMOptionInstrCommRateField *pQryMMOptionInstrCommRate)
        #int QryInstrumentOrderCommRate(CThostFtdcQryInstrumentOrderCommRateField *pQryInstrumentOrderCommRate)
        #int QryOptionInstrTradeCost(CThostFtdcQryOptionInstrTradeCostField *pQryOptionInstrTradeCost)
        #int QryOptionInstrCommRate(CThostFtdcQryOptionInstrCommRateField *pQryOptionInstrCommRate)
        #int QryExecOrder(CThostFtdcQryExecOrderField *pQryExecOrder)
        #int QryForQuote(CThostFtdcQryForQuoteField *pQryForQuote)
        #int QryQuote(CThostFtdcQryQuoteField *pQryQuote)
        #int QryCombInstrumentGuard(CThostFtdcQryCombInstrumentGuardField *pQryCombInstrumentGuard)
        #int QryCombAction(CThostFtdcQryCombActionField *pQryCombAction)

        int QryTransferSerial(const char *bid)
        int QryAccountRegister(const char *bid)
        int QryContractBank(const char *bid, const char *bbid)
        int QryParkOrder(const char *iid, const char *eid)
        int QryParkOrderAction(const char *iid, const char *eid)

        int QryTradingNotice()
        #int QryBrokerTradingParams(CThostFtdcQryBrokerTradingParamsField *pQryBrokerTradingParams)
        #int QryBrokerTradingAlgos(CThostFtdcQryBrokerTradingAlgosField *pQryBrokerTradingAlgos)
        #int QryCFMMCTradingAccountToken(CThostFtdcQueryCFMMCTradingAccountTokenField *pQueryCFMMCTradingAccountToken)

        int DepositMoney(const char *bid, const char *bbid, const char *day, double amount)
        int WithdrawMoney(const char *bid, const char *bbid, const char *day, double amount)
        int QryBankMoney(const char *bid, const char *bbid, const char *day)

        int ReqAllInstrument()
        void RegisterCb(void *context, CbTdFunc f)
        int Close()