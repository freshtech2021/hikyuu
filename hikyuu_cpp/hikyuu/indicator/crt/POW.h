/*
 * POW.h
 * 
 *  Copyright (c) 2019 hikyuu.org
 *
 *  Created on: 2019-4-2
 *      Author: fasiondog
 */

#pragma once
#ifndef INDICATOR_CRT_POW_H_
#define INDICATOR_CRT_POW_H_

#include "CVAL.h"

namespace hku {

/**
 * 乘幂
 * @details
 * <pre>
 * 用法：POW(A,B)返回A的B次幂
 * 例如：POW(CLOSE,3)求得收盘价的3次方
 * </pre>
 * @ingroup Indicator
 */
Indicator HKU_API POW(int n);
Indicator POW(price_t, int n);
Indicator POW(const Indicator& ind, int n);

inline Indicator POW(const Indicator& ind, int n) {
    return POW(n)(ind);
}

inline Indicator POW(price_t val, int n) {
    return POW(CVAL(val), n);
}

} /* namespace */

#endif /* INDICATOR_CRT_POW_H_ */
