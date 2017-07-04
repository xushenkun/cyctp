#include <stdio.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <Python.h>
#include "Util.h"

#ifdef __linux__ 
    #include <pthread.h>
    #include <ctime>
    #include <unistd.h>
    void print_log(int type, const char *msg, int code, char *detail1, char *detail2, char *detail3) {
        const time_t tNow = time(0);
        struct tm* tmNow;
        tmNow = localtime(&tNow);
        std::cout << tmNow->tm_hour << ":" << tmNow->tm_min << ":" << tmNow->tm_sec << " " << pthread_self() << " " << type << " " << msg << " [" << code << "] " << (detail1==NULL?"":detail1) << (detail1==NULL?"":" ") << (detail2==NULL?"":detail2) << (detail2==NULL?"":" ") << (detail3==NULL?"":detail3) << std::endl;
    };
    void msleep(unsigned milliseconds) {
        usleep(milliseconds * 1000);
    };
    void nowtime(char *timeret) {
        const time_t tNow = time(0);
        struct tm* tmNow;
        tmNow = localtime(&tNow);
        snprintf(timeret, 9, "%02d:%02d:%02d", tmNow->tm_hour, tmNow->tm_min, tmNow->tm_sec);
    };
#elif _WIN32
    #include <windows.h>
    void print_log(int type, const char *msg, int code, char *detail1, char *detail2, char *detail3) {
        SYSTEMTIME t; ::GetLocalTime(&t);
        std::cout << t.wHour << ":" << t.wMinute << ":" << t.wSecond << " " << GetCurrentThreadId() << " " << type << " " << msg << " [" << code << "] " << (detail1==NULL?"":detail1) << (detail1==NULL?"":" ") << (detail2==NULL?"":detail2) << (detail2==NULL?"":" ") << (detail3==NULL?"":detail3) << std::endl;
    };
    void msleep(unsigned milliseconds) {
        Sleep(milliseconds);
    };
    void nowtime(char *timeret) {
        SYSTEMTIME t; ::GetLocalTime(&t);
        sprintf_s(timeret, 9, "%02d:%02d:%02d", t.wHour, t.wMinute, t.wSecond);
    };
#endif


bool isErrorRspInfo(CThostFtdcRspInfoField *pRspInfo, bool bIsLast) {
    if (pRspInfo == NULL) {
        if (bIsLast) {
            //print_log("Rsp info is null", -1);
            return false;   //should be true, but ctp api server is wrong bug
        } else {
            return false;
        }
    }
    if (pRspInfo->ErrorID != 0) {
        print_log(-1, "Rsp info error", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
        return true;
    } else {
        //print_log("Rsp info success", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
        return false;
    }
};

int Callback(void *context, CbMdFunc cb, MD_CALLBACK_TYPE type, void *data, int rid, int last) {
    int ret = -1;
    if (context != NULL && cb != NULL) {
        PyGILState_STATE gilstate_save = PyGILState_Ensure();
        PyObject *value = PyLong_FromVoidPtr(data);
        if (value != NULL)
            ret = cb(context, type, value, rid, last);
        //print_log(type, "Callback", Py_REFCNT(value));
        Py_XDECREF(value);        
        PyGILState_Release(gilstate_save);
    }
    return ret;
};

int Callback(void *context, CbTdFunc cb, TD_CALLBACK_TYPE type, void *data, int rid, int last) {
    int ret = -1;
    if (context != NULL && cb != NULL) {
        PyGILState_STATE gilstate_save = PyGILState_Ensure();
        PyObject *value = PyLong_FromVoidPtr(data);
        if (value != NULL)
            ret = cb(context, type, value, rid, last);
        Py_XDECREF(value);    
        PyGILState_Release(gilstate_save);
    }
    return ret;
};
