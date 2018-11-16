using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;
using System.Runtime;
using System.Runtime.InteropServices;
using System.IO;
namespace Client
{
    class Program
    {
        [DllImport("user32.dll", EntryPoint = "SendMessage")]
        public static extern IntPtr SendMessage(IntPtr hwnd, int wMsg, IntPtr wParam, IntPtr lParam);
        static void Main(string[] args)
        {
            //string str = "";
            //string output = "";
            //Console.WriteLine("请输入命令");
            //str = "ipconfig";
            //CMD.In(str);
            //Thread.Sleep(50);
            //Console.WriteLine(CMD.Out());            
            //str = "dir";
            //CMD.In(str);
            //Thread.Sleep(100);
            //Console.WriteLine(CMD.Out());
            //CMD.Exit();
            //Console.WriteLine("输入回车结束");

            Console.Read();
        }
    }
}
