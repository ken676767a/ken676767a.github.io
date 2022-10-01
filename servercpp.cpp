#include <iostream>
#include <enet/enet.h>

#include <string>
#include <cstring>
#include <map>

class ClientData
{
private:
	int m_id;
	std::string m_username;

public:
	ClientData(int id) : m_id(id) {}

	void SetUsername(std::string username) { m_username = username; }

	int GetID() { return m_id; }
	std::string GetUsername() { return m_username; }
};

std::map<int, ClientData*> client_map;

void BroadcastPacket(ENetHost* server, const char* data)
{
	ENetPacket* packet = enet_packet_create(data, strlen(data) + 1, ENET_PACKET_FLAG_RELIABLE);
	enet_host_broadcast(server, 0, packet);
}

void SendPacket(ENetPeer* peer, const char* data)
{
	ENetPacket* packet = enet_packet_create(data, strlen(data) + 1, ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(peer, 0, packet);
}



int main(int argc, char ** argv)
{
	if (enet_initialize() != 0)
	{
		fprintf(stderr, "An error occurred while initializing ENet.\n");
		return EXIT_FAILURE;
	}
	atexit(enet_deinitialize);

	ENetAddress address;
	ENetHost* server;
	ENetEvent event;

	address.host = ENET_HOST_ANY;
	address.port = 7777;

	server = enet_host_create(&address, 32, 2, 0, 0);

	if (server == NULL)
	{
		fprintf(stderr, "An error occurred while trying to create an ENet server host!\n");
		return EXIT_FAILURE;
	}
	std::string p1a1=" ";
	std::string p1a2 = " ";
	std::string p2a1 = " ";
	std::string p2a2 = " ";
	// GAME LOOP START
	int new_player_id = 0; // An auto incrementing variable to give to any new clients
	while (true)
	{

		while (enet_host_service(server, &event, 10) > 0)
		{
			switch (event.type)
			{
			case ENET_EVENT_TYPE_CONNECT:
			{
				printf("A new client connected from %x:%u.\n",
					event.peer->address.host,
					event.peer->address.port);
				new_player_id++;
			
				if (new_player_id == 1)
				{
					BroadcastPacket(server, "p1");
				}
				if (new_player_id == 2)
				{
					BroadcastPacket(server, "p2");
				}
				if (new_player_id >= 3)
				{
					printf("too much player");
					BroadcastPacket(server, "too much player");
					exit(1);
				}

				break;
			}
			case ENET_EVENT_TYPE_RECEIVE:
			{
				printf("A packet of length %u containing %s was received from %x:%u on channel %u.\n",
					event.packet->dataLength,
					event.packet->data,
					event.peer->address.host,
					event.peer->address.port,
					event.channelID);
				std::string s=static_cast<std::string>(reinterpret_cast<char *>(event.packet->data));
			

				BroadcastPacket(server, static_cast< char*>(static_cast<void *>(event.packet->data)));
				
				enet_packet_destroy(event.packet);
				break;
			}
			case ENET_EVENT_TYPE_DISCONNECT:
			{
				printf("%x:%u disconnected.\n",
					event.peer->address.host,
					event.peer->address.port);

				char disconnected_data[126] = { '\0' };
			
				BroadcastPacket(server, disconnected_data);

				event.peer->data = NULL;
				break;
			}
			}
		}

	}
	// GAME LOOP END

	enet_host_destroy(server);

	return EXIT_SUCCESS;
}