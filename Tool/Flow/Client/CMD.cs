using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
namespace Client
{
    public delegate void DelReadStdOutput(string result);
    public delegate void DelReadErrOutput(string result);
    class CMD
    {
        static public event DelReadStdOutput ReadStdOutput;
        static public event DelReadErrOutput ReadErrOutput;
        static private List<string> outstrs;
        private static System.Diagnostics.Process cmd;
        static CMD()
        {
            outstrs = new List<string>();
            outstrs.Clear();
            ReadStdOutput += DelReadStdOutputAction;
            ReadErrOutput += DelReadErrOutputAction;
            cmd = new System.Diagnostics.Process();
            cmd.StartInfo.FileName = "cmd.exe";
            cmd.StartInfo.UseShellExecute = false;    //是否使用操作系统shell启动
            cmd.StartInfo.RedirectStandardInput = true;//接受来自调用程序的输入信息
            cmd.StartInfo.RedirectStandardOutput = true;//由调用程序获取输出信息
            cmd.StartInfo.RedirectStandardError = true;//重定向标准错误输出
            cmd.StartInfo.CreateNoWindow = true;//不显示程序窗口

            cmd.OutputDataReceived += new System.Diagnostics.DataReceivedEventHandler(p_OutputReceived);
            cmd.ErrorDataReceived += new System.Diagnostics.DataReceivedEventHandler(p_ErrOutputReceived);
            cmd.EnableRaisingEvents = true;
            cmd.Exited += new EventHandler(OnExit);
            cmd.Start();//启动程序
            cmd.BeginErrorReadLine();
            cmd.BeginOutputReadLine();
        }
        static public void OnExit(object sender,EventArgs e){
        }
        static public void p_OutputReceived(object sender, System.Diagnostics.DataReceivedEventArgs e)
        {
            if (e.Data != null)
            {
                DelReadStdOutputAction((string)e.Data);
            }
        }
        static public void p_ErrOutputReceived(object sender, System.Diagnostics.DataReceivedEventArgs e)
        {
            if (e.Data != null)
            {
                DelReadErrOutputAction((string)e.Data);
            }
        }
        static public void DelReadStdOutputAction(string str){
            outstrs.Add(str);
        }
        static public void DelReadErrOutputAction(string str)
        {
            Console.WriteLine("Error:"+str);
        }
        static public void In(string str)
        {
            outstrs.Clear();
            cmd.StandardInput.WriteLine(str);
            cmd.StandardInput.AutoFlush = true;
        }
        static public string Out()
        {
            if(outstrs.Count == 0)
                return "";
            else
            {
                string s = "";
                foreach (string  str in outstrs)
                {
                    s = s + str + "\n";
                }
                return s;
            }
        }
        static public void Exit(){
            if (cmd != null)
            {
                In("exit");
                System.Threading.Thread.Sleep(500);
                cmd.Close();
                cmd = null;
            }
        }
    }
}
