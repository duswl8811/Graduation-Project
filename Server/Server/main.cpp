#include "server.h"

HANDLE g_iocp;

mutex buf_lock;

std::chrono::high_resolution_clock::time_point point;
std::map <int, Monster> monsters;
std::map <int, Item> items;

int main()
{
	vector<thread> worker_threads;

	initialize();
	make_items();

	g_iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);

	for (int i = 0; i < 4; ++i)
		worker_threads.emplace_back(thread{ worker_thread });

	thread accept_thread{ do_accept };

	point = std::chrono::high_resolution_clock::now();
	thread make_thread{ make_obj };

	accept_thread.join();
	for (auto &th : worker_threads)
		th.join();

	CloseHandle(g_iocp);
}

void error_display(const char *msg, int err_no)
{
	WCHAR *lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, err_no, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
	cout << msg;
	wcout << L"����" << lpMsgBuf << endl;
	LocalFree(lpMsgBuf);
}
void initialize()
{
	for (auto &cl : clients) {
		cl.sock.connected = false;
	}
}
void make_items()
{
	// ��ǥ����
	srand(time(NULL));
	items[0].SetDraw(true);
	items[0].SetId(0);
	items[0].SetType(1);
	items[0].SetPos(351, 150, 351);
	for (int i = 1; i < MAX_ITEM; ++i)
	{
		float x = float(rand() % 1800 - 900);
		if (-350 <= x && x <= 350)
		{
			--i;
			continue;
		}
		float z = float(rand() % 1800 - 900);
		if (-350 <= z && z <= 350)
		{
			--i;
			continue;
		}
		int type = rand() % 2 + 1;
		items[i].SetDraw( true);
		items[i].SetId(i);
		items[i].SetType(type);
		items[i].SetPos(x, 150, z);
	}
}
int get_new_id()
{
	for (int i = 1; i < MAX_USER; ++i)
	{
		clients[i].SetLock();
		if (clients[i].sock.connected == false)
		{
			clients[i].sock.connected = true;
			clients[i].SetUnlock();
			return i;
		}
		clients[i].SetUnlock();
	}
	return -1;
}

