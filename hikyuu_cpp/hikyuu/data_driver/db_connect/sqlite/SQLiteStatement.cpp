/*
 * SQLiteStatement.cpp
 *
 *  Copyright (c) 2019, hiyuu.org
 * 
 *  Created on: 2019-7-11
 *      Author: linjinhai
 */

#include "SQLiteStatement.h"
#include "SQLiteConnect.h"

namespace hku {

SQLiteStatement
::SQLiteStatement(const DBConnectPtr& driver, const string& sql_statement)
: SQLStatementBase(driver, sql_statement),
  m_needs_reset(false),
  m_step_status(SQLITE_DONE),
  m_at_first_step(true),
  m_stmt(NULL) {
    m_db = (dynamic_cast<SQLiteConnect *>(driver.get()))->m_db;
    int status = sqlite3_prepare_v2(m_db.get(), 
                                 m_sql_string.c_str(),
                                 m_sql_string.size()+1,
                                 &m_stmt,
                                 NULL);
    if (status != SQLITE_OK) {
        sqlite3_finalize(m_stmt);
        HKU_THROW(sqlite3_errmsg(m_db.get()));
    }

    HKU_ASSERT_M(m_stmt != 0, "Invalid SQL statement: {}", m_sql_string);
}

SQLiteStatement::~SQLiteStatement() {
    sqlite3_finalize(m_stmt);
}

void SQLiteStatement::_reset() {
    if (m_needs_reset) {
        if (sqlite3_reset(m_stmt) != SQLITE_OK) {
            m_step_status = SQLITE_DONE;
            HKU_THROW(sqlite3_errmsg(m_db.get()));
        }
        m_needs_reset = false;
        m_step_status = SQLITE_DONE;
        m_at_first_step = true;
    }
}

void SQLiteStatement::sub_exec() {
    _reset();
    m_step_status = sqlite3_step(m_stmt);
    m_needs_reset = true;
    if (m_step_status != SQLITE_DONE && m_step_status != SQLITE_ROW) {
        HKU_THROW(sqlite3_errmsg(m_db.get()));
    }
}

bool SQLiteStatement::sub_moveNext () {
    if (m_step_status == SQLITE_ROW) {
        if (m_at_first_step) {
            m_at_first_step = false;
            return true;
        } else {
            m_step_status = sqlite3_step(m_stmt);
            if (m_step_status == SQLITE_DONE) {
                return false;
            } else if (m_step_status == SQLITE_ROW) {
                return true;
            } else {
                HKU_THROW(sqlite3_errmsg(m_db.get()));
            }
        }
    } else {
        return false;
    }
}

int SQLiteStatement::sub_getNumColumns() const {
    return (m_at_first_step == false) && (m_step_status == SQLITE_ROW)
           ? sqlite3_column_count(m_stmt)
           : 0;
}

void SQLiteStatement::sub_bindNull(int idx) {
    _reset();
    int status = sqlite3_bind_null(m_stmt, idx+1);
    HKU_ASSERT_M(status == SQLITE_OK, sqlite3_errmsg(m_db.get()));
}

void SQLiteStatement::sub_bindInt(int idx, int64 value) {
    _reset();
    int status = sqlite3_bind_int64(m_stmt, idx+1, value);
    HKU_ASSERT_M(status == SQLITE_OK, sqlite3_errmsg(m_db.get()));
}

void SQLiteStatement::sub_bindText(int idx, const string& item) {
    _reset();
    int status = sqlite3_bind_text(m_stmt, idx+1, item.c_str(), -1, SQLITE_TRANSIENT);
    HKU_ASSERT_M(status == SQLITE_OK, sqlite3_errmsg(m_db.get()));
}

void SQLiteStatement::sub_bindDouble(int idx, double item) {
    _reset();
    int status = sqlite3_bind_double(m_stmt, idx+1, item);
    HKU_ASSERT_M(status == SQLITE_OK, sqlite3_errmsg(m_db.get()));
}

void SQLiteStatement::sub_bindBlob(int idx, const string& item) {
    _reset();
    int status = sqlite3_bind_blob(m_stmt, idx+1, item.data(), item.size(), SQLITE_TRANSIENT);
    HKU_ASSERT_M(status == SQLITE_OK, sqlite3_errmsg(m_db.get()));
}

void SQLiteStatement::sub_getColumnAsInt64(int idx, int64& item) {
    item = sqlite3_column_int64(m_stmt, idx);
}

void SQLiteStatement::sub_getColumnAsDouble(int idx, double& item) {
    item = sqlite3_column_double(m_stmt, idx); 
}

void SQLiteStatement::sub_getColumnAsText(int idx, string& item) {
    const char* data = reinterpret_cast<const char*>(sqlite3_column_text(m_stmt, idx));
    item = (data != 0) ? string(data) : string();
}

void SQLiteStatement::sub_getColumnAsBlob(int idx, string& item) {
    const char* data = static_cast<const char*>(sqlite3_column_blob(m_stmt, idx));
    if (data == NULL) {
        throw null_blob_exception();
    }
    const int size = sqlite3_column_bytes(m_stmt, idx);
    item = std::string(data,size);
}

} /* namespace */