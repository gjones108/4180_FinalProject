using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO.Ports;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Xml.Linq;
using ClosedXML.Excel;

namespace _4180Project_COMConnector
{

    public partial class Form1 : Form
    {
        private static Mutex mut = new Mutex();

        private double lightData = -1;
        private double moistureData = -1;
        private double tempData = -1;
        string[] tempArr = new string[10];
        string[] lightArr = new string[10];
        string[] moistArr = new string[10];
        string[] humArr = new string[10];
        int menuSel = 0;

        public Form1()
        {
            InitializeComponent();
            //mbedCOM.ReadTimeout = 500;
            //mbedCOM.WriteTimeout = 500;
            mbedCOM.Open();
        }

        private void mbedCOM_DataReceived(object sender, System.IO.Ports.SerialDataReceivedEventArgs e)
        {
            //Console.WriteLine("trying");
            String inData;

            try
            {
                inData = mbedCOM.ReadLine();
                Console.WriteLine(inData);

                // successful data, try to display
                if (label1.InvokeRequired)
                {
                    Invoke(new EventHandler(DisplayData), inData, EventArgs.Empty);
                }
                else
                {
                    DisplayData(inData, EventArgs.Empty);
                }
            }
            catch (System.IO.IOException error)
            {
                //return;
            }
            catch (System.InvalidOperationException error)
            {
                //return;
            }
        }

        private void DisplayData(object sender, EventArgs e)
        {
            string data = (string)sender;
            //label1.Text = data;
            string first = "empty";
            try
            {
                first = data.Substring(0, 1);
            }
            catch { }
            string post_equals = "";

            int indexOfEquals = data.IndexOf("=");
            if (indexOfEquals != -1)
            {
                post_equals = data.Substring(indexOfEquals + 1);
            }

            if (first == "L")
            {
                // light
                lightlabel.Text = post_equals;
                for (int i = 9; i > 0; i--)
                {
                    lightArr[i] = lightArr[i - 1];
                }
                lightArr[0] = post_equals + "\n";


            }
            else if (first == "R")
            {
                // humidity
                humidlabel.Text = post_equals;
                for (int i = 9; i > 0; i--)
                {
                    humArr[i] = humArr[i - 1];
                }
                humArr[0] = post_equals + "\n";
            }
            else if (first == "T")
            {
                // temperature
                templabel.Text = post_equals;
                for (int i = 9; i > 0; i--)
                {
                    tempArr[i] = tempArr[i - 1];
                }
                tempArr[0] = post_equals + "\n";
            }
            else if (first == "M")
            {
                // moisture
                moistlabel.Text = post_equals;
                for (int i = 9; i > 0; i--)
                {
                    moistArr[i] = moistArr[i - 1];
                }
                moistArr[0] = post_equals + "\n";
            }
            else
            {
                // error

            }

            // store in array
            switch (menuSel)
            {
                case 1:
                    SelTitle.Text = "Light";
                    ArrayText.Text = string.Join("", lightArr);
                    break;
                case 2:
                    SelTitle.Text = "Humidity";
                    ArrayText.Text = string.Join("", humArr);
                    break;
                case 3:
                    SelTitle.Text = "Mositure";
                    ArrayText.Text = string.Join("", moistArr);
                    break;
                case 4:
                    SelTitle.Text = "Temperature";
                    ArrayText.Text = string.Join("", tempArr);
                    break;
                default:
                    break;
            }
            //mut.WaitOne();
            //String newData = inData;
            //mut.ReleaseMutex();

            SaveDataToFile(data, first, post_equals, DateTime.Now);
        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void label1_Click_1(object sender, EventArgs e)
        {

        }

        private void panel1_Paint(object sender, PaintEventArgs e)
        {

        }

        private void label4_Click(object sender, EventArgs e)
        {

        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (mbedCOM.IsOpen)
            {
                mbedCOM.Write(thresholdtxt.Text);
            }
        }

        private void label7_Click(object sender, EventArgs e)
        {

        }

        private void pictureBox3_Click(object sender, EventArgs e)
        {
            menuHideShow(1);
            menuSel = 4;
        }
        private void menuHideShow(int i)
        {
            if (i == 1)
            {
                pictureBox1.Hide();
                pictureBox2.Hide();
                pictureBox3.Hide();
                pictureBox4.Hide();
                button1.Hide();
                label1.Hide();
                label2.Hide();
                label3.Hide();
                label4.Hide();
                label5.Hide();
                label6.Hide();
                label7.Hide();
                lightlabel.Hide();
                moistlabel.Hide();
                templabel.Hide();
                humidlabel.Hide();
                thresholdtxt.Hide();
                button1.Hide();
                SelTitle.Show();
                ArrayText.Show();
                BackButton.Show();
            }
            else
            {
                pictureBox1.Show();
                pictureBox2.Show();
                pictureBox3.Show();
                pictureBox4.Show();
                button1.Show();
                label1.Show();
                label2.Show();
                label3.Show();
                label4.Show();
                label5.Show();
                label6.Show();
                label7.Show();
                lightlabel.Show();
                moistlabel.Show();
                templabel.Show();
                humidlabel.Show();
                thresholdtxt.Show();
                button1.Show();
                SelTitle.Hide();
                ArrayText.Hide();
                BackButton.Hide();
            }
        }

        private void BackButton_Click(object sender, EventArgs e)
        {
            menuHideShow(0);
            menuSel = 0;
        }

        private void pictureBox2_Click(object sender, EventArgs e)
        {
            menuHideShow(1);
            menuSel = 2;
        }

        private void pictureBox1_Click(object sender, EventArgs e)
        {
            menuHideShow(1);
            menuSel = 1;
        }

        private void pictureBox4_Click(object sender, EventArgs e)
        {
            menuHideShow(1);
            menuSel = 3;
        }

        private void SaveDataToFile(string data, string dataType, string dataValue, DateTime timestamp)
        {
            string filePath = "PHTdata.xlsx";


            try
            {
                XLWorkbook spreadsheet;

                // build or get spreadsheet
                if (File.Exists(filePath))
                {
                    spreadsheet = new XLWorkbook(filePath);
                }
                else
                {
                    spreadsheet = new XLWorkbook();
                    spreadsheet.Worksheets.Add("Data");

                    // add headers if the file is new
                    spreadsheet.Worksheet(1).Cell("A1").Value = "Type";
                    spreadsheet.Worksheet(1).Cell("B1").Value = "Value";
                    spreadsheet.Worksheet(1).Cell("C1").Value = "Timestamp";
                }

                // next available row
                int lastRow = spreadsheet.Worksheet(1).LastRowUsed()?.RowNumber() + 1 ?? 1;

                // populate data
                spreadsheet.Worksheet(1).Cell($"A{lastRow}").Value = dataType;
                spreadsheet.Worksheet(1).Cell($"B{lastRow}").Value = dataValue;
                spreadsheet.Worksheet(1).Cell($"C{lastRow}").Value = timestamp;

                spreadsheet.SaveAs(filePath);
            }
            catch (Exception ex)
            {
                Console.WriteLine("Error writing to Excel file: " + ex.Message);
            }
        }
    }
}