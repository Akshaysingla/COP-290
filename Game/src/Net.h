#ifndef NET_H
#define NET_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <assert.h>
#include <fstream>
#include <string.h>
#include <vector>
#include <map>
#include <stack>
#include <list>
#include <algorithm>
#include <functional>
#include <unistd.h>

namespace net
{
	

	class Address
	{
	public:
	
		Address()
		{
			address = 0;
			port = 0;
		}
	
		Address( unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned short port )
		{
			this->address = ( a << 24 ) | ( b << 16 ) | ( c << 8 ) | d;
			this->port = port;
		}
	
		Address( unsigned int address, unsigned short port )
		{
			this->address = address;
			this->port = port;
		}
	
		unsigned int GetAddress() const
		{
			return address;
		}
	
		unsigned short GetPort() const
		{ 
			return port;
		}
	
		bool operator == ( const Address & other ) const
		{
			return address == other.address && port == other.port;
		}
	
		bool operator != ( const Address & other ) const
		{
			return ! ( *this == other );
		}
		
		bool operator < ( const Address & other ) const
		{
			// note: this is so we can use address as a key in std::map
			if ( address < other.address )
				return true;
			if ( address > other.address )
				return false;
			else
				return port < other.port;
		}
	
	private:
	
		unsigned int address;
		unsigned short port;
	};

	// sockets

	class Socket
	{
	public:
	
		Socket()
		{
			socket = 0;
		}
	
		~Socket()
		{
			Close();
		}
	
		bool Open( unsigned short port )
		{
			socket = ::socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );

			if ( socket <= 0 )
			{
				printf( "failed to create socket\n" );
				socket = 0;
				return false;
			}

			// bind to port

			sockaddr_in address;
			address.sin_family = AF_INET;
			address.sin_addr.s_addr = INADDR_ANY;
			address.sin_port = htons( (unsigned short) port );
		
			if ( bind( socket, (const sockaddr*) &address, sizeof(sockaddr_in) ) < 0 )
			{
				printf( "failed to bind socket\n" );
				Close();
				return false;
			}

			// set non-blocking io
		
				int nonBlocking = 1;
				if ( fcntl( socket, F_SETFL, O_NONBLOCK, nonBlocking ) == -1 )
				{
					printf( "failed to set non-blocking socket\n" );
					Close();
					return false;
				}
		
			return true;
		}
	
		void Close()
		{
			if ( socket != 0 )
			{
				close( socket );
				socket = 0;
			}
		}
	
		bool IsOpen() const
		{
			return socket != 0;
		}
	
		bool Send( const Address & destination, const void * data, int size )
		{
            if ( socket == 0 )
				return false;

			sockaddr_in address;
			address.sin_family = AF_INET;
			address.sin_addr.s_addr = htonl( destination.GetAddress() );
			address.sin_port = htons( (unsigned short) destination.GetPort() );

			int sent_bytes = sendto( socket, (const char*)data, size, 0, (sockaddr*)&address, sizeof(sockaddr_in) );

			return sent_bytes == size;
		}
	
		int Receive( Address & sender, void * data, int size )
		{
			if ( socket == 0 )
				return false;
			
			sockaddr_in from;
			socklen_t fromLength = sizeof( from );

			int received_bytes = recvfrom( socket, (char*)data, size, 0, (sockaddr*)&from, &fromLength );

			if ( received_bytes <= 0 )
				return 0;

			unsigned int address = ntohl( from.sin_addr.s_addr );
			unsigned short port = ntohs( from.sin_port );
            

			sender = Address( address, port );

			return received_bytes;
		}
		
