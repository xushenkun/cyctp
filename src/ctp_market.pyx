#distutils: language = c++

from libc.stdlib cimport malloc, free
from cpython.string cimport PyString_AsString
from libc.string cimport strdup
import logging, logging.config
from multiprocessing.pool import ThreadPool
from threading import current_thread

cimport ctp_market

logger = logging.getLogger()#__name__)

cb_pool = ThreadPool(processes=2)

cdef int md_callback(void *context, ctp_market.MD_CALLBACK_TYPE t, void* data, int rid, int last):# with gil:
    if t == -1:#ctp_market.MD_RTN_DEPTH_MARKET_DATA:
         (<object>context).get_handler().route(t, <object>data, rid, last)
    else:
        async_result = cb_pool.apply_async(async_callback, args=(<object>context, t, <object>data, rid, last))
        async_result.get()   
        #logger.info('md callback: %s'%t)
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

cdef class CtpMarket:
    cdef ctp_market.CMarketCollector *collector
    def __cinit__(self, flowPath=".\\flow\\md\\", *args, **kwargs): 
        self.collector = new ctp_market.CMarketCollector(as_str(flowPath if flowPath else ""))            
    def __dealloc__(self):
        if self.collector is not NULL: del self.collector 
    @staticmethod
    def version():
        return ctp_market.CMarketCollector.Version()
    def connect(self, frontPaths):
        self.collector.RegisterCb(<void*>self, md_callback) 
        ids = to_cstring_array(frontPaths)
        ret = self.collector.Connect(ids, len(frontPaths))
        free(ids)
        return ret
    def login(self):
        return self.collector.Login(as_str(self.broker), as_str('0000'), as_str(self.user), as_str(self.pwd))
    def logout(self):
        return self.collector.Logout()
    def get_trading_day(self):
        return self.collector.GetTradingDay()
    def sub_market(self, instrument_ids):           
        ids = to_cstring_array(instrument_ids)
        ret = self.collector.SubMarket(ids, len(instrument_ids))
        free(ids)
        return ret
    def unsub_market(self, instrument_ids):
        ids = to_cstring_array(instrument_ids)
        ret = self.collector.UnSubMarket(ids, len(instrument_ids))
        free(ids)
        return ret
    def sub_ask_quote(self, instrument_ids):
        ids = to_cstring_array(instrument_ids)
        ret = self.collector.SubForQuote(ids, len(instrument_ids))
        free(ids)
        return ret
    def unsub_ask_quote(self, instrument_ids):
        ids = to_cstring_array(instrument_ids)
        ret = self.collector.UnSubForQuote(ids, len(instrument_ids))
        free(ids)
        return ret
    def get_handler(self):
        return self.handler
    #def join(self):        
    #    self._mdApi.Join() 
    def close(self):
        if self.collector: return self.collector.Close()

#print "Imported CTP Market Module"