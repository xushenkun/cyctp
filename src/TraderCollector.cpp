#include <string.h>
#include <Python.h>
#include "TraderCollector.h"
#include "ThostFtdcUserApiStruct.h"

using namespace std;

int CTraderCollector::Close() {
    if(this->m_Api != NULL) {
        if (this->m_logined)
            this->Logout();
        this->m_Api->RegisterSpi(NULL);
        this->m_Api->Release();
        //this->m_Api->Join();
        this->m_Api = NULL;
    }        
    return 0;
};

int CTraderCollector::Connect(char *frontPaths[], size_t nCount, THOST_TE_RESUME_TYPE priResume, THOST_TE_RESUME_TYPE pubResume) { 
    /*
    if(this->m_Api != NULL) {
        if (this->m_logined)
            this->Logout(NULL, NULL);
    }
    */
    for(int i=0; i<nCount; i++)
        this->m_Api->RegisterFront(frontPaths[i]);   
    this->m_Api->SubscribePrivateTopic(priResume);
    this->m_Api->SubscribePublicTopic(pubResume);
    this->m_Api->Init();
    return 0;
};

const char *CTraderCollector::Version() {
    return CThostFtdcTraderApi::GetApiVersion();
};

int CTraderCollector::Login(const char *broker, const char *brokerBranch, const char *user, const char *pwd) {
    if (this->m_Api != NULL && this->m_connected) {        
        CThostFtdcReqUserLoginField req;
        memset(&req, 0, sizeof(req));
        if (broker!=NULL && strlen(broker)>0) strcpy_s(this->m_brokerID, broker);
        if (brokerBranch!=NULL && strlen(brokerBranch)>0) strcpy_s(this->m_brokerBID, brokerBranch);
        if (user!=NULL && strlen(user)>0) strcpy_s(this->m_userID, user);
        if (pwd!=NULL && strlen(pwd)>0) strcpy_s(this->m_password, pwd);
        strcpy_s(req.BrokerID, this->m_brokerID);
        strcpy_s(req.UserID, this->m_userID);
        strcpy_s(req.Password, this->m_password);
        int iResult = this->m_Api->ReqUserLogin(&req, ++(this->m_requestID));
        return iResult;
    }
    return -999;
};

const char *CTraderCollector::GetTradingDay() {
    if (this->m_Api != NULL && this->m_connected && this->m_logined) {
        return this->m_Api->GetTradingDay();
    }
    return NULL;
};

int CTraderCollector::Logout() {
    if (this->m_Api != NULL && this->m_connected && this->m_logined) {
        CThostFtdcUserLogoutField req;
        memset(&req, 0, sizeof(req));
        strcpy_s(req.BrokerID, this->m_brokerID);
        strcpy_s(req.UserID, this->m_userID);
        int iResult = this->m_Api->ReqUserLogout(&req, ++(this->m_requestID));
        return iResult;
    }
    return -999;
};

int CTraderCollector::Auth(const char *broker, const char *user, const char *product, const char *code) {
    if (this->m_Api != NULL && this->m_connected) {
        CThostFtdcReqAuthenticateField req;
        memset(&req, 0, sizeof(req));
        if (broker!=NULL && strlen(broker)>0) strcpy_s(this->m_brokerID, broker);
        if (user!=NULL && strlen(user)>0) strcpy_s(this->m_userID, user);
        strcpy_s(req.BrokerID, this->m_brokerID);
        strcpy_s(req.UserID, this->m_userID);
        if (product!=NULL && strlen(product)>0) strcpy_s(req.UserProductInfo, product);
        if (code!=NULL && strlen(code)>0) strcpy_s(req.AuthCode, code);
        int iResult = this->m_Api->ReqAuthenticate(&req, ++(this->m_requestID));
        return iResult;
    }
    return -999;
};

int CTraderCollector::ChangeUserPwd(const char *opwd, const char *npwd) {
    if (this->m_Api != NULL && this->m_connected && this->m_logined) {
        CThostFtdcUserPasswordUpdateField req;
        memset(&req, 0, sizeof(req));
        strcpy_s(req.BrokerID, this->m_brokerID);
        strcpy_s(req.UserID, this->m_userID);
        if (opwd!=NULL && strlen(opwd)>0) strcpy_s(req.OldPassword, opwd);
        if (npwd!=NULL && strlen(npwd)>0) strcpy_s(req.NewPassword, npwd);
        int iResult = this->m_Api->ReqUserPasswordUpdate(&req, ++(this->m_requestID));
        return iResult;
    }
    return -999;
};

int CTraderCollector::ChangeTradingPwd(const char *opwd, const char *npwd, const char *currency) {
    if (this->m_Api != NULL && this->m_connected && this->m_logined) {
        CThostFtdcTradingAccountPasswordUpdateField req;
        memset(&req, 0, sizeof(req));
        strcpy_s(req.BrokerID, this->m_brokerID);
        strcpy_s(req.AccountID, this->m_userID);
        if (opwd!=NULL && strlen(opwd)>0) strcpy_s(req.OldPassword, opwd);
        if (npwd!=NULL && strlen(npwd)>0) strcpy_s(req.NewPassword, npwd);
        if (currency!=NULL && strlen(currency)>0) strcpy_s(req.CurrencyID, currency);
        int iResult = this->m_Api->ReqTradingAccountPasswordUpdate(&req, ++(this->m_requestID));
        return iResult;
    }
    return -999;
};

