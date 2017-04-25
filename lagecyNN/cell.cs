using System;
using System.Collections.Generic;
using System.Collections;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


namespace lagecyNN
{
    public class cell
    {
        public cell(double range)
        {
            this.d_range = range;
            this.dic_input = new Dictionary<int, double>();
            this.dic_weight = new Dictionary<int, double>();
            this.dic_nextCells = new Dictionary<int, cell>();
            this.dic_lastCells = new Dictionary<int, cell>();
            this.ran_generator = new Random(System.DateTime.Now.Millisecond);
            this.d_threshold = this.ran_generator.NextDouble();
        }
        public void AddNextCell(int hash, cell next)
        {
            this.dic_nextCells.Add(hash, next);
            next.d_range = this.d_range;
            next.AddLastCell(this.GetHashCode(), this);
        }
        public int RemoveNextCell(int hash)
        {
            cell next;
            if(this.dic_nextCells.TryGetValue(hash, out next))
            {
                next.RemoveLaseCell(this.GetHashCode());
                this.dic_nextCells.Remove(hash);
                return 0;
            }
            else
            {
                return -1;
            }

        }
        public sealed void AddLastCell(int hash, cell c)
        {
            this.dic_lastCells.Add(hash, c);
            this.dic_input.Add(hash, 0.0);
            this.dic_weight.Add(hash, this.ran_generator.NextDouble());
        }
        public sealed int RemoveLaseCell(int hash)
        {
            this.dic_lastCells.Remove(hash);
            this.dic_input.Remove(hash);
            this.dic_weight.Remove(hash);
            return 0;
        }

        public void Run()
        {
            
        }
        


        
        private double d_range;
        private double d_threshold;
        protected Dictionary<int, double> dic_input;
        private Dictionary<int, double> dic_weight;
        private Dictionary<int, cell> dic_lastCells;
        protected Dictionary<int, cell> dic_nextCells;
        private Random ran_generator;
        
        private double Active(double input)
        {
            return this.d_range / (1 + Math.Exp(-input));
        }
       
    }
    public class ioCell:cell
    {

    }
}
