using UnityEngine;
using System.Collections;
using UnityEngine.UI;
using System.Net.Sockets;
using System;

namespace CGSF
{
    public class UIRoot : MonoBehaviour
    {
        public GameObject sendButton = null;
        public InputField inputField = null;
        public Text connectionText = null;
        public ScrollRect outputScroll = null;

        TCPNetwork network = new TCPNetwork();

        public void Awake()
        {
            
        }

        // Use this for initialization
        void Start() {

        }

        // Update is called once per frame
        void Update() {
            if(network.IsConnected())
            {
                connectionText.text = "Connected";
            }
            else
            {
                connectionText.text = "Disconnected";
            }
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
            connectionText.text = "Connecting...";
            outputScroll.GetComponentInChildren<Text>().text = "sdffsdfdsd";
            outputScroll.GetComponentInChildren<Scrollbar>().enabled = true;
        }

        // .Net Thread
        void WriteCallback(IAsyncResult result)
        {
            network.GetStream().EndWrite(result);
            network.GetStream().Flush();
        }
    }
}