int CTraderCollector::InsertOrder(CThostFtdcInputOrderField *pInputOrder) {
    if (this->m_Api != NULL && this->m_connected && this->m_logined && pInputOrder!=NULL) {
        strcpy_s(pInputOrder->BrokerID, this->m_brokerID);
        strcpy_s(pInputOrder->InvestorID, this->m_userID);
        strcpy_s(pInputOrder->UserID, this->m_userID);
        strcpy_s(pInputOrder->AccountID, this->m_userID);
        strcpy_s(pInputOrder->CurrencyID, "CNY");
        int iResult = this->m_Api->ReqOrderInsert(pInputOrder, ++(this->m_requestID));
        return iResult;
    }
    return -999;
};
int CTraderCollector::InsertParkOrder(CThostFtdcParkedOrderField *pParkedOrder) {
    if (this->m_Api != NULL && this->m_connected && this->m_logined && pParkedOrder!=NULL) {
        strcpy_s(pParkedOrder->BrokerID, this->m_brokerID);
        strcpy_s(pParkedOrder->InvestorID, this->m_userID);
        strcpy_s(pParkedOrder->UserID, this->m_userID);
        strcpy_s(pParkedOrder->AccountID, this->m_userID);
        strcpy_s(pParkedOrder->CurrencyID, "CNY");
        int iResult = this->m_Api->ReqParkedOrderInsert(pParkedOrder, ++(this->m_requestID));
        return iResult;
    }
    return -999;
};
int CTraderCollector::ActionOrder(CThostFtdcInputOrderActionField *pInputOrderAction) {
    if (this->m_Api != NULL && this->m_connected && this->m_logined && pInputOrderAction!=NULL) {
        strcpy_s(pInputOrderAction->BrokerID, this->m_brokerID);
        strcpy_s(pInputOrderAction->InvestorID, this->m_userID);
        strcpy_s(pInputOrderAction->UserID, this->m_userID);
        int iResult = this->m_Api->ReqOrderAction(pInputOrderAction, ++(this->m_requestID));
        return iResult;
    }
    return -999;
};
int CTraderCollector::ActionParkOrder(CThostFtdcParkedOrderActionField *pParkedOrderAction) {
    if (this->m_Api != NULL && this->m_connected && this->m_logined && pParkedOrderAction!=NULL) {
        strcpy_s(pParkedOrderAction->BrokerID, this->m_brokerID);
        strcpy_s(pParkedOrderAction->InvestorID, this->m_userID);
        strcpy_s(pParkedOrderAction->UserID, this->m_userID);
        int iResult = this->m_Api->ReqParkedOrderAction(pParkedOrderAction, ++(this->m_requestID));
        return iResult;
    }
    return -999;
};
int CTraderCollector::QryMaxOrderVolume(const char *iid) {
    if (this->m_Api != NULL && this->m_connected && this->m_logined) {
        CThostFtdcQueryMaxOrderVolumeField req;
        memset(&req, 0, sizeof(req));
        strcpy_s(req.BrokerID, this->m_brokerID);
        strcpy_s(req.InvestorID, this->m_userID);
        if (iid!=NULL && strlen(iid)>0) strcpy_s(req.InstrumentID, iid);
        int iResult = this->m_Api->ReqQueryMaxOrderVolume(&req, ++(this->m_requestID));
        return iResult;
    }
    return -999;
};
int CTraderCollector::ConfirmSettleInfo() {
    if (this->m_Api != NULL && this->m_connected && this->m_logined) {
        CThostFtdcSettlementInfoConfirmField req;
        memset(&req, 0, sizeof(req));
        strcpy_s(req.BrokerID, this->m_brokerID);
        strcpy_s(req.InvestorID, this->m_userID);
        int iResult = this->m_Api->ReqSettlementInfoConfirm(&req, ++(this->m_requestID));
        return iResult;
    }
    return -999;
};
int CTraderCollector::RemovePark(const char *pid) {
    if (this->m_Api != NULL && this->m_connected && this->m_logined) {
        CThostFtdcRemoveParkedOrderField req;
        memset(&req, 0, sizeof(req));
        strcpy_s(req.BrokerID, this->m_brokerID);
        strcpy_s(req.InvestorID, this->m_userID);
        if (pid!=NULL && strlen(pid)>0) strcpy_s(req.ParkedOrderID, pid);
        int iResult = this->m_Api->ReqRemoveParkedOrder(&req, ++(this->m_requestID));
        return iResult;
    }
    return -999;
};
int CTraderCollector::RemoveParkAction(const char *rpid) {
    if (this->m_Api != NULL && this->m_connected && this->m_logined) {
        CThostFtdcRemoveParkedOrderActionField req;
        memset(&req, 0, sizeof(req));
        strcpy_s(req.BrokerID, this->m_brokerID);
        strcpy_s(req.InvestorID, this->m_userID);
        if (rpid!=NULL && strlen(rpid)>0) strcpy_s(req.ParkedOrderActionID, rpid);
        int iResult = this->m_Api->ReqRemoveParkedOrderAction(&req, ++(this->m_requestID));
        return iResult;
    }
    return -999;
};
int CTraderCollector::ExecOrderInsert(CThostFtdcInputExecOrderField *pInputExecOrder) {
    return -999;
};
int CTraderCollector::ExecOrderAction(CThostFtdcInputExecOrderActionField *pInputExecOrderAction) {
    return -999;
};
int CTraderCollector::AskQuote(const char *iid, const char *ref) {
    if (this->m_Api != NULL && this->m_connected && this->m_logined) {
        CThostFtdcInputForQuoteField req;
        memset(&req, 0, sizeof(req));
        strcpy_s(req.BrokerID, this->m_brokerID);
        strcpy_s(req.InvestorID, this->m_userID);
        if (iid!=NULL && strlen(iid)>0) strcpy_s(req.InstrumentID, iid);
        if (ref!=NULL && strlen(ref)>0) strcpy_s(req.ForQuoteRef, ref);
        strcpy_s(req.UserID, this->m_userID);
        int iResult = this->m_Api->ReqForQuoteInsert(&req, ++(this->m_requestID));
        return iResult;
    }
    return -999;
};
int CTraderCollector::AnswerQuote(const char *iid, double buy_price, double sell_price, int buy_volume, int sell_volume) {
    if (this->m_Api != NULL && this->m_connected && this->m_logined) {
        CThostFtdcInputQuoteField req;
        memset(&req, 0, sizeof(req));
        strcpy_s(req.BrokerID, this->m_brokerID);
        strcpy_s(req.InvestorID, this->m_userID);
        if (iid!=NULL && strlen(iid)>0) strcpy_s(req.InstrumentID, iid);
        strcpy_s(req.UserID, this->m_userID);
        req.AskPrice = sell_price;
        req.BidPrice = buy_price;
        req.AskVolume = sell_volume;
        req.BidVolume = buy_volume;
        int iResult = this->m_Api->ReqQuoteInsert(&req, ++(this->m_requestID));
        return iResult;
    }
    return -999;
};
int CTraderCollector::ActionQuote(CThostFtdcInputQuoteActionField *pInputQuoteAction) {
    return -999;
};
int CTraderCollector::BatchOrderAction(CThostFtdcInputBatchOrderActionField *pInputBatchOrderAction) {
    return -999;
};
int CTraderCollector::InsertCombAction(CThostFtdcInputCombActionField *pInputCombAction) {
    return -999;
};
int CTraderCollector::QryOrder() {
    if (this->m_Api != NULL && this->m_connected && this->m_logined) {
        CThostFtdcQryOrderField req;
        memset(&req, 0, sizeof(req));
        strcpy_s(req.BrokerID, this->m_brokerID);
        strcpy_s(req.InvestorID, this->m_userID);
        int iResult = this->m_Api->ReqQryOrder(&req, ++(this->m_requestID));
        return iResult;
    }
    return -999;
};
int CTraderCollector::QryTrade(const char *iid, const char *eid, const char *tid) {
    if (this->m_Api != NULL && this->m_connected && this->m_logined) {
        CThostFtdcQryTradeField req;
        memset(&req, 0, sizeof(req));
        strcpy_s(req.BrokerID, this->m_brokerID);
        strcpy_s(req.InvestorID, this->m_userID);
        if (iid!=NULL && strlen(iid)>0) strcpy_s(req.InstrumentID, iid);
        if (eid!=NULL && strlen(eid)>0) strcpy_s(req.ExchangeID, eid);
        if (tid!=NULL && strlen(tid)>0) strcpy_s(req.TradeID, tid);
        int iResult = this->m_Api->ReqQryTrade(&req, ++(this->m_requestID));
        return iResult;
    }
    return -999;
};
int CTraderCollector::QryInvestorPos(const char *iid) {
    if (this->m_Api != NULL && this->m_connected && this->m_logined) {
        CThostFtdcQryInvestorPositionField req;
        memset(&req, 0, sizeof(req));
        strcpy_s(req.BrokerID, this->m_brokerID);
        strcpy_s(req.InvestorID, this->m_userID);
        if (iid!=NULL && strlen(iid)>0) strcpy_s(req.InstrumentID, iid);
        int iResult = this->m_Api->ReqQryInvestorPosition(&req, ++(this->m_requestID));
        return iResult;
    }
    return -999;
};
int CTraderCollector::QryTradingAccount() {
    if (this->m_Api != NULL && this->m_connected && this->m_logined) {
        CThostFtdcQryTradingAccountField req;
        memset(&req, 0, sizeof(req));
        strcpy_s(req.BrokerID, this->m_brokerID);
        strcpy_s(req.InvestorID, this->m_userID);
        int iResult = this->m_Api->ReqQryTradingAccount(&req, ++(this->m_requestID));
        return iResult;
    }
    return -999;
};
int CTraderCollector::QryInvestor() {
    if (this->m_Api != NULL && this->m_connected && this->m_logined) {
        CThostFtdcQryInvestorField req;
        memset(&req, 0, sizeof(req));
        strcpy_s(req.BrokerID, this->m_brokerID);
        strcpy_s(req.InvestorID, this->m_userID);
        int iResult = this->m_Api->ReqQryInvestor(&req, ++(this->m_requestID));
        return iResult;
    }
    return -999;
};
int CTraderCollector::QryTradingCode(const char *eid, const char *cid, const char *cidt) {
    if (this->m_Api != NULL && this->m_connected && this->m_logined) {
        CThostFtdcQryTradingCodeField req;
        memset(&req, 0, sizeof(req));
        strcpy_s(req.BrokerID, this->m_brokerID);
        strcpy_s(req.InvestorID, this->m_userID);
        if (eid!=NULL && strlen(eid)>0) strcpy_s(req.ExchangeID, eid);
        if (cid!=NULL && strlen(cid)>0) strcpy_s(req.ClientID, cid);
        if (cidt!=NULL && strlen(cidt)==1) req.ClientIDType = cidt[0];
        int iResult = this->m_Api->ReqQryTradingCode(&req, ++(this->m_requestID));
        return iResult;
    }
    return -999;
};
int CTraderCollector::QryInstrumentMarginRate(const char *iid, const char *hedgeFlag) {
    if (this->m_Api != NULL && this->m_connected && this->m_logined) {
        CThostFtdcQryInstrumentMarginRateField req;
        memset(&req, 0, sizeof(req));
        strcpy_s(req.BrokerID, this->m_brokerID);
        strcpy_s(req.InvestorID, this->m_userID);
        if (iid!=NULL && strlen(iid)>0) strcpy_s(req.InstrumentID, iid);
        if (hedgeFlag!=NULL && strlen(hedgeFlag)==1) req.HedgeFlag = hedgeFlag[0];
        int iResult = this->m_Api->ReqQryInstrumentMarginRate(&req, ++(this->m_requestID));
        return iResult;
    }
    return -999;
};
int CTraderCollector::QryInstrumentCommissionRate(const char *iid) {
    if (this->m_Api != NULL && this->m_connected && this->m_logined) {
        CThostFtdcQryInstrumentCommissionRateField req;
        memset(&req, 0, sizeof(req));
        strcpy_s(req.BrokerID, this->m_brokerID);
        strcpy_s(req.InvestorID, this->m_userID);
        if (iid!=NULL && strlen(iid)>0) strcpy_s(req.InstrumentID, iid);
        int iResult = this->m_Api->ReqQryInstrumentCommissionRate(&req, ++(this->m_requestID));
        return iResult;
    }
    return -999;
};
int CTraderCollector::QryExchange(const char *eid) {
    if (this->m_Api != NULL && this->m_connected && this->m_logined) {
        CThostFtdcQryExchangeField req;
        memset(&req, 0, sizeof(req));
        if (eid!=NULL && strlen(eid)>0) strcpy_s(req.ExchangeID, eid);
        int iResult = this->m_Api->ReqQryExchange(&req, ++(this->m_requestID));
        return iResult;
    }
    return -999;
};
int CTraderCollector::QryProduct(const char *pid, const char *pcls) {
    if (this->m_Api != NULL && this->m_connected && this->m_logined) {
        CThostFtdcQryProductField req;
        memset(&req, 0, sizeof(req));
        if (pid!=NULL && strlen(pid)>0) strcpy_s(req.ProductID, pid);
        if (pcls!=NULL && strlen(pcls)>0) req.ProductClass = pcls[0];
        int iResult = this->m_Api->ReqQryProduct(&req, ++(this->m_requestID));
        return iResult;
    }
    return -999;
};
int CTraderCollector::QryInstrument(const char *iid, const char *eid, const char *pid) {
    if (this->m_Api != NULL && this->m_connected && this->m_logined) {
        CThostFtdcQryInstrumentField req;
        memset(&req, 0, sizeof(req));
        if (iid!=NULL && strlen(iid)>0) strcpy_s(req.InstrumentID, iid);
        if (eid!=NULL && strlen(eid)>0) strcpy_s(req.ExchangeID, eid);
        if (pid!=NULL && strlen(pid)>0) strcpy_s(req.ProductID, pid);        
        int iResult = this->m_Api->ReqQryInstrument(&req, ++(this->m_requestID));
        return iResult;
    }
    return -999;
};
int CTraderCollector::QryMarketData(const char *iid) {
    if (this->m_Api != NULL && this->m_connected && this->m_logined) {
        CThostFtdcQryDepthMarketDataField req;
        memset(&req, 0, sizeof(req));
        if (iid!=NULL && strlen(iid)>0) strcpy_s(req.InstrumentID, iid);
        int iResult = this->m_Api->ReqQryDepthMarketData(&req, ++(this->m_requestID));
        return iResult;
    }
    return -999;
};
int CTraderCollector::QrySettleInfo(const char *day) {
    if (this->m_Api != NULL && this->m_connected && this->m_logined) {
        CThostFtdcQrySettlementInfoField req;
        memset(&req, 0, sizeof(req));
        strcpy_s(req.BrokerID, this->m_brokerID);
        strcpy_s(req.InvestorID, this->m_userID);
        if (day!=NULL && strlen(day)>0) strcpy_s(req.TradingDay, day);
        int iResult = this->m_Api->ReqQrySettlementInfo(&req, ++(this->m_requestID));
        return iResult;
    }
    return -999;
};
int CTraderCollector::QryTransferBank(const char *bid, const char *bbid) {
    if (this->m_Api != NULL && this->m_connected && this->m_logined) {
        CThostFtdcQryTransferBankField req;
        memset(&req, 0, sizeof(req));
        if (bid!=NULL && strlen(bid)>0) strcpy_s(req.BankID, bid);
        if (bbid!=NULL && strlen(bbid)>0) strcpy_s(req.BankBrchID, bbid);
        int iResult = this->m_Api->ReqQryTransferBank(&req, ++(this->m_requestID));
        return iResult;
    }
    return -999;
};
int CTraderCollector::QryInvestorPosDetail(const char *iid) {
    if (this->m_Api != NULL && this->m_connected && this->m_logined) {
        CThostFtdcQryInvestorPositionDetailField req;
        memset(&req, 0, sizeof(req));
        strcpy_s(req.BrokerID, this->m_brokerID);
        strcpy_s(req.InvestorID, this->m_userID);
        if (iid!=NULL && strlen(iid)>0) strcpy_s(req.InstrumentID, iid);
        int iResult = this->m_Api->ReqQryInvestorPositionDetail(&req, ++(this->m_requestID));
        return iResult;
    }
    return -999;
};
int CTraderCollector::QryNotice() {
    if (this->m_Api != NULL && this->m_connected && this->m_logined) {
        CThostFtdcQryNoticeField req;
        memset(&req, 0, sizeof(req));
        strcpy_s(req.BrokerID, this->m_brokerID);
        int iResult = this->m_Api->ReqQryNotice(&req, ++(this->m_requestID));
        return iResult;
    }
    return -999;
};
int CTraderCollector::QrySettleInfoConfirm() {
    if (this->m_Api != NULL && this->m_connected && this->m_logined) {
        CThostFtdcQrySettlementInfoConfirmField req;
        memset(&req, 0, sizeof(req));
        strcpy_s(req.BrokerID, this->m_brokerID);
        strcpy_s(req.InvestorID, this->m_userID);
        int iResult = this->m_Api->ReqQrySettlementInfoConfirm(&req, ++(this->m_requestID));
        return iResult;
    }
    return -999;
};
int CTraderCollector::QryInvestorPosCombDetail(const char *ciid) {
    if (this->m_Api != NULL && this->m_connected && this->m_logined) {
        CThostFtdcQryInvestorPositionCombineDetailField req;
        memset(&req, 0, sizeof(req));
        strcpy_s(req.BrokerID, this->m_brokerID);
        strcpy_s(req.InvestorID, this->m_userID);
        if (ciid!=NULL && strlen(ciid)>0) strcpy_s(req.CombInstrumentID, ciid);
        int iResult = this->m_Api->ReqQryInvestorPositionCombineDetail(&req, ++(this->m_requestID));
        return iResult;
    }
    return -999;
};
int CTraderCollector::QryCFMMCTradingAccountKey(CThostFtdcQryCFMMCTradingAccountKeyField *pQryCFMMCTradingAccountKey) {
    return -999;
};
int CTraderCollector::QryEWarrantOffset(CThostFtdcQryEWarrantOffsetField *pQryEWarrantOffset) {
    return -999;
};
int CTraderCollector::QryInvestorProductGroupMargin(CThostFtdcQryInvestorProductGroupMarginField *pQryInvestorProductGroupMargin) {
    return -999;
};
int CTraderCollector::QryExchangeMarginRate(const char *iid, const char *hedgeFlag) {
    if (this->m_Api != NULL && this->m_connected && this->m_logined) {
        CThostFtdcQryExchangeMarginRateField req;
        memset(&req, 0, sizeof(req));
        strcpy_s(req.BrokerID, this->m_brokerID);
        if (iid!=NULL && strlen(iid)>0) strcpy_s(req.InstrumentID, iid);
        if (hedgeFlag!=NULL && strlen(hedgeFlag)==1) req.HedgeFlag = hedgeFlag[0];
        int iResult = this->m_Api->ReqQryExchangeMarginRate(&req, ++(this->m_requestID));
        return iResult;
    }
    return -999;
};
int CTraderCollector::QryExchangeMarginRateAdjust(const char *iid, const char *hedgeFlag) {
    if (this->m_Api != NULL && this->m_connected && this->m_logined) {
        CThostFtdcQryExchangeMarginRateAdjustField req;
        memset(&req, 0, sizeof(req));
        strcpy_s(req.BrokerID, this->m_brokerID);
        if (iid!=NULL && strlen(iid)>0) strcpy_s(req.InstrumentID, iid);
        if (hedgeFlag!=NULL && strlen(hedgeFlag)==1) req.HedgeFlag = hedgeFlag[0];
        int iResult = this->m_Api->ReqQryExchangeMarginRateAdjust(&req, ++(this->m_requestID));
        return iResult;
    }
    return -999;
};
int CTraderCollector::QryCurrencyRate(const char *fcid, const char *tcid) {
    if (this->m_Api != NULL && this->m_connected && this->m_logined) {
        CThostFtdcQryExchangeRateField req;
        memset(&req, 0, sizeof(req));
        strcpy_s(req.BrokerID, this->m_brokerID);
        if (fcid!=NULL && strlen(fcid)>0) strcpy_s(req.FromCurrencyID, fcid);
        if (tcid!=NULL && strlen(tcid)==1) strcpy_s(req.ToCurrencyID, fcid);
        int iResult = this->m_Api->ReqQryExchangeRate(&req, ++(this->m_requestID));
        return iResult;
    }
    return -999;
};
int CTraderCollector::QrySecAgentACIDMap(CThostFtdcQrySecAgentACIDMapField *pQrySecAgentACIDMap) {
    return -999;
};
int CTraderCollector::QryProductRate(const char *pid) {
    if (this->m_Api != NULL && this->m_connected && this->m_logined) {
        CThostFtdcQryProductExchRateField req;
        memset(&req, 0, sizeof(req));
        if (pid!=NULL && strlen(pid)>0) strcpy_s(req.ProductID, pid);
        int iResult = this->m_Api->ReqQryProductExchRate(&req, ++(this->m_requestID));
        return iResult;
    }
    return -999;
};
int CTraderCollector::QryProductGroup(const char *pid, const char *eid) {
    if (this->m_Api != NULL && this->m_connected && this->m_logined) {
        CThostFtdcQryProductGroupField req;
        memset(&req, 0, sizeof(req));
        if (pid!=NULL && strlen(pid)>0) strcpy_s(req.ProductID, pid);
        if (eid!=NULL && strlen(eid)>0) strcpy_s(req.ExchangeID, eid);
        int iResult = this->m_Api->ReqQryProductGroup(&req, ++(this->m_requestID));
        return iResult;
    }
    return -999;
};
int CTraderCollector::QryMMInstrumentCommissionRate(CThostFtdcQryMMInstrumentCommissionRateField *pQryMMInstrumentCommissionRate) {
    return -999;
};
int CTraderCollector::QryMMOptionInstrCommRate(CThostFtdcQryMMOptionInstrCommRateField *pQryMMOptionInstrCommRate) {
    return -999;
};
int CTraderCollector::QryInstrumentOrderCommRate(CThostFtdcQryInstrumentOrderCommRateField *pQryInstrumentOrderCommRate) {
    return -999;
};
int CTraderCollector::QryOptionInstrTradeCost(CThostFtdcQryOptionInstrTradeCostField *pQryOptionInstrTradeCost) {
    return -999;
};
int CTraderCollector::QryOptionInstrCommRate(CThostFtdcQryOptionInstrCommRateField *pQryOptionInstrCommRate) {
    return -999;
};
int CTraderCollector::QryExecOrder(CThostFtdcQryExecOrderField *pQryExecOrder) {
    return -999;
};
int CTraderCollector::QryForQuote(CThostFtdcQryForQuoteField *pQryForQuote) {
    return -999;
};
int CTraderCollector::QryQuote(CThostFtdcQryQuoteField *pQryQuote) {
    return -999;
};
int CTraderCollector::QryCombInstrumentGuard(CThostFtdcQryCombInstrumentGuardField *pQryCombInstrumentGuard) {
    return -999;
};
int CTraderCollector::QryCombAction(CThostFtdcQryCombActionField *pQryCombAction) {
    return -999;
};
int CTraderCollector::QryTransferSerial(const char *bid) {
    if (this->m_Api != NULL && this->m_connected && this->m_logined) {
        CThostFtdcQryTransferSerialField req;
        memset(&req, 0, sizeof(req));
        strcpy_s(req.BrokerID, this->m_brokerID);
        strcpy_s(req.AccountID, this->m_userID);
        if (bid!=NULL && strlen(bid)>0) strcpy_s(req.BankID, bid);
        strcpy_s(req.CurrencyID, "CNY");
        int iResult = this->m_Api->ReqQryTransferSerial(&req, ++(this->m_requestID));
        return iResult;
    }
    return -999;
};
int CTraderCollector::QryAccountRegister(const char *bid) {
    if (this->m_Api != NULL && this->m_connected && this->m_logined) {
        CThostFtdcQryAccountregisterField req;
        memset(&req, 0, sizeof(req));
        strcpy_s(req.BrokerID, this->m_brokerID);
        strcpy_s(req.AccountID, this->m_userID);
        if (bid!=NULL && strlen(bid)>0) strcpy_s(req.BankID, bid);
        int iResult = this->m_Api->ReqQryAccountregister(&req, ++(this->m_requestID));
        return iResult;
    }
    return -999;
};
int CTraderCollector::QryContractBank(const char *bid, const char *bbid) {
    if (this->m_Api != NULL && this->m_connected && this->m_logined) {
        CThostFtdcQryContractBankField req;
        memset(&req, 0, sizeof(req));
        strcpy_s(req.BrokerID, this->m_brokerID);
        if (bid!=NULL && strlen(bid)>0) strcpy_s(req.BankID, bid);
        if (bbid!=NULL && strlen(bbid)>0) strcpy_s(req.BankBrchID, bbid);
        int iResult = this->m_Api->ReqQryContractBank(&req, ++(this->m_requestID));
        return iResult;
    }
    return -999;
};
int CTraderCollector::QryParkOrder(const char *iid, const char *eid) {
    if (this->m_Api != NULL && this->m_connected && this->m_logined) {
        CThostFtdcQryParkedOrderField req;
        memset(&req, 0, sizeof(req));
        strcpy_s(req.BrokerID, this->m_brokerID);
        strcpy_s(req.InvestorID, this->m_userID);
        if (iid!=NULL && strlen(iid)>0) strcpy_s(req.InstrumentID, iid);
        if (eid!=NULL && strlen(eid)>0) strcpy_s(req.ExchangeID, eid);
        int iResult = this->m_Api->ReqQryParkedOrder(&req, ++(this->m_requestID));
        return iResult;
    }
    return -999;
};
int CTraderCollector::QryParkOrderAction(const char *iid, const char *eid) {
    if (this->m_Api != NULL && this->m_connected && this->m_logined) {
        CThostFtdcQryParkedOrderActionField req;
        memset(&req, 0, sizeof(req));
        strcpy_s(req.BrokerID, this->m_brokerID);
        strcpy_s(req.InvestorID, this->m_userID);
        if (iid!=NULL && strlen(iid)>0) strcpy_s(req.InstrumentID, iid);
        if (eid!=NULL && strlen(eid)>0) strcpy_s(req.ExchangeID, eid);
        int iResult = this->m_Api->ReqQryParkedOrderAction(&req, ++(this->m_requestID));
        return iResult;
    }
    return -999;
};
int CTraderCollector::QryTradingNotice() {
    if (this->m_Api != NULL && this->m_connected && this->m_logined) {
        CThostFtdcQryTradingNoticeField req;
        memset(&req, 0, sizeof(req));
        strcpy_s(req.BrokerID, this->m_brokerID);
        strcpy_s(req.InvestorID, this->m_userID);
        int iResult = this->m_Api->ReqQryTradingNotice(&req, ++(this->m_requestID));
        return iResult;
    }
    return -999;
};
int CTraderCollector::QryBrokerTradingParams(CThostFtdcQryBrokerTradingParamsField *pQryBrokerTradingParams) {
    return -999;
};
int CTraderCollector::QryBrokerTradingAlgos(CThostFtdcQryBrokerTradingAlgosField *pQryBrokerTradingAlgos) {
    return -999;
};
int CTraderCollector::QryCFMMCTradingAccountToken(CThostFtdcQueryCFMMCTradingAccountTokenField *pQueryCFMMCTradingAccountToken) {
    return -999;
};
int CTraderCollector::DepositMoney(const char *bid, const char *bbid, const char *day, double amount) {
    if (this->m_Api != NULL && this->m_connected && this->m_logined) {
        CThostFtdcReqTransferField req;
        memset(&req, 0, sizeof(req));
        strcpy_s(req.TradeCode, "202001");
        if (bid!=NULL && strlen(bid)>0) strcpy_s(req.BankID, bid);
        if (bbid!=NULL && strlen(bbid)>0) strcpy_s(req.BankBranchID, bbid);
        strcpy_s(req.BrokerID, this->m_brokerID);
        strcpy_s(req.BrokerBranchID, this->m_brokerBID);
        strcpy_s(req.AccountID, this->m_userID);
        strcpy_s(req.Password, this->m_password);
        strcpy_s(req.UserID, this->m_userID);
        if (day!=NULL && strlen(day)>0) strcpy_s(req.TradeDate, day);
        nowtime((char *)&req.TradeTime);
        req.TradeAmount = amount;
        strcpy_s(req.CurrencyID, "CNY");
        int iResult = this->m_Api->ReqFromBankToFutureByFuture(&req, ++(this->m_requestID));
        return iResult;
    }
    return -999;
};
int CTraderCollector::WithdrawMoney(const char *bid, const char *bbid, const char *day, double amount) {
    if (this->m_Api != NULL && this->m_connected && this->m_logined) {
        CThostFtdcReqTransferField req;
        memset(&req, 0, sizeof(req));
        strcpy_s(req.TradeCode, "202002");
        if (bid!=NULL && strlen(bid)>0) strcpy_s(req.BankID, bid);
        if (bbid!=NULL && strlen(bbid)>0) strcpy_s(req.BankBranchID, bbid);
        strcpy_s(req.BrokerID, this->m_brokerID);
        strcpy_s(req.BrokerBranchID, this->m_brokerBID);
        strcpy_s(req.AccountID, this->m_userID);
        strcpy_s(req.Password, this->m_password);
        strcpy_s(req.UserID, this->m_userID);
        if (day!=NULL && strlen(day)>0) strcpy_s(req.TradeDate, day);
        nowtime((char *)&req.TradeTime);
        req.TradeAmount = amount;
        strcpy_s(req.CurrencyID, "CNY");
        int iResult = this->m_Api->ReqFromFutureToBankByFuture(&req, ++(this->m_requestID));
        return iResult;
    }
    return -999;
};
int CTraderCollector::QryBankMoney(const char *bid, const char *bbid, const char *day) {
    if (this->m_Api != NULL && this->m_connected && this->m_logined) {
        CThostFtdcReqQueryAccountField req;
        memset(&req, 0, sizeof(req));
        strcpy_s(req.TradeCode, "204002");
        if (bid!=NULL && strlen(bid)>0) strcpy_s(req.BankID, bid);
        if (bbid!=NULL && strlen(bbid)>0) strcpy_s(req.BankBranchID, bbid);
        strcpy_s(req.BrokerID, this->m_brokerID);
        strcpy_s(req.BrokerBranchID, this->m_brokerBID);
        strcpy_s(req.AccountID, this->m_userID);
        strcpy_s(req.Password, this->m_password);
        strcpy_s(req.UserID, this->m_userID);
        if (day!=NULL && strlen(day)>0) strcpy_s(req.TradeDate, day);
        nowtime((char *)&req.TradeTime);
        strcpy_s(req.CurrencyID, "CNY");
        req.SecuPwdFlag = THOST_FTDC_BPWDF_BlankCheck;
        int iResult = this->m_Api->ReqQueryBankAccountMoneyByFuture(&req, ++(this->m_requestID));
        return iResult;
    }
    return -999;
};

