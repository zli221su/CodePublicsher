////////////////////////////////////////////////////////////////////////
// MainWindow.xaml.cs - Provide methods to support MainWindow.xaml    //
// ver 2.0                                                            //
// Zhi Li, CSE687 - Object Oriented Design, Spring 2019               //
////////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * MainWindow.xaml.cs provides text change, button click, status bar updates
 * methods in C++ CodePublisher.
 *
 * Required Files:
 * ---------------
 * Translater.h, Translater.cpp          // Translation between C# and C++ codes.
 *
 * Maintenance History:
 * --------------------
 * ver 2.0 : 25 Apr 2019
 * - Supporting new format of communicating with remote server
 *
 * --------------------
 * ver 1.0 : 9 Apr 2019
 * - first release
 *
*/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Forms;
using System.Threading;
using System.IO;
using MsgPassingCommunication;
using System.Windows.Documents;

namespace Project3
{
    public partial class MainWindow : Window
    {
        private string SubDir { get; set; }
        private string RootDirectory { get; set; }
        private string WDirectory { get; set; }
        private List<string> Patterns { get; set; }
        private List<string> Regexes { get; set; }
        private List<string> Analysers { get; set; }
        private bool OpenSourceCode { get; set; }
        private HashSet<string> receivedKeys = new HashSet<string>();
        Translater translater;
        Thread workerThread;  // pulls messages from shim and updates the GUI
        volatile bool running = true; // better to use locking

        internal CsEndPoint clientEndPoint_;
        internal CsEndPoint serverEndPoint_;

        private Dictionary<string, Action<CsMessage>> dispatcher_
          = new Dictionary<string, Action<CsMessage>>();
        internal string saveFilesPath;
        internal string sendFilesPath;

        public MainWindow()
        {
            InitializeComponent();
            Console.Title = "Code Publisher Demo GUI Console";
        }

        // ------------< Window loading, initialize variables, start translater and work thread >---------------
        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            Patterns = new List<string>();
            Regexes = new List<string>();
            Analysers = new List<string>();
            // following three analysers are turned on by default
            Analysers.Add("comment");
            Analysers.Add("scope");
            Analysers.Add("dependency");

            // start Comm
            clientEndPoint_ = new CsEndPoint();
            clientEndPoint_.machineAddress = App.clientMachineAddress;
            clientEndPoint_.port = Convert.ToInt32(App.clientPort);
            Console.WriteLine("client url: " + clientEndPoint_.machineAddress + clientEndPoint_.port);

            serverEndPoint_ = new CsEndPoint();
            serverEndPoint_.machineAddress = App.serverMachineAddress;
            serverEndPoint_.port = Convert.ToInt32(App.serverPort);
            Console.WriteLine("server url: " + serverEndPoint_.machineAddress + serverEndPoint_.port);

            // start translater
            translater = new Translater();
            translater.listen(clientEndPoint_);

            // set variables
            OpenSourceCode = false; // converted files are opened by default browser by default
            
            WDirectory = App.remoteRoot;
            TxtPathSet(WDirectory);
            txtPatterns.Text = "*.h *.cpp";
            txtRegexes.Text = "(CodeA)(.*)";

            // start work thread
            workerThread = new Thread(() =>
            {
                ReceivingThreadProc();
            });
            workerThread.IsBackground = true;
            workerThread.Start();

            // load dispatcher
            loadDispatcher();

            // client files saving position
            sendFilesPath = translater.setSendFilePath(App.sendFilePath);
            saveFilesPath = translater.setSaveFilePath(App.saveFilePath);

            // Auto demo
            if (App.AutoDemo1 == true)
            {
                PostMsg("publish");
            }
            else if (App.AutoDemo2 == true)
            {
                txtRegexes.Text = "(P)(.*)";
                PostMsg("publish");
            }
        }

        // -----< Window closed event handler >-----------
        private void Window_Closed(object sender, EventArgs e)
        {
            running = false;
        }

        //----< load all dispatcher processing >---------------------------
        private void loadDispatcher()
        {
            DispatcherLoadGetDirs();
            DispatcherLoadGetFiles();
            DispatcherLoadConvertFiles();
        }

        //----< add client processing for message with key >---------------
        private void addClientProc(string key, Action<CsMessage> clientProc)
        {
            dispatcher_[key] = clientProc;
        }

