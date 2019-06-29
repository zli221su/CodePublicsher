using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Threading.Tasks;
using System.Windows;

namespace Project3
{
    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : Application
    {
        public static bool AutoDemo1 = false;
        public static bool AutoDemo2 = false;
        public static string clientMachineAddress = "localhost";
        public static string clientPort = "8082";
        public static string serverMachineAddress = "localhost";
        public static string serverPort = "8080";
        public static string remoteRoot = "..";
        public static string sendFilePath = "../../../ClientFilesFolders/SendFiles";
        public static string saveFilePath = "../../../ClientFilesFolders/SaveFiles";

        public static string[] Args;

        // -----------< receive cmd to run WPF >-------------
        private void Application_Startup(object sender, StartupEventArgs e)
        {
            if (e.Args.Length > 0)
            {
                Args = e.Args;
                // extracting client url
                string clientUrl = Args[0];
                int colonPosition = clientUrl.IndexOf(':');
                clientMachineAddress = clientUrl.Substring(0, colonPosition - 0);
                clientPort = clientUrl.Substring(colonPosition + 1, clientUrl.Length - (colonPosition + 1));

                // extracting server url
                string serverUrl = Args[1];
                colonPosition = serverUrl.IndexOf(':');
                serverMachineAddress = serverUrl.Substring(0, colonPosition - 0);
                serverPort = serverUrl.Substring(colonPosition + 1, serverUrl.Length - (colonPosition + 1));

                // set remote root path
                if (Args.Length > 2 && Args[2] != "AutoDemo1" && Args[2] != "AutoDemo2")
                {
                    remoteRoot = Args[2];
                }

                // set file storage files
                if (Args.Length > 3 && Args[3] != "AutoDemo1" && Args[3] != "AutoDemo2")
                {
                    sendFilePath = Args[3];
                }
                if (Args.Length > 4 && Args[4] != "AutoDemo1" && Args[4] != "AutoDemo2")
                {
                    saveFilePath = Args[4];
                }

                // set auto demo if provided
                AutoDemo1 = Args[Args.Length - 1] == "AutoDemo1";
                AutoDemo2 = Args[Args.Length - 1] == "AutoDemo2";

                Console.WriteLine("Following commendLine received: ");
                foreach (var arg in Args)
                {
                    Console.Write(" " + arg);
                }
                Console.WriteLine();
            }
        }
    }
}