int CTraderCollector::ReqAllInstrument() {
    if (this->m_Api != NULL && this->m_connected && this->m_logined) {
        CThostFtdcQryInstrumentField req;
        memset(&req, 0, sizeof(req));
        int iResult = this->m_Api->ReqQryInstrument(&req, ++(this->m_requestID));
        return iResult;
    }
    return -999;
}; 

void CTraderCollector::OnFrontConnected() {
    this->m_connected = true;
    Callback(this->m_context, this->m_cb, TD_FRONT_CONNECTED, (void*)NULL, 0, 0);
};

void CTraderCollector::OnFrontDisconnected(int nReason){
    this->m_connected = false;
    Callback(this->m_context, this->m_cb, TD_FRONT_DISCONNECTED, (void*)&nReason, 0, 0);
};
    
void CTraderCollector::OnHeartBeatWarning(int nTimeLapse){
    Callback(this->m_context, this->m_cb, TD_HEART_BEAT_WARN, (void*)&nTimeLapse, 0, 0);
};

void CTraderCollector::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
    if (!isErrorRspInfo(pRspInfo, bIsLast)) {
        this->m_logined = true;
        this->m_frontID = pRspUserLogin->FrontID;
        this->m_sessionID = pRspUserLogin->SessionID;
        Callback(this->m_context, this->m_cb, TD_RSP_USER_LOGIN, (void*)pRspUserLogin, nRequestID, (int)bIsLast);
    }
};