        //----< load getDirs processing into directory block >-------
        private void DispatcherLoadGetDirs()
        {
            Action<CsMessage> getDirs = (CsMessage rcvMsg) =>
            {
                Action clrDirs = () =>
                {
                    lstDirs.Items.Clear();
                };
                Dispatcher.Invoke(clrDirs, new Object[] { });
                var enumer = rcvMsg.attributes.GetEnumerator();
                while (enumer.MoveNext())
                {
                    string key = enumer.Current.Key;
                    if (key.Contains("dir"))
                    {
                        Action<string> doDir = (string dir) =>
                        {
                            lstDirs.Items.Add(dir);
                        };
                        Dispatcher.Invoke(doDir, new Object[] { enumer.Current.Value });
                    }
                }
                Action insertUp = () =>
                {
                    lstDirs.Items.Insert(0, "..");
                    statusBarText.Text = "\"getDirs\" reply received!";
                };
                Dispatcher.Invoke(insertUp, new Object[] { });
            };
            addClientProc("getDirs", getDirs);
        }

        //----< load getFiles processing into files block >------
        private void DispatcherLoadGetFiles()
        {
            Action<CsMessage> getFiles = (CsMessage rcvMsg) =>
            {
                Action clrFiles = () =>
                {
                    lstFiles.Items.Clear();
                };
                Dispatcher.Invoke(clrFiles, new Object[] { });
                var enumer = rcvMsg.attributes.GetEnumerator();
                while (enumer.MoveNext())
                {
                    string key = enumer.Current.Key;
                    if (key.Contains("file"))
                    {
                        Action<string> doFile = (string file) =>
                        {
                            lstFiles.Items.Add(file);
                        };
                        Dispatcher.Invoke(doFile, new Object[] { enumer.Current.Value });
                    }
                }
                Action upDateStatusBar = () =>
                {
                    statusBarText.Text = "\"getFiles\" reply received!";
                };
                Dispatcher.Invoke(upDateStatusBar, new Object[] { });
            };
            addClientProc("getFiles", getFiles);
        }

        //----< load converted files for displaying >------
        private void DispatcherLoadConvertFiles()
        {
            Action<CsMessage> convertFiles = (CsMessage rcvMsg) =>
            {
                Console.Write("\n  converted files received");
                Action clrConvertedFiles = () =>
                {
                    lstConvertedFiles.Items.Clear();
                };
                Dispatcher.Invoke(clrConvertedFiles, new Object[] { });
                var enumer = rcvMsg.attributes.GetEnumerator();
                int filesCount = 0;
                while (enumer.MoveNext())
                {
                    string key = enumer.Current.Key;
                    if (key.Contains("fileFound"))
                    {
                        filesCount++;
                        Action<string> doConverted = (string file) =>
                        {
                            string fileName = System.IO.Path.GetFileName(file);
                            lstConvertedFiles.Items.Add(fileName);
                            TabControl1.SelectedIndex = 1;
                        };
                        Dispatcher.Invoke(doConverted, new Object[] { enumer.Current.Value });
                    }
                }
                Action upDateStatusBar = () =>
                {
                    statusBarText.Text = filesCount + " files and their dependencies are converted and received!";
                };
                Dispatcher.Invoke(upDateStatusBar, new Object[] { });
            };
            addClientProc("filesFound", convertFiles);
        }

        // -----------< Function running in "get" thread >-------------
        private void ReceivingThreadProc()
        {
            while (running)
            {
                CsMessage msg = translater.getMessage();
                try
                {
                    string msgId = msg.value("command");
                    Console.Write("\n  client getting message \"{0}\"", msgId);
                    Dispatcher.Invoke(() =>
                    {
                        statusBarText.Text = "Message from server got!";
                        InterpretReceivedMsgs(msg);
                    });
                }
                catch (Exception ex)
                {
                    Console.Write("\n  {0}", ex.Message);
                    msg.show();
                }
            }
        }

        // ----------< do action for a message >-----------
        private void InterpretReceivedMsgs(CsMessage msg)
        {
            string msgId = msg.value("command");
            if (dispatcher_.ContainsKey(msgId))
                dispatcher_[msgId].Invoke(msg);
        }

        // ------------< post converting information to server (C++ Code Publisher in this solution) >--------------
        private void PostMsg(string command)
        {
            CsMessage msg = AssembleMsg();
            msg.add("to", CsEndPoint.toString(serverEndPoint_));
            msg.add("from", CsEndPoint.toString(clientEndPoint_));
            msg.add("command", command);
            Dispatcher.Invoke(() =>
            {
                statusBarText.Text = "Message: " + command + " sent to the server. Waiting for reply!...";
            });
            translater.postMessage(msg);
        }

