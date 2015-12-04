
using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using UnityEngine;

namespace CGSF
{
    public class TCPNetwork
    {
        TcpClient tcp = new TcpClient();
        byte[] buffer = new byte[8196];
        public bool IsConnected()
        {
            if (tcp == null)
                return false;

            return tcp.Connected;
        }

        public NetworkStream GetStream()
        {
            return tcp.GetStream();
        }

        public bool AsyncConnect(string ip, int port)
        {
            IPAddress ipAddress = IPAddress.Parse(ip);
            IPEndPoint ipEndpoint = new IPEndPoint(ipAddress, port);     

            //Asynchronous call
            IAsyncResult asyncConnect = tcp.BeginConnect(ipAddress, port, connectCallback, null);

            
            return true;
        }
        
        public void connectCallback(IAsyncResult asyncConnect)
        {
            tcp.EndConnect(asyncConnect);

            tcp.GetStream().BeginRead(buffer, 0, buffer.Length, ReadCallback, null);
        }

        void ReadCallback(IAsyncResult result)
        {            
            int read = 0;
            read = tcp.GetStream().EndRead(result);
            if (read == 0)
            {
                // Connection has been closed.
                //tcp.GetStream().Close();
                return;
            }

            // read recurse
            tcp.GetStream().Flush();
            tcp.GetStream().BeginRead(buffer, 0, buffer.Length, ReadCallback, null);
        }        
    }
}
