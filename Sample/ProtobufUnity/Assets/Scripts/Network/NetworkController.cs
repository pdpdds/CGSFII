using UnityEngine;
using System.Collections;
using System.Net.Sockets;
using System;

namespace CGSF
{
    public class NetworkController : Singleton<NetworkController>
    {
        TCPNetwork network = new TCPNetwork();
        public GameObject SendButton = null;
        // Use this for initialization
        void Start()
        {
            //network.AsyncConnect("192.168.0.13", 25251);
        }

        // Update is called once per frame
        static bool connected = false;
        void Update()
        {
          /*  if(connected == false && network.IsConnected() == true)
            {
                connected = true;

                SAMPLE.Hello message = new SAMPLE.Hello();
                message.message = "Hello!!";
                byte[] data = CGSFNet.CGSFNetProtocol.Encode<SAMPLE.Hello>(message, (ushort)SAMPLE.ID.HELLO);
                NetworkStream stream = network.GetStream();
                stream.BeginWrite(data, 0, data.Length, WriteCallback, null);
            }*/
        }

        // .Net Thread
        void WriteCallback(IAsyncResult result)
        {            
        }
    }
}