void CTraderCollector::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
    if (!isErrorRspInfo(pRspInfo, bIsLast)) {
        this->m_logined = false;
        this->m_frontID = -1;
        this->m_sessionID = -1;
        Callback(this->m_context, this->m_cb, TD_RSP_USER_LOGOUT, (void*)pUserLogout, nRequestID, (int)bIsLast);
    }
};

void CTraderCollector::OnRspAuthenticate(CThostFtdcRspAuthenticateField *pRspAuthenticateField, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    if (!isErrorRspInfo(pRspInfo, bIsLast)) {
        Callback(this->m_context, this->m_cb, TD_RSP_AUTH, (void*)pRspAuthenticateField, nRequestID, (int)bIsLast);
    }
};

void CTraderCollector::OnRspUserPasswordUpdate(CThostFtdcUserPasswordUpdateField *pUserPasswordUpdate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    if (!isErrorRspInfo(pRspInfo, bIsLast)) {
        Callback(this->m_context, this->m_cb, TD_RSP_PWD_UPDATE, (void*)pUserPasswordUpdate, nRequestID, (int)bIsLast);
    }
};

void CTraderCollector::OnRspTradingAccountPasswordUpdate(CThostFtdcTradingAccountPasswordUpdateField *pTradingAccountPasswordUpdate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    if (!isErrorRspInfo(pRspInfo, bIsLast)) {
        Callback(this->m_context, this->m_cb, TD_RSP_TRADING_PWD_UPDATE, (void*)pTradingAccountPasswordUpdate, nRequestID, (int)bIsLast);
    }
};

