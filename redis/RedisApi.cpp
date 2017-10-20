#include "RedisApi.h"


RedisApi::RedisApi()
{
	m_cmd.reserve(RESERVE);
}

std::string RedisApi::rpush(
	const std::string& a_key,
	const std::string& a_data)
{
	m_cmd  = "*3\r\n"
	         "$5\r\n"
	         "rpush\r\n"
	         "$";
	m_cmd += std::to_string(a_key.length());
	m_cmd += "\r\n";
	m_cmd += a_key;
	m_cmd += "\r\n"
	         "$";
	m_cmd += std::to_string(a_data.length());
	m_cmd += "\r\n";
	m_cmd += a_data;
	return m_cmd;
}

std::string RedisApi::blpop(
	const std::string& a_key)
{
	m_cmd  = "*3\r\n"
	         "$5\r\n"
	         "blpop\r\n"
	         "$";
	m_cmd += std::to_string(a_key.length());
	m_cmd += "\r\n";
	m_cmd += a_key;
	m_cmd += "\r\n"
	         "$1\r\n"
	         "0";
	return m_cmd;
}

std::string RedisApi::llen(
	const std::string& a_key)
{
	m_cmd  = "*2\r\n"
	         "$4\r\n"
	         "llen\r\n"
	         "$";
	m_cmd += std::to_string(a_key.length());
	m_cmd += "\r\n";
	m_cmd += a_key;
	return m_cmd;
}

std::string RedisApi::publish(
	const std::string& a_key,
	const std::string& a_data)
{
	m_cmd  = "*3\r\n"
	         "$7\r\n"
	         "publish\r\n"
	         "$";
	m_cmd += std::to_string(a_key.length());
	m_cmd += "\r\n";
	m_cmd += a_key;
	m_cmd += "\r\n"
	         "$";
	m_cmd += std::to_string(a_data.length());
	m_cmd += "\r\n";
	m_cmd += a_data;
	return m_cmd;
}

std::string RedisApi::subscribe(
	const std::string& a_key)
{
	m_cmd  = "*2\r\n"
	         "$9\r\n"
	         "subscribe\r\n"
	         "$";
	m_cmd += std::to_string(a_key.length());
	m_cmd += "\r\n";
	m_cmd += a_key;
	return m_cmd;
}

std::string RedisApi::hset(
	const std::string& a_hash,
	const std::string& a_key,
	const std::string& a_data)
{
	m_cmd  = "*4\r\n"
	         "$4\r\n"
	         "hset\r\n"
	         "$";
	m_cmd += std::to_string(a_hash.length());
	m_cmd += "\r\n";
	m_cmd += a_hash;
	m_cmd += "\r\n"
	         "$";
	m_cmd += std::to_string(a_key.length());
	m_cmd += "\r\n";
	m_cmd += a_key;
	m_cmd += "\r\n"
	         "$";
	m_cmd += std::to_string(a_data.length());
	m_cmd += "\r\n";
	m_cmd += a_data;
	return m_cmd;
}

std::string RedisApi::hget(
	const std::string& a_hash,
	const std::string& a_key)
{
	m_cmd  = "*3\r\n"
	         "$4\r\n"
	         "hget\r\n"
	         "$";
	m_cmd += std::to_string(a_hash.length());
	m_cmd += "\r\n";
	m_cmd += a_hash;
	m_cmd += "\r\n"
	         "$";
	m_cmd += std::to_string(a_key.length());
	m_cmd += "\r\n";
	m_cmd += a_key;
	return m_cmd;
}

std::string RedisApi::hgetall(
	const std::string& a_hash)
{
	m_cmd  = "*2\r\n"
	         "$7\r\n"
	         "hgetall\r\n"
	         "$";
	m_cmd += std::to_string(a_hash.length());
	m_cmd += "\r\n";
	m_cmd += a_hash;
	return m_cmd;
}

std::string RedisApi::hkeys(const std::string& a_hash)
{
	m_cmd  = "*2\r\n"
	         "$5\r\n"
	         "hkeys\r\n"
	         "$";
	m_cmd += std::to_string(a_hash.length());
	m_cmd += "\r\n";
	m_cmd += a_hash;
	return m_cmd;
}

std::string RedisApi::hdel(
	const std::string& a_hash,
	const std::string& a_key)
{
	m_cmd  = "*3\r\n"
	         "$4\r\n"
	         "hdel\r\n"
	         "$";
	m_cmd += std::to_string(a_hash.length());
	m_cmd += "\r\n";
	m_cmd += a_hash;
	m_cmd += "\r\n"
	         "$";
	m_cmd += std::to_string(a_key.length());
	m_cmd += "\r\n";
	m_cmd += a_key;
	return m_cmd;
}

