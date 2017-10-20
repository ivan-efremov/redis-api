/**
 * @file RedisCommand.h
 */

#pragma once

#include <memory>
#include <string>
#include "RedisApi.h"


/**
 * class RedisCommand.
 * @short Базовый класс комманд для redis.
 */
class RedisCommand
{
public:
	                    RedisCommand();
	virtual            ~RedisCommand();
	virtual std::string send(const std::string& a_key, const std::string& a_data) = 0;
	virtual std::string recv(const std::string& a_key) = 0;
	std::string         getLength(const std::string& a_key);
	void                setType(const std::string& a_type);
	std::string         getType() const;
protected:
	PRedisApi           m_redisApi;
	std::string         m_type;
};

typedef std::shared_ptr<RedisCommand> PRedisCommand;


/**
 * class RedisCommandList.
 * @short Команды redis для типа данный "list".
 */
class RedisCommandList:
	public RedisCommand
{
public:
	virtual            ~RedisCommandList();
	virtual std::string send(const std::string& a_key, const std::string& a_data);
	virtual std::string recv(const std::string& a_key);
};

typedef std::shared_ptr<RedisCommandList> PRedisCommandList;


/**
 * class RedisCommandChannel.
 * @short Команды redis для типа данный "channel".
 */
class RedisCommandChannel:
	public RedisCommand
{
public:
	virtual            ~RedisCommandChannel();
	virtual std::string send(const std::string& a_key, const std::string& a_data);
	virtual std::string recv(const std::string& a_key);
};


/**
 * class RedisCommandHash.
 * @short Команды redis для типа данный "hash".
 */
class RedisCommandHash:
	public RedisCommand
{
public:
	                    RedisCommandHash(const std::string& a_hash);
	virtual            ~RedisCommandHash();
	virtual std::string send(const std::string& a_key, const std::string& a_data);
	virtual std::string recv(const std::string& a_key);
	std::string         getHash() const;
private:
	std::string         m_hash;
};

typedef std::shared_ptr<RedisCommandHash> PRedisCommandHash;