void CTraderCollector::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    if (!isErrorRspInfo(pRspInfo, bIsLast)) {
        Callback(this->m_context, this->m_cb, TD_RSP_ORDER_INSERT, (void*)pInputOrder, nRequestID, (int)bIsLast);
    }
};

void CTraderCollector::OnRspParkedOrderInsert(CThostFtdcParkedOrderField *pParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    if (!isErrorRspInfo(pRspInfo, bIsLast)) {
        Callback(this->m_context, this->m_cb, TD_RSP_PACK_ORDER_INSERT, (void*)pParkedOrder, nRequestID, (int)bIsLast);
    }
};

void CTraderCollector::OnRspParkedOrderAction(CThostFtdcParkedOrderActionField *pParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    if (!isErrorRspInfo(pRspInfo, bIsLast)) {
        Callback(this->m_context, this->m_cb, TD_RSP_PACK_ORDER_ACTION, (void*)pParkedOrderAction, nRequestID, (int)bIsLast);
    }
};

void CTraderCollector::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    if (!isErrorRspInfo(pRspInfo, bIsLast)) {
        Callback(this->m_context, this->m_cb, TD_RSP_ORDER_ACTION, (void*)pInputOrderAction, nRequestID, (int)bIsLast);
    }
};

void CTraderCollector::OnRspQueryMaxOrderVolume(CThostFtdcQueryMaxOrderVolumeField *pQueryMaxOrderVolume, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    if (!isErrorRspInfo(pRspInfo, bIsLast)) {
        Callback(this->m_context, this->m_cb, TD_RSP_QRY_MAX_ORDER_VOLUME, (void*)pQueryMaxOrderVolume, nRequestID, (int)bIsLast);
    }
};

void CTraderCollector::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    if (!isErrorRspInfo(pRspInfo, bIsLast)) {
        Callback(this->m_context, this->m_cb, TD_RSP_SETTLE_INFO_CONFIRM, (void*)pSettlementInfoConfirm, nRequestID, (int)bIsLast);
    }
};

void CTraderCollector::OnRspRemoveParkedOrder(CThostFtdcRemoveParkedOrderField *pRemoveParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    if (!isErrorRspInfo(pRspInfo, bIsLast)) {
        Callback(this->m_context, this->m_cb, TD_RSP_REMOVE_PACK, (void*)pRemoveParkedOrder, nRequestID, (int)bIsLast);
    }
};

