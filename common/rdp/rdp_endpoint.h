#ifndef RDP_ENDPOINT_H
#define RDP_ENDPOINT_H

#include "rdp_runtime.h"

class RDPConnection;

// owns one rdplib endpoint.  the application owns its connections
class RDPEndpoint
{
public:
	RDPEndpoint();
	~RDPEndpoint();

	RDPEndpoint(const RDPEndpoint &) = delete;
	RDPEndpoint &operator=(const RDPEndpoint &) = delete;

	// expected_connections should be 1 for a client with a single peer.  any other value selects the 2048 bucket connection table; this is not a connection limit
	// a nonzero local_port requests 4 MiB send and receive socket buffers.  zero retains operating system defaults for client endpoints
	int Open(RDPRuntime &runtime, uint16 local_port, uint32 expected_connections = 100, uint32 flags = RDPLIB_USE_CRC);
	int Close();
	bool IsOpen() const
	{
		return m_endpoint != nullptr;
	}

	int Process(int32 timeout_ms = 0);
	// the caller owns the returned connection.  result may be null.  Accept returns null with RDPLIB_OK when no connection is pending
	RDPConnection *Accept(int *result = nullptr);
	RDPConnection *Connect(const char *host, uint16 port, int *result = nullptr);
	uint16 LocalPort() const;

private:
	void DiscardConnectionless();

	rdplib_endpoint_t *m_endpoint;
};

#endif