void disconnect(int id)
{
	clients[id].SetLock();
	closesocket(clients[id].sock.socket);
	clients[id].sock.connected = false;
	clients[id].SetUnlock();
	for (int i = 1; i <= MAX_USER; ++i) {
		if (false == clients[i].sock.connected) continue;
		send_remove_player_packet(i,id);
	}
	cout << "�������� ID:" << id << endl;
}
void worker_thread()
{
	while (true)
	{
		DWORD io_byte;
		ULONG		key;
		OVER_EX *lpover_ex;

		bool is_error = GetQueuedCompletionStatus(g_iocp, &io_byte, &key, reinterpret_cast<LPWSAOVERLAPPED*>(&lpover_ex), INFINITE);
		if (FALSE == is_error)
		{
			int err_no = WSAGetLastError();
			if (64 != err_no)
				error_display("GQCS ", err_no);
			else {
				disconnect(key);
				continue;
			}
		}
		if (0 == io_byte) {
			disconnect(key);
			continue;
		}
		if (lpover_ex->is_recv) {
			int rest_size = io_byte;
			char *ptr = lpover_ex->messageBuffer;
			char packet_size = 0;
			if (0 < clients[key].sock.prev_size)
			{
				char packet_data[8] = { 0 };
				int Ipos = 0;
				for (Ipos = 0; Ipos < 8; ++Ipos)
				{
					if (clients[key].sock.packet_buf[Ipos] != 124)
						packet_data[Ipos] = clients[key].sock.packet_buf[Ipos];
					else
					{
						break;
					}
				}

				packet_size = atoi(packet_data) + 8;
			}
			while (rest_size > 0) {
				if (0 == packet_size)
				{
					char packet_data[8] = { 0 };
					int Ipos = 0;
					for (Ipos = 0; Ipos < 8; ++Ipos)
					{
						if (ptr[Ipos] != 124)
							packet_data[Ipos] = ptr[Ipos];
						else
						{
							break;
						}
					}

					packet_size = atoi(packet_data) + 8;
				}
				int required = packet_size - clients[key].sock.prev_size;
				if (rest_size >= required) {
					memcpy(clients[key].sock.packet_buf + clients[key].sock.prev_size, ptr, required);
					process_packet(key, packet_size, clients[key].sock.packet_buf);
					rest_size -= required;
					ptr += required;
					packet_size = 0;
				}
				else {
					memcpy(clients[key].sock.packet_buf + clients[key].sock.prev_size,
						ptr, rest_size);
					rest_size = 0;
				}
			}
			do_recv(key);
		}
		else {
			delete lpover_ex;
		}
	}
}
void do_accept()
{
	// Winsock Start - windock.dll �ε�
	WSADATA WSAData;
	if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0)
	{
		cout << "Error - Can not load 'winsock.dll' file\n";
		return;
	}

	// 1. ���ϻ���  
	SOCKET listenSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (listenSocket == INVALID_SOCKET)
	{
		cout << "Error - Invalid socket\n";
		return;
	}

	// �������� ��ü����
	SOCKADDR_IN serverAddr;
	memset(&serverAddr, 0, sizeof(SOCKADDR_IN));
	serverAddr.sin_family = PF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);
	serverAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	// 2. ���ϼ���
	if (::bind(listenSocket, (struct sockaddr*)&serverAddr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
	{
		cout << "Error - Fail bind\n";
		// 6. ��������
		closesocket(listenSocket);
		// Winsock End
		WSACleanup();
		return;
	}

	// 3. ���Ŵ�⿭����
	if (listen(listenSocket, 5) == SOCKET_ERROR)
	{
		cout << "Error - Fail listen\n";
		// 6. ��������
		closesocket(listenSocket);
		// Winsock End
		WSACleanup();
		return;
	}

	SOCKADDR_IN clientAddr;
	int addrLen = sizeof(SOCKADDR_IN);
	memset(&clientAddr, 0, addrLen);
	SOCKET clientSocket;
	DWORD flags;

	while (1)
	{
		clientSocket = accept(listenSocket, (struct sockaddr *)&clientAddr, &addrLen);
		BOOL optval = TRUE;
		setsockopt(clientSocket, IPPROTO_TCP, TCP_NODELAY, (char*)&optval, sizeof(optval));
		if (clientSocket == INVALID_SOCKET)
		{
			printf("Error - Accept Failure\n");
			return;
		}

		int new_id = get_new_id();
		cout << "���� ���̵�: " << new_id << endl;
		clients[new_id].init(new_id);

		memset(&clients[new_id].sock.over, 0x00, sizeof(struct OVER_EX));
		memset(&clients[new_id].sock.packet_buf, 0x00, MAX_BUFFER);

		clients[new_id].sock.socket = clientSocket;
		clients[new_id].sock.over.dataBuffer.len = MAX_BUFFER;
		clients[new_id].sock.over.dataBuffer.buf = clients[new_id].sock.over.messageBuffer;
		clients[new_id].sock.over.is_recv = true;
		flags = 0;

		CreateIoCompletionPort(reinterpret_cast<HANDLE>(clientSocket), g_iocp, new_id, 0);

		//iocp���� �� Ʈ��� �Ѿ��� �׷��� �̰� �Ʒ���
		clients[new_id].sock.connected = true;

		//send_login_ok_packet(new_id);	// �� ���̵� Ŭ�󿡰� �˷��ֱ�

		// ó�� ���� �� �ٸ� ĳ����, ���� ���� ������ ������ 
		//send_all_player_packet(new_id);
		//send_put_item_packet(new_id);
		send_init_packet(new_id);

		//new_id ������ �̹� �����ִ� �÷��̾�鿡�� �˸���
		send_put_player_packet(new_id);


		do_recv(new_id);
	}

	// 6-2. ���� ��������
	closesocket(listenSocket);

	// Winsock End
	WSACleanup();

	return;
}
void do_recv(char id)
{
	DWORD flags = 0;

	SOCKET client_s = clients[id].sock.socket;

	OVER_EX *over = &clients[id].sock.over;

	over->dataBuffer.len = MAX_BUFFER;
	over->dataBuffer.buf = over->messageBuffer;
	ZeroMemory(&(over->overlapped), sizeof(WSAOVERLAPPED));
	if (WSARecv(client_s, &over->dataBuffer, 1, NULL, &flags, &(over->overlapped), NULL) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			cout << "Error - Fail WSARecv(error_code : " << WSAGetLastError << ")" << endl;
		}
	}
}
void make_obj()
{
	int monster_id = 0;
	auto delta = point - std::chrono::high_resolution_clock::now();
	if (std::chrono::duration_cast<std::chrono::seconds>(delta).count()>60)	// 60�� ����
	{
		// monsters map �� �ֱ�
			float x = float(rand() % 1800 - 900);
			float z = float(rand() % 1800 - 900);

			monsters[monster_id].SetDraw(true);
			monsters[monster_id].SetPos(x, 100, z);
			monsters[monster_id].SetHp(100);

		send_put_monster_packet(monster_id);
		point = std::chrono::high_resolution_clock::now();
		++monster_id;
	}
}