void CTraderCollector::OnRspRemoveParkedOrderAction(CThostFtdcRemoveParkedOrderActionField *pRemoveParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    if (!isErrorRspInfo(pRspInfo, bIsLast)) {
        Callback(this->m_context, this->m_cb, TD_RSP_REMOVE_PACK_ACTION, (void*)pRemoveParkedOrderAction, nRequestID, (int)bIsLast);
    }
};

void CTraderCollector::OnRspExecOrderInsert(CThostFtdcInputExecOrderField *pInputExecOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
};

void CTraderCollector::OnRspExecOrderAction(CThostFtdcInputExecOrderActionField *pInputExecOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
};

void CTraderCollector::OnRspForQuoteInsert(CThostFtdcInputForQuoteField *pInputForQuote, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    if (!isErrorRspInfo(pRspInfo, bIsLast)) {
        Callback(this->m_context, this->m_cb, TD_RSP_FOR_QUOTE_INSERT, (void*)pInputForQuote, nRequestID, (int)bIsLast);
    }
};

void CTraderCollector::OnRspQuoteInsert(CThostFtdcInputQuoteField *pInputQuote, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    if (!isErrorRspInfo(pRspInfo, bIsLast)) {
        Callback(this->m_context, this->m_cb, TD_RSP_QUOTE_INSERT, (void*)pInputQuote, nRequestID, (int)bIsLast);
    }
};

void CTraderCollector::OnRspQuoteAction(CThostFtdcInputQuoteActionField *pInputQuoteAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
};

void CTraderCollector::OnRspBatchOrderAction(CThostFtdcInputBatchOrderActionField *pInputBatchOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
};

void CTraderCollector::OnRspCombActionInsert(CThostFtdcInputCombActionField *pInputCombAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
};

void CTraderCollector::OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    if (!isErrorRspInfo(pRspInfo, bIsLast)) {
        Callback(this->m_context, this->m_cb, TD_RSP_QRY_ORDER, (void*)pOrder, nRequestID, (int)bIsLast);
    }
};

void CTraderCollector::OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    if (!isErrorRspInfo(pRspInfo, bIsLast)) {
        Callback(this->m_context, this->m_cb, TD_RSP_QRY_TRADE, (void*)pTrade, nRequestID, (int)bIsLast);
    }
};

void CTraderCollector::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    if (!isErrorRspInfo(pRspInfo, bIsLast)) {
        Callback(this->m_context, this->m_cb, TD_RSP_QRY_INVESTOR_POS, (void*)pInvestorPosition, nRequestID, (int)bIsLast);
    }
};

void CTraderCollector::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    if (!isErrorRspInfo(pRspInfo, bIsLast)) {
        Callback(this->m_context, this->m_cb, TD_RSP_QRY_TRADING_ACCOUNT, (void*)pTradingAccount, nRequestID, (int)bIsLast);
    }
};

void CTraderCollector::OnRspQryInvestor(CThostFtdcInvestorField *pInvestor, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    if (!isErrorRspInfo(pRspInfo, bIsLast)) {
        Callback(this->m_context, this->m_cb, TD_RSP_QRY_INVESTOR, (void*)pInvestor, nRequestID, (int)bIsLast);
    }
};

void CTraderCollector::OnRspQryTradingCode(CThostFtdcTradingCodeField *pTradingCode, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    if (!isErrorRspInfo(pRspInfo, bIsLast)) {
        Callback(this->m_context, this->m_cb, TD_RSP_QRY_TRADING_CODE, (void*)pTradingCode, nRequestID, (int)bIsLast);
    }
};

void CTraderCollector::OnRspQryInstrumentMarginRate(CThostFtdcInstrumentMarginRateField *pInstrumentMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    if (!isErrorRspInfo(pRspInfo, bIsLast)) {
        Callback(this->m_context, this->m_cb, TD_RSP_QRY_INSTRUMENT_MARGIN_RATE, (void*)pInstrumentMarginRate, nRequestID, (int)bIsLast);
    }
};

void CTraderCollector::OnRspQryInstrumentCommissionRate(CThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    if (!isErrorRspInfo(pRspInfo, bIsLast)) {
        Callback(this->m_context, this->m_cb, TD_RSP_QRY_INSTRUMENT_COMMISSION_RATE, (void*)pInstrumentCommissionRate, nRequestID, (int)bIsLast);
    }
};

void CTraderCollector::OnRspQryExchange(CThostFtdcExchangeField *pExchange, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    if (!isErrorRspInfo(pRspInfo, bIsLast)) {
        Callback(this->m_context, this->m_cb, TD_RSP_QRY_EXCHANGE, (void*)pExchange, nRequestID, (int)bIsLast);
    }
};

void CTraderCollector::OnRspQryProduct(CThostFtdcProductField *pProduct, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    if (!isErrorRspInfo(pRspInfo, bIsLast)) {
        Callback(this->m_context, this->m_cb, TD_RSP_QRY_PRODUCT, (void*)pProduct, nRequestID, (int)bIsLast);
    }
};

void CTraderCollector::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    if (!isErrorRspInfo(pRspInfo, bIsLast)) {
        Callback(this->m_context, this->m_cb, TD_RSP_QRY_INSTRUMENT, (void*)pInstrument, nRequestID, (int)bIsLast);
    }
};

void CTraderCollector::OnRspQryDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    if (!isErrorRspInfo(pRspInfo, bIsLast)) {
        Callback(this->m_context, this->m_cb, TD_RSP_QRY_DEPTH_MARKET_DATA, (void*)pDepthMarketData, nRequestID, (int)bIsLast);
    }
};

void CTraderCollector::OnRspQrySettlementInfo(CThostFtdcSettlementInfoField *pSettlementInfo, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){    
    if (!isErrorRspInfo(pRspInfo, bIsLast)) {
        Callback(this->m_context, this->m_cb, TD_RSP_QRY_SETTLE_INFO, (void*)pSettlementInfo, nRequestID, (int)bIsLast);
    }
};

void CTraderCollector::OnRspQryTransferBank(CThostFtdcTransferBankField *pTransferBank, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    if (!isErrorRspInfo(pRspInfo, bIsLast)) {
        Callback(this->m_context, this->m_cb, TD_RSP_QRY_TRANSFER_BANK, (void*)pTransferBank, nRequestID, (int)bIsLast);
    }    
};

void CTraderCollector::OnRspQryInvestorPositionDetail(CThostFtdcInvestorPositionDetailField *pInvestorPositionDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    if (!isErrorRspInfo(pRspInfo, bIsLast)) {
        Callback(this->m_context, this->m_cb, TD_RSP_QRY_INVESTOR_POS_DETAIL, (void*)pInvestorPositionDetail, nRequestID, (int)bIsLast);
    }
};

void CTraderCollector::OnRspQryNotice(CThostFtdcNoticeField *pNotice, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    if (!isErrorRspInfo(pRspInfo, bIsLast)) {
        Callback(this->m_context, this->m_cb, TD_RSP_QRY_NOTICE, (void*)pNotice, nRequestID, (int)bIsLast);
    }
};

void CTraderCollector::OnRspQrySettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    if (!isErrorRspInfo(pRspInfo, bIsLast)) {
        Callback(this->m_context, this->m_cb, TD_RSP_QRY_SETTLE_INFO_CONFIRM, (void*)pSettlementInfoConfirm, nRequestID, (int)bIsLast);
    }
};

void CTraderCollector::OnRspQryInvestorPositionCombineDetail(CThostFtdcInvestorPositionCombineDetailField *pInvestorPositionCombineDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    if (!isErrorRspInfo(pRspInfo, bIsLast)) {
        Callback(this->m_context, this->m_cb, TD_RSP_QRY_INVESTOR_POS_COMB_DETAIL, (void*)pInvestorPositionCombineDetail, nRequestID, (int)bIsLast);
    }
};

void CTraderCollector::OnRspQryCFMMCTradingAccountKey(CThostFtdcCFMMCTradingAccountKeyField *pCFMMCTradingAccountKey, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
};

void CTraderCollector::OnRspQryEWarrantOffset(CThostFtdcEWarrantOffsetField *pEWarrantOffset, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
};

void CTraderCollector::OnRspQryInvestorProductGroupMargin(CThostFtdcInvestorProductGroupMarginField *pInvestorProductGroupMargin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
};

