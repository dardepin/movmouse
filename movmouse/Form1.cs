using System;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;
using System.Windows.Forms;
using System.Collections.Generic;
using System.Runtime.InteropServices;

using HWND = System.IntPtr;

namespace movmouse
{
    public partial class Form1 : Form
    {
        private bool running = false;

        HWND WHandle;

        Thread scanthread;
        public Form1()
        { InitializeComponent(); }

        private void Form1_Load(object sender, EventArgs e)
        { reload(); }
        private void reload()
        {
            this.WindowsComboBox.DataSource = null;
            this.WindowsComboBox.Items.Clear();
            IDictionary<IntPtr, string> processes = GetOpenWindows();

            if (processes.Any())
            {
                this.WindowsComboBox.DataSource = new BindingSource(processes, null);
                this.WindowsComboBox.DisplayMember = "Value";
                this.WindowsComboBox.ValueMember = "Key";
            }
            return;
        }
        private void stop()
        {
            this.StartButton.Text = (this.running) ? "START" : "STOP";
            this.NotifyLabel.Text = (!this.running) ? "Running. Press STOP or F10 for stopping" : "Select Window and Press START";
            this.ReloadButton.Enabled = this.running;
            this.running = !this.running;
            this.WindowsComboBox.Enabled = !this.running;

            if (this.running)
            {
                this.WHandle = (HWND)this.WindowsComboBox.SelectedValue;
                if (this.WHandle == IntPtr.Zero) this.NotifyLabel.Text = "Select window first";
                else
                {
                    this.scanthread = new Thread(move);
                    this.scanthread.Start();
                }
            }

            return;
        }
        private void move()
        {
            RECT rect;

            Random r = new Random();

            while (this.running)
            {
                if (GetWindowRect(this.WHandle, out rect))
                {
                    SetForegroundWindow(this.WHandle);
                    SendMessage(this.WHandle, WM_SYSCOMMAND, SC_RESTORE, 0);
                    SetFocus(this.WHandle);

                    long x = r.Next()  % rect.right + rect.left;
                    long y = r.Next() % rect.top + rect.bottom;
                    SetCursorPos((int)x, (int)y);
                }
                System.Threading.Thread.Sleep(2000);

                /*if(this.NotifyLabel.InvokeRequired) this.NotifyLabel.Invoke(new Action(() => { this.NotifyLabel.Text = "Running " + idx++.ToString(); }));
                else this.NotifyLabel.Text = "Running " + idx++.ToString();*/
            }
            return;
        }
        private void ReloadButton_Click(object sender, EventArgs e)
        {
            this.reload();
            return;
        }
        private void StartButton_Click(object sender, EventArgs e)
        {
            /*if(this.running)//then stop
            {
                this.running = false;

                this.StartButton.Text = "START";
                this.ReloadButton.Enabled = true;
            }
            else //then start
            {
                this.running = true;
                this.StartButton.Text = "STOP";
                this.ReloadButton.Enabled = false;
            }*/
            this.stop();
            return;
        }
        protected override bool ProcessCmdKey(ref Message msg, Keys keyData)
        {
            if (keyData == Keys.F10 && this.running)
            { this.NotifyLabel.Text = "Stop key pressed"; this.stop(); }
            return true;
        }
        private IDictionary<HWND, string> GetOpenWindows()
        {
            HWND shellWindow = GetShellWindow();
            Dictionary<HWND, string> windows = new Dictionary<HWND, string>();

            EnumWindows(delegate (HWND hWnd, int lParam)
            {
                if (hWnd == shellWindow) return true;
                if (!IsWindowVisible(hWnd)) return true;

                int length = GetWindowTextLength(hWnd);
                if (length == 0) return true;

                StringBuilder builder = new StringBuilder(length);
                GetWindowText(hWnd, builder, length + 1);

                windows[hWnd] = builder.ToString();
                return true;

            }, 0);

            return windows;
        }

        static Int32 WM_SYSCOMMAND = 0x0112;
        static Int32 SC_RESTORE = 0xF120;
        private delegate bool EnumWindowsProc(HWND hWnd, int lParam);
        // dll imports
        [DllImport("USER32.DLL")]
        private static extern bool EnumWindows(EnumWindowsProc enumFunc, int lParam);
        [DllImport("USER32.DLL")]
        private static extern int GetWindowText(HWND hWnd, StringBuilder lpString, int nMaxCount);
        [DllImport("USER32.DLL")]
        private static extern int GetWindowTextLength(HWND hWnd);
        [DllImport("USER32.DLL")]
        private static extern bool IsWindowVisible(HWND hWnd);
        [DllImport("USER32.DLL")]
        private static extern IntPtr GetShellWindow();
        [DllImport("USER32.DLL")]
        private static extern int SetForegroundWindow(IntPtr hWnd);
        [DllImport("USER32.DLL")]
        private static extern bool SendMessage(IntPtr hWnd, Int32 msg, Int32 wParam, Int32 lParam);
        [DllImport("USER32.DLL")]
        private static extern long SetCursorPos(int x, int y);
        [DllImport("USER32.DLL")]
        private static extern bool ClientToScreen(IntPtr hWnd, ref POINT point);
        [DllImport("USER32.DLL")]
        private static extern bool GetWindowRect(IntPtr hWnd, out RECT lpRect);
        [DllImport("USER32.DLL")]
        private static extern HWND SetFocus(IntPtr hWnd);
        private struct POINT
        {
            public int x;
            public int y;

            public POINT(int X, int Y)
            {
                x = X;
                y = Y;
            }
        }
        public struct RECT
        {
            public int left;        // x position of upper-left corner
            public int top;         // y position of upper-left corner
            public int right;       // x position of lower-right corner
            public int bottom;      // y position of lower-right corner
        }
    }
}
