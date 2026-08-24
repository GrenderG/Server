#include "rdp_connection.h"

RDPMessage::RDPMessage()
	: m_message(nullptr)
{
}

RDPMessage::~RDPMessage()
{
	Reset();
}

const uint8 *RDPMessage::Data() const
{
	return m_message != nullptr ? static_cast<const uint8 *>(rdplib_message_data(m_message)) : nullptr;
}

uint32 RDPMessage::Size() const
{
	return m_message != nullptr ? rdplib_message_size(m_message) : 0;
}

uint8 RDPMessage::StreamNumber() const
{
	return m_message != nullptr ? rdplib_message_stream(m_message) : 0;
}

uint16 RDPMessage::Flags() const
{
	return m_message != nullptr ? rdplib_message_flags(m_message) : 0;
}

void RDPMessage::Reset(rdplib_message_t *message)
{
	if (m_message != nullptr)
		rdplib_message_release(m_message);

	m_message = message;
}

RDPConnection::RDPConnection(rdplib_connection_t *connection)
	: m_connection(connection),
	  m_terminal_result(NoData),
	  m_disconnect_reason(0)
{
}

RDPConnection::~RDPConnection()
{
	Close(0);
}

int RDPConnection::EnableKeepalive()
{
	return m_connection != nullptr ? rdplib_connection_enable_keepalive(m_connection) : RDPLIB_ERROR_NOT_USABLE;
}

int RDPConnection::EnableKeepalive(uint32 interval_ms)
{
	return m_connection != nullptr ? rdplib_connection_enable_keepalive_with_interval(m_connection, interval_ms) : RDPLIB_ERROR_NOT_USABLE;
}

int RDPConnection::SetDataRate(uint32 bytes_per_second)
{
	return m_connection != nullptr ? rdplib_connection_set_data_rate(m_connection, bytes_per_second) : RDPLIB_ERROR_NOT_USABLE;
}

int RDPConnection::SetSendBufferSize(uint32 bytes)
{
	return m_connection != nullptr ? rdplib_connection_set_send_buffer_size(m_connection, bytes) : RDPLIB_ERROR_NOT_USABLE;
}

int RDPConnection::Send(const void *data, uint32 bytes, uint32 stream, uint32 flags)
{
	if (m_connection == nullptr || m_terminal_result != NoData)
		return RDPLIB_ERROR_NOT_USABLE;

	int result = rdplib_connection_send(m_connection, data, bytes, stream, flags);
	bool reliable = (flags & RDPLIB_SEND_RELIABLE) != 0;
	bool connection_lost = result == RDPLIB_CONNECTION_SEND_HISTORY_FULL ||
		(result == RDPLIB_CONNECTION_SEND_BUFFER_FULL && reliable) ||
		result == RDPLIB_CONNECTION_SEND_NOT_CONNECTED ||
		result == RDPLIB_CONNECTION_SEND_FIN_SENT ||
		result == RDPLIB_CONNECTION_SEND_PEER_STOPPED;

	if (connection_lost)
	{
		m_terminal_result = ConnectionLost;
		m_disconnect_reason = RDPLIB_DISCONNECT_REASON_SEND_ERROR;
	}

	return result;
}

RDPConnection::ReceiveResult RDPConnection::Receive(RDPMessage *message, uint32 *disconnect_reason)
{
	if (message == nullptr)
		return NoData;

	message->Reset();
	if (disconnect_reason != nullptr)
		*disconnect_reason = 0;

	if (m_terminal_result != NoData)
	{
		if (disconnect_reason != nullptr && m_terminal_result == ConnectionLost)
			*disconnect_reason = m_disconnect_reason;
		return m_terminal_result;
	}

	if (m_connection == nullptr)
		return NoData;

	rdplib_message_t *received_message = rdplib_connection_pop_message(m_connection);
	if (received_message == nullptr)
		return NoData;

	if (rdplib_message_is_disconnect(received_message))
	{
		rdplib_disconnect_info_t information{};
		if (rdplib_connection_get_disconnect_info(m_connection, &information) == RDPLIB_OK)
			m_disconnect_reason = information.reason;

		rdplib_message_release(received_message);
		m_terminal_result = ConnectionLost;
		if (disconnect_reason != nullptr)
			*disconnect_reason = m_disconnect_reason;
		return m_terminal_result;
	}

	bool has_fin = rdplib_message_has_fin(received_message) != 0;
	uint32 bytes = rdplib_message_size(received_message);
	if (has_fin)
		m_terminal_result = PeerClosed;

	if (has_fin && bytes == 0)
	{
		rdplib_message_release(received_message);
		return PeerClosed;
	}

	message->Reset(received_message);
	return MessageReceived;
}

void RDPConnection::DiscardMessages()
{
	if (m_connection == nullptr)
		return;

	rdplib_message_t *message;
	while ((message = rdplib_connection_pop_message(m_connection)) != nullptr)
		rdplib_message_release(message);
}

void RDPConnection::Close(uint32 linger_timeout_ms)
{
	if (m_connection == nullptr)
		return;

	DiscardMessages();

	rdplib_connection_t *connection = m_connection;
	m_connection = nullptr;

	(void)rdplib_connection_begin_close(connection, linger_timeout_ms);
	rdplib_connection_release(connection);
}

int RDPConnection::GetRemoteAddress(uint8 address[4], uint16 &port) const
{
	return m_connection != nullptr ? rdplib_connection_get_remote_ipv4(m_connection, address, &port) : RDPLIB_ERROR_NOT_USABLE;
}

int RDPConnection::GetStatistics(rdplib_connection_perf_stats_t &statistics) const
{
	return m_connection != nullptr ? rdplib_connection_get_perf_stats(m_connection, &statistics) : RDPLIB_ERROR_NOT_USABLE;
}

int RDPConnection::SetPacketDropCallback(rdplib_packet_drop_callback_t callback, void *context)
{
	return m_connection != nullptr ? rdplib_connection_set_packet_drop_callback(m_connection, callback, context) : RDPLIB_ERROR_NOT_USABLE;
}
