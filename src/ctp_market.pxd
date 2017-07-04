#distutils: language = c++

cdef extern from "Util.h":
    cpdef enum MD_CALLBACK_TYPE:
        MD_FRONT_CONNECTED = 0
        MD_FRONT_DISCONNECTED
        MD_HEART_BEAT_WARN
        MD_RSP_USER_LOGIN
        MD_RSP_USER_LOGOUT
        MD_RSP_ERROR
        MD_RSP_SUB_MARKET_DATA
        MD_RSP_UNSUB_MARKET_DATA
        MD_RSP_SUB_FOR_QUOTE
        MD_RSP_UNSUB_FOR_QUOTE
        MD_RTN_DEPTH_MARKET_DATA
        MD_RTN_FOR_QUOTE
    ctypedef int (*CbMdFunc)(void *context, MD_CALLBACK_TYPE type, void* data, int rid, int last)

cdef extern from "MarketCollector.h" nogil:
    cdef cppclass CThostFtdcMdSpi:
        pass
    cdef cppclass CMarketCollector(CThostFtdcMdSpi):
        CMarketCollector(const char *flowPath)
        int Connect(char *frontPaths[], size_t nCount)
        @staticmethod
        const char *Version()
        int Login(const char *broker, const char *brokerBranch, const char *user, const char *pwd)
        int Logout()
        const char *GetTradingDay() 
        int SubMarket(char *instrumentID)
        int SubMarket(char *instrumentIDs[], size_t nCount)
        int UnSubMarket(char *instrumentID)
        int UnSubMarket(char *instrumentIDs[], size_t nCount)  
        int SubForQuote(char *instrumentID)
        int SubForQuote(char *instrumentIDs[], size_t nCount)
        int UnSubForQuote(char *instrumentID)
        int UnSubForQuote(char *instrumentIDs[], size_t nCount)

        void RegisterCb(void *context, CbMdFunc f)
        int Close()
    