std::string RedisApi::zadd(
	const std::string& a_key,
	const std::string& a_score,
	const std::string& a_data)
{
	m_cmd  = "*4\r\n"
	         "$4\r\n"
	         "zadd\r\n"
	         "$";
	m_cmd += std::to_string(a_key.length());
	m_cmd += "\r\n";
	m_cmd += a_key;
	m_cmd += "\r\n"
	         "$";
	m_cmd += std::to_string(a_score.length());
	m_cmd += "\r\n";
	m_cmd += a_score;
	m_cmd += "\r\n"
	         "$";
	m_cmd += std::to_string(a_data.length());
	m_cmd += "\r\n";
	m_cmd += a_data;
	return m_cmd;
}

std::string RedisApi::zcount(
	const std::string& a_key,
	const std::string& a_min,
	const std::string& a_max)
{
	m_cmd  = "*4\r\n"
	         "$6\r\n"
	         "zcount\r\n"
	         "$";
	m_cmd += std::to_string(a_key.length());
	m_cmd += "\r\n";
	m_cmd += a_key;
	m_cmd += "\r\n"
	         "$";
	m_cmd += std::to_string(a_min.length());
	m_cmd += "\r\n";
	m_cmd += a_min;
	m_cmd += "\r\n"
	         "$";
	m_cmd += std::to_string(a_max.length());
	m_cmd += "\r\n";
	m_cmd += a_max;
	return m_cmd;
}

std::string RedisApi::zrange(
	const std::string& a_key,
	const std::string& a_min,
	const std::string& a_max)
{
	m_cmd  = "*5\r\n"
	         "$6\r\n"
	         "zrange\r\n"
	         "$";
	m_cmd += std::to_string(a_key.length());
	m_cmd += "\r\n";
	m_cmd += a_key;
	m_cmd += "\r\n"
	         "$";
	m_cmd += std::to_string(a_min.length());
	m_cmd += "\r\n";
	m_cmd += a_min;
	m_cmd += "\r\n"
	         "$";
	m_cmd += std::to_string(a_max.length());
	m_cmd += "\r\n";
	m_cmd += a_max;
	m_cmd += "\r\n"
	         "$"
	         "10"
	         "\r\n"
	         "withscores";
	return m_cmd;
}

std::string RedisApi::zrangebyscore(
	const std::string& a_key,
	const std::string& a_min,
	const std::string& a_max)
{
	m_cmd  = "*4\r\n"
	         "$13\r\n"
	         "zrangebyscore\r\n"
	         "$";
	m_cmd += std::to_string(a_key.length());
	m_cmd += "\r\n";
	m_cmd += a_key;
	m_cmd += "\r\n"
	         "$";
	m_cmd += std::to_string(a_min.length());
	m_cmd += "\r\n";
	m_cmd += a_min;
	m_cmd += "\r\n"
	         "$";
	m_cmd += std::to_string(a_max.length());
	m_cmd += "\r\n";
	m_cmd += a_max;
	return m_cmd;
}

std::string RedisApi::zremrangebyscore(
	const std::string& a_key,
	const std::string& a_min,
	const std::string& a_max)
{
	m_cmd  = "*4\r\n"
	         "$16\r\n"
	         "zremrangebyscore\r\n"
	         "$";
	m_cmd += std::to_string(a_key.length());
	m_cmd += "\r\n";
	m_cmd += a_key;
	m_cmd += "\r\n"
	         "$";
	m_cmd += std::to_string(a_min.length());
	m_cmd += "\r\n";
	m_cmd += a_min;
	m_cmd += "\r\n"
	         "$";
	m_cmd += std::to_string(a_max.length());
	m_cmd += "\r\n";
	m_cmd += a_max;
	return m_cmd;
}

std::string RedisApi::select(
	const std::string& a_db)
{
	m_cmd  = "*2\r\n"
	         "$6\r\n"
	         "select\r\n"
	         "$";
	m_cmd += std::to_string(a_db.length());
	m_cmd += "\r\n";
	m_cmd += a_db;
	return m_cmd;
}

std::string RedisApi::keys(const std::string& a_pattern)
{
	m_cmd  = "*2\r\n"
	         "$4\r\n"
	         "keys\r\n"
	         "$";
	m_cmd += std::to_string(a_pattern.length());
	m_cmd += "\r\n";
	m_cmd += a_pattern;
	return m_cmd;
}

std::string RedisApi::del(const std::string& a_key)
{
	m_cmd  = "*2\r\n"
	         "$3\r\n"
	         "del\r\n"
	         "$";
	m_cmd += std::to_string(a_key.length());
	m_cmd += "\r\n";
	m_cmd += a_key;
	return m_cmd;
}