	private:
		int socket;
	};
	
	// connection
	
	class Connection
	{
	public:
		
		Connection( unsigned int protocolId, float timeout )
		{
			this->protocolId = protocolId;
			this->timeout = timeout;
			running = false;
			ClearData();
		}
		
		Connection(){
			running = false;
		}
		virtual ~Connection()
		{
			if ( IsRunning() )
				Stop();
		}
		
		bool Start( int port )
		{
			printf( "start connection on port %d\n", port );
			if ( !socket.Open( port ) )
				return false;
			running = true;
			OnStart();
			return true;
		}
		
		void Stop()
		{
			printf( "stop connection\n" );
			bool connected = IsConnected();
			ClearData();
			socket.Close();
			running = false;
			if ( connected )
				OnDisconnect();
			OnStop();
		}

		void dum_Stop()
		{
			printf( "stop connection\n" );
			bool connected = IsConnected();
			ClearData();
			//socket.Close();
			running = false;
			if ( connected )
				OnDisconnect();
			OnStop();
		}
		
        bool IsRunning(){
            return running == true;
        }
		
		bool IsConnected() const
		{
			return state == Connected;
		}

		virtual void Update( float deltaTime )
		{
			timeoutAccumulator += deltaTime;
			if ( timeoutAccumulator > timeout )
			{
					printf( "connect timed out\n" );
					ClearData();
					OnDisconnect();
            }
		}
		
		virtual bool Send_Packet( const unsigned char data[], int size , Address to_address, int protocol)
		{
			if ( to_address.GetAddress() == 0 )
				return false;
			unsigned char packet[size+4];
			packet[0] = (unsigned char) ( protocol >> 24 );
			packet[1] = (unsigned char) ( ( protocol >> 16 ) & 0xFF );
			packet[2] = (unsigned char) ( ( protocol >> 8 ) & 0xFF );
			packet[3] = (unsigned char) ( ( protocol ) & 0xFF );
			memcpy( &packet[4], data, size );
			return socket.Send( to_address, packet, size + 4 );
		}
		
		virtual int Receive_Packet( unsigned char data[], int size , Address & from_address , int protocol  )
		{
			unsigned char packet[size+4];
			int bytes_read = socket.Receive( from_address, packet, size + 4 );
			if ( bytes_read == 0 )
				return 0;
			if ( bytes_read <= 4 )
				return 0;
			if ( packet[0] != (unsigned char) ( protocol >> 24 ) ||
				 packet[1] != (unsigned char) ( ( protocol >> 16 ) & 0xFF ) ||
				 packet[2] != (unsigned char) ( ( protocol >> 8 ) & 0xFF ) ||
				 packet[3] != (unsigned char) ( protocol & 0xFF ) )
				return 0;
			if ( !IsConnected() )
			{
				state = Connected;
				OnConnect();
			}
			else
			{
				timeoutAccumulator = 0.0f;
				memcpy( data, &packet[4], bytes_read - 4 );
				return bytes_read - 4;
			}
			return 0;
		}
        
        virtual bool SendPacket( const unsigned char data[], int size , Address to_address, int protocol)
        {
            if ( to_address.GetAddress() == 0 )
                return false;
            unsigned char packet[size+4];
            packet[0] = (unsigned char) ( protocol >> 24 );
            packet[1] = (unsigned char) ( ( protocol >> 16 ) & 0xFF );
            packet[2] = (unsigned char) ( ( protocol >> 8 ) & 0xFF );
            packet[3] = (unsigned char) ( ( protocol ) & 0xFF );
            memcpy( &packet[4], data, size );
            return socket.Send( to_address, packet, size + 4 );
        }
        
        virtual int ReceivePacket( unsigned char data[], int size , Address & from_address , int protocol  )
        {
            unsigned char packet[size+4];
            int bytes_read = socket.Receive( from_address, packet, size + 4 );
            if ( bytes_read == 0 )
                return 0;
            if ( bytes_read <= 4 )
                return 0;
            if ( packet[0] != (unsigned char) ( protocol >> 24 ) ||
                packet[1] != (unsigned char) ( ( protocol >> 16 ) & 0xFF ) ||
                packet[2] != (unsigned char) ( ( protocol >> 8 ) & 0xFF ) ||
                packet[3] != (unsigned char) ( protocol & 0xFF ) )
                return 0;
            if ( !IsConnected() )
            {
                state = Connected;
                OnConnect();
            }
            else
            {
                memcpy( data, &packet[4], bytes_read - 4 );
                return bytes_read - 4;
            }
            return 0;
        }
		
		int GetHeaderSize() const
		{
			return 4;
		}
		
	protected:
		
		virtual void OnStart()		{}
		virtual void OnStop()		{}
		virtual void OnConnect()    {}
		virtual void OnDisconnect() {}
			
	private:
		
		void ClearData()
		{
			state = Disconnected;
			timeoutAccumulator = 0.0f;
		}
	
		enum State
		{
			Disconnected,
			Connected
		};
        State state;
		unsigned int protocolId;
		float timeout;
        bool running;
		Socket socket;
		float timeoutAccumulator;
	};
    
    
    struct PacketData
    {
        unsigned int sequence;			
        float time;					
        int size;						
    };
    
    inline bool sequence_more_recent( unsigned int s1, unsigned int s2, unsigned int max_sequence )
    {
        return ( s1 > s2 ) && ( s1 - s2 <= max_sequence/2 ) || ( s2 > s1 ) && ( s2 - s1 > max_sequence/2 );
    }
    
    class PacketQueue : public std::list<PacketData>
    {
    public:
        
        bool exists( unsigned int sequence )
        {
            for ( iterator itor = begin(); itor != end(); ++itor )
                if ( itor->sequence == sequence )
                    return true;
            return false;
        }
        
        void insert_sorted( const PacketData & p, unsigned int max_sequence )
        {
            if ( empty() )
            {
                push_back( p );
            }
            else
            {
                if ( !sequence_more_recent( p.sequence, front().sequence, max_sequence ) )
                {
                    push_front( p );
                }
                else if ( sequence_more_recent( p.sequence, back().sequence, max_sequence ) )
                {
                    push_back( p );
                }
                else
                {
                    for ( PacketQueue::iterator itor = begin(); itor != end(); itor++ )
                    {
                        if ( sequence_more_recent( itor->sequence, p.sequence, max_sequence ) )
                        {
                            insert( itor, p );
                            break;
                        }
                    }
                }
            }
        }
        
        void verify_sorted( unsigned int max_sequence )
        {
            PacketQueue::iterator prev = end();
            for ( PacketQueue::iterator itor = begin(); itor != end(); itor++ )
            {
                if ( prev != end() )
                {
                    prev = itor;
                }
            }
        }
    };

	
	class ReliabilitySystem
	{
	public:
		
		ReliabilitySystem( unsigned int max_sequence = 0xFFFFFFFF , unsigned int rtt_max = 1.0f )
		{
			this->rtt_maximum = rtt_max;
			this->max_sequence = max_sequence;
			Reset();
		}
		
		void Reset()
		{
			local_sequence = 0;
			remote_sequence = 0;
			sentQueue.clear();
			receivedQueue.clear();
			pendingAckQueue.clear();
			ackedQueue.clear();
			sent_packets = 0;
			recv_packets = 0;
			lost_packets = 0;
			acked_packets = 0;
			sent_bandwidth = 0.0f;
			acked_bandwidth = 0.0f;
			rtt = 0.0f;
			rtt_maximum = 1.0f;
		}
		
		void PacketSent( int size )
		{
			if ( sentQueue.exists( local_sequence ) )
			{
				printf( "local sequence %d exists\n", local_sequence );				
				for ( PacketQueue::iterator itor = sentQueue.begin(); itor != sentQueue.end(); ++itor )
					printf( " + %d\n", itor->sequence );
			}
			PacketData data;
			data.sequence = local_sequence;
			data.time = 0.0f;
			data.size = size;
			sentQueue.push_back( data );
			pendingAckQueue.push_back( data );
			sent_packets++;
			local_sequence++;
			if ( local_sequence > max_sequence )
				local_sequence = 0;
		}
		
		void PacketReceived( unsigned int sequence, int size )
		{
			recv_packets++;
			if ( receivedQueue.exists( sequence ) )
				return;
			PacketData data;
			data.sequence = sequence;
			data.time = 0.0f;
			data.size = size;
			receivedQueue.push_back( data );
			if ( sequence_more_recent( sequence, remote_sequence, max_sequence ) )
				remote_sequence = sequence;
		}

		unsigned int GenerateAckBits()
		{
			return generate_ack_bits( GetRemoteSequence(), receivedQueue, max_sequence );
		}
		
		void ProcessAck( unsigned int ack, unsigned int ack_bits )
		{
			process_ack( ack, ack_bits, pendingAckQueue, ackedQueue, acks, acked_packets, rtt, max_sequence );
		}
				
		void Update( float deltaTime )
		{
			acks.clear();
			AdvanceQueueTime( deltaTime );
			UpdateQueues();
			UpdateStats();
		}
		
		void Validate()
		{
			sentQueue.verify_sorted( max_sequence );
			receivedQueue.verify_sorted( max_sequence );
			pendingAckQueue.verify_sorted( max_sequence );
			ackedQueue.verify_sorted( max_sequence );
		}

		// utility functions

		static bool sequence_more_recent( unsigned int s1, unsigned int s2, unsigned int max_sequence )
		{
			return ( s1 > s2 ) && ( s1 - s2 <= max_sequence/2 ) || ( s2 > s1 ) && ( s2 - s1 > max_sequence/2 );
		}
		
		static int bit_index_for_sequence( unsigned int sequence, unsigned int ack, unsigned int max_sequence )
		{
			if ( sequence > ack )
			{
 				return ack + ( max_sequence - sequence );
			}
			else
			{
 				return ack - 1 - sequence;
			}
		}
		
		static unsigned int generate_ack_bits( unsigned int ack, const PacketQueue & received_queue, unsigned int max_sequence )
		{
			unsigned int ack_bits = 0;
			for ( PacketQueue::const_iterator itor = received_queue.begin(); itor != received_queue.end(); itor++ )
			{
				if ( itor->sequence == ack || sequence_more_recent( itor->sequence, ack, max_sequence ) )
					break;
				int bit_index = bit_index_for_sequence( itor->sequence, ack, max_sequence );
				if ( bit_index <= 31 )
					ack_bits |= 1 << bit_index;
			}
			return ack_bits;
		}
		
		static void process_ack( unsigned int ack, unsigned int ack_bits, 
								 PacketQueue & pending_ack_queue, PacketQueue & acked_queue, 
								 std::vector<unsigned int> & acks, unsigned int & acked_packets, 
								 float & rtt, unsigned int max_sequence )
		{
			if ( pending_ack_queue.empty() )
				return;
				
			PacketQueue::iterator itor = pending_ack_queue.begin();
			while ( itor != pending_ack_queue.end() )
			{
				bool acked = false;
				
				if ( itor->sequence == ack )
				{
					acked = true;
				}
				else if ( !sequence_more_recent( itor->sequence, ack, max_sequence ) )
				{
					int bit_index = bit_index_for_sequence( itor->sequence, ack, max_sequence );
					if ( bit_index <= 31 )
						acked = ( ack_bits >> bit_index ) & 1;
				}
				
				if ( acked )
				{
					rtt += ( itor->time - rtt ) * 0.1f;

					acked_queue.insert_sorted( *itor, max_sequence );
					acks.push_back( itor->sequence );
					acked_packets++;
					itor = pending_ack_queue.erase( itor );
				}
				else
					++itor;
			}
		}
		
				
		unsigned int GetLocalSequence() const
		{
			return local_sequence;
		}

		unsigned int GetRemoteSequence() const
		{
			return remote_sequence;
		}
		
		unsigned int GetMaxSequence() const
		{
			return max_sequence;
		}
				
 		void GetAcks( unsigned int ** acks, int & count )
		{
			*acks = &this->acks[0];
			count = (int) this->acks.size();
		}
		
		unsigned int GetSentPackets() const
		{
			return sent_packets;
		}
		
		unsigned int GetReceivedPackets() const
		{
			return recv_packets;
		}

		unsigned int GetLostPackets() const
		{
			return lost_packets;
		}

		unsigned int GetAckedPackets() const
		{
			return acked_packets;
		}

		float GetSentBandwidth() const
		{
			return sent_bandwidth;
		}

		float GetAckedBandwidth() const
		{
			return acked_bandwidth;
		}

		float GetRoundTripTime() const
		{
			return rtt;
		}
		
		int GetHeaderSize() const
		{
			return 12;
		}

	protected:
		
		void AdvanceQueueTime( float deltaTime )
		{
			for ( PacketQueue::iterator itor = sentQueue.begin(); itor != sentQueue.end(); itor++ )
				itor->time += deltaTime;

			for ( PacketQueue::iterator itor = receivedQueue.begin(); itor != receivedQueue.end(); itor++ )
				itor->time += deltaTime;

			for ( PacketQueue::iterator itor = pendingAckQueue.begin(); itor != pendingAckQueue.end(); itor++ )
				itor->time += deltaTime;

			for ( PacketQueue::iterator itor = ackedQueue.begin(); itor != ackedQueue.end(); itor++ )
				itor->time += deltaTime;
		}
		
		void UpdateQueues()
		{
			const float epsilon = 0.001f;

			while ( sentQueue.size() && sentQueue.front().time > rtt_maximum + epsilon )
				sentQueue.pop_front();

			if ( receivedQueue.size() )
			{
				const unsigned int latest_sequence = receivedQueue.back().sequence;
				const unsigned int minimum_sequence = latest_sequence >= 34 ? ( latest_sequence - 34 ) : max_sequence - ( 34 - latest_sequence );
				while ( receivedQueue.size() && !sequence_more_recent( receivedQueue.front().sequence, minimum_sequence, max_sequence ) )
					receivedQueue.pop_front();
			}

			while ( ackedQueue.size() && ackedQueue.front().time > rtt_maximum * 2 - epsilon )
				ackedQueue.pop_front();

			while ( pendingAckQueue.size() && pendingAckQueue.front().time > rtt_maximum + epsilon )
			{
				pendingAckQueue.pop_front();
				lost_packets++;
			}
		}
		
		void UpdateStats()
		{
			int sent_bytes_per_second = 0;
			for ( PacketQueue::iterator itor = sentQueue.begin(); itor != sentQueue.end(); ++itor )
				sent_bytes_per_second += itor->size;
			int acked_packets_per_second = 0;
			int acked_bytes_per_second = 0;
			for ( PacketQueue::iterator itor = ackedQueue.begin(); itor != ackedQueue.end(); ++itor )
			{
				if ( itor->time >= rtt_maximum )
				{
					acked_packets_per_second++;
					acked_bytes_per_second += itor->size;
				}
			}
			sent_bytes_per_second /= rtt_maximum;
			acked_bytes_per_second /= rtt_maximum;
			sent_bandwidth = sent_bytes_per_second * ( 8 / 1000.0f );
			acked_bandwidth = acked_bytes_per_second * ( 8 / 1000.0f );
		}
		
	private:
		
		unsigned int max_sequence;			
		unsigned int local_sequence;		
		unsigned int remote_sequence;		
		
		unsigned int sent_packets;			
		unsigned int recv_packets;			
		unsigned int lost_packets;			
		unsigned int acked_packets;			

		float sent_bandwidth;				
		float acked_bandwidth;				
		float rtt;							
		float rtt_maximum;					

		std::vector<unsigned int> acks;		

		PacketQueue sentQueue;				
		PacketQueue pendingAckQueue;		
		PacketQueue receivedQueue;			
		PacketQueue ackedQueue;				
	};

