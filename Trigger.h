/*
 * @file Trigger.h
 */
#pragma once
#include <map>
#include <thread>
#include <atomic>
#include "redis/RedisIO.h"



/**
 * class SpinLock.
 */
class SpinLock
{
public:
	void lock() {
		while(m_locked.test_and_set(std::memory_order_acquire));
	}
	void unlock() {
		m_locked.clear(std::memory_order_release);
	}
private:
	std::atomic_flag m_locked = ATOMIC_FLAG_INIT;
};


/**
 * class Trigger.
 * @short Триггер - состояние объекта.
 */
class Trigger
{
	struct RedisInfo {
	    PRedisIO  m_redisIO;
	    PRedisApi m_redisApi;
	              RedisInfo(PRedisIO a_redisIO, PRedisApi a_redisApi):
	                  m_redisIO(a_redisIO),m_redisApi(a_redisApi)
	    {
	    }
	};
	typedef std::shared_ptr<RedisInfo>            PRedisInfo;
	typedef std::map<std::thread::id, PRedisInfo> ThreadPRedisInfo;
public:
	                  Trigger(const std::string& a_name,
	                          const std::string& a_server,
	                          const std::string& a_port);
	void              setState(const std::string& a_key);
	bool              getState(const std::string& a_key);
	time_t            getTime(const std::string& a_key);
	void              clear(const std::string& a_key);
	void              clearByMask(const std::string& a_key);
	std::string       getName() const;
private:
	void              reconnect(std::thread::id a_threadId);
	void              clrRedisInfo();
	PRedisInfo        getRedisInfo();
private:
	const std::string m_name;
	const std::string m_server;
	const std::string m_port;
	SpinLock          m_lock;
	ThreadPRedisInfo  m_redisInfo;
};

typedef std::shared_ptr<Trigger> PTrigger;
