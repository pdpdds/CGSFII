using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;

using CgsfNET64Lib;
using CGSFNETCommon;

namespace EchoServerNet
{
    class ServerNetwork : CgsfNET64
    {
        override public bool ProcessPacket() 
        {

            Thread cur_thread = Thread.CurrentThread;
            DevLog.Write(string.Format("Thread Id:{0}", cur_thread.ManagedThreadId));
            return true;
        }
        
    }
}
