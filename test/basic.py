# -*- coding: utf-8 -*-

import os, sys, time
import logging, logging.config
reload(sys)
sys.setdefaultencoding('utf-8')

from cyctp import Market, Trader
from cyctp import *

logger = logging.getLogger()#__name__)

if __name__ == '__main__':    
    logging.config.fileConfig(os.path.dirname(os.path.abspath(__file__))+os.path.sep+"logger.conf")

    real = True
    broker = "8090" if real else None
    user = "xxx" if real else None
    pwd = "xxx" if real else None
    trader_fronts = ["tcp://123.127.45.138:41205", "tcp://123.127.45.145:41205"] if real else None
    market_fronts = ["tcp://123.127.45.138:41213", "tcp://123.127.45.145:41213"] if real else None

    market = Market(frontPaths=market_fronts, broker=broker, user=user, pwd=pwd) if real else Market()
    trader = Trader(market=market, frontPaths=trader_fronts, broker=broker, user=user, pwd=pwd) if real else Trader(market=market)

    time.sleep(3)

    while True:
        ex = raw_input("Input command or exit\n")
        sys.stdout.flush()
        ex = ex.lower() if ex else None
        if ex == 'exit':
            break
        elif ex == 'day':
            logger.info(trader.get_trading_day())
        elif ex == 'notice':
            trader.query_notice()
        elif ex == 'tradingnotice':
            trader.query_trading_notice()
        elif ex == 'investor':
            trader.query_investor()
        elif ex == 'exchange':
            trader.query_exchange()
        elif ex == 'product':
            trader.query_product()
        elif ex == 'instrument':
            trader.query_instrument()
        elif ex == 'instruments':
            trader.query_instruments()
        elif ex == 'position':
            trader.query_position()
        elif ex == 'positiondetail':
            trader.query_position_detail()
        elif ex == 'positioncombdetail':
            trader.query_position_comb_detail()
        elif ex == 'parkorder':
            trader.query_park_order()
        elif ex == 'parkorderaction':
            trader.query_park_order_action()
        elif ex == 'order':
            trader.query_order()
        elif ex == 'maxvolume':
            trader.query_order_max_volume("rb1705")
        elif ex == 'trade':
            trader.query_trade()
        elif ex == 'accountregister':
            trader.query_account_register()
        elif ex == 'transferbank':
            trader.query_transfer_bank()
        elif ex == 'contractbank':
            trader.query_contract_bank()
        elif ex == 'tradeaccount':
            trader.query_trade_account()
        elif ex == 'tradecode':
            trader.query_trade_code()
        elif ex == 'depositmoney':
            trader.deposit_money("13", 1)
        elif ex == 'withdrawmoney':
            trader.withdraw_money("13", 1)
        elif ex == 'transfer':
            trader.query_transfer("13")
        elif ex == 'bankmoney':
            trader.query_bank_money("13")
        elif ex == 'marketdata':
            trader.query_market_data("v1711")

        elif ex == 'askquote':
            trader.ask_quote("rb1705")

        elif ex == 'insertlimitpriceorder':
            trader.insert_limit_price_order(iid="v1711", direction=ORDER_DIRECTION_BUY, volume=1, price=1.0, offset_flags=[OFFSET_FLAG_OPEN], hedge_flags=[HEDGE_FLAG_SPECULATION])
        elif ex == 'insertmarketpriceorder':
            trader.insert_market_price_order(iid="v1711", direction=ORDER_DIRECTION_BUY, volume=1, offset_flags=[OFFSET_FLAG_OPEN], hedge_flags=[HEDGE_FLAG_SPECULATION])
        elif ex == 'insertconditionpriceorder':
            trader.insert_condition_order(iid="v1711", direction=ORDER_DIRECTION_BUY, volume=1, stop_price=1.0, contingent_condition=CONDITION_TRIGGER_LAST_LE_STOP, time_condition=TIME_CONDITION_GFOR_DAY, offset_flags=[OFFSET_FLAG_OPEN], hedge_flags=[HEDGE_FLAG_SPECULATION])
        elif ex == 'insertfokorder':
            trader.insert_fok_order(iid="v1711", direction=ORDER_DIRECTION_BUY, volume=1, price=1.0, offset_flags=[OFFSET_FLAG_OPEN], hedge_flags=[HEDGE_FLAG_SPECULATION])
        elif ex == 'insertfakorder':
            trader.insert_fak_order(iid="v1711", direction=ORDER_DIRECTION_BUY, volume=1, price=1.0, offset_flags=[OFFSET_FLAG_OPEN], hedge_flags=[HEDGE_FLAG_SPECULATION])

        elif ex == 'deleteorderbyref':
            trader.delete_order_by_ref(fid='8', sid='23234324', order_ref='000000002')
        elif ex == 'deleteorderbysysid':
            trader.delete_order_by_sysid(eid="DCE", order_sys_id="TJBD_0000124")

        elif ex == 'deletepark':
            trader.delete_park('232323')
        elif ex == 'deleteparkaction':
            trader.delete_park_action('232323')

        elif ex == 'instrumentmarginrate':
            trader.query_instrument_margin_rate("rb1705", HEDGE_FLAG_SPECULATION)
        elif ex == 'instrumentcommissionrate':
            trader.query_instrument_commission_rate("rb1705")

        elif ex == 'exchangemarginrate':
            trader.query_exchange_margin_rate("rb1705", HEDGE_FLAG_SPECULATION)
        elif ex == 'exchangerateadjust':
            trader.query_exchange_margin_rate_adjust("rb1705", HEDGE_FLAG_SPECULATION)
        elif ex == 'currencyrate':
            trader.query_currency_rate("CNY", "USD")
        elif ex == 'productrate':
            trader.query_product_rate("MA")
        elif ex == 'productgroup':
            trader.query_product_group("MA", None)

    print 'exit command while'