void SendPacket(const int type, const int id, const void *packet, const int packet_size)
{
	if (clients[id].sock.socket != NULL) {
		OVER_EX *over = new OVER_EX;
		ZeroMemory(&over->overlapped, sizeof(over->overlapped));
		over->is_recv = false;
		char p_size[MAX_BUFFER]{ 0 };

		// Ŭ���̾�Ʈ���� ��Ŷ ���۽� <��Ŷũ�� | ��Ŷ Ÿ�� 8����Ʈ ���� ������> ���� ������ �Ѵ�.
		itoa(packet_size, p_size, 10);// 10������ ù �Ķ���͸� 2��° �Ķ���Ϳ� ���ڿ��� ��ȯ�Ͽ� ����
		int buf_len = (int)strlen(p_size);// ������ ������ �б� ���� 4����Ʈ�� 4
		p_size[buf_len] = '|';// �����Ѱ� �ڿ� | �ֱ� 4|
		p_size[buf_len + 1] = int(type);// �� �ڿ� Ÿ�� �ֱ� 4|\x1 

		// ��Ŷ ����� �̸� ���ļ� ��������Ѵ�.
		memcpy(over->messageBuffer, packet, packet_size);

		for (int i = 8; i < packet_size + 8; ++i) {
			p_size[i] = over->messageBuffer[i - 8];
		}
		ZeroMemory(&over->messageBuffer, sizeof(over->messageBuffer));
		memcpy(over->messageBuffer, p_size, packet_size + 8);

		over->dataBuffer.buf = reinterpret_cast<CHAR *>(p_size);
		over->dataBuffer.len = packet_size + 8;
		int res = WSASend(clients[id].sock.socket, &over->dataBuffer, 1, NULL, 0, &over->overlapped, NULL);
		if (0 != res) {
			int error_no = WSAGetLastError();
			if (WSA_IO_PENDING != error_no) {
				error_display((char *)"SendPacket:WSASend", error_no);
			}
		}
	}
}

