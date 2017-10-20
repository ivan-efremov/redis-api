#include <utility>
#include <exception>
#include "RedisIO.h"
#include <boost/algorithm/string.hpp>


/**
 * class RedisIO.
 */
RedisIO::RedisIO(
	const std::string& a_server,
	const std::string& a_port)
:
	m_resolveHost(false),
	m_server(a_server),
	m_port(a_port),
	m_resolverName(m_ioService),
	m_socket(m_ioService),
	m_istream(&m_response)
{
	try {
		m_line.reserve(4096);
		// check resolve host
		for(char ch : a_server) {
			if(std::isalpha(ch)) {
				m_resolveHost = true;
				break;
			}
		}
		connect();
	} catch(const std::exception& err) {
	}
}

RedisIO::~RedisIO()
{
}

void RedisIO::setSocketOptions()
{
	m_socket.set_option(ba::ip::tcp::no_delay(true));
	m_socket.set_option(ba::socket_base::receive_buffer_size(32768));
	m_socket.set_option(ba::socket_base::send_buffer_size(65536));
}

void RedisIO::connect()
{
	m_error = ba::error::host_not_found;
	if(m_resolveHost) {
		ResolverIterator itend;
		ResolverIterator it(
			m_resolverName.resolve(
				ba::ip::tcp::resolver::query(
					ba::ip::tcp::v4(), m_server, m_port
				)
			)
		);
		while(m_error && it != itend) {
			if(m_socket.is_open()) m_socket.close();
			m_socket.open(ba::ip::tcp::v4());
			setSocketOptions();
			m_socket.connect(*it++, m_error);
		}
	} else {
		if(m_socket.is_open()) m_socket.close();
		m_socket.open(ba::ip::tcp::v4());
		setSocketOptions();
		m_socket.connect(
			ba::ip::tcp::endpoint(
				ba::ip::address::from_string(m_server), atoi(m_port.c_str())
			),
			m_error
		);
	}
	if(m_error) throw bs::system_error(m_error);
}

void RedisIO::close()
{
	boost::system::error_code ignored;
	m_socket.shutdown(
		ba::ip::tcp::socket::shutdown_both,
		ignored
	);
	m_socket.close(ignored);
	m_response.consume(m_response.size());
	m_istream.clear();
	m_error.clear();
}

void RedisIO::sendRequest(const std::string& a_str)
{
	ba::write(m_socket, ba::buffer(a_str + "\r\n"), m_error);
	if(m_error) {
		throw std::runtime_error(
				std::string("Can't write data to redis server: ") + m_error.message()
			);
	}
}

std::string RedisIO::getResponse()
{
	do {
		m_line.clear();
		ba::read_until(m_socket, m_response, "\r\n", m_error);
		if(m_error) {
			throw std::runtime_error(
					std::string("Can't read data from redis server: ") + m_error.message()
				);
		}
		std::getline(m_istream, m_line); // read line data
		boost::algorithm::trim(m_line);  // remove \r\n\t
	} while(m_line.empty());
	return m_line;
}

RedisIO::PListValue RedisIO::api(
	const std::string& a_str)
{
	long long   n = 0LL;
	bool        protocol = true;
	PListValue  listValue(std::make_shared<ListValue>());
	if(!a_str.empty()) {
		// if a str is empty, we wait data from server
		sendRequest(a_str);
	}
	do {
		std::string data(getResponse());
		if(protocol) {
			switch(data[0]) {
				case '$': // Bulk Strings
					if(data[1] == '-') { // nil
						listValue->push_back("nil");
					} else {
						protocol = false;
						++n;
					}
					break;
				case ':': // Integers
				case '+': // Simple Strings
					listValue->push_back(data.substr(1));
					break;
				case '*': // Arrays
					n = std::stoll(data.substr(1));
					if(n < 0LL) {
						throw std::runtime_error("Redis error: bad array size " + data);
					}
					break;
				case '-': // error
					throw std::runtime_error("Redis error: " + data);
				case '\0': // empty data
					throw std::runtime_error("Redis error: bad protocol, reply is empty");
			}
		} else {
			protocol = true;
			listValue->push_back(data);
		}
	} while(n--);
	return listValue;
}

bs::error_code RedisIO::error() const
{
	return m_error;
}

