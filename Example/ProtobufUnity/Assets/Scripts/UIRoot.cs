using UnityEngine;
using System.Collections;
using UnityEngine.UI;
using System.Net.Sockets;
using System;

namespace CGSF
{
    public class UIRoot : MonoBehaviour {

        public GameObject sendButton = null;
        public InputField inputField = null;

        TCPNetwork network = new TCPNetwork();

        // Use this for initialization
        void Start() {

        }

        // Update is called once per frame
        void Update() {

        }

        public void OnSendButton()
        {
            if(network.IsConnected())
            {
                network.GetStream().Flush();

                SAMPLE.Hello message = new SAMPLE.Hello();
                message.message = inputField.text;
                byte[] data = CGSFNet.CGSFNetProtocol.Encode<SAMPLE.Hello>(message, (ushort)SAMPLE.ID.HELLO);
                NetworkStream stream = network.GetStream();
                stream.BeginWrite(data, 0, data.Length, WriteCallback, null);
            }
        }

        public void OnConnect()
        {
            network.AsyncConnect("192.168.0.13", 25251);
        }

        // .Net Thread
        void WriteCallback(IAsyncResult result)
        {
            network.GetStream().EndWrite(result);
            network.GetStream().Flush();
        }
    }
}