void process_packet(const int id, const int packet_size, const char * buf)
{
	if (packet_size == 8)
		return;
	//��Ŷũ�� | Ÿ�� ���� 8����Ʈ �׵ڿ� ������
	int Ipos = 0;
	for (Ipos = 0; Ipos < 8; ++Ipos)
	{
		if (buf[Ipos] == 124)
			break;
	}
	char *get_packet = new char[packet_size] {0};
	for (int i = 8; i < packet_size; ++i)
		get_packet[i - 8] = buf[i];

	switch (buf[Ipos + 1])
	{
	case CS_INFO:
	{
		auto client_Check_info = Game::Protocol::GetClientView(get_packet);
		vec3 p = { client_Check_info->position()->x(), client_Check_info->position()->y(), client_Check_info->position()->z() };
		vec3 r = { client_Check_info->rotation()->x(), client_Check_info->rotation()->y(), client_Check_info->rotation()->z() };
		int ani = client_Check_info->animator();
		float x = client_Check_info->dirX();
		float z = client_Check_info->dirZ();
		clients[id].SetLock();
		clients[id].SetPos(p);
		clients[id].SetRotation(r);
		clients[id].SetAnimator(ani);
		clients[id].SetDirX(x);
		clients[id].SetDirZ(z);
		clients[id].SetUnlock();
		//cout << "ID:"<<id<<" pos: " << p.x << "," << p.y << "," << p.z << endl;
	}
	break;
	case CS_GET_ITEM:
	{
		int item = get_packet[0];
		items[item].SetDraw(false);
		for (int i = 1; i <= MAX_USER; ++i)
		{
			if (i == id) continue;
			if (clients[i].sock.connected == false) continue;
		send_remove_item_packet(i, item);
		}
	}
	break;
	case CS_MONSTER_STATUS:
	{

	}
	break;
	default:
		break;
	}
	send_my_status_to_all_packet(id);

}

