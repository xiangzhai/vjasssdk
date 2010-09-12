library AStructCoreNetHost requires ALibraryCoreDebugMisc, AStructCoreGeneralVector

	/**
	* Requires Reinventing the Craft.
	*/
	struct AHost
		public static constant integer commandConnect = 0
		public static constant integer commandDisconnect = 1
		public static constant integer commandAccepted = 2
		public static constant integer commandChat = 3
		public static constant integer commandNetMessage = 4 // placeholder for extra messages
		private static constant real connectionTimeOut = 5.0
		// construction members
		private integer m_maxClients
		private integer m_port
		// members
		private integer m_socket
		//private integer m_clients
		//private integer m_buffer
		//private integer m_listener
		private AStringVector m_clients

		//! runtextmacro optional A_STRUCT_DEBUG("\"AHost\"")

		private static method bufferConnectionAccepted takes nothing returns integer
			local integer buffer = CreateBuffer()
			call BufferWriteUInt(buffer, thistype.commandAccepted)
			return buffer
		endmethod

		public method connect takes string ip returns boolean
			local integer buffer = thistype.bufferConnectionAccepted()
			local integer result = SocSendUDPMessage(this.m_socket, ip, this.m_port, buffer)
			local real time
			call DestroyBuffer(buffer)

			if (result != 0) then
				debug call this.print("Error while connecting to client " + ip + ":" + port + " - " + I2S(SocGetSocketLastError()) + ".")
				return false
			endif

			set time = connectionTimeOut
			//loop

			debug call this.print("Connected to " + ip + ":" + I2S(port) + " successfully.")
			call this.m_clients.pushBack(ip)


			return true
		endmethod

		public method disconnect takes integer socketIndex returns boolean
			local boolean result = SocCloseSocket(this.m_sockets[socketIndex])
			call this.m_sockets.erase(socketIndex)
			return result
		endmethod

		private method receiver takes nothing returns nothing
			local integer message
			local integer command
			loop
				exitwhen (not SocTCPConnected(this.m_listener))
				set message = SocReceiveMessage(this.m_listener, 100, this.m_buffer)
				/// @todo Get sender?!
				set command = BufferReadInt(this.m_buffer)
				debug call this.print("Received command " + I2S(command))
				if (command == thistype.commandConnect) then
					if (this.m_clients < this.m_maxClients) then
						call this.connect(SocGetLastInIP(), SocGetLastInPort())
					debug else
						debug call this.print("Reached player maximum.")
					endif
				elseif (command == thistype.commandDisconnect) then
					//if (
				endif
				call BufferClear(this.m_buffer)
			endloop
		endmethod

		public static method create takes integer m_maxClients returns thistype
			local thistype this = thistype.allocate()
			if (not SocNetConnected()) then
				debug call this.print("Connect socket network.")
				call SocStart()
			endif
			// construction members
			set this.m_maxClients = m_maxClients
			// members
			set this.m_clients = 0
			set this.m_buffer = CreateBuffer()
			set this.m_listener = SocTCPListen(6112, 255, 1)
			debug call this.print("Created listener from IP " +  SocTCPIP(this.m_listener) + ".")
			call this.receiver.execute()
			return this
		endmethod

		public method onDestroy takes nothing returns nothing
			// members
			call DestroyBuffer(this.m_buffer)
			call SocCloseSocket(this.m_listener)
			loop
				exitwhen (this.m_sockets.empty())
				call SocCloseSocket(this.m_sockets.back())
				call this.m_sockets.popBack()
			endloop
			call this.m_sockets.destroy()
		endmethod
	endstruct

	/// @todo Add hooks which hook native Create functions etc. and send commands to all clients.

endlibrary