/*
 * KDataTempCsvDriver.h
 *
 *  Created on: 2017年7月30日
 *      Author: fasiondog
 */

#pragma once
#ifndef DATA_DRIVER_KDATATEMPCSVDRIVER_H_
#define DATA_DRIVER_KDATATEMPCSVDRIVER_H_

#include "KDataDriver.h"

namespace hku {

/**
 * 获取临时载入的CSV文件
 */
class KDataTempCsvDriver: public KDataDriver {
public:
    KDataTempCsvDriver(const string& day_filename, const string& min_filename);
    virtual ~KDataTempCsvDriver();

    /**
     * 将指定类型的K线数据加载至缓存
     * @param market 市场简称
     * @param code   证券代码
     * @param kType  K线类型
     * @param start_ix 欲加载的起始位置
     * @param end_ix 欲加载的结束位置，不包含自身
     * @param out_buffer [out] 缓存指针
     */
    virtual void loadKData(const string& market, const string& code,
            KQuery::KType kType, size_t start_ix, size_t end_ix,
            KRecordListPtr out_buffer) ;

    /**
     * 获取指定类型的K线数据量
     * @param market 市场简称
     * @param code   证券代码
     * @param kType  K线类型
     * @return
     */
    virtual size_t getCount(const string& market, const string& code,
            KQuery::KType kType);

    /**
     * 获取指定日期范围对应的K线记录索引
     * @param market 市场简称
     * @param code   证券代码
     * @param query  查询条件
     * @param out_start [out] 对应K线记录位置
     * @param out_end [out] 对应的K线记录位置
     * @return
     */
    virtual bool getIndexRangeByDate(const string& market, const string& code,
            const KQuery& query, size_t& out_start, size_t& out_end);

    /**
     * 获取指定的K线记录
     * @param market 市场简称
     * @param code   证券代码
     * @param pos    K线记录索引
     * @param kType  K线类型
     * @return
     */
    virtual KRecord getKRecord(const string& market, const string& code,
              size_t pos, KQuery::KType kType);

private:
    void _get_title_column(const string&);
    void _get_token(const string&);
    string _get_filename();

private:
    string m_day_filename;
    string m_min_filename;

    enum COLUMN {
        DATE = 0,
        OPEN = 1,
        HIGH = 2,
        LOW = 3,
        CLOSE = 4,
        VOLUME = 5,
        AMOUNT = 6,
        LAST = 7
    };

    size_t m_column[LAST];
    vector<string> m_token_buf;
};

} /* namespace hku */

#endif /* DATA_DRIVER_KDATATEMPCSVDRIVER_H_ */