//------------------------------packet------------------------------
void send_login_ok_packet(int id)
{
	flatbuffers::FlatBufferBuilder builder;
	builder.Clear();
	clients[id].SetLock();
	int i = clients[id].GetId();
	int hp = clients[id].GetHp();
	int ani = clients[id].GetAnimator();
	float x = clients[id].GetDirX();
	float z = clients[id].GetDirZ();
	auto name = builder.CreateString(clients[id].GetName());
	float h = clients[id].GetHorizontal();
	float v = clients[id].GetVertical();
	auto pos = clients[id].GetPos();
	auto rot = clients[id].GetRotation();
	clients[id].SetUnlock();
	auto data = CreateClient_info(builder, i, hp, ani, x, z, h, v, name, &Vec3(pos.x, pos.y, pos.z), &Vec3(rot.x,rot.y,rot.z));
	builder.Finish(data);
	SendPacket(SC_ID, id, builder.GetBufferPointer(), builder.GetSize());
}
void send_put_player_packet(int id)
{
	flatbuffers::FlatBufferBuilder builder;
	builder.Clear();
	clients[id].SetLock();
	int i = clients[id].GetId();
	int hp = clients[id].GetHp();
	int ani = clients[id].GetAnimator();
	float x = clients[id].GetDirX();
	float z = clients[id].GetDirZ();
	auto name = builder.CreateString(clients[id].GetName());
	float h = clients[id].GetHorizontal();
	float v = clients[id].GetVertical();
	auto pos = clients[id].GetPos();
	auto rot = clients[id].GetRotation();
	clients[id].SetUnlock();
	auto data = CreateClient_info(builder, i, hp, ani, x, z, h, v, name, &Vec3(pos.x, pos.y, pos.z), &Vec3(rot.x, rot.y, rot.z));
	builder.Finish(data);
	for (int to = 1; to <= MAX_USER; ++to)
	{
		clients[to].SetLock();
		if (clients[to].sock.connected == true && to != id)
		{
			clients[to].SetUnlock();
			SendPacket(SC_PUT_PLAYER, to, builder.GetBufferPointer(), builder.GetSize());
			continue;
		}
		clients[to].SetUnlock();
	}
}
void send_all_player_packet(int to)
{
	flatbuffers::FlatBufferBuilder builder;
	builder.Clear();
	std::vector<flatbuffers::Offset<Client_info>> clients_data;

	for (int i = 1; i<MAX_USER+1;++i)
	{
		clients[i].SetLock();
		if (clients[i].sock.connected == false || clients[i].GetHp() == 0)
		{
			clients[i].SetUnlock();
			continue;
		}
		if (clients[i].GetId() == to)
		{
			clients[i].SetUnlock();
			continue;
		}
		int id = clients[i].GetId();
		int hp = clients[i].GetHp();
		int ani = clients[i].GetAnimator();
		float x = clients[i].GetDirX();
		float z = clients[i].GetDirZ();
		auto name = builder.CreateString(clients[i].GetName());
		float h = clients[i].GetHorizontal();
		float v = clients[i].GetVertical();
		auto pos = clients[i].GetPos();
		auto rot = clients[i].GetRotation();
		
		clients[i].SetUnlock();

		auto data = CreateClient_info(builder, id, hp, ani, x, z, h, v, name, &Vec3(pos.x, pos.y, pos.z), &Vec3(rot.x, rot.y, rot.z));
		clients_data.emplace_back(data);
	}
	if (clients_data.size() == 0)
		return;
	auto full_data = builder.CreateVector(clients_data);
	auto p = CreateClient_Collection(builder, full_data);
	builder.Finish(p);

	SendPacket(SC_ALL_PLAYER_DATA, to, builder.GetBufferPointer(), builder.GetSize());
}
void send_my_status_to_all_packet(int id)
{
	flatbuffers::FlatBufferBuilder builder;
	builder.Clear();
	clients[id].SetLock();
	int i = clients[id].GetId();
	int hp = clients[id].GetHp();
	int ani = clients[id].GetAnimator();
	float x = clients[i].GetDirX();
	float z = clients[i].GetDirZ();
	auto name = builder.CreateString(clients[id].GetName());
	float h = clients[id].GetHorizontal();
	float v = clients[id].GetVertical();
	auto pos = clients[id].GetPos();
	auto rot = clients[id].GetRotation();
	clients[id].SetUnlock();
	auto data = CreateClient_info(builder,i,hp,ani,x,z,h,v,name,&Vec3(pos.x,pos.y,pos.z),&Vec3(rot.x,rot.y,rot.z));
	builder.Finish(data);
	for (int to = 1; to <= MAX_USER; ++to)
	{
		if (clients[to].sock.connected == true&& to != id)
			SendPacket(SC_PLAYER_STATUS, to, builder.GetBufferPointer(), builder.GetSize());
	}
}

void send_remove_player_packet(int to, int obj)
{
	int i = obj;
	SendPacket(SC_REMOVE_PLAYER, to, &i, sizeof(i));
}

void send_put_monster_packet(int monster_id) 
{
	flatbuffers::FlatBufferBuilder builder;
	builder.Clear();
	monsters[monster_id].SetLock();
	int i = monsters[monster_id].GetId();
	int hp = monsters[monster_id].GetHp();
	int ani = monsters[monster_id].GetAnimator();
	float x = monsters[monster_id].GetDirX();
	float z = monsters[monster_id].GetDirZ();
	auto pos = monsters[monster_id].GetPos();
	auto rot = monsters[monster_id].GetRotation();
	monsters[monster_id].SetUnlock();
	auto data = CreateMonster_info(builder, i, hp, ani, x, z, &Vec3(pos.x, pos.y, pos.z), &Vec3(rot.x, rot.y, rot.z));
	builder.Finish(data);
	for (int to = 1; to <= MAX_USER; ++to)
	{
		if (clients[to].sock.connected == true)
			SendPacket(SC_PUT_MONSTER, to, builder.GetBufferPointer(), builder.GetSize());
	}
}

