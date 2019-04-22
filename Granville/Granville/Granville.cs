using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace Granville
{
    public class Granville
    {
        private const int A = 2;
        private const int B = 300000;

        private volatile int step;

        private volatile Barrier barrier;

        private volatile object setSLock = new object();
        private volatile IList<int> SetS = new List<int>();

        public Granville(int numThreads)
        {
            this.step = numThreads;
            this.barrier = new Barrier(numThreads);
            SetS.Add(1);
        }

        public void IsGranville(int startNr)
        {
            int currentSetSize = 0;
            int divSum = 0;
            int i, nr;
            int rem = B % step - startNr + step;

            lock (setSLock)
            {
                currentSetSize = SetS.Count();
            }

            for (nr = startNr + A; nr < B; nr += step)
            {
                divSum = 0;
                for (i = 0; i < currentSetSize; i++)
                {
                    if (nr % SetS[i] == 0 && SetS[i] < nr)
                    {
                        divSum += SetS[i];
                    }
                    if (divSum > nr || SetS[i] >= nr)
                    {
                        break;
                    }
                }

                if (divSum <= nr)
                {
                    if (divSum == nr)
                    {
                        Console.WriteLine("Found " + nr);
                    }

                    lock (setSLock)
                    {
                        SetS.Add(nr);
                    }
                }

                //If it is at the last iteration it shouldn't wait
                //This is to avoid threads hanging for other threads that exited the loop already
                if (nr + rem < B)
                {
                    barrier.SignalAndWait();
                }

                lock (setSLock)
                {
                    currentSetSize = SetS.Count();
                }

                if (nr + rem < B)
                {
                    barrier.SignalAndWait();
                }
            }
        }
    }
}
