#include "Counter.h"
#include <boost/algorithm/string.hpp>


Counter::Counter(
        const std::string& a_name,
        const std::string& a_server,
        const std::string& a_port)
:
        m_name(a_name),
        m_redisIO(std::make_shared<RedisIO>(a_server, a_port)),
        m_redisApi(std::make_shared<RedisApi>())
{
        try {
                m_redisIO->api(m_redisApi->select("1"));
        } catch(const std::exception&) {
                // ignore error
        }
}

std::string Counter::makeKey(
        const std::string&       a_correlationId,
        const std::string&       a_ruleIndex,
        PListGroupByGroupByValue a_listGroupByGroupByValue)
{
        std::string key;
        key.reserve(128);
        key += m_name;
        key += "|";
        key += a_correlationId;
        key += "|";
        key += a_ruleIndex;
        std::for_each(
                a_listGroupByGroupByValue->begin(),
                a_listGroupByGroupByValue->end(),
                [&key](const GroupByGroupByValue& item)
        {
                key += "|";
                key += item.first;
                key += "|";
                key += item.second;
        });
        return key;
}

void Counter::addEvent(
        time_t                   a_time,
        const std::string&       a_correlationId,
        const std::string&       a_ruleIndex,
        const std::string&       a_eventId,
        PListGroupByGroupByValue a_listGroupByGroupByValue)
{
        std::string key(
                makeKey(a_correlationId, a_ruleIndex, a_listGroupByGroupByValue)
        );
        do {
                try {
                        m_redisIO->api(
                                m_redisApi->zadd(key, std::to_string(a_time), a_eventId)
                        );
                        break;
                } catch(const std::exception& err) {
/*                        g_log->print(LOGLEVEL_ERROR, "Counter::addEvent(): %s: %s",
                                err.what(), m_redisIO->error().message().c_str()
                        );
*/                        reconnect();
                }
        } while(1);
}

PListStr Counter::getEventList(
        const std::string&       a_correlationId,
        const std::string&       a_ruleIndex,
        int                      a_period,
        PListGroupByGroupByValue a_listGroupByGroupByValue)
{
        RedisIO::PListValue lv;
        std::string key(
                makeKey(a_correlationId, a_ruleIndex, a_listGroupByGroupByValue)
        );
        time_t now(time(NULL));
        do {
                try {
                        lv = m_redisIO->api(
                                m_redisApi->zrangebyscore(
                                        key, std::to_string(now - a_period), std::to_string(now)
                                )
                        );
                        break;
                } catch(const std::exception& err) {
/*                        g_log->print(LOGLEVEL_ERROR, "Counter::getEventList(): %s: %s",
                                err.what(), m_redisIO->error().message().c_str()
                        );
*/                        reconnect();
                }
        } while(1);
        return lv;
}

size_t Counter::getCounter(
        const std::string&       a_correlationId,
        const std::string&       a_ruleIndex,
        int                      a_period,
        PListGroupByGroupByValue a_listGroupByGroupByValue)
{
        RedisIO::PListValue lv;
        std::string key(
                makeKey(a_correlationId, a_ruleIndex, a_listGroupByGroupByValue)
        );
        time_t now(time(NULL));
        do {
                try {
                        lv = m_redisIO->api(
                                m_redisApi->zcount(
                                        key, std::to_string(now - a_period), std::to_string(now)
                                )
                        );
                        if(!lv->empty()) {
                                return std::stol(lv->front());
                        }
                        break;
                } catch(const std::exception& err) {
/*                        g_log->print(LOGLEVEL_ERROR, "Counter::getCounter(): %s: %s",
                                err.what(), m_redisIO->error().message().c_str()
                        );
*/                        reconnect();
                }
        } while(1);
        return 0;
}

