/*
 * test_SUM.cpp
 *
 *  Created on: 2019年4月2日
 *      Author: fasiondog
 */

#ifdef TEST_ALL_IN_ONE
    #include <boost/test/unit_test.hpp>
#else
    #define BOOST_TEST_MODULE test_hikyuu_indicator_suite
    #include <boost/test/unit_test.hpp>
#endif

#include <fstream>
#include <hikyuu/StockManager.h>
#include <hikyuu/indicator/crt/SUM.h>
#include <hikyuu/indicator/crt/KDATA.h>
#include <hikyuu/indicator/crt/PRICELIST.h>

using namespace hku;

/**
 * @defgroup test_indicator_SUM test_indicator_SUM
 * @ingroup test_hikyuu_indicator_suite
 * @{
 */

/** @par 检测点 */
BOOST_AUTO_TEST_CASE( test_SUM ) {
    Indicator result;

    PriceList a;
    for (int i = 0; i < 10; ++i) {
        a.push_back(i);
    }

    Indicator data = PRICELIST(a);

    /** @arg n = 0 */
    result = SUM(data, 0);
    BOOST_CHECK(result.name() == "SUM");
    BOOST_CHECK(result.size() == 10);
    BOOST_CHECK(result.discard() == 0);
    price_t sum = 0;
    for (int i = 0; i <10; ++i) {
        sum += data[i];
        BOOST_CHECK(result[i] == sum);
    }

    /** @arg n = 1 */
    result = SUM(data, 1);
    BOOST_CHECK(result.size() == 10);
    BOOST_CHECK(result.discard() == 0);
    for (int i = 0; i < 10; ++i) {
        BOOST_CHECK(result[i] == data[i]);
    }

    /** @arg n = 9 */
    result = SUM(data, 9);
    BOOST_CHECK(result.size() == 10);
    BOOST_CHECK(result.discard() == 8);
    BOOST_CHECK(result[8] == 36);
    BOOST_CHECK(result[9] == 45);

    /** @arg n = 10 */
    result = SUM(data, 10);
    BOOST_CHECK(result.size() == 10);
    BOOST_CHECK(result.discard() == 9);
    BOOST_CHECK(result[9] == 45);
}


//-----------------------------------------------------------------------------
// test export
//-----------------------------------------------------------------------------
#if HKU_SUPPORT_SERIALIZATION

/** @par 检测点 */
BOOST_AUTO_TEST_CASE( test_SUM_export ) {
    StockManager& sm = StockManager::instance();
    string filename(sm.tmpdir());
    filename += "/SUM.xml";

    Stock stock = sm.getStock("sh000001");
    KData kdata = stock.getKData(KQuery(-20));
    Indicator x1 = SUM(CLOSE(kdata), 3);
    {
        std::ofstream ofs(filename);
        boost::archive::xml_oarchive oa(ofs);
        oa << BOOST_SERIALIZATION_NVP(x1);
    }

    Indicator x2;
    {
        std::ifstream ifs(filename);
        boost::archive::xml_iarchive ia(ifs);
        ia >> BOOST_SERIALIZATION_NVP(x2);
    }

    BOOST_CHECK(x1.size() == x2.size());
    BOOST_CHECK(x1.discard() == x2.discard());
    BOOST_CHECK(x1.getResultNumber() == x2.getResultNumber());
    for (size_t i = x1.discard(); i < x1.size(); ++i) {
        BOOST_CHECK_CLOSE(x1[i], x2[i], 0.00001);
    }
}
#endif /* #if HKU_SUPPORT_SERIALIZATION */

/** @} */