        // -----------< Helper Function - Assemble message for posting >------------
        private CsMessage AssembleMsg()
        {
            CsMessage msg = new CsMessage();
            msg.add("SubDir", SubDir);
            msg.add("Directory", WDirectory);
            msg.add("path", WDirectory);
            //msg.add("OutputPath", OutputPath);
            for (int i = 0; i < Patterns.Count(); i++)
            {
                msg.add("Pattern" + (i + 1), Patterns[i]);
            }
            for (int i = 0; i < Regexes.Count(); i++)
            {
                msg.add("Regex" + (i + 1), Regexes[i]);
            }
            if (Analysers != null)
            {
                for (int i = 0; i < Analysers.Count(); i++)
                {
                    msg.add("Analyser" + (i + 1), Analysers[i]);
                }
            }
            return msg;
        }

        // -------< helper function to handle text change, and trigger text change in the whole GUI >--------
        private void TxtPathSet(string path)
        {
            txtPath.Text = path;
            WDirectory = path;

            PostMsg("getDirs");
            PostMsg("getFiles");
        }

        // ----------< post msg to server for publishing >-----------
        private void BtnPublish_Click(object sender, RoutedEventArgs e)
        {
            PostMsg("publish");
        }

        // -----------< Double click actions to open or selecct a file or a directory >----------

        private void LstDirs_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            if (lstDirs.SelectedIndex == -1) return;

            string selectedItem = lstDirs.SelectedItem.ToString();
            if (selectedItem == "..")
            {
                if (WDirectory == App.remoteRoot)
                {
                    Dispatcher.Invoke(() =>
                    {
                        statusBarText.Text = "Root path reached. Unable to go to parent folder from root path!";
                    });
                    return;
                }
                int lastSlashPosition = WDirectory.LastIndexOf('\\');
                if (lastSlashPosition != -1)
                {
                    selectedItem = WDirectory.Substring(0, lastSlashPosition);
                }
            }
            else
            {
                selectedItem = selectedItem.Substring(0, selectedItem.Length);
                selectedItem = System.IO.Path.Combine(WDirectory, selectedItem);
            }
            TxtPathSet(selectedItem);
            PostMsg("getDirs");
            PostMsg("getFiles");
        }

        private void LstConvertedFiles_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            if (lstConvertedFiles.SelectedIndex == -1) return;
            string selectedItem = lstConvertedFiles.SelectedItem.ToString();
            selectedItem = System.IO.Path.Combine(saveFilesPath, selectedItem);
            selectedItem = System.IO.Path.GetFullPath(selectedItem);

            if (OpenSourceCode)
            {
                string contents = File.ReadAllText(selectedItem);
                CodePopUp popup = new CodePopUp();
                popup.codeView.Text = contents;
                popup.Show();
            }
            else
            {
                System.Diagnostics.Process.Start(selectedItem);
            }

        }

        // ----------< Method to handle text change >--------------

        private void TxtRegexes_TextChanged(object sender, TextChangedEventArgs e)
        {
            Regexes = new List<string>(txtRegexes.Text.Split(' '));
        }

        private void TxtPatterns_TextChanged(object sender, TextChangedEventArgs e)
        {
            Patterns = new List<string>(txtPatterns.Text.Split(' '));
        }

        // Following methods handle actions for check boxes

        private void CbRecurse_Checked(object sender, RoutedEventArgs e)
        {
            SubDir = "true";
        }

        private void CbRecurse_Unchecked(object sender, RoutedEventArgs e)
        {
            SubDir = "false";
        }

        private void CbComment_Checked(object sender, RoutedEventArgs e)
        {
            if (Analysers == null)
            {
                Analysers = new List<string>();
            }
            if (!Analysers.Contains("comment"))
            {
                Analysers.Add("comment");
            }
        }

        private void CbComment_Unchecked(object sender, RoutedEventArgs e)
        {
            if (Analysers == null)
            {
                return;
            }
            Analysers.Remove("comment");
        }

        private void CbDependency_Checked(object sender, RoutedEventArgs e)
        {
            if (Analysers == null)
            {
                Analysers = new List<string>();
            }
            if (!Analysers.Contains("dependency"))
            {
                Analysers.Add("dependency");
            }
        }

        private void CbDependency_Unchecked(object sender, RoutedEventArgs e)
        {
            if (Analysers == null)
            {
                return;
            }
            Analysers.Remove("dependency");
        }

        private void CbScope_Checked(object sender, RoutedEventArgs e)
        {
            if (Analysers == null)
            {
                Analysers = new List<string>();
            }
            if (!Analysers.Contains("scope"))
            {
                Analysers.Add("scope");
            }
        }

        private void CbScope_Unchecked(object sender, RoutedEventArgs e)
        {
            if (Analysers == null)
            {
                return;
            }
            Analysers.Remove("scope");
        }

        private void OpenMethod_Checked(object sender, RoutedEventArgs e)
        {
            OpenSourceCode = true;
        }

        private void OpenMethod_Unchecked(object sender, RoutedEventArgs e)
        {
            OpenSourceCode = false;
        }
    }
}
