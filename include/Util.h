#if !defined(UTIL_H)
#define UTIL_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if defined(ISLIB) && defined(WIN32)
#ifdef LIB_UTIL_API_EXPORT
#define UTIL_API_EXPORT __declspec(dllexport)
#else
#define UTIL_API_EXPORT __declspec(dllimport)
#endif
#else
#define UTIL_API_EXPORT 
#endif

#include "ThostFtdcUserApiStruct.h"

#ifdef __linux__
#define strcpy_s strcpy
#endif

void print_log(int type, const char *msg, int code=0, char *detail1=NULL, char *detail2=NULL, char *detail3=NULL);
void msleep(unsigned milliseconds);
void nowtime(char *time);
bool isErrorRspInfo(CThostFtdcRspInfoField *pRspInfo, bool bIsLast);

enum MD_CALLBACK_TYPE {
    MD_FRONT_CONNECTED = 0,
    MD_FRONT_DISCONNECTED,
    MD_HEART_BEAT_WARN,
    MD_RSP_USER_LOGIN,
    MD_RSP_USER_LOGOUT,
    MD_RSP_ERROR,
    MD_RSP_SUB_MARKET_DATA,
    MD_RSP_UNSUB_MARKET_DATA,
    MD_RSP_SUB_FOR_QUOTE,
    MD_RSP_UNSUB_FOR_QUOTE,
    MD_RTN_DEPTH_MARKET_DATA,
    MD_RTN_FOR_QUOTE
};

