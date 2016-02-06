using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using CgsfNET64Lib;

namespace EchoServerNet
{
    class ServerNetwork : CgsfNET64
    {
        override public bool ProcessPacket() 
        {
            return true;
        }
    }
}
