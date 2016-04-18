
using System;
using System.IO;

namespace CGSFNet
{
    public class CGSFNetProtocol
    {
        //-------------------------------------------------------------------------------------
        // Packet Layouts:
        //-------------------------------------------------------------------------------------
        // SignatureStart	(2 bytes little endian ushort) = 16018
        // PacketLength		(2 bytes little endian ushort)
        // PacketID(unused)	(2 bytes little endian ushort) = 1
        // MessageLength	(2 bytes little endian ushort)
        // Header			(PacketLength – 10 - MessageLength bytes google protocol buffer)
        // Message			(MessageLength bytes google protocol buffer)
        // SignatureEnd		(2 bytes little endian ushort) = 16108
        //-------------------------------------------------------------------------------------

        const ushort SIGNATURE_START = 16018;
        const ushort SIGNATURE_END = 16108;
        public ushort length = 0;

        //------------------------------------------------------------------------------------
        // statics
        public static byte[] Encode<T>(T message, ushort messageId)
        {
            MemoryStream mem2 = new MemoryStream();
            ProtoBuf.Serializer.Serialize<T>(mem2, message);
            byte[] messageBytes = mem2.ToArray();
            ushort packetLength = (ushort)(messageBytes.Length + 8);

            // SignatureStart	(2 bytes little endian ushort) = 16018
            MemoryStream mem = new MemoryStream();
            byte[] bytes = UshortToLittleEndianBytes(SIGNATURE_START);
            mem.Write(bytes, 0, 2);

            // PacketLength		(2 bytes little endian ushort)
            bytes = UshortToLittleEndianBytes(packetLength);
            mem.Write(bytes, 0, 2);

            // PacketID(unused)	(2 bytes little endian ushort) = 1
            bytes = UshortToLittleEndianBytes(messageId);
            mem.Write(bytes, 0, 2);
            
            // Header			(PacketLength – 10 - MessageLength bytes google protocol buffer)
            mem.Write(messageBytes, 0, messageBytes.Length);

            // SignatureEnd		(2 bytes little endian ushort) = 16108
            bytes = UshortToLittleEndianBytes(SIGNATURE_END);
            mem.Write(bytes, 0, 2);

            bytes = mem.ToArray();
            mem.Close();
            mem2.Close();

            return bytes;
        }

       public static SAMPLE.Hello Decode(byte[] buffer, int offset, int count)
        {
            if (count < 10) // header + tail
                return null;           

            // SignatureStart	(2 bytes little endian ushort) = 16018
            int size = 2;
            ushort signature = LittleEndianBytesToUshort(buffer, offset);
            if (signature != SIGNATURE_START)
                return null;
            offset += size;

            // PacketLength		(2 bytes little endian ushort)
            size = 2;
            ushort packetLength = LittleEndianBytesToUshort(buffer, offset);
            if (count < packetLength)
                return null;
            offset += size;

            // PacketID(unused)	(2 bytes little endian ushort) = 1
            size = 2;
            //ushort unused = LittleEndianBytesToUshort(buffer, offset);
            offset += size;

            // MessageLength	(2 bytes little endian ushort)
            size = 2;
            ushort messageLength = LittleEndianBytesToUshort(buffer, offset);
            offset += size;

            // Header			(PacketLength – 10 - MessageLength bytes google protocol buffer)
            size = packetLength - 8;            

            MemoryStream mem = new MemoryStream();
            mem.Write(buffer, offset, size);
            mem.Seek(0, SeekOrigin.Begin);
            SAMPLE.Hello header = ProtoBuf.Serializer.Deserialize<SAMPLE.Hello>(mem);
            mem.Close();

            offset += size;            

            // SignatureEnd		(2 bytes little endian ushort) = 16108
            size = 2;
            signature = LittleEndianBytesToUshort(buffer, offset);
            if (signature != SIGNATURE_END)
                return null;

            return header;
        }

        static byte[] UshortToLittleEndianBytes(ushort val)
        {
            byte[] bytes = BitConverter.GetBytes(val);
            if (!BitConverter.IsLittleEndian)
                Array.Reverse(bytes, 0, 2);
            return bytes;
        }

        static ushort LittleEndianBytesToUshort(byte[] buffer, int start)
        {
            byte[] bytes = new byte[2] { buffer[start], buffer[start + 1] };
            if (!BitConverter.IsLittleEndian)
                Array.Reverse(bytes, 0, 2);
            return BitConverter.ToUInt16(bytes, 0);
        }       
    }
}
