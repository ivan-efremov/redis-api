#include "RedisCommand.h"


/**
 * class RedisCommand.
 */
RedisCommand::RedisCommand():
	m_redisApi(std::make_shared<RedisApi>())
{
}

RedisCommand::~RedisCommand()
{
}

void RedisCommand::setType(const std::string& a_type)
{
	m_type = a_type;
}

std::string RedisCommand::getType() const
{
	return m_type;
}

std::string RedisCommand::getLength(const std::string& a_key)
{
	return m_redisApi->llen(a_key);
}


/**
 * class RedisCommandList.
 */
RedisCommandList::~RedisCommandList()
{
}

std::string RedisCommandList::send(const std::string& a_key, const std::string& a_data)
{
	return m_redisApi->rpush(a_key, a_data);
}

std::string RedisCommandList::recv(const std::string& a_key)
{
	return m_redisApi->blpop(a_key);
}


/**
 * class RedisCommandChannel.
 */
RedisCommandChannel::~RedisCommandChannel()
{
}

std::string RedisCommandChannel::send(const std::string& a_key, const std::string& a_data)
{
	return m_redisApi->publish(a_key, a_data);
}

std::string RedisCommandChannel::recv(const std::string& a_key)
{
	return m_redisApi->subscribe(a_key);
}

/**
 * class RedisCommandHash.
 */
RedisCommandHash::RedisCommandHash(const std::string& a_hash):
	m_hash(a_hash)
{
}

RedisCommandHash::~RedisCommandHash()
{
}

std::string RedisCommandHash::send(const std::string& a_key, const std::string& a_data)
{
	return m_redisApi->hset(m_hash, a_key, a_data);
}

std::string RedisCommandHash::recv(const std::string& a_key)
{
	return m_redisApi->hget(m_hash, a_key);
}

std::string RedisCommandHash::getHash() const
{
	return m_hash;
}
