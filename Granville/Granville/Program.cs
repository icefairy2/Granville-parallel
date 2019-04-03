using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace Granville
{
    class Program
    {
        static void IsGranville(int numStart)
        {
            int i;
        }

        static void Main(string[] args)
        {
            int numThreads = 1;
            int i;
            var threads = new List<Thread>();

            //Argument verification
            if (args.Length > 1)
            {
                Console.WriteLine("Usage: GRANVILLE.EXE <NR_THREADS>");
                return;
            }

            //Argument parsing
            if (args.Length == 1)
            {
                numThreads = int.Parse(args[0]);
            }

            //Measure time
            var watch = System.Diagnostics.Stopwatch.StartNew();

            //Create threads
            for (i = 0; i < numThreads; i++)
            {
                var th = new Thread(() => IsGranville(i));
                th.Start();
                threads.Add(th);
            }

            //Join threads
            for (i = 0; i < numThreads; i++)
            {
                threads[i].Join();
            }

            watch.Stop();

            Console.WriteLine("Time in ms: " + watch.ElapsedMilliseconds);
        }
    }
}
