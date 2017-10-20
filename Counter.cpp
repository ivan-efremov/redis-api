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


std::string Counter::getName() const
{
	return m_name;
}
