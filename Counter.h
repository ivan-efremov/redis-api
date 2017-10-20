/*
 * @file Counter.h
 */

#pragma once
#include "redis/RedisIO.h"


typedef std::list<std::string>                    ListStr;
typedef std::shared_ptr<ListStr>                  PListStr;


/**
 * class Counter.
 * @short Счетчик объектов.
 */
class Counter
{
public:
	            Counter(const std::string& a_name,
	                    const std::string& a_server,
	                    const std::string& a_port);
	void        add(time_t                   a_time,
	                const std::string&       a_key,
	                const std::string&       a_object);
	PListStr    getObjectsList(const std::string& a_key);
	size_t      getCounter(const std::string& a_key);
	size_t      delKey(const std::string& a_key,
	                   time_t             a_time = 0);
	PListStr    getKeys();
	std::string getName() const;
private:
	void        reconnect();
private:
        const std::string m_name;
        PRedisIO          m_redisIO;
        PRedisApi         m_redisApi;
};

typedef std::shared_ptr<Counter> PCounter;

