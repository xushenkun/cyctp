#if !defined(MARKET_COLLECTOR_H)
#define MARKET_COLLECTOR_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include "ThostFtdcMdApi.h"
#include "Util.h"

#if defined(ISLIB) && defined(WIN32)
#ifdef LIB_MARKET_COLLECTOR_API_EXPORT
#define MARKET_COLLECTOR_API_EXPORT __declspec(dllexport)
#else
#define MARKET_COLLECTOR_API_EXPORT __declspec(dllimport)
#endif
#else
#define MARKET_COLLECTOR_API_EXPORT 
#endif
class CMarketCollector : public CThostFtdcMdSpi
{
public:
    virtual void OnFrontConnected();    
    virtual void OnFrontDisconnected(int nReason);        
    virtual void OnHeartBeatWarning(int nTimeLapse);
    virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    virtual void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    virtual void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    virtual void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    
    //only for zhengshang market maker
    virtual void OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    //only for zhengshang market maker
    virtual void OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    
    virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);
    virtual void OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp);

    CMarketCollector(const char *flowPath) {  
        memset(m_brokerID, 0, sizeof(m_brokerID));
        memset(m_brokerBID, 0, sizeof(m_brokerBID));
        memset(m_userID, 0, sizeof(m_userID));
        memset(m_password, 0, sizeof(m_password));    
        this->m_requestID = 0; 
        this->m_connected = false;           
        this->m_logined = false;        
        this->m_instruments = NULL;
        this->m_instrumentCount = 0;
        this->m_context = NULL;
        this->m_cb = NULL;
        this->m_Api = CThostFtdcMdApi::CreateFtdcMdApi(flowPath, false, false);
        this->m_Api->RegisterSpi(this);        
    };
    virtual ~CMarketCollector() {
        this->Close();
    };
    virtual int Close();
    virtual int Connect(char *frontPaths[], size_t nCount);
    static const char *Version();
    virtual int Login(const char *broker, const char *brokerBranch, const char *user, const char *pwd);
    virtual const char *GetTradingDay();
    virtual int Logout();
    virtual int SubMarket(char *instrumentID);
    virtual int SubMarket(char *instrumentIDs[], size_t nCount);
    virtual int UnSubMarket(char *instrumentID);
    virtual int UnSubMarket(char *instrumentIDs[], size_t nCount);
    virtual int SubForQuote(char *instrumentID);
    virtual int SubForQuote(char *instrumentIDs[], size_t nCount);

    //only for zhengshang market maker
    virtual int UnSubForQuote(char *instrumentID);  
    //only for zhengshang market maker
    virtual int UnSubForQuote(char *instrumentIDs[], size_t nCount);    


    virtual void RegisterCb(void *context, CbMdFunc f) {
        this->m_context = context;
        this->m_cb = f;
    };    
protected:
    CThostFtdcMdApi *m_Api;
    char m_brokerID[11];
    char m_brokerBID[31];
    char m_userID[16];
    char m_password[41];
    int m_requestID;
    bool m_connected;    
    bool m_logined;
    char **m_instruments;
    int m_instrumentCount;

    CbMdFunc m_cb;
    void *m_context;
};

#endif