enum TD_CALLBACK_TYPE {
    TD_FRONT_CONNECTED = 0,
    TD_FRONT_DISCONNECTED,
    TD_HEART_BEAT_WARN,
    TD_RSP_AUTH,
    TD_RSP_USER_LOGIN,
    TD_RSP_USER_LOGOUT,
    TD_RSP_PWD_UPDATE,
    TD_RSP_TRADING_PWD_UPDATE,
    TD_RSP_ORDER_INSERT,
    TD_RSP_PACK_ORDER_INSERT,
    TD_RSP_PACK_ORDER_ACTION,
    TD_RSP_ORDER_ACTION,
    TD_RSP_QRY_MAX_ORDER_VOLUME,
    TD_RSP_SETTLE_INFO_CONFIRM,
    TD_RSP_REMOVE_PACK,
    TD_RSP_REMOVE_PACK_ACTION,
    TD_RSP_EXEC_ORDER_INSERT,
    TD_RSP_EXEC_ORDER_ACTION,
    TD_RSP_FOR_QUOTE_INSERT,
    TD_RSP_QUOTE_INSERT,
    TD_RSP_QUOTE_ACTION,
    TD_RSP_BATCH_ORDER_ACTION,
    TD_RSP_COMB_ACTION_INSERT,
    TD_RSP_QRY_ORDER,
    TD_RSP_QRY_TRADE,
    TD_RSP_QRY_INVESTOR_POS,
    TD_RSP_QRY_TRADING_ACCOUNT,
    TD_RSP_QRY_INVESTOR,
    TD_RSP_QRY_TRADING_CODE,
    TD_RSP_QRY_INSTRUMENT_MARGIN_RATE,
    TD_RSP_QRY_INSTRUMENT_COMMISSION_RATE,
    TD_RSP_QRY_EXCHANGE,
    TD_RSP_QRY_PRODUCT,
    TD_RSP_QRY_INSTRUMENT,
    TD_RSP_QRY_DEPTH_MARKET_DATA,
    TD_RSP_QRY_SETTLE_INFO,
    TD_RSP_QRY_TRANSFER_BANK,
    TD_RSP_QRY_INVESTOR_POS_DETAIL,
    TD_RSP_QRY_NOTICE,
    TD_RSP_QRY_SETTLE_INFO_CONFIRM,
    TD_RSP_QRY_INVESTOR_POS_COMB_DETAIL,
    TD_RSP_QRY_CFMMC_TRADING_ACCOUNT_KEY,
    TD_RSP_QRY_EWARRANT_OFFSET,
    TD_RSP_QRY_INVESTOR_PRODUCT_GROUP_MARGIN,
    TD_RSP_QRY_EXCHANGE_MARGIN_RATE,
    TD_RSP_QRY_EXCHANGE_MARGIN_RATE_ADJUST,
    TD_RSP_QRY_EXCHANGE_RATE,
    TD_RSP_QRY_SEC_AGENT_ACID_MAP,
    TD_RSP_QRY_PRODUCT_EXCH_RATE,
    TD_RSP_QRY_PRODUCT_GROUP,
    TD_RSP_QRY_MM_INSTRUMENT_COMMISSION_RATE,
    TD_RSP_QRY_MM_OPTION_INSTR_COMM_RATE,
    TD_RSP_QRY_INSTRUMENT_ORDER_COMM_RATE,
    TD_RSP_QRY_OPTION_INSTR_TRADE_COST,
    TD_RSP_QRY_OPTION_INSTR_COMM_RATE,
    TD_RSP_QRY_EXEC_ORDER,
    TD_RSP_QRY_FOR_QUOTE,
    TD_RSP_QRY_QUOTE,
    TD_RSP_QRY_COMB_INSTRUMENT_GUARD,
    TD_RSP_QRY_COMB_ACTION,
    TD_RSP_QRY_TRANSFER_SERIAL,
    TD_RSP_QRY_ACCOUNT_REGISTER,
    TD_RSP_ERROR,
    TD_RTN_ORDER,
    TD_RTN_TRADE,
    TD_ERR_RTN_ORDER_INSERT,
    TD_ERR_RTN_ORDER_ACTION,
    TD_RTN_INSTRUMENT_STATUS,
    TD_RTN_BULLETIN,
    TD_RTN_TRADING_NOTICE,
    TD_RTN_ERROR_CONDITIONAL_ORDER,
    TD_RTN_EXEC_ORDER,
    TD_ERR_RTN_EXEC_ORDER_INSERT,
    TD_ERR_RTN_EXEC_ORDER_ACTION,
    TD_ERR_RTN_FOR_QUOTE_INSERT,
    TD_RTN_QUOTE,
    TD_ERR_RTN_QUOTE_INSERT,
    TD_ERR_RTN_QUOTE_ACTION,
    TD_RTN_FOR_QUOTE_RSP,
    TD_RTN_CFMMC_TRADING_ACCOUNT_TOKEN,
    TD_ERR_RTN_BATCH_ORDER_ACTION,
    TD_RTN_COMB_ACTION,
    TD_ERR_RTN_COMB_ACTION_INSERT,
    TD_RSP_QRY_CONTRACT_BANK,
    TD_RSP_QRY_PACK_ORDER,
    TD_RSP_QRY_PACK_ORDER_ACTION,
    TD_RSP_QRY_TRADING_NOTICE,
    TD_RSP_QRY_BROKER_TRADING_PARAMS,
    TD_RSP_QRY_BROKER_TRADING_ALGOS,
    TD_RSP_QRY_CFMMC_TRADING_ACCOUNT_TOKEN,
    TD_RTN_FROM_BANK_TO_FUTURE_BY_BANK,
    TD_RTN_FROM_FUTURE_TO_BANK_BY_BANK,
    TD_RTN_REPEAL_FROM_BANK_TO_FUTURE_BY_BANK,
    TD_RTN_REPEAL_FROM_FUTURE_TO_BANK_BY_BANK,
    TD_RTN_FROM_BANK_TO_FUTURE_BY_FUTURE,
    TD_RTN_FROM_FUTURE_TO_BANK_BY_FUTURE,
    TD_RTN_REPEAL_FROM_BANK_TO_FUTURE_BY_FUTURE_MANUAL,
    TD_RTN_REPEAL_FROM_FUTURE_TO_BANK_BY_FUTURE_MANUAL,
    TD_RTN_QUERY_BANK_BALANCE_BY_FUTURE,
    TD_ERR_RTN_BANK_TO_FUTURE_BY_FUTURE,
    TD_ERR_RTN_FUTURE_TO_BANK_BY_FUTURE,
    TD_ERR_RTN_REPEAL_BANK_TO_FUTURE_BY_FUTURE_MANUAL,
    TD_ERR_RTN_REPEAL_FUTURE_TO_BANK_BY_FUTURE_MANUAL,
    TD_ERR_RTN_QRY_BANK_BALANCE_BY_FUTURE,
    TD_RTN_REPEAL_FROM_BANK_TO_FUTURE_BY_FUTURE,
    TD_RTN_REPEAL_FROM_FUTURE_TO_BANK_BY_FUTURE,
    TD_RSP_FROM_BANK_TO_FUTURE_BY_FUTURE,
    TD_RSP_FROM_FUTURE_TO_BANK_BY_FUTURE,
    TD_RSP_QRY_BANK_ACCOUNT_MONEY_BY_FUTURE,
    TD_RTN_OPEN_ACCOUNT_BY_BANK,
    TD_RTN_CANCEL_ACCOUNT_BY_BANK,
    TD_RTN_CHANGE_ACCOUNT_BY_BANK
};

typedef int (*CbMdFunc)(void *context, MD_CALLBACK_TYPE type, void* data, int rid, int last);
typedef int (*CbTdFunc)(void *context, TD_CALLBACK_TYPE type, void* data, int rid, int last);

int Callback(void *context, CbMdFunc cb, MD_CALLBACK_TYPE type, void *data, int rid, int last);

int Callback(void *context, CbTdFunc cb, TD_CALLBACK_TYPE type, void *data, int rid, int last);

#endif
