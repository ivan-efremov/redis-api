/**
 * @file RedisIO.h
 */

#pragma once

#include <list>
#include <string>
#include <memory>
#include <iostream>
#include <boost/asio.hpp>
#include "RedisCommand.h"


namespace ba  = boost::asio;
namespace bs  = boost::system;


/**
 * class RedisIO.
 * @short Класс ввода вывода для redis's.
 */
class RedisIO
{
public:
	typedef ba::io_service                   IOService;
	typedef ba::ip::tcp::resolver            Resolver;
	typedef ba::ip::tcp::resolver::iterator  ResolverIterator;
	typedef ba::ip::tcp::socket              Socket;
	typedef ba::streambuf                    Streambuf;
	typedef std::list<std::string>           ListValue;
	typedef ListValue::iterator              ItListValue;
	typedef std::shared_ptr<ListValue>       PListValue;
public:
	                     RedisIO(const std::string& a_server,
	                             const std::string& a_port);
	virtual             ~RedisIO();
	void                 connect();
	void                 close();
	PListValue           api(const std::string& a_str);
	bs::error_code       error() const;
protected:
	void                 setSocketOptions();
	void                 sendRequest(const std::string& a_str);
	std::string          getResponse();
protected:
	bool                 m_resolveHost;
	const std::string    m_server;
	const std::string    m_port;
	IOService            m_ioService;
	Resolver             m_resolverName;
	Socket               m_socket;
	Streambuf            m_response;
	std::istream         m_istream;
	std::string          m_line;
	bs::error_code       m_error;
};

typedef std::shared_ptr<RedisIO> PRedisIO;

