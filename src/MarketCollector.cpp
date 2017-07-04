#include <string.h>
#include "MarketCollector.h"
#include "ThostFtdcUserApiStruct.h"

using namespace std;

int CMarketCollector::Close() {
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

int CMarketCollector::Connect(char *frontPaths[], size_t nCount) {  
    /*
    if(this->m_Api != NULL) {
        if (this->m_logined)
            this->Logout(NULL, NULL);
    }
    */
    for (int i=0; i<nCount; i++)
        this->m_Api->RegisterFront(frontPaths[i]);
    this->m_Api->Init();
    return 0;
};

const char *CMarketCollector::Version() {
    return CThostFtdcMdApi::GetApiVersion();
};

int CMarketCollector::Login(const char *broker, const char *brokerBranch, const char *user, const char *pwd) {
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

const char *CMarketCollector::GetTradingDay() {
    if (this->m_Api != NULL && this->m_connected && this->m_logined) {
        return this->m_Api->GetTradingDay();
    }
    return NULL;
};

int CMarketCollector::Logout() {
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

int CMarketCollector::SubMarket(char *instrumentID) {
    return this->SubMarket(&(instrumentID), 1);
};

int CMarketCollector::SubMarket(char *instrumentIDs[], size_t nCount) {
    if (this->m_Api != NULL && this->m_connected && this->m_logined && instrumentIDs!=NULL && nCount>0) {
        int iResult = this->m_Api->SubscribeMarketData(instrumentIDs, (int)nCount);
        return iResult;
    }
    return -999;
};

int CMarketCollector::UnSubMarket(char *instrumentID) {
    return this->UnSubMarket(&(instrumentID), 1);
};

int CMarketCollector::UnSubMarket(char *instrumentIDs[], size_t nCount) {    
    if (this->m_Api != NULL && this->m_connected && this->m_logined && instrumentIDs!=NULL && nCount>0) {
        int iResult = this->m_Api->UnSubscribeMarketData(instrumentIDs, (int)nCount);
        return iResult;
    }
    return -999;
};

int CMarketCollector::SubForQuote(char *instrumentID) {
    return this->SubForQuote(&(instrumentID), 1);
};

int CMarketCollector::SubForQuote(char *instrumentIDs[], size_t nCount) {
    if (this->m_Api != NULL && this->m_connected && this->m_logined && instrumentIDs!=NULL && nCount>0) {
        int iResult = this->m_Api->SubscribeForQuoteRsp(instrumentIDs, (int)nCount);
        return iResult;
    }
    return -999;
};

int CMarketCollector::UnSubForQuote(char *instrumentID) {
    return this->UnSubForQuote(&(instrumentID), 1);
};

int CMarketCollector::UnSubForQuote(char *instrumentIDs[], size_t nCount) {
        if (this->m_Api != NULL && this->m_connected && this->m_logined && instrumentIDs!=NULL && nCount>0) {
        int iResult = this->m_Api->UnSubscribeForQuoteRsp(instrumentIDs, (int)nCount);
        return iResult;
    }
    return -999;
};

void CMarketCollector::OnFrontConnected() {
    this->m_connected = true;
    Callback(this->m_context, this->m_cb, MD_FRONT_CONNECTED, NULL, 0, 0);
};

void CMarketCollector::OnFrontDisconnected(int nReason){
    this->m_connected = false;
    Callback(this->m_context, this->m_cb, MD_FRONT_DISCONNECTED, (void*)&nReason, 0, 0);
};
    
void CMarketCollector::OnHeartBeatWarning(int nTimeLapse){
    Callback(this->m_context, this->m_cb, MD_HEART_BEAT_WARN, (void*)&nTimeLapse, 0, 0);
};

void CMarketCollector::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
    if (!isErrorRspInfo(pRspInfo, bIsLast)) {
        this->m_logined = true;
        Callback(this->m_context, this->m_cb, MD_RSP_USER_LOGIN, (void*)pRspUserLogin, nRequestID, (int)bIsLast);
    }
};

void CMarketCollector::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
    if (!isErrorRspInfo(pRspInfo, bIsLast)) {
        this->m_logined = false;
        Callback(this->m_context, this->m_cb, MD_RSP_USER_LOGOUT, (void*)pUserLogout, nRequestID, (int)bIsLast);
    }
};

void CMarketCollector::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
    Callback(this->m_context, this->m_cb, MD_RSP_ERROR, (void*)pRspInfo, nRequestID, (int)bIsLast);
};

void CMarketCollector::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
    if (!isErrorRspInfo(pRspInfo, bIsLast)) {
        Callback(this->m_context, this->m_cb, MD_RSP_SUB_MARKET_DATA, (void*)pSpecificInstrument, nRequestID, (int)bIsLast);
    }
};

void CMarketCollector::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
    if (!isErrorRspInfo(pRspInfo, bIsLast)) {
        Callback(this->m_context, this->m_cb, MD_RSP_UNSUB_MARKET_DATA, (void*)pSpecificInstrument, nRequestID, (int)bIsLast);
    }
};

void CMarketCollector::OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
    if (!isErrorRspInfo(pRspInfo, bIsLast)) {
        Callback(this->m_context, this->m_cb, MD_RSP_SUB_FOR_QUOTE, (void*)pSpecificInstrument, nRequestID, (int)bIsLast);
    }
};

void CMarketCollector::OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
    if (!isErrorRspInfo(pRspInfo, bIsLast)) {
        Callback(this->m_context, this->m_cb, MD_RSP_UNSUB_FOR_QUOTE, (void*)pSpecificInstrument, nRequestID, (int)bIsLast);
    }
};

void CMarketCollector::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData) {
    Callback(this->m_context, this->m_cb, MD_RTN_DEPTH_MARKET_DATA, (void*)pDepthMarketData, 0, 0);
};

void CMarketCollector::OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp) {
    Callback(this->m_context, this->m_cb, MD_RTN_FOR_QUOTE, (void*)pForQuoteRsp, 0, 0);
};