void CTraderCollector::OnRspQryExchangeMarginRate(CThostFtdcExchangeMarginRateField *pExchangeMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    if (!isErrorRspInfo(pRspInfo, bIsLast)) {
        Callback(this->m_context, this->m_cb, TD_RSP_QRY_EXCHANGE_MARGIN_RATE, (void*)pExchangeMarginRate, nRequestID, (int)bIsLast);
    }
};

void CTraderCollector::OnRspQryExchangeMarginRateAdjust(CThostFtdcExchangeMarginRateAdjustField *pExchangeMarginRateAdjust, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    if (!isErrorRspInfo(pRspInfo, bIsLast)) {
        Callback(this->m_context, this->m_cb, TD_RSP_QRY_EXCHANGE_MARGIN_RATE_ADJUST, (void*)pExchangeMarginRateAdjust, nRequestID, (int)bIsLast);
    }
};

void CTraderCollector::OnRspQryExchangeRate(CThostFtdcExchangeRateField *pExchangeRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    if (!isErrorRspInfo(pRspInfo, bIsLast)) {
        Callback(this->m_context, this->m_cb, TD_RSP_QRY_EXCHANGE_RATE, (void*)pExchangeRate, nRequestID, (int)bIsLast);
    }
};

void CTraderCollector::OnRspQrySecAgentACIDMap(CThostFtdcSecAgentACIDMapField *pSecAgentACIDMap, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
};

void CTraderCollector::OnRspQryProductExchRate(CThostFtdcProductExchRateField *pProductExchRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    if (!isErrorRspInfo(pRspInfo, bIsLast)) {
        Callback(this->m_context, this->m_cb, TD_RSP_QRY_PRODUCT_EXCH_RATE, (void*)pProductExchRate, nRequestID, (int)bIsLast);
    }
};

void CTraderCollector::OnRspQryProductGroup(CThostFtdcProductGroupField *pProductGroup, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    if (!isErrorRspInfo(pRspInfo, bIsLast)) {
        Callback(this->m_context, this->m_cb, TD_RSP_QRY_PRODUCT_GROUP, (void*)pProductGroup, nRequestID, (int)bIsLast);
    }
};

void CTraderCollector::OnRspQryMMInstrumentCommissionRate(CThostFtdcMMInstrumentCommissionRateField *pMMInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
};

void CTraderCollector::OnRspQryMMOptionInstrCommRate(CThostFtdcMMOptionInstrCommRateField *pMMOptionInstrCommRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
};

void CTraderCollector::OnRspQryInstrumentOrderCommRate(CThostFtdcInstrumentOrderCommRateField *pInstrumentOrderCommRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
};

void CTraderCollector::OnRspQryOptionInstrTradeCost(CThostFtdcOptionInstrTradeCostField *pOptionInstrTradeCost, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
};

void CTraderCollector::OnRspQryOptionInstrCommRate(CThostFtdcOptionInstrCommRateField *pOptionInstrCommRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
};

void CTraderCollector::OnRspQryExecOrder(CThostFtdcExecOrderField *pExecOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
};

void CTraderCollector::OnRspQryForQuote(CThostFtdcForQuoteField *pForQuote, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
};

void CTraderCollector::OnRspQryQuote(CThostFtdcQuoteField *pQuote, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
};

void CTraderCollector::OnRspQryCombInstrumentGuard(CThostFtdcCombInstrumentGuardField *pCombInstrumentGuard, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
};

void CTraderCollector::OnRspQryCombAction(CThostFtdcCombActionField *pCombAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
};

void CTraderCollector::OnRspQryTransferSerial(CThostFtdcTransferSerialField *pTransferSerial, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    if (!isErrorRspInfo(pRspInfo, bIsLast)) {
        Callback(this->m_context, this->m_cb, TD_RSP_QRY_TRANSFER_SERIAL, (void*)pTransferSerial, nRequestID, (int)bIsLast);
    }
};

void CTraderCollector::OnRspQryAccountregister(CThostFtdcAccountregisterField *pAccountregister, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    if (!isErrorRspInfo(pRspInfo, bIsLast)) {
        Callback(this->m_context, this->m_cb, TD_RSP_QRY_ACCOUNT_REGISTER, (void*)pAccountregister, nRequestID, (int)bIsLast);
    }
};

void CTraderCollector::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    Callback(this->m_context, this->m_cb, TD_RSP_ERROR, (void*)pRspInfo, nRequestID, bIsLast);
};

void CTraderCollector::OnRtnOrder(CThostFtdcOrderField *pOrder){
    Callback(this->m_context, this->m_cb, TD_RTN_ORDER, (void*)pOrder, 0, 0);
};

void CTraderCollector::OnRtnTrade(CThostFtdcTradeField *pTrade){
    Callback(this->m_context, this->m_cb, TD_RTN_TRADE, (void*)pTrade, 0, 0);
};

void CTraderCollector::OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo){
    isErrorRspInfo(pRspInfo, true);
    Callback(this->m_context, this->m_cb, TD_ERR_RTN_ORDER_INSERT, (void*)pInputOrder, 0, 0);
};

void CTraderCollector::OnErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo){
    isErrorRspInfo(pRspInfo, true);
    Callback(this->m_context, this->m_cb, TD_ERR_RTN_ORDER_ACTION, (void*)pOrderAction, 0, 0);
};

void CTraderCollector::OnRtnInstrumentStatus(CThostFtdcInstrumentStatusField *pInstrumentStatus) {
    Callback(this->m_context, this->m_cb, TD_RTN_INSTRUMENT_STATUS, (void*)pInstrumentStatus, 0, 0);
};

void CTraderCollector::OnRtnBulletin(CThostFtdcBulletinField *pBulletin){
};

void CTraderCollector::OnRtnTradingNotice(CThostFtdcTradingNoticeInfoField *pTradingNoticeInfo){    
    Callback(this->m_context, this->m_cb, TD_RTN_TRADING_NOTICE, (void*)pTradingNoticeInfo, 0, 0);
};

void CTraderCollector::OnRtnErrorConditionalOrder(CThostFtdcErrorConditionalOrderField *pErrorConditionalOrder){
};

void CTraderCollector::OnRtnExecOrder(CThostFtdcExecOrderField *pExecOrder){
};

void CTraderCollector::OnErrRtnExecOrderInsert(CThostFtdcInputExecOrderField *pInputExecOrder, CThostFtdcRspInfoField *pRspInfo){
};

void CTraderCollector::OnErrRtnExecOrderAction(CThostFtdcExecOrderActionField *pExecOrderAction, CThostFtdcRspInfoField *pRspInfo){
};

void CTraderCollector::OnErrRtnForQuoteInsert(CThostFtdcInputForQuoteField *pInputForQuote, CThostFtdcRspInfoField *pRspInfo){
    isErrorRspInfo(pRspInfo, true);
    Callback(this->m_context, this->m_cb, TD_ERR_RTN_FOR_QUOTE_INSERT, (void*)pInputForQuote, 0, 0);
};

void CTraderCollector::OnRtnQuote(CThostFtdcQuoteField *pQuote){
};

void CTraderCollector::OnErrRtnQuoteInsert(CThostFtdcInputQuoteField *pInputQuote, CThostFtdcRspInfoField *pRspInfo){
};

void CTraderCollector::OnErrRtnQuoteAction(CThostFtdcQuoteActionField *pQuoteAction, CThostFtdcRspInfoField *pRspInfo){
};

void CTraderCollector::OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp){
};

void CTraderCollector::OnRtnCFMMCTradingAccountToken(CThostFtdcCFMMCTradingAccountTokenField *pCFMMCTradingAccountToken){
};

void CTraderCollector::OnErrRtnBatchOrderAction(CThostFtdcBatchOrderActionField *pBatchOrderAction, CThostFtdcRspInfoField *pRspInfo){
};

void CTraderCollector::OnRtnCombAction(CThostFtdcCombActionField *pCombAction){
};

void CTraderCollector::OnErrRtnCombActionInsert(CThostFtdcInputCombActionField *pInputCombAction, CThostFtdcRspInfoField *pRspInfo){
};

void CTraderCollector::OnRspQryContractBank(CThostFtdcContractBankField *pContractBank, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    if (!isErrorRspInfo(pRspInfo, bIsLast)) {
        Callback(this->m_context, this->m_cb, TD_RSP_QRY_CONTRACT_BANK, (void*)pContractBank, nRequestID, (int)bIsLast);
    }
};

