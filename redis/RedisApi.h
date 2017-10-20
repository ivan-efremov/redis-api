/**
 * @file RedisApi.h
 */

#pragma once

#include <memory>
#include <string>


/**
 * class RedisApi.
 * @short API редиса, методы формируют строку запроса.
 */
class RedisApi
{
	enum {
		RESERVE = 2048
	};
public:
	            RedisApi();
	// list
	std::string rpush(const std::string& a_key, const std::string& a_data);
	std::string blpop(const std::string& a_key);
	std::string llen(const std::string& a_key);
	// channel
	std::string publish(const std::string& a_key, const std::string& a_data);
	std::string subscribe(const std::string& a_key);
	// hash
	std::string hset(const std::string& a_hash, const std::string& a_key, const std::string& a_data);
	std::string hget(const std::string& a_hash, const std::string& a_key);
	std::string hgetall(const std::string& a_hash);
	std::string hkeys(const std::string& a_hash);
	std::string hdel(const std::string& a_hash, const std::string& a_key);
	// sorted_set
	std::string zadd(const std::string& a_key, const std::string& a_score, const std::string& a_data);
	std::string zcount(const std::string& a_key, const std::string& a_min, const std::string& a_max);
	std::string zrange(const std::string& a_key, const std::string& a_min, const std::string& a_max);
	std::string zrangebyscore(const std::string& a_key, const std::string& a_min, const std::string& a_max);
	std::string zremrangebyscore(const std::string& a_key, const std::string& a_min, const std::string& a_max);
	// db
	std::string select(const std::string& a_db);
	std::string keys(const std::string& a_pattern);
	std::string del(const std::string& a_key);
private:
	std::string m_cmd;
};

typedef std::shared_ptr<RedisApi> PRedisApi;