size_t Counter::getKeysCount(
        const std::string&       a_correlationId,
        const std::string&       a_ruleIndex,
        PListGroupByGroupByValue a_listGroupByGroupByValue,
        const std::string&       a_fnGroupBy)
{
        RedisIO::PListValue lv;
        std::string key(
                makeKey(a_correlationId, a_ruleIndex, a_listGroupByGroupByValue)
        );
        size_t n(0);
        do {
                try {
                        lv = m_redisIO->api(
                                m_redisApi->keys(key + "|" + a_fnGroupBy + "|*")
                        );
                #if 0
                        std::for_each(lv->begin(), lv->end(), [](const std::string& a_key) {
                                g_log->print(LOGLEVEL_DEBUG, "Counter::getKeysCount(): %s", a_key.c_st
                        });
                #endif
                        n = lv->size();
                        break;
                } catch(const std::exception& err) {
/*                        g_log->print(LOGLEVEL_ERROR, "Counter::getKeysCount(): %s: %s",
                                err.what(), m_redisIO->error().message().c_str()
                        );
*/                        reconnect();
                }
        } while(1);
        return n;
}

time_t Counter::getFirstEventTime(
        const std::string&       a_correlationId,
        const std::string&       a_ruleIndex,
        PListGroupByGroupByValue a_listGroupByGroupByValue)
{
        RedisIO::PListValue lv;
        std::string key(
                makeKey(a_correlationId, a_ruleIndex, a_listGroupByGroupByValue)
        );
        time_t t(0);
        do {
                try {
                        lv = m_redisIO->api(
                                m_redisApi->zrange(key, "0", "0")
                        );
                #if 0
                        std::for_each(lv->begin(), lv->end(), [](const std::string& a_key) {
                                g_log->print(LOGLEVEL_DEBUG, "Counter::getFirstEventTime(): %s", a_key
                        });
                #endif
                        if(lv->size() > 1) {
                                auto it(lv->begin());
                                ++it;
                                t = std::stol(*it);
                        }
                        break;
                } catch(const std::exception& err) {
/*                        g_log->print(LOGLEVEL_ERROR, "Counter::getFirstEventTime(): %s: %s",
                                err.what(), m_redisIO->error().message().c_str()
                        );
*/                        reconnect();
                }
        } while(1);
        return t;
}

RedisIO::PListValue Counter::getKeys()
{
        do {
                try {
                        RedisIO::PListValue lv(
                                m_redisIO->api(
                                        m_redisApi->keys(
                                                m_name + '*'
                                        )
                                )
                        );
                        return lv;
                } catch(const std::exception& err) {
/*                        g_log->print(LOGLEVEL_ERROR, "Counter::getKeys(): %s: %s",
                                err.what(), m_redisIO->error().message().c_str()
                        );
*/                        reconnect();
                }
        } while(1);
        return std::make_shared<RedisIO::ListValue>();
}

size_t Counter::delKey(
        const std::string& a_key,
        time_t             a_time)
{
        do {
                try {
                        RedisIO::PListValue res;
                        if(a_time) {
                                res = m_redisIO->api(
                                        m_redisApi->zremrangebyscore(
                                                a_key, "0", std::to_string(a_time)
                                        )
                                );
                        } else {
                                res = m_redisIO->api(
                                        m_redisApi->del(a_key)
                                );
                        }
                        if(res && !res->empty()) {
                                return std::stol(res->front());
                        }
                } catch(const std::exception& err) {
/*                        g_log->print(LOGLEVEL_ERROR, "Counter::delKeys(): %s: %s",
                                err.what(), m_redisIO->error().message().c_str()
                        );
*/                        reconnect();
                }
        } while(1);
        return 0;
}

size_t Counter::cleanup(time_t a_time)
{
        size_t n = 0;
        RedisIO::PListValue lv;
        do {
                try {
                        lv = m_redisIO->api(
                                m_redisApi->keys(
                                        m_name + "|*"
                                )
                        );
                        std::for_each(lv->begin(), lv->end(),
                                      [this, &a_time, &n](const std::string& a_key)
                        {
                                RedisIO::PListValue res = this->m_redisIO->api(
                                        this->m_redisApi->zremrangebyscore(
                                                a_key, "0", std::to_string(a_time)
                                        )
                                );
                                if(!res->empty()) {
                                        n += std::stol(res->front());
                                }
                        });
                        break;
                } catch(const std::exception& err) {
/*                        g_log->print(LOGLEVEL_ERROR, "Counter::cleanup(): %s: %s",
                                err.what(), m_redisIO->error().message().c_str()
                        );
*/                        reconnect();
                }
        } while(1);
        return n;
}

void Counter::reset(
        const std::string& a_correlationId,
        const std::string& a_groupBy,
        const std::string& a_groupByValue)
{
        RedisIO::PListValue lv;
        do {
                try {
                        lv = m_redisIO->api(
                                m_redisApi->keys(
                                        m_name + "|" + a_correlationId + "|*"
                                )
                        );
                        std::for_each(lv->begin(), lv->end(),
                                [this,a_groupBy,a_groupByValue](const std::string& a_key)
                        {
                                // counter|32|0|src.ip|187.11.213.143
                                std::string gr, grVal;
                                std::vector<std::string> vk;
                                boost::split(vk, a_key, boost::is_any_of("|"));
                                gr = vk.at(3);
                                grVal = vk.at(4);
                                if(gr == a_groupBy && grVal == a_groupByValue) {
                                        this->m_redisIO->api(
                                                this->m_redisApi->del(
                                                        a_key
                                                )
                                        );
                                }
                        });
                        break;
                } catch(const std::exception& err) {
/*                        g_log->print(LOGLEVEL_ERROR, "Counter::reset(): %s: %s",
                                err.what(), m_redisIO->error().message().c_str()
                        );
*/                        reconnect();
                }
        } while(1);
}

void Counter::reconnect()
{
        try {
                m_redisIO->close();
                sleep(1);
                m_redisIO->connect();
                m_redisIO->api(m_redisApi->select("1"));
        } catch(const std::exception& err) {
/*                g_log->print(LOGLEVEL_ERROR, "Counter::reconnect(): %d: %s",
                        m_redisIO->error().value(), err.what()
                );
*/        }
}

std::string Counter::getName() const
{
        return m_name;
}
