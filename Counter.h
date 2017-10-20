#pragma once
#include "redis/RedisIO.h"


typedef std::pair<std::string, std::string>       GroupByGroupByValue;
typedef std::list<GroupByGroupByValue>            ListGroupByGroupByValue;
typedef std::shared_ptr<ListGroupByGroupByValue>  PListGroupByGroupByValue;
typedef std::list<std::string>                    ListStr;
typedef std::shared_ptr<ListStr>                  PListStr;
typedef std::list<int>                            ListInt;
typedef std::shared_ptr<ListInt>                  PListInt;


/**
 * class Counter.
 * @short Счетчик эвентов.
 */
class Counter
{
public:
                    Counter(const std::string& a_name,
                            const std::string& a_server,
                            const std::string& a_port);
        void        addEvent(time_t                   a_time,
                             const std::string&       a_correlationId,
                             const std::string&       a_ruleIndex,
                             const std::string&       a_eventId,
                             PListGroupByGroupByValue a_listGroupByGroupByValue);
        PListStr    getEventList(const std::string&       a_correlationId,
                                 const std::string&       a_ruleIndex,
                                 int                      a_period,
                                 PListGroupByGroupByValue a_listGroupByGroupByValue);
        size_t      getCounter(const std::string&       a_correlationId,
                               const std::string&       a_ruleIndex,
                               int                      a_period,
                               PListGroupByGroupByValue a_listGroupByGroupByValue);
        size_t      getKeysCount(const std::string&       a_correlationId,
                                 const std::string&       a_ruleIndex,
                                 PListGroupByGroupByValue a_listGroupByGroupByValue,
                                 const std::string&       a_fnGroupBy);
        time_t      getFirstEventTime(const std::string&       a_correlationId,
                                      const std::string&       a_ruleIndex,
                                      PListGroupByGroupByValue a_listGroupByGroupByValue);
        size_t      delKey(const std::string& a_key,
                           time_t             a_time = 0);
        RedisIO::PListValue getKeys();
        void        reset(const std::string& a_correlationId,
                          const std::string& a_groupBy,
                          const std::string& a_groupByValue);
        size_t      cleanup(time_t a_time);
        void        reconnect();
        std::string getName() const;
private:
        std::string makeKey(const std::string&       a_correlationId,
                            const std::string&       a_ruleIndex,
                            PListGroupByGroupByValue a_listGroupByGroupByValue);
private:
        const std::string m_name;
        PRedisIO          m_redisIO;
        PRedisApi         m_redisApi;
};
typedef std::shared_ptr<Counter> PCounter;