void CTraderCollector::OnRspQryParkedOrder(CThostFtdcParkedOrderField *pParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    if (!isErrorRspInfo(pRspInfo, bIsLast)) {
        Callback(this->m_context, this->m_cb, TD_RSP_QRY_PACK_ORDER, (void*)pParkedOrder, nRequestID, (int)bIsLast);
    }
};

void CTraderCollector::OnRspQryParkedOrderAction(CThostFtdcParkedOrderActionField *pParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    if (!isErrorRspInfo(pRspInfo, bIsLast)) {
        Callback(this->m_context, this->m_cb, TD_RSP_QRY_PACK_ORDER_ACTION, (void*)pParkedOrderAction, nRequestID, (int)bIsLast);
    }
};

void CTraderCollector::OnRspQryTradingNotice(CThostFtdcTradingNoticeField *pTradingNotice, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    if (!isErrorRspInfo(pRspInfo, bIsLast)) {
        Callback(this->m_context, this->m_cb, TD_RSP_QRY_TRADING_NOTICE, (void*)pTradingNotice, nRequestID, (int)bIsLast);
    }
};

void CTraderCollector::OnRspQryBrokerTradingParams(CThostFtdcBrokerTradingParamsField *pBrokerTradingParams, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
};

void CTraderCollector::OnRspQryBrokerTradingAlgos(CThostFtdcBrokerTradingAlgosField *pBrokerTradingAlgos, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
};

void CTraderCollector::OnRspQueryCFMMCTradingAccountToken(CThostFtdcQueryCFMMCTradingAccountTokenField *pQueryCFMMCTradingAccountToken, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
};

void CTraderCollector::OnRtnFromBankToFutureByBank(CThostFtdcRspTransferField *pRspTransfer){
    Callback(this->m_context, this->m_cb, TD_RTN_FROM_BANK_TO_FUTURE_BY_BANK, (void*)pRspTransfer, 0, 0);
};

void CTraderCollector::OnRtnFromFutureToBankByBank(CThostFtdcRspTransferField *pRspTransfer){
    Callback(this->m_context, this->m_cb, TD_RTN_FROM_FUTURE_TO_BANK_BY_BANK, (void*)pRspTransfer, 0, 0);
};

void CTraderCollector::OnRtnRepealFromBankToFutureByBank(CThostFtdcRspRepealField *pRspRepeal){
    Callback(this->m_context, this->m_cb, TD_RTN_REPEAL_FROM_BANK_TO_FUTURE_BY_BANK, (void*)pRspRepeal, 0, 0);
};

void CTraderCollector::OnRtnRepealFromFutureToBankByBank(CThostFtdcRspRepealField *pRspRepeal){
    Callback(this->m_context, this->m_cb, TD_RTN_REPEAL_FROM_FUTURE_TO_BANK_BY_BANK, (void*)pRspRepeal, 0, 0);
};

void CTraderCollector::OnRtnFromBankToFutureByFuture(CThostFtdcRspTransferField *pRspTransfer){
    Callback(this->m_context, this->m_cb, TD_RTN_FROM_BANK_TO_FUTURE_BY_FUTURE, (void*)pRspTransfer, 0, 0);
};

void CTraderCollector::OnRtnFromFutureToBankByFuture(CThostFtdcRspTransferField *pRspTransfer){
    Callback(this->m_context, this->m_cb, TD_RTN_FROM_FUTURE_TO_BANK_BY_FUTURE, (void*)pRspTransfer, 0, 0);
};

void CTraderCollector::OnRtnRepealFromBankToFutureByFutureManual(CThostFtdcRspRepealField *pRspRepeal){
    Callback(this->m_context, this->m_cb, TD_RTN_REPEAL_FROM_BANK_TO_FUTURE_BY_FUTURE_MANUAL, (void*)pRspRepeal, 0, 0);
};

void CTraderCollector::OnRtnRepealFromFutureToBankByFutureManual(CThostFtdcRspRepealField *pRspRepeal){
    Callback(this->m_context, this->m_cb, TD_RTN_REPEAL_FROM_FUTURE_TO_BANK_BY_FUTURE_MANUAL, (void*)pRspRepeal, 0, 0);
};

void CTraderCollector::OnRtnQueryBankBalanceByFuture(CThostFtdcNotifyQueryAccountField *pNotifyQueryAccount){
    Callback(this->m_context, this->m_cb, TD_RTN_QUERY_BANK_BALANCE_BY_FUTURE, (void*)pNotifyQueryAccount, 0, 0);
};

void CTraderCollector::OnErrRtnBankToFutureByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo){
    isErrorRspInfo(pRspInfo, true);
    Callback(this->m_context, this->m_cb, TD_ERR_RTN_BANK_TO_FUTURE_BY_FUTURE, (void*)pReqTransfer, 0, 0);
};

void CTraderCollector::OnErrRtnFutureToBankByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo){
    isErrorRspInfo(pRspInfo, true);
    Callback(this->m_context, this->m_cb, TD_ERR_RTN_FUTURE_TO_BANK_BY_FUTURE, (void*)pReqTransfer, 0, 0);
};

void CTraderCollector::OnErrRtnRepealBankToFutureByFutureManual(CThostFtdcReqRepealField *pReqRepeal, CThostFtdcRspInfoField *pRspInfo){
    isErrorRspInfo(pRspInfo, true);
    Callback(this->m_context, this->m_cb, TD_ERR_RTN_REPEAL_BANK_TO_FUTURE_BY_FUTURE_MANUAL, (void*)pReqRepeal, 0, 0);
};

void CTraderCollector::OnErrRtnRepealFutureToBankByFutureManual(CThostFtdcReqRepealField *pReqRepeal, CThostFtdcRspInfoField *pRspInfo){
    isErrorRspInfo(pRspInfo, true);
    Callback(this->m_context, this->m_cb, TD_ERR_RTN_REPEAL_FUTURE_TO_BANK_BY_FUTURE_MANUAL, (void*)pReqRepeal, 0, 0);
};

void CTraderCollector::OnErrRtnQueryBankBalanceByFuture(CThostFtdcReqQueryAccountField *pReqQueryAccount, CThostFtdcRspInfoField *pRspInfo){
    isErrorRspInfo(pRspInfo, true);
    Callback(this->m_context, this->m_cb, TD_ERR_RTN_QRY_BANK_BALANCE_BY_FUTURE, (void*)pReqQueryAccount, 0, 0);
};

void CTraderCollector::OnRtnRepealFromBankToFutureByFuture(CThostFtdcRspRepealField *pRspRepeal){
    Callback(this->m_context, this->m_cb, TD_RTN_REPEAL_FROM_BANK_TO_FUTURE_BY_FUTURE, (void*)pRspRepeal, 0, 0);
};

void CTraderCollector::OnRtnRepealFromFutureToBankByFuture(CThostFtdcRspRepealField *pRspRepeal){
    Callback(this->m_context, this->m_cb, TD_RTN_REPEAL_FROM_FUTURE_TO_BANK_BY_FUTURE, (void*)pRspRepeal, 0, 0);
};

void CTraderCollector::OnRspFromBankToFutureByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    if (!isErrorRspInfo(pRspInfo, bIsLast)) {
        Callback(this->m_context, this->m_cb, TD_RSP_FROM_BANK_TO_FUTURE_BY_FUTURE, (void*)pReqTransfer, nRequestID, (int)bIsLast);
    }
};

void CTraderCollector::OnRspFromFutureToBankByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    if (!isErrorRspInfo(pRspInfo, bIsLast)) {
        Callback(this->m_context, this->m_cb, TD_RSP_FROM_FUTURE_TO_BANK_BY_FUTURE, (void*)pReqTransfer, nRequestID, (int)bIsLast);
    }
};

void CTraderCollector::OnRspQueryBankAccountMoneyByFuture(CThostFtdcReqQueryAccountField *pReqQueryAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    if (!isErrorRspInfo(pRspInfo, bIsLast)) {
        Callback(this->m_context, this->m_cb, TD_RSP_QRY_BANK_ACCOUNT_MONEY_BY_FUTURE, (void*)pReqQueryAccount, nRequestID, (int)bIsLast);
    }
};

void CTraderCollector::OnRtnOpenAccountByBank(CThostFtdcOpenAccountField *pOpenAccount){
};

void CTraderCollector::OnRtnCancelAccountByBank(CThostFtdcCancelAccountField *pCancelAccount){
};

void CTraderCollector::OnRtnChangeAccountByBank(CThostFtdcChangeAccountField *pChangeAccount){
};