void send_put_item_packet(int id)
{
	flatbuffers::FlatBufferBuilder builder;
	builder.Clear();
	std::vector<flatbuffers::Offset<Item_info>> items_data;

	for (int item_id = 0; item_id < MAX_ITEM; ++item_id)
	{
		items[item_id].SetLock();
		int i = items[item_id].GetId();
		int t = items[item_id].GetType();
		auto pos = items[item_id].GetPos();
		items[item_id].SetUnlock();
		auto data = CreateItem_info(builder, i, t, &Vec3(pos.x, pos.y, pos.z));
		items_data.emplace_back(data);
	}
	auto full_data = builder.CreateVector(items_data);
	auto p = CreateItem_Collection(builder, full_data);
	builder.Finish(p);
	for (int to = 1; to <= MAX_USER; ++to)
	{
		if (clients[to].sock.connected == true)
			SendPacket(SC_PUT_ITEM, to, builder.GetBufferPointer(), builder.GetSize());
	}
}

void send_remove_item_packet(int id, int item)
{
	int i = item;
	SendPacket(SC_REMOVE_ITEM, id, &i, sizeof(i));
}

void send_init_packet(int id)
{
	flatbuffers::FlatBufferBuilder builder;
	builder.Clear();
	std::vector<flatbuffers::Offset<Item_info>> items_data;
	std::vector<flatbuffers::Offset<Client_info>> clients_data;
	std::vector<flatbuffers::Offset<Monster_info>> monsters_data;
	for (int item_id = 0; item_id < MAX_ITEM; ++item_id)
	{
		items[item_id].SetLock();
		int i = items[item_id].GetId();
		int t = items[item_id].GetType();
		auto pos = items[item_id].GetPos();
		items[item_id].SetUnlock();
		auto data = CreateItem_info(builder, i, t, &Vec3(pos.x, pos.y, pos.z));
		items_data.emplace_back(data);
	}
	auto full_items_data = builder.CreateVector(items_data);

	for (int i = 1; i < MAX_USER + 1; ++i)
	{
		clients[i].SetLock();
		if (clients[i].sock.connected == false || clients[i].GetHp() == 0)
		{
			clients[i].SetUnlock();
			continue;
		}
		//if (clients[i].GetId() == id)
		//{
		//	clients[i].SetUnlock();
		//	continue;
		//}
		int id = clients[i].GetId();
		int hp = clients[i].GetHp();
		int ani = clients[i].GetAnimator();
		float x = clients[i].GetDirX();
		float z = clients[i].GetDirZ();
		auto name = builder.CreateString(clients[i].GetName());
		float h = clients[i].GetHorizontal();
		float v = clients[i].GetVertical();
		auto pos = clients[i].GetPos();
		auto rot = clients[i].GetRotation();

		clients[i].SetUnlock();

		auto data = CreateClient_info(builder, id, hp, ani, x, z, h, v, name, &Vec3(pos.x, pos.y, pos.z), &Vec3(rot.x, rot.y, rot.z));
		clients_data.emplace_back(data);
	}
	if (clients_data.size() == 0)
		return;
	auto full_clients_data = builder.CreateVector(clients_data);
	auto full_monsters_data = builder.CreateVector(monsters_data);
	auto p = CreateInit_Collection(builder, id, full_items_data, full_monsters_data, full_clients_data);
	builder.Finish(p);
	SendPacket(SC_INIT_DATA, id, builder.GetBufferPointer(), builder.GetSize());

}
//------------------------------packet------------------------------

/*	
		inline const Game::Protocol::Client_info *GetClientView(const void *buf) {
			return flatbuffers::GetRoot<Game::Protocol::Client_info>(buf);
		}
		inline const Game::Protocol::Monster_info *GetMonsterView(const void *buf) {
			return flatbuffers::GetRoot<Game::Protocol::Monster_info>(buf);
		}
		inline const Game::Protocol::Item_info *GetItemView(const void *buf) {
			return flatbuffers::GetRoot<Game::Protocol::Item_info>(buf);
		}
		
*/