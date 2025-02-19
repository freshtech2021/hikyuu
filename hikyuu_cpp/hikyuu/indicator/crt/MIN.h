/*
 * MIN.h
 *
 *  Created on: 2019年4月8日
 *      Author: fasiondog
 */

#pragma once
#ifndef INDICATOR_CRT_MIN_H_
#define INDICATOR_CRT_MIN_H_

#include "CVAL.h"

namespace hku {

/**
 * 求最小值
 * @details
 * <pre>
 * 用法: MIN(A,B)返回A和B中的较小值
 * 例如: MIN(CLOSE,OPEN)返回开盘价和收盘价中的较小值
 * </pre>
 * @ingroup Indicator
 */
Indicator MIN(const Indicator&, const Indicator&);
Indicator MIN(const Indicator&, price_t val);
Indicator MIN(price_t val, const Indicator& ind);

inline Indicator MIN(const Indicator& ind1, const Indicator& ind2) {
    Indicator result = IF(ind1 < ind2, ind1, ind2);
    result.name("MIN");
    return result;
}

inline Indicator MIN(const Indicator& ind, price_t val) {
    Indicator result = IF(ind < val, ind, val);
    result.name("MIN");
    return result;
}

inline Indicator MIN(price_t val, const Indicator& ind) {
    Indicator result = IF(val < ind, val, ind);
    result.name("MIN");
    return result;
}

}


#endif /* INDICATOR_CRT_MIN_H_ */
