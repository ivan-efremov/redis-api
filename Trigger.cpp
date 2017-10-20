#include "Trigger.h"


Trigger::Trigger(const std::string& a_name)
:
    m_name(a_name)
{
}

void Trigger::reconnect(std::thread::id a_threadId)
{
    PRedisIO redisIO;
    PRedisApi redisApi;
    PRedisInfo redisInfo;
    do {
	try {
	    redisIO = std::make_shared<RedisIO>(
		"127.0.0.1",
		"6379"
	    );
	    redisApi = std::make_shared<RedisApi>();
	    redisIO->api(redisApi->select("1"));
	    redisInfo = std::make_shared<RedisInfo>(redisIO, redisApi);
	    m_lock.lock();
	    m_redisInfo[a_threadId] = redisInfo;
	    m_lock.unlock();
	} catch(const std::exception& err) {
/*	    g_log->print(LOGLEVEL_ERROR, "Trigger::reconnect(): %d: %s",
		redisIO->error().value(), err.what()
	    );*/
	    sleep(1);
	}
    } while(redisIO->error().value());
}

void Trigger::clrRedisInfo()
{
    m_lock.lock();
    m_redisInfo.clear();
    m_lock.unlock();
}

Trigger::PRedisInfo Trigger::getRedisInfo()
{
    PRedisInfo redisInfo;
    std::thread::id threadId = std::this_thread::get_id();
    do {
	m_lock.lock();
	redisInfo = m_redisInfo[threadId];
	m_lock.unlock();
	if(redisInfo) {
	    break;
	} else {
	    reconnect(threadId);
	}
    } while(1);
    return redisInfo;
}

void Trigger::setState(const std::string& a_key)
{
    PRedisInfo redisInfo = getRedisInfo();
    try {
	RedisIO::PListValue result(
	    redisInfo->m_redisIO->api(
		redisInfo->m_redisApi->hset(m_name, a_key, std::to_string(std::time(NULL)))
	    )
	);
    } catch(const std::exception& err) {
	clrRedisInfo();
//	g_log->print(LOGLEVEL_ERROR, "Trigger::setState(): %s", err.what());
    }
}

bool Trigger::getState(const std::string& a_key)
{
    PRedisInfo redisInfo = getRedisInfo();
    try {
	RedisIO::PListValue result(
	    redisInfo->m_redisIO->api(
		redisInfo->m_redisApi->hget(m_name, a_key)
	    )
	);
	return (result->size() && result->front() != "nil") ? true : false;
    } catch(const std::exception& err) {
	clrRedisInfo();
//	g_log->print(LOGLEVEL_ERROR, "Trigger::getState(): %s", err.what());
    }
    return false;
}

time_t Trigger::getTime(const std::string& a_key)
{
    PRedisInfo redisInfo = getRedisInfo();
    try {
	RedisIO::PListValue result(
	    redisInfo->m_redisIO->api(
		redisInfo->m_redisApi->hget(m_name, a_key)
	    )
	);
	return (result->size() && result->front() != "nil") ? std::stoul(result->front()) : 0UL;
    } catch(const std::exception& err) {
	clrRedisInfo();
//	g_log->print(LOGLEVEL_ERROR, "Trigger::getTime(): %s", err.what());
    }
    return 0UL;
}

void Trigger::clear(const std::string& a_key)
{
    PRedisInfo redisInfo = getRedisInfo();
    try {
	RedisIO::PListValue result(
	    redisInfo->m_redisIO->api(
		redisInfo->m_redisApi->hdel(m_name, a_key)
	    )
	);
    } catch(const std::exception& err) {
	clrRedisInfo();
//	g_log->print(LOGLEVEL_ERROR, "Trigger::clear(): %s", err.what());
    }
}

void Trigger::clearByMask(const std::string& a_key)
{
    PRedisInfo redisInfo = getRedisInfo();
    try {
	RedisIO::PListValue result(
	    redisInfo->m_redisIO->api(
		redisInfo->m_redisApi->hkeys(m_name)
	    )
	);
	std::for_each(result->begin(), result->end(), [this,&a_key](const std::string& item) {
	    if(item.compare(0, a_key.length(), a_key) == 0) {
		this->clear(item);
	    }
	});
    } catch(const std::exception& err) {
	clrRedisInfo();
//	g_log->print(LOGLEVEL_ERROR, "Trigger::clearByMask(): %s", err.what());
    }
}