class ReliableConnection : public Connection
	{
	public:
		
		ReliableConnection( unsigned int protocolId, float timeout,unsigned int max_sequence = 0xFFFFFFFF)
			: Connection( protocolId, timeout )
		{
            for (int i = 0 ; i <4 ;i++){
                reliabilitySystem[i].Reset();
                timeoutAccumulator[i] = 0.0f;
                connect[i] = true;
            }
            this->timeout = timeout;
		}
	
		~ReliableConnection()
		{
			if ( IsRunning() )
				Stop();
		}
		
		// overriden functions from "Connection"
				
        bool SendPacket( const unsigned char data[], int size , Address & add , int protocol, int i )
		{
			const int header = 12;
			unsigned char packet[header+size];
			unsigned int seq = reliabilitySystem[i].GetLocalSequence();
			unsigned int ack = reliabilitySystem[i].GetRemoteSequence();
			unsigned int ack_bits = reliabilitySystem[i].GenerateAckBits();
			WriteHeader( packet, seq, ack, ack_bits );
			memcpy( packet + header, data, size );
 			if ( !Connection::SendPacket( packet, size + header , add , protocol ) )
				return false;
			reliabilitySystem[i].PacketSent( size );
			return true;
		}
        
        int getstate(int i){
            return connect[i];
        }
		
		int ReceivePacket( unsigned char data[], int size , Address & add , int protocol ,int & playerno )
		{
			const int header = 12;
			if ( size <= header )
				return false;
			unsigned char packet[header+size];
			int received_bytes = Connection::ReceivePacket( packet, size + header , add ,protocol );
			if ( received_bytes == 0 )
				return false;
			if ( received_bytes <= header )
				return false;
            int i;
            for (i = 0 ; i < 4 ; i++){
                if (addr[i] == add){
                    break;
                }
            }
            playerno = i;
            timeoutAccumulator[i] = 0.0f;
			unsigned int packet_sequence = 0;
			unsigned int packet_ack = 0;
			unsigned int packet_ack_bits = 0;
			ReadHeader( packet, packet_sequence, packet_ack, packet_ack_bits );
			reliabilitySystem[i].PacketReceived( packet_sequence, received_bytes - header );
			reliabilitySystem[i].ProcessAck( packet_ack, packet_ack_bits );
            memcpy( data, packet + header, received_bytes - header );
            return received_bytes - header;
		}
        
        void Update( float deltaTime , int i )
        {
            timeoutAccumulator[i] += deltaTime;
            reliabilitySystem[i].Update( deltaTime );
            if ( timeoutAccumulator[i] > timeout )
            {
                connect[i] = false;
                ClearData(i);
            }
        }
        
        ReliabilitySystem GetReliabilitySystem(int i){
            return reliabilitySystem[i];
        }
		
		int GetHeaderSize(int i) const
		{
			return Connection::GetHeaderSize() + reliabilitySystem[i].GetHeaderSize();
		}
        
        void receive_addresses(Address  addr0,Address addr1, Address addr2 , Address addr3){
            addr[0] = addr0;
            addr[1] = addr1;
            addr[2] = addr2;
            addr[3] = addr3;
        }

		// unit test controls
		
	protected:		
		
		void WriteInteger( unsigned char * data, unsigned int value )
		{
			data[0] = (unsigned char) ( value >> 24 );
			data[1] = (unsigned char) ( ( value >> 16 ) & 0xFF );
			data[2] = (unsigned char) ( ( value >> 8 ) & 0xFF );
			data[3] = (unsigned char) ( value & 0xFF );
		}

		void WriteHeader( unsigned char * header, unsigned int sequence, unsigned int ack, unsigned int ack_bits )
		{
			WriteInteger( header, sequence );
			WriteInteger( header + 4, ack );
			WriteInteger( header + 8, ack_bits );
		}
		
		void ReadInteger( const unsigned char * data, unsigned int & value )
		{
 			value = ( ( (unsigned int)data[0] << 24 ) | ( (unsigned int)data[1] << 16 ) | 
				      ( (unsigned int)data[2] << 8 )  | ( (unsigned int)data[3] ) );				
		}
		
		void ReadHeader( const unsigned char * header, unsigned int & sequence, unsigned int & ack, unsigned int & ack_bits )
		{
			ReadInteger( header, sequence );
			ReadInteger( header + 4, ack );
			ReadInteger( header + 8, ack_bits );
		}

		virtual void OnStop(int i)
		{
			ClearData(i);
		}
		
		virtual void OnDisconnect(int i)
		{
			ClearData(i);
		}
		
	private:
        
		void ClearData(int i)
		{
			reliabilitySystem[i].Reset();
		}
        ReliabilitySystem reliabilitySystem[4];
        float timeout;
        float timeoutAccumulator[4];
        bool connect[4];
        Address addr[4];
	};

