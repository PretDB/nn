using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace test
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            Random rand = new Random(System.DateTime.Now.Millisecond);
            Dictionary<int, double> dic = new Dictionary<int, double>();
            c1[] c = new c1[4];
            for(int a = 0; a < 4; a++)
            {
                c[a] = new c1(rand.NextDouble());
            }
            foreach(c1  aa in c)
            {
                dic.Add(aa.GetHashCode(), aa.Value);
            }
        }
    }
    public class c1
    {
        public c1()
        {
            this.value = 3;
        }
        public c1(double value)
        {
            this.value = value;
        }
        private double value;
        public double Value
        {
            get
            {
                return this.value;
            }
        }
    }
}