class FlowControl
{
public:
    
    FlowControl()
    {
        printf( "flow control initialized\n" );
	send_rate = 30.0f;
        Reset();
    }
    void Reset()
    {
        mode = Fast;
        penalty_time = 2.0f;
        good_conditions_time = 0.0f;
        penalty_reduction_accumulator = 0.0f;
    }
    
    void Update( float deltaTime, float rtt )
    {
        const float RTT_Threshold = 250.0f;
        
        if ( mode == Fast )
        {
            if ( rtt > RTT_Threshold )
            {
                printf( "*** dropping to bad mode ***\n" );
                mode = Slow;
                if ( good_conditions_time < 10.0f && penalty_time < 60.0f )
                {
                    penalty_time *= 2.0f;
                    if ( penalty_time > 60.0f )
                        penalty_time = 60.0f;
                    printf( "penalty time increased to %.1f\n", penalty_time );
                }
                good_conditions_time = 0.0f;
                penalty_reduction_accumulator = 0.0f;
                return;
            }
            
            good_conditions_time += deltaTime;
            penalty_reduction_accumulator += deltaTime;
            
            if ( penalty_reduction_accumulator > 10.0f && penalty_time > 1.0f )
            {
                penalty_time /= 2.0f;
                if ( penalty_time < 1.0f )
                    penalty_time = 1.0f;
                printf( "penalty time reduced to %.1f\n", penalty_time );
                penalty_reduction_accumulator = 0.0f;
            }
        }
        
        if ( mode == Slow )
        {
            if ( rtt <= RTT_Threshold )
                good_conditions_time += deltaTime;
            else
                good_conditions_time = 0.0f;
            
            if ( good_conditions_time > penalty_time )
            {
                printf( "*** upgrading to good mode ***\n" );
                good_conditions_time = 0.0f;
                penalty_reduction_accumulator = 0.0f;
                mode = Fast;
                return;
            }
        }
    }
    
    float GetSendRate()
    {
        return mode == Fast ? 30.0f : 10.0f;
    }
    
    float updateSendRate(){
    	return send_rate;
    }
private:
    
    enum Mode
    {
        Fast,
        Slow
    };
    
    Mode mode;
    float penalty_time;
    float good_conditions_time;
    float penalty_reduction_accumulator;
    float send_rate;
};
};

#endif
